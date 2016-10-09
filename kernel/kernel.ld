ENTRY(__entry)

SECTIONS {
	/*PROVIDE(kernel_init = 0xC0100000);*/

	. = 0x100000;
	PROVIDE(__tempload_begin = .);
	.tempload.text : {
		*(.tempload.text)
		. = ALIGN(4096);
	}
	.tempload.data : {
		*(.tempload.data)
		. = ALIGN(4096);
	}
	PROVIDE(__tempload_end = .);

	. += 0xC0000000;
	PROVIDE(__text_begin = .);
	.text : AT(ADDR(.text) - 0xC0000000) {
		*(.text)
		. = ALIGN(4096);
	}
	PROVIDE(__text_end = .);
	PROVIDE(__data_begin = .);
	.data : AT(ADDR(.data) - 0xC0000000) {
		*(.data)
		*(.rodata)
		. = ALIGN(4096);
	}
	PROVIDE(__data_end = .);
	PROVIDE(__bss_begin = .);
	.bss : AT(ADDR(.bss) - 0xC0000000) {
		*(.bss)
		. = ALIGN(4096);
	}
	PROVIDE(__bss_end = .);
	PROVIDE(__stab_begin = .);
	.stab : AT(ADDR(.stab) - 0xC0000000) {
		*(.stab)
		. = ALIGN(4096);
	}
	PROVIDE(__stab_end = .);
	PROVIDE(__stabstr_begin = .);
	.stabstr : AT(ADDR(.stabstr) - 0xC0000000) {
		*(.stabstr)
		. = ALIGN(4096);
	}
	PROVIDE(__stabstr_end = .);

	. = 0xC03FF000;
	PROVIDE(pde_kern = .);
	. = 0xC0400000;
	PROVIDE(ptes_kern = .);
	. = 0xC0800000;
	PROVIDE(pmm_free_space_start = .);
	. = 0xC1000000;
	PROVIDE(pmm_free_space_end = .);

	/DISCARD/ : {
		*(.comment)
		*(.note)
		*(.eh_frame)
	}
}
