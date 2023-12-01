#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "donnell.h"
#include "pixel.h"
#include "symvis.h"

uint8_t Pixel_Blend(uint8_t alpha, uint8_t value1, uint8_t value2) {
    int ret;

    ret = (255 - alpha) * value1 + alpha * value2;
    return ret / 255;
}

DONNELL_EXPORT DonnellPixel *Donnell_Pixel_Create() {
    DonnellPixel *pixel;

    pixel = malloc(sizeof(DonnellPixel));
    if (!pixel) {
        return NULL;
    }

    return pixel;
}

DONNELL_EXPORT DonnellPixel *Donnell_Pixel_CreateEasy(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    DonnellPixel *pixel;

    pixel = malloc(sizeof(DonnellPixel));
    if (!pixel) {
        return NULL;
    }

    pixel->red = red;
    pixel->green = green;
    pixel->blue = blue;
    pixel->alpha = alpha;

    return pixel;
}
