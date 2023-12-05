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

DONNELL_EXPORT DonnellPixel *Donnell_Pixel_Blend(DonnellPixel *a, DonnellPixel* b) {
	unsigned int cr;
	unsigned int cg;
	unsigned int cb;
	unsigned int ca;

	cr = (a->red * a->alpha + b->red * (255 - a->alpha) / 255 * b->alpha) / 255;
	cg = (a->green * a->alpha + b->green * (255 - a->alpha) / 255 * b->alpha) / 255;
	cb = (a->blue * a->alpha + b->blue * (255 - a->alpha) / 255 * b->alpha) / 255;
	ca = a->alpha + b->alpha * (255 - a->alpha) / 255;
		
	printf("%d %d %d %d\n", cr, cg, cb,  (a->red * a->alpha + b->red * b->alpha * (255 - a->alpha) / 255)/ca);
	//printf("%d %d %d %d\n", cr, cg, cb, ca);


	return Donnell_Pixel_CreateEasy(cr, cg, cb, ca);
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
