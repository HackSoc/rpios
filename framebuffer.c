#include <stddef.h>
#include "framebuffer.h"
#include "mailbox.h"

static volatile uint32_t *fb;
extern uint8_t font[];
//Workaround for a horrible gcc bug
static struct fbcfg fbcfg = {0};

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
