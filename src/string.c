#include <string.h>

void *memcpy(void *dest, const void *src, size_t n) {
    for (size_t i = 0; i < n; i++) {
        *(char *)(dest + i) = *(char *)(src + i);
    }
    return dest;
}

size_t strlen(const char *s) {
    int i = 0;
    while (s[i] != '\0') i++;
    return i;
}
