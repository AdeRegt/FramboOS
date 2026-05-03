all: build_c_files

LUA_DIR = ../lua/origin
LUA_SRCS = $(filter-out $(LUA_DIR)/lua.c $(LUA_DIR)/luac.c $(LUA_DIR)/onelua.c $(LUA_DIR)/lmathlib.c $(LUA_DIR)/liolib.c $(LUA_DIR)/loslib.c $(LUA_DIR)/ldblib.c $(LUA_DIR)/lstrlib.c $(LUA_DIR)/lcorolib.c $(LUA_DIR)/lutf8lib.c $(LUA_DIR)/loadlib.c, $(wildcard $(LUA_DIR)/*.c))
CSOURCES := $(wildcard */*.c */*/*.c *.c */*/*/*.c */*.S */*/*.S */*/*/*.S *.S) $(LUA_SRCS)
build_c_files: $(CSOURCES)
	cp ../lua/spawner/* ../lua/origin
	nasm -f elf64 ./geheugen/interrupts/gdt.asm -o ./gdt.o
	gcc -o ./../../vfsroot/kernel64.sys $(CSOURCES)  ./gdt.o -Iinclude -I../lua/origin -m64 -ffreestanding -mgeneral-regs-only -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -Werror -ffreestanding -Xlinker -T ./linkerscript.ld -nostdlib -lgcc -ffreestanding -g -no-pie -D ENABLE_GDT -D ENABLE_INTERUPTS -D ENABLE_PAGING -D ENABLE_XHCI -DLUA_32BITS -DLUA_USE_C89 -msoft-float -mno-mmx -mno-sse -mno-sse2 -Xlinker -z -Xlinker noexecstack
	rm ./gdt.o