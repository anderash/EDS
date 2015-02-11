        .syntax unified
	
	      .include "efm32gg.s"

	/////////////////////////////////////////////////////////////////////////////
	//
  // Exception vector table
  // This table contains addresses for all exception handlers
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .section .vectors
	
	      .long   stack_top               /* Top of Stack                 */
	      .long   _reset                  /* Reset Handler                */
	      .long   dummy_handler           /* NMI Handler                  */
	      .long   dummy_handler           /* Hard Fault Handler           */
	      .long   dummy_handler           /* MPU Fault Handler            */
	      .long   dummy_handler           /* Bus Fault Handler            */
	      .long   dummy_handler           /* Usage Fault Handler          */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* SVCall Handler               */
	      .long   dummy_handler           /* Debug Monitor Handler        */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* PendSV Handler               */
	      .long   dummy_handler           /* SysTick Handler              */

	      /* External Interrupts */
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO even handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO odd handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler

	      .section .text

	      // Test skriving

	/////////////////////////////////////////////////////////////////////////////
	//
	// Reset handler
  // The CPU will start executing here after a reset
	//
	/////////////////////////////////////////////////////////////////////////////



	      .globl  _reset
	      .type   _reset, %function
        .thumb_func


_reset: 

	ldr r1 , cmu_base_addr  		 // load CMU addr

	
	ldr r2, [r1 , #CMU_HFPERCLKEN0] 	 // load HeperCLK Enable

	
	mov r3, #1    				// set bit GPIO clk
	lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
	orr r2, r2, r3

	
	str r2, [r1, #CMU_HFPERCLKEN0]		// store new value

	


	//// GPIO setup ////

						
	ldr r2, gpio_pa_base_addr	// set base address
		
	mov r1, #0x2			// Set drive strength
	str r1, [r2, #GPIO_CTRL]

	

	ldr r1, =0x55555555		// set pins 8-15 to output
	str r1, [r2, #GPIO_MODEH]

	ldr r1, =0xffff			// set output to the gamepad
	str r1, [r2, #GPIO_DOUT]


	ldr r2, gpio_pc_base_addr	// set base address

	ldr r1, =0x33333333        	// set pins 0-7 to input
	str r1, [r2, #GPIO_MODEL]

	mov r1, #0xff			// enable internal pull-up
	str r1, [r2, #GPIO_DOUT]


	ldr r2, gpio_base_addr		// set base address

	ldr r1, =0x22222222		
	str r1, [r2, #GPIO_EXTIPSELL]


	mov r1, #0xff
	str r1, [r2, #GPIO_EXTIFALL]    // set interrupt on neg-edge
	str r1, [r2, #GPIO_EXTIRISE]    // set interrupt on pos-edge
	str r1, [r2, #GPIO_IEN]		// enables interrupt generation


	ldr r3, iser0_addr		// enable interrupt handling
	ldr r1, =0x802
	str r1, [r3]

	mov r3, #6			// enable deep sleep and automatic 
	ldr r2, =SCR			// sleep on return from interrupt handler
	str r3, [r2]	

	wfi				// go into sleep mode
	      b .  // do nothing
	
	/////////////////////////////////////////////////////////////////////////////
	//
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
gpio_handler:  


	ldr r2, gpio_base_addr

	ldr r4, [r2, #GPIO_IF]		// getting the source of the interrupt
	str r4, [r2, #GPIO_IFC]		// clear the interrupt


	ldr r2, gpio_pc_base_addr
	ldr r4, [r2, #GPIO_DIN]		// get input from gamepad buttons
	lsl r4, r4, #8			// shift left from 0-7 to 8-15 in order
					// in order to control LEDs

	ldr r2, gpio_pa_base_addr
	str r4, [r2, #GPIO_DOUT]	// set LED values

	
	bx lr				// branch back to sleep
	      b .  // do nothing

	cmu_base_addr:
		.long CMU_BASE
	
	iser0_addr:
		.long ISER0

	gpio_base_addr:
		.long GPIO_BASE

	gpio_pa_base_addr:
		.long GPIO_PA_BASE

	gpio_pc_base_addr:
		.long GPIO_PC_BASE



	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
dummy_handler:  
        b .  // do nothing


	
