OBJCOPY = i386-elf-objcopy
ISO_DIR = isodir
KERNEL = kernel/arch/i386/build/fausto
KERNEL_BIN = $(ISO_DIR)/kernel.bin 
BOOTLOADER_BIN = bootloader/build/fausto_bootloader.bin
IMG = $(ISO_DIR)/system.img

.PHONY: iso clean

all: help

help:
	echo -e "Targets available: \n-iso\n-clean"

iso: $(KERNEL) $(BOOTLOADER_BIN)
	mkdir -p $(ISO_DIR)
	$(OBJCOPY) -O binary $(KERNEL) $(KERNEL_BIN)
	dd if=/dev/zero of=$(IMG) bs=1M count=10
	dd if=$(BOOTLOADER_BIN) of=$(IMG) conv=notrunc bs=512 count=1 seek=0
	dd if=$(KERNEL_BIN) of=$(IMG) conv=notrunc bs=512 seek=1
clean:
	rm -rf $(ISO_DIR)
