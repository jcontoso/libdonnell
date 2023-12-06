#include <ft2build.h>
#include FT_FREETYPE_H

#include "bidi.h"
#include "donnell.h"

/* Internal functions for FreeType text rendering */

#ifndef DONNELL_FREETYPE
#define DONNELL_FREETYPE

typedef void (*FreeTypeCopyToBufferFunction)(DonnellImageBuffer *, DonnellPixel *, FT_Bitmap *, unsigned int, unsigned int);

void FreeType_Init(void);
void FreeType_Cleanup(void);
FT_Library *FreeType_GetLibrary(void);
FT_Int32 FreeType_GetFlags(void);
FreeTypeCopyToBufferFunction *FreeType_GetBufferCopyFunction(void);
int FreeType_MeasureAndRender(DonnellImageBuffer *buffer, DonnellSize *size, DonnellPixel *color, FriBidiString *string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellBool return_max_asc, DonnellFontOptions font_options);
#endif
