
SECTIONS {
		
	.text : { . = ALIGN(4) ; * (.text) }
	.data : { __data_start = . ; * (.data) __data_end = . ; }
	.bss : { __bss_start = . ; * (.bss) * (COMMON) __bss_end = . ; }
}

