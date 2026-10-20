ARCH = i386
ARCHDIR = arch/$(ARCH)
CC = $(ARCH)-elf-gcc
CCFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Iinclude
CCFLAGS_LIBC = $(CCFLAGS) -D__is_libc
CCFLAGS_LIBK = $(CCFLAGS) -D__is_libk -I../kernel/include
AR = $(ARCH)-elf-ar

srcs = $(wildcard */*.c)
objs_libc = $(srcs:.c=.c.libc.o)
objs_libk = $(srcs:.c=.c.libk.o)

# TODO: libc
TARGET_LIBK = libk.a
# TARGET_LIBC = libc.a

.PHONY: clean

all: $(TARGET_LIBK)

$(TARGET_LIBK): $(objs_libk)
	@echo " AR  $@"
	@$(AR) rcs $@ $(objs_libk)

%.c.libk.o: %.c
	@echo " CC  $@"
	@$(CC) -c $< -o $@ $(CCFLAGS_LIBK)

%.c.libc.o: %.c
	@echo " CC  $@"
	@$(CC) -c $< -o $@ $(CCFLAGS_LIBC)
 
clean:
	@echo " RM  $(objs_libc)"
	@rm -f $(objs_libc) 
	@echo " RM  $(objs_libk)"
	@rm -f $(objs_libk) 
	@echo " RM  $(TARGET_LIBK)"
	@rm -f $(TARGET_LIBK) 
