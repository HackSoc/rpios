#include "hacksoc_logo.h"

#include "../framebuffer.h"
#include "../main.h"
#include "../mem.h"
#include "../mailbox.h"
#include "../rng.h"
#include <stdbool.h>

void hacksoc_logo_animation() {
    int32_t pos_x = DISPLAY_WIDTH / 2, pos_y = DISPLAY_HEIGHT / 2,
        speed_x = 0, speed_y = 0;

    // Speed is chosen randomly (rng_init must be called)
    uint32_t speed_range = 8;
    while (speed_x == 0) speed_x = rng_read() % speed_range - speed_range / 2;
    while (speed_y == 0) speed_y = rng_read() % speed_range - speed_range / 2;
        
    while (1) {
        // Blit the image, then paint black over it
        blit_image(pos_x, pos_y, hacksoc_logo_xpm);
        delay(1000000);
        for (uint32_t x = pos_x; x < pos_x + HACKSOC_LOGO_WIDTH; x++) {
            for (uint32_t y = pos_y; y < pos_y + HACKSOC_LOGO_HEIGHT; y++) {
                setPixel(x, y, 0);
            }
        }

        pos_x += speed_x;
        pos_y += speed_y;

        // When we hit the edge, invert speed and move slightly out of the 
        // colision range
        if (pos_x + HACKSOC_LOGO_WIDTH >= DISPLAY_WIDTH) {
            speed_x = -speed_x;
            pos_x = DISPLAY_WIDTH - 1 - HACKSOC_LOGO_WIDTH;
        } else if (pos_x <= 0) {
            speed_x = -speed_x;
            pos_x = 1;
        } else if (pos_y + HACKSOC_LOGO_HEIGHT >= DISPLAY_HEIGHT) {
            speed_y = -speed_y;
            pos_y = DISPLAY_HEIGHT - 1 - HACKSOC_LOGO_HEIGHT;
        } else if (pos_y <= 0) {
            speed_y = -speed_y;
            pos_y = 1;
        }
    }
}