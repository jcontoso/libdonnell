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
    DonnellPixel *result;
    double aa;
    double ba;

    if ((!a) || (!b)) {
        return NULL;
    }

    aa = a->alpha / 255.0;
    ba = b->alpha / 255.0;
    result = Donnell_Pixel_Create();
    result->red = (DonnellUInt8)(aa * a->red + ba * (1 - aa) * b->red);
    result->green = (DonnellUInt8)(aa * a->green + ba * (1 - aa) * b->green);
    result->blue = (DonnellUInt8)(aa * a->blue + ba * (1 - aa) * b->blue);
    result->alpha = (DonnellUInt8)(255 * (aa + ba * (1 - aa)));

    return result;
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
