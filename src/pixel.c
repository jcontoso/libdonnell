#include <stdio.h>
#include <stdlib.h>

#include "donnell.h"
#include "symvis.h"

DONNELL_EXPORT DonnellPixel *Donnell_Pixel_Create() {
    DonnellPixel *pixel;

    pixel = malloc(sizeof(DonnellPixel));
    if (!pixel) {
        return NULL;
    }

    return pixel;
}

DONNELL_EXPORT DonnellPixel *Donnell_Pixel_Blend(DonnellPixel *a, DonnellPixel *b) {
    unsigned int result_r;
    unsigned int result_g;
    unsigned int result_b;
    unsigned int result_a;

    if ((!a) || (!b)) {
        return NULL;
    }

    result_r = ((255 - b->alpha) * a->red + b->alpha * b->red) / 255;
    result_g = ((255 - b->alpha) * a->green + b->alpha * b->green) / 255;
    result_b = ((255 - b->alpha) * a->blue + b->alpha * b->blue) / 255;
    result_a = a->alpha;

    return Donnell_Pixel_CreateEasy(result_r, result_g, result_b, result_a);
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

DONNELL_EXPORT void Donnell_Pixel_Free(DonnellPixel *pixel) {
    if (pixel) {
        free(pixel);
    }
}
