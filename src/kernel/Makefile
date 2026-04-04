all: build_c_files

CSOURCES := $(wildcard */*.c */*/*.c *.c */*/*/*.c */*.S */*/*.S */*/*/*.S *.S)
build_c_files: $(CSOURCES)
	nasm -f elf64 ./geheugen/interrupts/gdt.asm -o ./gdt.o
	gcc -o ./../../vfsroot/kernel64.sys $(CSOURCES) ./gdt.o -Iinclude -m64 -ffreestanding -mgeneral-regs-only -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -Werror -ffreestanding -Xlinker -T ./linkerscript.ld -nostdlib -lgcc -ffreestanding -g -no-pie -D ENABLE_GDT -D ENABLE_INTERUPTS -D ENABLE_PAGING -D ENABLE_XHCI -D XHCI_XHCI_TREAD -Xlinker -z -Xlinker noexecstack
	rm ./gdt.o