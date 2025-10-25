all: build_c_files

CSOURCES := $(wildcard */*.c */*/*.c *.c)
build_c_files: $(CSOURCES)
	gcc -o ./../../vfsroot/kernel64.sys $(CSOURCES) -Iinclude -m64 -ffreestanding -mgeneral-regs-only -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -Werror -ffreestanding -Xlinker -T ./linkerscript.ld -nostdlib -lgcc -ffreestanding -g -no-pie