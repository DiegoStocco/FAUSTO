#include <stddef.h>
#include <string.h>


char* strcat(char* dst, const char* src) {
	size_t dst_len = strlen(dst);

	int i = 0;
	while (src[i] != 0) {
		dst[dst_len+i] = src[i];
	}
	return dst;
}
