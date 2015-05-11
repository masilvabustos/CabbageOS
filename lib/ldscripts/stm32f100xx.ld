
SECTIONS {
	
	__end_of_ram = ALIGN(ORIGIN(SRAM) + LENGTH(SRAM), 4) ;

	.cm3.isr_vector ORIGIN(FLASH) : {
		LONG(__end_of_ram)
		LONG(Reset)
		LONG(NMI_Handler)
		LONG(HardFault_Handler)
		LONG(MemManage_Handler)
		LONG(BusFault_Handler)
		. = 0x2c ;
		LONG(SVC_Handler)
		LONG(DebugMon_Handler)
		. = 0x38 ;
		LONG(PSV_Handler)
		LONG(SysTick_Handler)

		* (.isr_vector)

	} > FLASH

	.text : { 
		* (.text) 
	} > FLASH

	.data : { 
		* (.data) 
	} > SRAM

	.bss : {
		 _bss_start = . ;
		* (.bss) * (COMMON)
		_bss_end = . ;
	} > SRAM

	__edata = . ;

	.stm32f100xx.preripherals_registers : {
		. = 0x800 ;
		GPIOA_CRL 	= . + 0x00 ;
		GPIOA_CRH 	= . + 0x04 ;
		GPIOA_IDR 	= . + 0x08 ;
		GPIOA_ODR 	= . + 0x0C ;
		GPIOA_BSRR 	= . + 0x10 ;
		GPIOA_BRR	= . + 0x14 ;
		GPIOA_LCKR	= . + 0x18 ;
	} > PERIPH
}

