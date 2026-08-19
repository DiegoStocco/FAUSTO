ARCH = i386
ARCHDIR = arch/$(ARCH)
LIBS = ../libc/libk.a
LIBS_INCLUDEDIR = ../libc/include
CC = $(ARCH)-elf-gcc
CCFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -m32 -fno-pie -Iinclude -I$(LIBS_INCLUDEDIR)
CCLINKFLAGS = -ffreestanding -O2 -nostdlib
LFLAGS = -lgcc
AS = $(ARCH)-elf-as

arch_srcs = $(wildcard $(ARCHDIR)/*.c)
arch_objs = $(arch_srcs:.c=.c.o)

kernel_srcs = $(wildcard kernel/*.c)
kernel_objs = $(kernel_srcs:.c=.c.o)

asm_srcs = $(wildcard $(ARCHDIR)/*.S)
asm_objs = $(asm_srcs:.S=.S.o)

linker = $(ARCHDIR)/linker.ld

TARGET = fausto

.PHONY: clean

$(TARGET): $(arch_objs) $(asm_objs) $(kernel_objs) $(linker) $(LIBS)
	$(CC) -T $(linker) -o $@ $(CCLINKFLAGS) $(asm_objs) $(kernel_objs) $(arch_objs) $(LIBS) $(LFLAGS) 

%.c.o: %.c
	$(CC) -c $< -o $@ $(CCFLAGS)

%.S.o: %.S
	mkdir -p $(ARCHDIR)/build/
	$(AS) $< -o $@

clean:
	rm -f $(arch_objs)
	rm -f $(kernel_objs)
	rm -f $(asm_objs)
	rm -f $(TARGET)

