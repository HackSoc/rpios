#pragma once
#include <stdint.h>

#define RNG_BASE ((volatile uint32_t *) 0x3F104000)

void rng_init();
uint32_t rng_read();
