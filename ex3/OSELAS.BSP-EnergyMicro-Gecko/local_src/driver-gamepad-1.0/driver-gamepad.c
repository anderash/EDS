#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h> //fasync
#include <linux/proc_fs.h>
#include <linux/ioport.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/moduleparam.h>
#include <linux/kdev_t.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <linux/unistd.h> //read
#include <asm/uaccess.h>
#include <asm/signal.h>
#include <asm/siginfo.h>
#include <linux/interrupt.h>
#include "efm32gg.h"

#define DRIVER_NAME "gamepad"
#define DEV_NR_COUNT 1
#define GPIO_EVEN_IRQ_LINE 17
#define GPIO_ODD_IRQ_LINE 18

static int __init my_init(void);
static void __exit my_exit(void);
static int my_open(struct inode *inode, struct file *filp);
static int my_release(struct inode *inode, struct file *filp);
static ssize_t my_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
static ssize_t my_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);
irqreturn_t gpio_interrupt_handler(int irq, void *dev_id, struct pt_regs *regs);
static int my_fasync(int fd, struct file * filp, int mode); 

static dev_t device_nr;
struct cdev my_cdev;
struct class* cl;
struct fasync_struct * async_queue; 

static struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.read = my_read ,
	.write = my_write ,
	.open = my_open ,
	.release = my_release,
	.fasync = my_fasync,
};



irqreturn_t gpio_interrupt_handler(int irq, void *dev_id, struct pt_regs *regs)
{

	printk(KERN_ALERT "Handling GPIO interrupt\n");
	iowrite32(ioread32(GPIO_IF), GPIO_IFC);
	if (async_queue) {
		kill_fasync (& async_queue, SIGIO, POLL_IN);

	}
	return IRQ_HANDLED;

}

static int __init my_init(void)
{
	printk(KERN_ALERT "Attempting to load gamepad driver module\n");
	int result;

	/* Dynamically allocate device numbers */
	result = alloc_chrdev_region(&device_nr, 0, DEV_NR_COUNT, DRIVER_NAME);
	if (result < 0) {
		printk(KERN_ALERT "Failed to allocate device numbers\n");
		return -1;
	}

	if (request_mem_region(GPIO_PC_MODEL, 1, DRIVER_NAME) == NULL ) {
		printk(KERN_ALERT "Error requesting GPIO_PC_MODEL memory region, already in use?\n");
		return -1;
	}
	if (request_mem_region(GPIO_PC_DOUT, 1, DRIVER_NAME) == NULL ) {
		printk(KERN_ALERT "Error requesting GPIO_PC_DOUT memory region, already in use?\n");
		return -1;
	}
	if (request_mem_region(GPIO_PC_DIN, 1, DRIVER_NAME) == NULL ) {
		printk(KERN_ALERT "Error requesting GPIO_PC_DIN memory region, already in use?\n");
		return -1;
	}
	//ioremap_nocache(GPIO_PA_BASE, GPIO_IFC - GPIO_PA_BASE);
	/* Init gpio as in previous exercises.
	* For portability, these writes should be performed with a base address
	* obtained from the ioremap_nocache call above and an offset. What we are
	* doing below is possible since we're not using virtual memory.
	*/
	iowrite32(0x33333333, GPIO_PC_MODEL);
	iowrite32(0xFF, GPIO_PC_DOUT);
	iowrite32(0x22222222, GPIO_EXTIPSELL);
	/* Setup for interrupts */
	 request_irq(GPIO_EVEN_IRQ_LINE, (irq_handler_t)gpio_interrupt_handler, 0, DRIVER_NAME, &my_cdev);
	request_irq(GPIO_ODD_IRQ_LINE, (irq_handler_t)gpio_interrupt_handler, 0, DRIVER_NAME, &my_cdev);

	/* add device */
	cdev_init(&my_cdev, &my_fops);
	my_cdev.owner = THIS_MODULE;
	cdev_add(&my_cdev, device_nr, DEV_NR_COUNT);
	cl = class_create(THIS_MODULE, DRIVER_NAME);
	device_create(cl, NULL, device_nr, NULL, DRIVER_NAME);

	/* Actually enable interrupts */
	iowrite32(0xFF, GPIO_EXTIFALL);
	iowrite32(0x00FF, GPIO_IEN);
	iowrite32(0xFF, GPIO_IFC);

	printk(KERN_INFO "Gamepad driver loaded.\n");
	return 0;
}


static void __exit my_exit(void)
{
	printk("Unloading gamepad driver\n");

	//remove_proc_entry("fasync", NULL);
	/* Disable interrupts */
	iowrite32(0x0000, GPIO_IEN);
	/* Free irq */
	free_irq(GPIO_EVEN_IRQ_LINE, &my_cdev);
	free_irq(GPIO_ODD_IRQ_LINE, &my_cdev);
	/* Release memory regions */
	release_mem_region(GPIO_PC_MODEL, 1);
	release_mem_region(GPIO_PC_DIN, 1);
	release_mem_region(GPIO_PC_DOUT, 1);
	/* Remove device */
	device_destroy(cl, device_nr);
	class_destroy(cl);
	cdev_del(&my_cdev);
	/* Dealloc the device numbers */
	unregister_chrdev_region(device_nr, DEV_NR_COUNT);
}

static int my_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "Gamepad driver opened\n");
	return 0;
}
static int my_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "Gamepad driver closed\n");
	return 0;	
}
static ssize_t my_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
	 /* Read gpio button status and write to buff */
	uint32_t data = ioread32(GPIO_PC_DIN);
	copy_to_user(buff, &data, 1);
	return 1;
}
static ssize_t my_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
	printk(KERN_INFO "Writing to buttons doesn't make sense.");
	return 1;
}


static int my_fasync (int fd, struct file * filp, int mode) 
{
	return fasync_helper (fd, filp, mode, & async_queue); 
}

module_init(my_init);
module_exit(my_exit);

MODULE_DESCRIPTION("Driver for the gamepad");
MODULE_LICENSE("GPL");
