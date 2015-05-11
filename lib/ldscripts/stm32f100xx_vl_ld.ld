
SECTIONS
     {
       	. = 0x0;
       	.text : { *(.text) }
	.pdgm_launchpad : { __pdgm_launchpad_vector = . ; * (.pdgm_launchpad)}
       	.data : { *(.data) }
	.periph_registers 0x40000000 : {
		. = 0x10800 ;
		* (.gpio_registers)
		main.o (*.text)
	}
       	.bss : { *(.bss) }
     }

