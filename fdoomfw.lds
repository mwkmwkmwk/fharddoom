SECTIONS {
	. = 0x80000000;
	start : { *(start) }
	.text : { *(.text*) }
	.rodata : { *(.rodata* .srodata*) }
	_data_image_start = .;
	. = 0xc0000000;
	_data_start = .;
	.data 0xc0000000 : AT (_data_image_start) { *(.data* .sdata*) }
	_data_end = .;
	.bss : { *(.bss* .sbss*) }
	_bss_end = .;
}
