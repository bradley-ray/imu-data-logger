ENTRY(_reset);
MEMORY {
	flash(rx) : ORIGIN = 0x08000000, LENGTH = 32k
	ram(rwx) : ORIGIN = 0x20000000, LENGTH = 12k
}
_estack = ORIGIN(ram) + LENGTH(ram);

_Min_Heap_Size = 0x200 ; /* required amount of heap */
_Min_Stack_Size = 0x400 ; /* required amount of stack */

SECTIONS {
	.vectors 	: { KEEP(*(.vectors)) }											> flash
	.text		: { *(.text.reset) *(.text.default_handler) *(.text*) } 		> flash
	.rodata		: { *(.rodata*) }												> flash

	.data : {
		_sdata = .;
		*(.first_data)
		*(.data SORT(.data.*))
		_edata = .;
	} > ram AT > flash
	_sidata = LOADADDR(.data);

	.bss : {
		_sbss = .;
		*(.bss SORT(.bss.*) COMMON)
		_ebss = .;
	} > ram

	._user_heap_stack : {
		. = ALIGN(8);
		PROVIDE ( end = . );
		PROVIDE ( _end = . );
		. = . + _Min_Heap_Size;
		. = . + _Min_Stack_Size;
			. = ALIGN(8);
	} > ram
}
