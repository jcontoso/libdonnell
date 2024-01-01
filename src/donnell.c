#include <stdio.h>

#include "donnell.h"

#include "bidi.h"
#include "guiprimitives.h"
#include "stockimages.h"
#include "symvis.h"
#include "textrenderer.h"
#include "textutils.h"

DONNELL_EXPORT void Donnell_Init(void) {
    TextRenderer_Init();
    StockImages_Init();
    GuiPrimitives_Init();
}

DONNELL_EXPORT void Donnell_Cleanup(void) {
    TextRenderer_Cleanup();
    StockImages_Cleanup();
    GuiPrimitives_Cleanup();
}

DONNELL_EXPORT unsigned int Donnell_GetVersionMajor(void) {
	return MAJOR_VERSION;
}

DONNELL_EXPORT unsigned int Donnell_GetVersionMinor(void) {
	return MINOR_VERSION;
}

DONNELL_EXPORT void Donnell_Rect_Center(DonnellRect *dest, DonnellRect *src1, DonnellRect *src2) {
    if ((!dest) || (!src1) || (!src2)) {
        return;
    }

    dest->w = src1->w;
    dest->h = src1->h;
    dest->x = src2->x + ((src1->w - src2->w) / 2);
    dest->y = src2->y + ((src1->h - src2->h) / 2);
}
