
.syntax unified

PendSV_Handler:
	tst lr,0b100
	ite eq
	mrseq r0,msp
	mrsne r0,psp
	stm r0!,{r4-r11}
	
