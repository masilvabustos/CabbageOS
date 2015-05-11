
__early_init:
	bx lr
	.size __early_init, . - __early_init
	.type __early_init STT_FUNC
	.weak __early_init

Reset:
	bl __early_init
	bl _kstart
	b Reset
	.globl _kstart
	.globl Reset
	.size Reset,.-Reset
	.type Reset STT_FUNC

NMI_Handler:
HardFault_Handler:
SVC_Handler:
PSV_Handler:
SysTick_Handler:
UsageFault_Handler:
MemManage_Handler:
BusFault_Handler:
DebugMon_Handler:

	b .

	.globl NMI_Handler
	.globl HardFault_Handler
	.globl SVC_Handler
	.globl PSV_Handler
	.globl SysTick_Handler
	.globl UsageFault_Handler
	.globl MemManage_Handler
	.globl BusFault_Handler
	.globl DebugMon_Handler




