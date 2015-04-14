
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <asm/io.h>

#include "driver-gamepad.h"

/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

static int __init template_init(void)
{
	printk("Hello World, gamepad driver is setting up\n");
	
	base = ioremap_nocache();

	iowrite32(0x33333333, *GPIO_PC_MODEL);
	iowrite32(0xFF, *GPIO_PC_DOUT);

	
	iowrite32(0x22222222, *GPIO_EXTIPSELL);
	iowrite32(0xFF, *GPIO_EXTIFALL);
	iowrite32(0xFF, *GPIO_EXTIRISE);
	iowrite32(0xFF, *GPIO_IEN);
	
	/*
	*GPIO_PA_CTRL 		= 2;  				// set high drive strength
	*GPIO_PA_MODEH		= 0x55555555; // set pins A8-15 as output
	*GPIO_PA_DOUT 		= 0xff00; 		// turn off all LEDs(LEDs are active low)

	*GPIO_PC_MODEL 		= 0x33333333; // Set pins 0-7 to input
	*GPIO_PC_DOUT 		= 0xFF; 			// Enable internal pull-up

	*GPIO_EXTIPSELL 	= 0x22222222; // 
	*GPIO_EXTIFALL		= 0xFF;				//Set interrupt on neg-edge
	//*GPIO_EXTIRISE	= 0xFF;				// Set interrupt on pos-edge
	*GPIO_IEN			= 0xFF;				// Enable interrupt generation
	*/
	return 0;
}


/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit template_cleanup(void)
{
	 printk("Short life for a small module...\n");
}

module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

