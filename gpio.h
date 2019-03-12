#pragma once
#include <stdint.h>

#define GPIO_BASE ((volatile uint32_t * const) 0x3F200000)
#define GPFSEL0 (GPIO_BASE + 0)
#define GPFSEL1 (GPIO_BASE + 1)
#define GPFSEL2 (GPIO_BASE + 2)
#define GPFSEL3 (GPIO_BASE + 3)
#define GPFSEL4 (GPIO_BASE + 4)
#define GPFSEL5 (GPIO_BASE + 5)
// reserved
#define GPSET0 (GPIO_BASE + 7)
#define GPSET1 (GPIO_BASE + 8)
// reserved
#define GPCLR0 (GPIO_BASE + 10)
#define GPCLR1 (GPIO_BASE + 11)
// reserved
#define GPLEV0 (GPIO_BASE + 13)
#define GPLEV1 (GPIO_BASE + 14)
// reserved
#define GPEDS0 (GPIO_BASE + 16)
#define GPEDS1 (GPIO_BASE + 17)
// reserved
#define GPREN0 (GPIO_BASE + 19) // rising edge detect enable
#define GPREN1 (GPIO_BASE + 20)
// reserved
#define GPFEN0 (GPIO_BASE + 22) // falling edge detect enable
#define GPFEN1 (GPIO_BASE + 23)
// reserved
#define GPHEN0 (GPIO_BASE + 25) // high detect enable
#define GPHEN1 (GPIO_BASE + 26)
// reserved
#define GPLEN0 (GPIO_BASE + 28) // low detect enable
#define GPLEN1 (GPIO_BASE + 29)
// reserved
#define GPAREN0 (GPIO_BASE + 31) // GPIO Pin Async. Rising Edge Detect 0
#define GPAREN1 (GPIO_BASE + 32)
// reserved
#define GPAFEN0 (GPIO_BASE + 34) // GPIO Pin Async. Falling Edge Detect 0
#define GPAFEN1 (GPIO_BASE + 35)
// reserved
#define GPPUD (GPIO_BASE + 37) // GPIO Pin Pull-up/down Enable
#define GPPUDCLK0 (GPIO_BASE + 38) // GPIO Pin Pull-up/down Enable Clock 0 
#define GPPUDCLK1 (GPIO_BASE + 39) // GPIO Pin Pull-up/down Enable Clock 1

void gpio_set_alt_function(int gpio, int alt);
void gpio_set_pull(int gpio, int pull);