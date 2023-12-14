#include <ft2build.h>
#include FT_FREETYPE_H

#include "bidi.h"
#include "donnell.h"

/* Internal functions for FreeType text rendering */

#ifndef DONNELL_FREETYPE
#define DONNELL_FREETYPE

void FreeType_Init(void);
void FreeType_Cleanup(void);
FT_Library *FreeType_GetLibrary(void);
FT_Int32 FreeType_GetFlags(void);
void FreeType_CopyToBuffer(DonnellImageBuffer *buffer, DonnellPixel *color, FT_Bitmap *bitmap, unsigned int a, unsigned int b, DonnellBool non_ideal_size, unsigned int wanted_size);
int FreeType_MeasureAndRender(DonnellImageBuffer *buffer, DonnellSize *size, DonnellPixel *color, FriBidiString *string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellBool return_max_asc, DonnellFontOptions font_options);
#endif
