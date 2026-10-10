#include <stddef.h>
#include <string.h>


char* strcat(char* dst, const char* src) {
	size_t dst_len = strlen(dst);

	size_t i = 0;
	while (src[i] != 0) {
		dst[dst_len+i] = src[i];
		i++;
	}
	return dst;
}
