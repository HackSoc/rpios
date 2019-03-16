#include "mem.h"
#include <stdint.h>

void *memset(void *s, int c, size_t n)
{
    for(uint8_t *p = s; p < (uint8_t *) s + n; p++)
    {
        *p = (uint8_t) c;
    }
    return s;
}
