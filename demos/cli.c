#include <stddef.h>
#include <stdint.h>
#include "../framebuffer.h"
#include "../mem.h"
#include "../qrcode.h"
#include "../rng.h"
#include "../uart.h"
#include "../hacksoc.xpm"

#define MIN(a, b) ({ __typeof__(a) _min_a = (a); __typeof__(b) _min_b = (b); _min_a > _min_b ? _min_b : _min_a; })
#define MAX(a, b) ({ __typeof__(a) _max_a = (a); __typeof__(b) _max_b = (b); _max_a > _max_b ? _max_a : _max_b; })

static uint32_t x = 0;
static uint32_t y = 0;

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

void put(uint8_t *str) {
    uint8_t chr;
    while (chr = *str++) {
        tty_write(chr);
    }
}

void putn(uint8_t *str, size_t n) {
    for (uint8_t chr, i = 0; (chr = *str) && i < n; str++, i++) {
        tty_write(chr);
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
        while (!uart_read_ready()) {
            draw_char('_', x, y);
        }
        draw_char(' ', x, y);
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

uint8_t *strchrnul(const uint8_t *s, uint8_t c) {
    size_t i;
    for (i = 0; s[i] != c && s[i] != '\0'; i++);
    return (uint8_t *)s + i;
}

uint8_t strncmp(const uint8_t *s1, const uint8_t *s2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (s1[i] < s2[i]) {
            return -1;
        }
        if (s1[i] > s2[i]) {
            return 1;
        }
    }
    return 0;
}

void cli() {
    uint8_t line[241] = {0};
    rng_init();
    while (1) {
        tty_write('>');
        tty_write(' ');
        gets(line, 241);
        uint8_t *sep = strchrnul(line, ' ');
        if (line[0] == '\0') {
        } else if (strncmp(line, (uint8_t*)"echo", MAX(sep - line, 4)) == 0) {
            puts(sep + 1); // buffer overreads? shusb nou
        } else if (strncmp(line, (uint8_t*)"help", MAX(sep - line, 4)) == 0) {
            puts((uint8_t*)"Available commands: echo, help, random, qr, image");
        } else if (strncmp(line, (uint8_t*)"random", MAX(sep - line, 6)) == 0) {
            for (int dx = 0; dx < 128; dx++)
                for (int dy = 0; dy < 128; dy++)
                    setPixel(x + dx, y + dy, rng_read());
            y += 128;
        } else if (strncmp(line, (uint8_t*)"qr", MAX(sep - line, 2)) == 0) {
            // XXX: encoding a too-long string causes Bad Things™ to happen
            QRCode qrcode;
            uint8_t qrbytes[qrcode_getBufferSize(3)]; // with version 3, we can store approximately 50 8-bit characters (more in the reduced character sets)
            qrcode_initText(&qrcode, qrbytes, 3, ECC_LOW, (const char *)sep + 1);
            int scale = 6;
            for (int qrx = -4; qrx < (qrcode.size + 4); qrx++) {
                for (int qry = -4; qry < (qrcode.size + 4); qry++)
                    if ((qrx < 0) || (qrx >= qrcode.size) || (qry < 0) || (qry >= qrcode.size) || (!qrcode_getModule(&qrcode, qrx, qry))) {
                        uart_write('#');
                        uart_write('#');
                        for (int dx = 0; dx < scale; dx++)
                            for (int dy = 0; dy < scale; dy++)
                                setPixel(x + (qrx + 4) * scale + dx, y + (qry + 4) * scale + dy, 0xffffffff);
                    } else {
                        uart_write(' ');
                        uart_write(' ');
                        for (int dx = 0; dx < scale; dx++)
                            for (int dy = 0; dy < scale; dy++)
                                setPixel(x + (qrx + 4) * scale + dx, y + (qry + 4) * scale + dy, 0);
                    }
                uart_write('\n');
            }
            y += (qrcode.size + 8) * scale;
        } else if (strncmp(line, (uint8_t*)"image", MAX(sep - line, 5)) == 0) {
            y += blit_image(x, y, hacksoc_xpm) & 0xffff;
        } else {
            putn(line, sep - line);
            puts((uint8_t*)": command not found");
        }
        memset(line, 0, 241);
    }
}
