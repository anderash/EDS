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

 .long 0x1000
 .long _reset

_reset: 

	// load CMU addr
	ldr r1 , cmu_base_addr

	// load HeperCLK Enable
	ldr r2, [r1 , #CMU_HFPERCLKEN0]

	// set bit GPIO clk
	mov r3, #1
	lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
	orr r2, r2, r3

	// store new value
	str r2, [r1, #CMU_HFPERCLKEN0]

	


	//// GPIO setup ////



	// Set drive strgh
	ldr r2, gpio_pa_base_addr
	mov r1, #0x2
	str r1, [r2, #GPIO_CTRL]

	// Set pins 8-15

	ldr r1, =0x55555555
	str r1, [r2, #GPIO_MODEH]

	ldr r1, =0xff00
	str r1, [r2, #GPIO_DOUT]


	ldr r2, gpio_pc_base_addr

	ldr r1, =0x33333333
	str r1, [r2, #GPIO_MODEL]

	mov r1, #0xff
	str r1, [r2, #GPIO_DOUT]


	ldr r2, gpio_base_addr

	ldr r1, =0x22222222
	str r1, [r2, #GPIO_EXTIPSELL]


	mov r1, #0xff
	str r1, [r2, #GPIO_EXTIFALL]
	str r1, [r2, #GPIO_EXTIRISE]
	str r1, [r2, #GPIO_IEN]


	ldr r3, iser0_addr
	ldr r1, =0x802
	str r1, [r3, #0x00]

	mov r3, #6
	ldr r2, =SCR
	str r3, [r2]	

	wfi	
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

	ldr r4, [r2, #GPIO_IF]
	str r4, [r2, #GPIO_IFC]

	ldr r2, gpio_pc_base_addr

	ldr r4, [r2, #GPIO_DIN]

	lsl r4, r4, #8

	
	ldr r2, gpio_pa_base_addr
	str r4, [r2, #GPIO_DOUT]



	bx lr
	      b .  // Interupt osvosv

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


	
