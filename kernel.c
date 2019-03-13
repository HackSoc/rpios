#include <stdint.h>
#include "mem.h"
#include "uart.h"
#include "framebuffer.h"

static uint32_t x = 0;
static uint32_t y = 0;

void halt()
{
    while(1){}
}

void tty_write(uint8_t chr)
{
    uart_write(chr);
    if(chr == '\b'){
        uart_write(' ');
        uart_write('\b');
        x-=8;
        draw_char(' ', x, y);
    } else if(chr == '\n'){
        x=0;
        y+=16;
    } else {
        draw_char(chr, x, y);
        x+=8;
    }
}

void puts(uint8_t *str)
{
    uint8_t chr;
    while(chr = *str++)
    {
        tty_write(chr);
    }
    tty_write('\n');
}

uint8_t *gets(uint8_t *str, size_t len)
{
    uint8_t *current_chr = str;
    while(1){
        uint8_t input_chr = uart_read();
        switch(input_chr){
            case '\r':
            case '\n':
                tty_write('\n');
                return str;
            case '\b':
            case 0x7F:
                if(current_chr > str) {
                    tty_write('\b');
                    current_chr--;
                    *current_chr = '\0';
                }
                break;
            default:
                if(current_chr >= str+len-1)
                    continue;
                tty_write(input_chr);
                *current_chr = input_chr;
                current_chr++;
                break;
        }
    }
}

#define ARM_AUX_CONTROL_SMP (1 << 6)
#define ARM_CONTROL_BRANCH_PREDICTION      (1 << 11)
#define ARM_CONTROL_L1_INSTRUCTION_CACHE   (1 << 12)

void kmain()
{
    uart_init();
    if(!fb_init(1920, 1080))
        halt();

    uint32_t nAuxControl;
    asm volatile ("mrc p15, 0, %0, c1, c0,  1" : "=r" (nAuxControl));
    nAuxControl |= ARM_AUX_CONTROL_SMP;
    asm volatile ("mcr p15, 0, %0, c1, c0,  1" : : "r" (nAuxControl));   // SMP bit must be set according to ARM TRM

    uint32_t nControl;
    asm volatile ("mrc p15, 0, %0, c1, c0,  0" : "=r" (nControl));
    nControl |= ARM_CONTROL_BRANCH_PREDICTION | ARM_CONTROL_L1_INSTRUCTION_CACHE;
    asm volatile ("mcr p15, 0, %0, c1, c0,  0" : : "r" (nControl) : "memory");

    go();
    hacksoc_logo_animation();
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
