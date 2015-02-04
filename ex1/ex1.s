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

	CMU_BASE = 0x400c8000
	CMU_HFPERCLKEN0 = 0x044
	CMU_HFPERCLKEN0_GPIO = 13



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

	cmu_base_addr:
		.long CMU_BASE


	//// GPIO setup ////

	GPIO_BASE = 0x40006000
	GPIO_PA_CTRL = 0x00

	// Set drive strgh
	ldr r2, gpio_base_addr
	mov r1, #0x2
	str r1, [r2, #GPIO_PA_CTRL]


	// Set pins 8-15
	GPIO_PA_MODEH = 0x008

	ldr r1, =0x55555555
	str r1, [r2, #GPIO_PA_MODEH]

	GPIO_PC_MODEL = 0x04c

	ldr r1, =0x33333333
	str r1, [r2, #GPIO_PC_MODEL]
	
	GPIO_PC_DOUT = 0x054

	mov r1, #0x00
	str r1, [r2, #GPIO_PC_DOUT]

	GPIO_PA_DOUT = 0x00c

	ldr r1, =0xaaff
	str r1, [r2, #GPIO_PA_DOUT]



	
	      b .  // do nothing
	
	/////////////////////////////////////////////////////////////////////////////
	//
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
gpio_handler:  

	GPIO_EXTIPSELL = 0x100

	ldr r1, =0x22222222
	str r1, [r2, #GPIO_EXTIPSELL]


	GPIO_EXTIFALL = 0x10c
	mov r1, #0xff
	str r1, [r2, #GPIO_EXTIFALL]


	GPIO_EXTIRISE = 0x108
	// mov r1, #0xff
	str r1, [r2, #GPIO_EXTIRISE]


	GPIO_IEN = 0x110
	// mov r1, #0xff
	str r1, [r2, #GPIO_IEN]

	ldr r3, iser0_addr
	ldr r1, =0x802
	str r1, [r3, #0x00]




	      b .  // Interupt osvosv
	
iser0_addr:
		.long ISER0

	gpio_base_addr:
		.long GPIO_BASE

	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
dummy_handler:  
        b .  // do nothing


	
