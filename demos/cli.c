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

uint8_t *gets(uint8_t *str, size_t len)
{
    uint8_t *current_chr = str;
    while(1){
        uint8_t input_chr = uart_read();
        switch(input_chr){
            case '\r':
            case '\n':
                draw_string("\n");
                uart_write('\n');
                return str;
            case '\b':
            case 0x7F:
                if(current_chr > str) {
                    draw_string("\b");
                    uart_write('\b');
                    uart_write(' ');
                    uart_write('\b');
                    current_chr--;
                    *current_chr = '\0';
                }
                break;
            default:
                if(current_chr >= str+len-1)
                    continue;
                char buf[2] = {0};
                buf[0] = input_chr;
                draw_string(buf);
                uart_write(input_chr);
                *current_chr = input_chr;
                current_chr++;
                break;
        }
    }
}

void cli() {
    uint8_t line[241] = {0};
    rng_init();
    while (1) {
        draw_string("> ");
        uart_write('>');
        uart_write(' ');
        gets(line, 241);
        uint8_t *sep = strchrnul(line, ' ');
        if (line[0] == '\0') {
        } else if (strncmp(line, (uint8_t*)"echo", MAX(sep - line, 4)) == 0) {
            draw_string(sep + 1); // buffer overreads? shusb nou
            draw_string("\n");
        } else if (strncmp(line, (uint8_t*)"help", MAX(sep - line, 4)) == 0) {
            draw_string("Available commands: echo, help, random, qr, image\n");
        } else if (strncmp(line, (uint8_t*)"random", MAX(sep - line, 6)) == 0) {
            reserve_space(128);
            for (int dx = 0; dx < 128; dx++)
                for (int dy = 0; dy < 128; dy++)
                    setPixel(cursor.x + dx, cursor.y + dy, rng_read());
            cursor.y += 128;
        } else if (strncmp(line, (uint8_t*)"qr", MAX(sep - line, 2)) == 0) {
            // XXX: encoding a too-long string causes Bad Things™ to happen
            QRCode qrcode;
            uint8_t qrbytes[qrcode_getBufferSize(3)]; // with version 3, we can store approximately 50 8-bit characters (more in the reduced character sets)
            qrcode_initText(&qrcode, qrbytes, 3, ECC_LOW, (const char *)sep + 1);
            int scale = 6;
            reserve_space((qrcode.size + 8) * scale);
            for (int qrx = -4; qrx < (qrcode.size + 4); qrx++) {
                for (int qry = -4; qry < (qrcode.size + 4); qry++)
                    if ((qrx < 0) || (qrx >= qrcode.size) || (qry < 0) || (qry >= qrcode.size) || (!qrcode_getModule(&qrcode, qrx, qry))) {
                        uart_write('#');
                        uart_write('#');
                        for (int dx = 0; dx < scale; dx++)
                            for (int dy = 0; dy < scale; dy++)
                                setPixel(cursor.x + (qrx + 4) * scale + dx, cursor.y + (qry + 4) * scale + dy, 0xffffffff);
                    } else {
                        uart_write(' ');
                        uart_write(' ');
                        for (int dx = 0; dx < scale; dx++)
                            for (int dy = 0; dy < scale; dy++)
                                setPixel(cursor.x + (qrx + 4) * scale + dx, cursor.y + (qry + 4) * scale + dy, 0);
                    }
                uart_write('\n');
            }
            cursor.y += (qrcode.size + 8) * scale;
        } else if (strncmp(line, (uint8_t*)"image", MAX(sep - line, 5)) == 0) {
            reserve_space(128);
            cursor.y += blit_image(cursor.x, cursor.y, hacksoc_xpm) & 0xffff;
        } else {
            line[sep - line] = '\0';
            draw_string(line);
            draw_string(": command not found\n");
        }
        memset(line, 0, 241);
    }
}
