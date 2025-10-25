all:
	$(MAKE) -C src

copy_to_sector: 
	cp -r vfsroot/* $(COPYTO)