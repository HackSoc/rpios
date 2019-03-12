#include "gpio.h"

#include "main.h"

void gpio_set_alt_function(int gpio, int alt)
{
	int off;
	volatile uint32_t *ptr = GPFSEL0;
	if(gpio <= 9) // 0-9
	{
		off = gpio * 3;
	}
	else if(gpio <= 19) // 10-19
	{
		off = (gpio-10) * 3;
		ptr = GPFSEL1;
	}
	else if(gpio <= 29) // 20-29
	{
		off = (gpio-20) * 3;
		ptr = GPFSEL2;
	}
	else if(gpio <= 39) // 30-39
	{
		off = (gpio-30) * 3;
		ptr = GPFSEL3;
	}
	else if(gpio <= 49) // 40-49
	{
		off = (gpio-40) * 3;
		ptr = GPFSEL4;
	}
	else if(gpio <= 53) // 50-53
	{
		off = (gpio-50) * 3;
		ptr = GPFSEL5;
	}
	uint32_t a = *ptr;
	a &= ~(7<<off);
	a |= alt << off;
	*ptr = a;
}

void gpio_set_pull(int gpio, int pull)
{
	*GPPUD = pull;
    delay(150);
    *GPPUDCLK0 = (1<<gpio);
    delay(150);
    *GPPUDCLK0=0;
}
