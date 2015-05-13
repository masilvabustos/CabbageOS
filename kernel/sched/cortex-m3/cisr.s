
__early_setup:
	bx lr
	.size __early_setup, . - __early_setup
	.type __early_setup STT_FUNC
	.weak __early_setup

Reset:
	bl __early_setup
	bl kernel_init
	b Reset
	.globl kernel_init
	.globl Reset
	.size Reset,.-Reset
	.type Reset STT_FUNC
	.weak Reset

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




