#include <stdint.h>

struct fbcfg {
    uint32_t width;
    uint32_t height;
    uint32_t vwidth;
    uint32_t vheight;
    uint32_t pitch;
    uint32_t depth;
    uint32_t x;
    uint32_t y;
    uint32_t addr;
    uint32_t size;
} __attribute__ ((aligned (16)));

volatile uint32_t *fb_init(uint32_t width, uint32_t height);
void setPixel(uint32_t, uint32_t, uint32_t);
void draw_char(uint8_t, uint32_t, uint32_t);
uint64_t blit_image(uint32_t x, uint32_t y, char *xpm[]);
