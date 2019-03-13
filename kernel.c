#include <stdint.h>
#include "mem.h"
#include "uart.h"
#include "framebuffer.h"

#include "demos/cli.h"

static uint32_t x = 0;
static uint32_t y = 0;

void halt()
{
    while(1){}
}

void kmain()
{
    uart_init();
    if(!fb_init(1920, 1080))
        halt();
    cli();
    halt();
}

int raise(int tni) {
    return 0;
    // thanks
    // I
    // hate
    // it
    // .
}
