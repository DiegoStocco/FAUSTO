#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// base must be between 2 and 16
static int print_uint(uint64_t n, int base, bool capitalize) {
    int cnt = 0;
    char first_letter;
    int digit;
    uint64_t x = 1;

    if(capitalize) first_letter = 'A';
    else first_letter = 'a';

    while(x*base < n) x *= base;

    do {
        digit = n / x;
        n -= digit * x;
        x /= base;

        if(0 <= digit && digit <= 9)
            putchar(digit + '0');
        else
            putchar(digit - 10 + first_letter);
        cnt++;
    } while(n);

    // do {
    //     digit = n % base;
    //     n /= base;
    //
    //     if(0 <= digit && digit <= 9)
    //         putchar(digit + '0');
    //     else
    //         putchar(digit - 10 + first_letter);
    //     cnt++;
    // } while(n);
    return cnt;
}

static int print_int(int64_t n, int base, bool capitalize) {
    int cnt = 0;
    if(n < 0) {
        putchar('-');
        cnt++;
        n *= -1;
    }
    cnt += print_uint((unsigned int)n, base, capitalize);
    return cnt;
}

// minimal printf implementation. Does not support any modifier or floating point numbers
int printf(const char * restrict fmt, ...) {
    int count = 0;
    va_list args;
    const char *str;

    va_start(args, fmt);

    while(*fmt) {
        if(*fmt == '%') {
            fmt++;
            switch(*fmt) {
            case '%':
                putchar('%');
                count++;
                break;
            case 'c':
                putchar(va_arg(args, int));
                count++;
                break;
            case 's':
                str = va_arg(args, char*);
                while(*str) {
                    putchar(*str);
                    str++;
                    count++;
                }
                break;
            case 'd':
            case 'i':
                count += print_int(va_arg(args, int), 10, false);
                break;
            case 'b':
                count += print_uint(va_arg(args, unsigned int), 2, false);
                break;
            case 'o':
                count += print_uint(va_arg(args, unsigned int), 8, false);
                break;
            case 'x':
                count += print_uint(va_arg(args, unsigned int), 16, false);
                break;
            case 'X':
                count += print_uint(va_arg(args, unsigned int), 16, true);
                break;
            case 'u':
                count += print_uint(va_arg(args, unsigned int), 10, false);
                break;
            case 'p':
                putchar('0');
                putchar('x');
                count += 2 + print_uint((uint64_t)va_arg(args, void*), 16, true);
                break;
            default:
                goto fail;
            }
        }
        else {
            putchar(*fmt);
            count++;
        }
        fmt++;
    }

    va_end(args);
    return count;

fail:
    va_end(args);
    return -1;
}
