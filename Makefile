all:
	$(MAKE) -C src

image: all
	rm -f framboos.img
	dd if=/dev/zero of=framboos.img bs=1M count=128
	parted framboos.img --script mklabel gpt
	parted framboos.img --script mkpart ESP fat32 1MiB 100%
	mformat -i framboos.img@@1M -F
	mmd -i framboos.img@@1M ::EFI
	mmd -i framboos.img@@1M ::EFI/BOOT
	mcopy -i framboos.img@@1M vfsroot/EFI/BOOT/BOOTX64.EFI ::EFI/BOOT/BOOTX64.EFI
	mcopy -i framboos.img@@1M vfsroot/kernel64.sys ::kernel64.sys
	mcopy -i framboos.img@@1M vfsroot/shell.bin ::shell.bin
	mcopy -i framboos.img@@1M vfsroot/fasm.bin ::fasm.bin

copy_to_sector: 
	cp -r vfsroot/* $(COPYTO)