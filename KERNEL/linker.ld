ENTRY(kernel)
SECTIONS
{
    . = 0x2000;
    .text : {
    	*(.multiboot)
    	*(.text)
    }
    .rodata : {*(.rodata)}
    .data : {*(.data)}
    .bss : {*(bss)}
}

