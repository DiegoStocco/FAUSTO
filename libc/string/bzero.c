#include <string.h>
#include <stddef.h>

void bzero(void *a, size_t n) {
	for (int i = 0; i < n; i++) {
		memcpy(a, "\0", 1);
		a++;
	}
}
