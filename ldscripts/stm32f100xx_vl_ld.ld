
SECTIONS
     {
       . = 0x0abfd;
       .text : { *(.text) }
	.pdgm_launchpad : { __pdgm_launchpad_vector = . ; * (.pdgm_launchpad)}
       .data : { *(.data) }
       .bss : { *(.bss) }
     }

