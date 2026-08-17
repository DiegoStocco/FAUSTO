AS = i386-elf-as
LD = i386-elf-ld
OBJCOPY = i386-elf-objcopy
SRCS = $(wildcard src/*.S)
OBJS := $(patsubst src/%.S, build/%.o, $(SRCS))
TARGET = build/fausto_bootloader.bin
TARGET_ELF = build/fausto_bootloader.elf
linker = src/linker.ld

.PHONY: clean 

all: $(TARGET)
	
$(OBJS): $(SRCS)
	mkdir -p build/ 
	$(AS) --32 -o $@ $<

$(TARGET_ELF): $(OBJS) $(linker)
	$(LD) -m elf_i386 -T $(linker) -o $@ $<

$(TARGET): $(TARGET_ELF)
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf build/


