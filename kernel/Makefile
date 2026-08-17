ARCH = i386
ARCHDIR = arch/$(ARCH)
CC = $(ARCH)-elf-gcc
CCFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Iinclude
CCLINKFLAGS = -ffreestanding -O2 -nostdlib
LFLAGS = -lgcc
AS = $(ARCH)-elf-as

arch_srcs = $(wildcard $(ARCHDIR)/*.c)
arch_objs = $(patsubst $(ARCHDIR)/%.c, $(ARCHDIR)/build/%.o, $(arch_srcs))

kernel_srcs = $(wildcard kernel/*.c)
kernel_objs = $(patsubst kernel/%.c, $(ARCHDIR)/build/%.o, $(kernel_srcs))

asm_srcs = $(wildcard $(ARCHDIR)/*.S)
asm_objs = $(patsubst $(ARCHDIR)/%.S, $(ARCHDIR)/build/%.o, $(asm_srcs))

linker = $(ARCHDIR)/linker.ld

TARGET = $(ARCHDIR)/build/fausto

.PHONY: clean

$(TARGET): $(arch_objs) $(asm_objs) $(kernel_objs) $(linker)
	$(CC) -T $(linker) -o $@ $(CCLINKFLAGS) $(arch_objs) $(asm_objs) $(kernel_objs) $(LFLAGS) 

$(arch_objs): $(arch_srcs)
	$(CC) -c $< -o $@ $(CCFLAGS)
 
$(kernel_objs): $(kernel_srcs)
	$(CC) -c $< -o $@ $(CCFLAGS)

$(asm_objs): $(asm_srcs)
	mkdir -p $(ARCHDIR)/build/
	$(AS) $< -o $@

clean:
	rm -f $(ARCHDIR)/build/*

