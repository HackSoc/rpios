#include <stdbool.h>
#include <stddef.h>
#include "framebuffer.h"
#include "mailbox.h"

static volatile uint32_t *fb;
extern uint8_t font[];
//Workaround for a horrible gcc bug
static struct fbcfg fbcfg = {0};

struct Coord cursor = {0};

volatile uint32_t *fb_init(uint32_t width, uint32_t height)
{
    fbcfg.width = width;
    fbcfg.height = height;
    fbcfg.vwidth = width;
    fbcfg.vheight = height;
    fbcfg.depth = 32;
    mbox_write(((uint32_t) &fbcfg)+0xC0000000, 1);
    if(!mbox_read(1)){
        fb = (volatile uint32_t *) (fbcfg.addr-0xC0000000);
        return fb;
    }
    return NULL;
}

void setPixel(uint32_t x, uint32_t y, uint32_t colour)
{
    *(fb+x+fbcfg.width*y) = colour;
}

uint32_t getPixel(uint32_t x, uint32_t y) {
    return *(fb+x+fbcfg.width*y);
}

void draw_char(uint8_t chr, uint32_t x, uint32_t y)
{
    for (uint32_t py = 0; py < 16; py++){
        uint8_t row = *(font+chr*16+py);
        for (uint32_t px = 0; px < 8; px++)
        {
            setPixel(x + px, y + py, row & (1 << px) ? 0xFFFFFFFF : 0x0);
        }
    }
}

void draw_string(char *s) {
    for (uint32_t i = 0; s[i] != '\0'; i++) {
        switch (s[i]) {
            case '\n':
                cursor.x = 0;
                cursor.y += 16;
                break;
            case '\t':
                cursor.x += 8 * 8;
                break;
            case '\b':
                cursor.x -= 8;
                draw_char(' ', cursor.x, cursor.y);
                break;
            default:
                draw_char(s[i], cursor.x, cursor.y);
                cursor.x += 8;
                break;
        }
        if (cursor.x >= 1920) {
            // Wrap to next line
            cursor.x = 0;
            cursor.y += 16;
        }
        if (cursor.y >= 1080) {
            // TODO: DMA
            // Scroll screen
            for (uint32_t y = 16; y < 1080; y++) {
                for (uint32_t x = 0; x < 1920; x++) {
                    setPixel(x, y - 16, getPixel(x, y));
                }
            }
            // Clear bottom row
            for (uint32_t y = 1080 - 16; y < 1080; y++) {
                for (uint32_t x = 0; x < 1920; x++) {
                    setPixel(x, y, 0);
                }
            }
            // Put the cursor back on the screen
            cursor.y -= 16;
        }
    }
}

uint64_t blit_image(uint32_t x, uint32_t y, char *xpm[]) {
    int i = 0;
    int width = 0;
    for (; xpm[0][i] != ' '; i++) {
        width = width * 10 + xpm[0][i] - '0';
    }
    i++;
    int height = 0;
    for (; xpm[0][i] != ' '; i++) {
        height = height * 10 + xpm[0][i] - '0';
    }
    i++;
    int colors = 0;
    for (; xpm[0][i] != ' '; i++) {
        colors = colors * 10 + xpm[0][i] - '0';

    }
    i++;
    int chars = 0;
    for (; xpm[0][i] != '\0'; i++) {
        chars = chars * 10 + xpm[0][i] - '0';
    }
    // TODO: support chars > 1

    struct Color {
        char ch;
        uint32_t co;
        bool transparent;
    };
    struct Color colortable[colors]; // VLAs are great, what are you talking about

    for (int i = 0; i < colors; i++) {
        if ((xpm[i + 1][4] == 'N') || (xpm[i + 1][4] == 'n')) {
            // transparent pixel, rest of the word is "none"
            colortable[i] = (struct Color){.ch = xpm[i + 1][0], .co = 0, .transparent = true};
        } else {
            uint32_t color = 0;
            for (int j = 0; j < 6; j++) {
                char c = xpm[i + 1][j + 5];
                color = color * 16 + (('A' <= c) && (c <= 'F') ? c - ('A' - 10) : c - '0');
            }
            colortable[i] = (struct Color){.ch = xpm[i + 1][0], .co = color, .transparent = false};
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            uint8_t k;
            for (k = 0; k < colors; k++) {
                if (xpm[i + colors + 1][j] == colortable[k].ch) {
                    break;
                }
            }
            if (!colortable[k].transparent) {
                setPixel(j + x, i + y, colortable[k].co);
            }
        }
    }

    return ((uint64_t)height << 32) | width;
}
