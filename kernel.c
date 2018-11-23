#include <stdint.h>
#include "mem.h"
#include "uart.h"
#include "framebuffer.h"
extern uint8_t font[];

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
        chr = ' ';
        x-=8;
    }
    for (uint32_t py = 0; py < 16; py++){
        uint8_t row = *(font+chr*16+py);
        for (uint32_t px = 0; px < 8; px++)
        {
            if((row & (1 << px)) != 0) {
                setPixel(x + px, y + py, 0xFFFFFFFF);
            }
        }
    }
    if(chr == '\b') {
        x-=8;
    } else if(chr == '\n'){
        x=0;
        y+=16;
    } else {
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

void kmain()
{
    uart_init();
    if(!fb_init(1920, 1080))
        halt();
    uint8_t line[241] = {0};
    while(1){
        puts(gets(line, 241));
        memset(line, 0, 241);
    }
    halt();
}
