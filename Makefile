OBJCOPY = i386-elf-objcopy
ISO_DIR = isodir
KERNEL = kernel/fausto
KERNEL_BIN = $(ISO_DIR)/kernel.bin 
BOOTLOADER_BIN = bootloader/build/fausto_bootloader.bin
IMG = $(ISO_DIR)/floppy.img

.PHONY: iso clean help sub bootloader libc kernel

all: help

sub: bootloader libc kernel

help:
	@echo -e "Targets available: \n"\
		"-iso\n"\
		"-clean\n"\
		"-sub(bootloader + libc + kernel)\n"\
		"-bootloader\n"\
		"-libc\n"\
		"-kernel"

qemu-run:
	qemu-system-i386 -drive format=raw,file=$(IMG)

bootloader:
	$(MAKE) -C bootloader/

kernel:
	$(MAKE) -C kernel/

libc:
	$(MAKE) -C libc/

iso: $(KERNEL) $(BOOTLOADER_BIN)
	mkdir -p $(ISO_DIR)
	$(OBJCOPY) -O binary $(KERNEL) $(KERNEL_BIN)
	#dd if=/dev/zero of=$(IMG) bs=512 count=2880
	dd if=/dev/zero of=$(IMG) bs=1M count=10
	#dd if=$(BOOTLOADER_BIN) of=$(IMG) conv=notrunc bs=512 count=1 seek=0
	#dd if=$(KERNEL_BIN) of=$(IMG) conv=notrunc bs=512 seek=25
	cat $(BOOTLOADER_BIN) $(KERNEL_BIN) > $(IMG)
clean:
	rm -rf $(ISO_DIR)
	make -C bootloader/ clean
	make -C libc/ clean
	make -C kernel/ clean
