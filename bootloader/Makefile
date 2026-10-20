AS = i386-elf-as
LD = i386-elf-ld
OBJCOPY = i386-elf-objcopy
SRCS = $(wildcard src/*.S)
OBJS := $(SRCS:.S=.S.o)
TARGET = build/fausto_bootloader.bin
TARGET_ELF = build/fausto_bootloader.elf
linker = src/linker.ld

.PHONY: clean 

all: $(TARGET)
	
%.S.o: %.S
	@echo " MKDIR  build"
	@mkdir -p build/ 
	@echo " AS  $@"
	@$(AS) --32 -o $@ $<

$(TARGET_ELF): $(OBJS) $(linker)
	@echo " LD  $@"
	@$(LD) -m elf_i386 -T $(linker) -o $@ $(OBJS)

$(TARGET): $(TARGET_ELF)
	@echo " OBJCOPY  $(TARGET_ELF)"
	@$(OBJCOPY) -O binary $(TARGET_ELF) $@

clean:
	@echo " RM $(OBJS)"
	@rm -f $(OBJS)
	@echo " RM $(TARGET)"
	@rm -f $(TARGET)
	@echo " RM $(TARGET_ELF)"
	@rm -f $(TARGET_ELF)
