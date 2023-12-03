#include <stdio.h>
#include <stdlib.h>

#include "donnell.h"
#include "pixel.h"
#include "symvis.h"

DonnellUInt8 Pixel_Blend(DonnellUInt8 alpha, DonnellUInt8 value1, DonnellUInt8 value2) {
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

DONNELL_EXPORT DonnellPixel *Donnell_Pixel_CreateEasy(DonnellUInt8 red, DonnellUInt8 green, DonnellUInt8 blue, DonnellUInt8 alpha) {
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
