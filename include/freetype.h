#include "bidi.h"
#include "donnell.h"

/* Internal functions for FreeType text rendering */

#ifndef DONNELL_FREETYPE
#define DONNELL_FREETYPE

void FreeType_Init(void);
void FreeType_Cleanup(void);
int FreeType_MeasureAndRender(DonnellImageBuffer* buffer, DonnellSize* size, DonnellPixel* color, FriBidiString* string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellFont req_font);

#endif
