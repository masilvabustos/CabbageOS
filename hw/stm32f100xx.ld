

ASSERT(DEFINED(Default_Handler), "Default_Handler not defined!")

SECTIONS {
	
	__end_of_ram__ = ALIGN(ORIGIN(SRAM) + LENGTH(SRAM), 4) ;

	.cm3.isr_vector ORIGIN(FLASH) : {
		LONG(__end_of_ram__)
		/* ARM Thumb interwork requires the LSB to be set.	*
		 * (Can ld do this automatically?) 			*/ 			
		LONG(_start 		| 1)
		PROVIDE(Default_Handler		= _start);
		PROVIDE(NMI_Handler 		= Default_Handler);
		PROVIDE(HardFault_Handler 	= Default_Handler);
 		PROVIDE(MemManage_Handler 	= Default_Handler);
		PROVIDE(BusFault_Handler 	= Default_Handler);
		PROVIDE(SVC_Handler		= Default_Handler);
		PROVIDE(PendSV_Handler 		= Default_Handler);
		PROVIDE(SysTick_Handler 	= Default_Handler);
		LONG(NMI_Handler 	| 1)
		LONG(HardFault_Handler 	| 1)
		LONG(MemManage_Handler 	| 1)
		LONG(BusFault_Handler 	| 1)
		/*LONG(UsageFault_Handler | 1)*/
		. = 0x2c ; /* Reserved */
		LONG(SVC_Handler 	| 1)
		/*LONG(DebugMon_Handler	| 1)*/
		. = 0x38 ; /* Reserved */
		LONG(PendSV_Handler	| 1)
		LONG(SysTick_Handler	| 1)
		
		. = 0x40 ;
		* (.irqsr_vector)
		
		. = 0x134 ;

	} > FLASH


	.text : { 
		. = ALIGN(4) ;
		* (.text) 
	} > FLASH
	
	__data_lma__ = . ;
	.data : AT(__data_lma__) { 
		__data_vma__ = . ;
		* (.data) 
	} > SRAM
	__init_data_end = . ;

	.bss : {
		* (.bss)
	} > SRAM

	__brk = . ;

	.stm32f100xx.peripherals_registers : {

		/* TODO: Add remaing registers */
		. = 0x10800 ;
		GPIOA_CRL 	= . + 0x00 ;
		GPIOA_CRH 	= . + 0x04 ;
		GPIOA_IDR 	= . + 0x08 ;
		GPIOA_ODR 	= . + 0x0C ;
		GPIOA_BSRR 	= . + 0x10 ;
		GPIOA_BRR	= . + 0x14 ;
		GPIOA_LCKR	= . + 0x18 ;
		
		. = 0x11000 ;
		GPIOC_CRL 	= . + 0x00 ;
		GPIOC_CRH 	= . + 0x04 ;
		GPIOC_IDR 	= . + 0x08 ;
		GPIOC_ODR 	= . + 0x0C ;
		GPIOC_BSRR 	= . + 0x10 ;
		GPIOC_BRR	= . + 0x14 ;
		GPIOC_LCKR	= . + 0x18 ;

		. = 0x21000 ;
		RCC_CR		= . + 0x00 ;
		RCC_CFGR	= . + 0x04 ;
		RCC_CIR		= . + 0x08 ;
		RCC_APB2RSTR	= . + 0x0C ;
		RCC_APB1RSTR 	= . + 0x10 ;
		RCC_AHBENR	= . + 0x14 ;
		RCC_APB2ENR	= . + 0x18 ;
		RCC_APB1ENR	= . + 0x1C ;
		RCC_BDCR	= . + 0x20 ;
		RCC_CSR		= . + 0x24 ;
		RCC_CFGR2	= . + 0x2C ;


	} > PERIPH

	.stm32f100xx.peripherals_bitband_alias 0x42000000 : {

		. 		= (RCC_APB2ENR 	- ORIGIN(PERIPH)) * 32 ;
		IOPCEN		=  . +  4 * 4 ;

	} > PERIPH	
}

