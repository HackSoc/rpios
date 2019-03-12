#include <stdint.h>
#include <stdbool.h>
#include "rng.h"

struct Rng {
    volatile uint32_t ctrl;
    volatile uint32_t status;
    volatile uint32_t data;
};

static struct Rng *rng;
static bool initialised = false;

void rng_init() {
    rng = (struct Rng *)RNG_BASE;
    initialised = true;
    rng->ctrl = 1;
}

uint32_t rng_read() {
    uint32_t status;
    do {
        status = rng->status >> 24;
    } while (status == 0);
    return rng->data;
}
