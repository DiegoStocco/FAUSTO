#include <stdio.h>

#ifdef __is_libk
#include <kernel/tty.h>
#endif

int putchar(int c) {
    char ch = c;

#ifdef __is_libk
    terminal_putchar(ch);

#else // __is_libc
    // TODO: syscall, file descriptor and stuff
#endif

    return ch; // Not failing for now
}
