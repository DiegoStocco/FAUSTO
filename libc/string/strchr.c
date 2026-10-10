#include <string.h>


char* strchr(const char* s, int c){
	size_t i = 0;
	while (s[i]) {
		if (s[i] == c) return (char*)&s[i];
		i++;
	}
	return NULL;
}
