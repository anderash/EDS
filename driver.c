#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/ioport.h>
#include <linux/device.h>
#include <linux/moduleparam.h>
#include <linux/kdev_t.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/signal.h>
#include <asm/siginfo.h>
#include <linux/interrupt.h>

#include "efm32gg.h"

#define GPIO_EVEN_IRQ_LINE 17
#define GPIO_ODD_IRQ_LINE 18

/* Prototypes */
static int __init gamepad_init(void);
static void __exit gamepad_exit(void);
static int gamepad_open(struct inode*, struct file*);
static int gamepad_release(struct inode*, struct file*);
static ssize_t gamepad_read(struct file*, char* __user, size_t, loff_t*);
static ssize_t gamepad_write(struct file*, char* __user, size_t, loff_t*);
static irqreturn_t gpio_interrupt(int, void*, struct pt_regs*);
static int gamepad_fasync(int, struct file*, int mode);

static struct file_operations gamepad_fops =
{
    .owner = THIS_MODULE,
    .open = gamepad_open,
    .read = gamepad_read,
    /*.write = gamepad_write,*/
    .release = gamepad_release,
    .fasync = gamepad_fasync,
};
 
static dev_t dev_num;
static struct cdev gamepad_cdev;
static struct class *cl;
static struct fasync_struct* async_queue;

/* Memory pointers */
static void* gpio_pc;
static void* gpio_intrpt;

static uint32_t last_input;
 
static int __init gamepad_init(void)
{
    /* TODO magne err handling */

    /* Allocate device region w/number */
    alloc_chrdev_region(&dev_num, 0, 1, "gamepad");

    /* Request memory region */
    request_mem_region(GPIO_PC_BASE, 24, "gamepad");
    gpio_pc = ioremap_nocache(GPIO_PC_BASE, 24); 
    request_mem_region(GPIO_INTRPT_BASE, 32, "gamepad");
    gpio_intrpt = ioremap_nocache(GPIO_INTRPT_BASE, 32); // 32 er lengden av minnet fra EXTIPSELL til slutten av IFC?

    /* Init GPIO */
    iowrite32(0x33333333, gpio_pc + GPIO_MODEL);
    iowrite8 (0xFF, gpio_pc + GPIO_DOUT);
    iowrite32(0x22222222, gpio_intrpt + GPIO_INTRPT_EXTIPSELL);

    /* Setup interrupt lines */
    request_irq(
        GPIO_EVEN_IRQ_LINE, 
        (irq_handler_t) gpio_interrupt, 
        0, "gamepad", &gamepad_cdev
    );
    request_irq(
        GPIO_ODD_IRQ_LINE, 
        (irq_handler_t) gpio_interrupt, 
        0, "gamepad", &gamepad_cdev
    );

    /* Enable interrupts */
    iowrite8(0xFF, gpio_intrpt + GPIO_INTRPT_EXTIFALL);
    iowrite8(0x00, gpio_intrpt + GPIO_INTRPT_EXTIRISE);
    iowrite16(0x00FF, gpio_intrpt + GPIO_INTRPT_IEN);
    iowrite8(0xFF, gpio_intrpt + GPIO_INTRPT_IFC);

    /* Register device in the system */
    cdev_init(&gamepad_cdev, &gamepad_fops);
    gamepad_cdev.owner = THIS_MODULE;
    gamepad_cdev.ops = &gamepad_fops;
    cdev_add(&gamepad_cdev, dev_num, 1);

    /* Making the driver visible to user space */
    cl = class_create(THIS_MODULE, "gamepad");
    device_create(cl, NULL, dev_num, NULL, "gamepad");

    printk(KERN_INFO "Gamepad kernel module initialized\n");
    return 0;
}

static void __exit gamepad_exit(void)
{
    /* disable interrupt */
    iowrite32(0x0000, gpio_intrpt + GPIO_INTRPT_IEN);

    /* irq */
    free_irq(GPIO_EVEN_IRQ_LINE, &gamepad_cdev);
    free_irq(GPIO_ODD_IRQ_LINE, &gamepad_cdev);

    /* memory */
    iounmap(gpio_pc);
    release_mem_region(GPIO_PC_BASE, 24);
    release_mem_region(GPIO_INTRPT_BASE, 32);

    /* device */
    device_destroy(cl, dev_num) ;
    class_destroy(cl);
    cdev_del(&gamepad_cdev);

    /* device num */
    unregister_chrdev_region(dev_num, 1);
    
    
    printk("Gamepad kernel module cleaned up\n");
}

/* FOPS */
static int gamepad_open(struct inode* inode, struct file* filp) {return 0;}
static int gamepad_release(struct inode* inode, struct file* filp) {return 0;}

/* 
    For our purposes, a read of the gamepad equates to the last interrupt signal
    that was triggered. The logic should be that you receive the signal, then
    immediately do a read from the gamepad
*/
static ssize_t gamepad_read(
    struct file* filp, char* __user buff, 
    size_t count, loff_t* offp
) {
    /* GPIO_PC_DIN */
    /* uint32_t button_state = ioread32(gpio_pc + GPIO_Px_DIN); */
    /* printk(KERN_INFO "GPIO read state\n"); */
    /* copy_to_user(buff, &button_state, 1); */
    copy_to_user(buff, &last_input, 1);
    return 1;
}

static ssize_t gamepad_write(
    struct file* filp, char* __user buff, 
    size_t count, loff_t* offp
) {
    return 1;
}

static irqreturn_t gpio_interrupt(int irq, void* dev_id, struct pt_regs* regs)
{
    /* printk(KERN_INFO "GPIO interrupt\n"); */
    last_input = ioread8(gpio_intrpt + GPIO_INTRPT_IF);
    uint32_t button_state = ioread8(gpio_pc + GPIO_DIN) ^ 0xFF;
    /* clear interrupt */
    iowrite8(last_input, gpio_intrpt + GPIO_INTRPT_IFC);
    /* For some reason, we sometimes get signals on both edges, this
       should abort the signal when the button state does not match the
       interrupt */
    if (async_queue && (last_input & button_state) != 0) {
        /* send signal */
        kill_fasync(&async_queue, SIGIO, POLL_IN);
    }
    return IRQ_HANDLED;
}

static int gamepad_fasync(int fd, struct file* filp, int mode)
{
    return fasync_helper(fd, filp, mode, &async_queue);
}


module_init(gamepad_init);
module_exit(gamepad_exit);

MODULE_DESCRIPTION("Kernel module for the gamepad used in TDT4258 fall 2014.");
MODULE_LICENSE("GPL");