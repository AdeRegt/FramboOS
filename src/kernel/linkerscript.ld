/**
Dit is het linkerscript.
Hiermee word aangegeven aan de compiler hoe de kernel er uit moet zien
De target is een 64 bit ELF file
De entry point is kernel_main 
En de kernel begint op  0x10000000
**/
OUTPUT_FORMAT(elf64-x86-64)
ENTRY(kernel_main)

SECTIONS
{
	. = 0x10000000;
	_KernelStart = .;
	.text : ALIGN(0x1000)
	{
		*(.text)
	}
	.data : ALIGN(0x1000)
	{
		*(.data)
	}
	.rodata : ALIGN(0x1000)
	{
		*(.rodata)
	}
	.bss : ALIGN(0x1000)
	{
		*(COMMON)
		*(.bss)
	}
	_KernelEnd = .;
}