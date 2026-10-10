OBJCOPY = i386-elf-objcopy
IMG_DIR = imgdir
KERNEL = kernel/fausto
KERNEL_BIN = $(IMG_DIR)/kernel.bin 
BOOTLOADER_BIN = bootloader/build/fausto_bootloader.bin
IMG = $(IMG_DIR)/floppy.img

.PHONY: image clean help sub bootloader libc kernel qemu-run run

all: help

sub: bootloader libc kernel

help:
	@echo -e "Targets available: \n"\
		"-image\n"\
		"-clean\n"\
		"-sub(bootloader + libc + kernel)\n"\
		"-bootloader\n"\
		"-libc\n"\
		"-kernel\n"\
		"-run"

qemu-run:
	qemu-system-i386 -fda $(IMG)
	

bootloader:
	$(MAKE) -C bootloader/

kernel: | libc
	$(MAKE) -C kernel/

libc:
	$(MAKE) -C libc/

run: image qemu-run

image: | sub
	mkdir -p $(IMG_DIR)
	$(OBJCOPY) -O binary $(KERNEL) $(KERNEL_BIN)
	dd if=/dev/zero of=$(IMG) bs=512 count=2880
	cat $(BOOTLOADER_BIN) $(KERNEL_BIN) > $(IMG)
clean:
	rm -rf $(IMG_DIR)
	$(MAKE) -C bootloader/ clean
	$(MAKE) -C libc/ clean
	$(MAKE) -C kernel/ clean
