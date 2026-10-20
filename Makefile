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
	@echo "# ------ [ Making bootloader ] ------ #" 
	@$(MAKE) --no-print-directory -C bootloader/  

kernel: | libc
	@echo "# ------ [ Making kernel ] ------ #" 
	@$(MAKE) --no-print-directory -C kernel/

libc:
	@echo "# ------ [ Making libc ] ------ #"
	@$(MAKE) --no-print-directory -C libc/

run: image qemu-run

image: | sub
	@mkdir -p $(IMG_DIR)
	@echo " OBJCOPY  $(KERNEL_BIN)"
	@$(OBJCOPY) -O binary $(KERNEL) $(KERNEL_BIN)
	@echo " DD  $(IMG)"
	@dd if=/dev/zero of=$(IMG) bs=512 count=2880
	@cat $(BOOTLOADER_BIN) $(KERNEL_BIN) > $(IMG)
	@truncate -s 1474560 $(IMG)
clean:
	@echo " RM $(IMG_DIR)" 
	@rm -rf $(IMG_DIR)
	@$(MAKE) --no-print-directory -C bootloader/ clean
	@$(MAKE) --no-print-directory -C libc/ clean
	@$(MAKE) --no-print-directory -C kernel/ clean
