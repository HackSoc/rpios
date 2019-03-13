#include <stdint.h>
#include "timer.h"

struct Timer {
    volatile uint32_t cs;
    volatile uint32_t clo;
    volatile uint32_t chi;
    volatile uint32_t c0;
    volatile uint32_t c1;
    volatile uint32_t c2;
    volatile uint32_t c3;
};

void delay_us(uint32_t us) {
    volatile struct Timer *timer = (struct Timer *)TIMER_BASE;
    uint32_t start_time = timer->clo;
    uint64_t time_passed;
    do {
        time_passed = timer->clo - start_time;
    } while (time_passed < us);
}

void delay_ms(uint32_t ms) {
    for (uint32_t n = 0; n < ms; n++) {
        delay_us(1000);
    }
}
