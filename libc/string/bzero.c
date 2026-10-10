#include <string.h>
#include <stddef.h>

void bzero(void *a, size_t n) {
	for (size_t i = 0; i < n; i++) {
		*(char*)a = 0;
		a++;
	}
}
