
.syntax unified

SVC_Handler:
	tst lr,0x4
	ite eq
	mrseq r1,msp
	mrsne r1,psp
	ldr r0,[r1,#24]
	and r0,r0,#0xff
	b sys_dispatch_syscall
	.size SVC_Handler,.-SVC_Handler
	.type SVC_Handler STT_FUNC

	
