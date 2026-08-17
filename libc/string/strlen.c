#include <stddef.h>
#include <string.h>

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

char* strcpy(char* dst, const char* src) {
	size_t i = 0;
	do { dst[i] = src[i]; } while(src[i++]);
	return dst;
}

int strcmp(const char* s1, const char* s2) {
	int s1_len = strlen(s1);
	int s2_len = strlen(s2);
	if (s1_len != s2_len) return 1;
	
	for (int i = 0; i < s1_len; i++) {
		if (s1[i] != s2[i]) return 1;
	}
	return 0;
}


