
_start:
	.globl _start
	b __main_prologe
	.weak __main_prologe
	b exokernel_main
	.globl exokernel_main

