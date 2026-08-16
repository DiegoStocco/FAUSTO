ARCH = i386
CC = $(ARCH)-elf-gcc
CCFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
CCLINKFLAGS = -ffreestanding -O2 -nostdlib
LFLAGS = -lgcc
AS = $(ARCH)-elf-as
kernel_srcs = $(wildcard kernel/*.c)
kernel_objs = $(patsubst kernel/%.c, arch/$(ARCH)/build/%.o, $(kernel_srcs))

asm_srcs = $(wildcard arch/$(ARCH)/*.S)
asm_objs = $(patsubst arch/$(ARCH)/%.S, arch/$(ARCH)/build/%.o, $(asm_srcs))

linker = arch/$(ARCH)/linker.ld

TARGET = arch/$(ARCH)/build/fausto

.PHONY: clean

$(TARGET): $(asm_objs) $(kernel_objs) $(linker)
	$(CC) -T $(linker) -o $@ $(CCLINKFLAGS) $(asm_objs) $(kernel_objs) $(LFLAGS) 
 
$(kernel_objs): $(kernel_srcs)
	$(CC) -c $< -o $@ $(CCFLAGS)

$(asm_objs): $(asm_srcs)
	mkdir -p arch/$(ARCH)/build/
	$(AS) $< -o $@

clean:
	rm -f arch/$(ARCH)/build/*

