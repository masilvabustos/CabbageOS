
SECTIONS {
		
	.text : { _ktext_start = . ; * (.text) _ktext_end = . ; }
	.data : { _kdata_start = . ; * (.data) _kdata_end = . ; }
	.bss : { _kbss_start = . ; * (.bss) * (COMMON) _kbss_end = . ; }
}

