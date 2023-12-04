#include "bidi.h"
#include "donnell.h"

/* Internal functions for text rendering */

#ifndef DONNELL_TEXTRENDERER
#define DONNELL_TEXTRENDERER

typedef int (*TextRendererFunction)(DonnellImageBuffer *, DonnellSize *, DonnellPixel *, FriBidiString *, unsigned int, unsigned int, unsigned int, DonnellBool, DonnellFontOptions);

int TextRenderer_MeasureAndRender(DonnellImageBuffer *buffer, DonnellSize *size, DonnellPixel *color, FriBidiString *string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellBool return_max_asc, DonnellFontOptions font_options);
void TextRenderer_Init(void);
void TextRenderer_Cleanup(void);

#endif
