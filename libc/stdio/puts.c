#include <stdio.h>

int puts(const char *str) {
    while(*str) {
        putchar(*str);
        str++;
    }
    putchar('\n');
    return 1; // SUCCESS
}
