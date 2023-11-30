#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "bidi.h"
#include "donnell.h"
#include "freetype.h"
#include "symvis.h"
#include "textutils.h"

DONNELL_EXPORT void Donnell_GraphicsPrimitives_DrawTextLine(DonnellImageBuffer *buffer, DonnellPixel *color, char *utf8string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellFont req_font) {
    FriBidiString *string;

    if ((!buffer) || (!color) || (!utf8string) || (x < 0) || (y < 0) || (pixel_size < 0)) {
        return;
    }

    string = FriBidiString_ConvertFromUTF8(utf8string);
    FriBidiString_Handle(string);
    FreeType_MeasureAndRender(buffer, NULL, color, string, x, y, pixel_size, req_font);
    FriBidiString_Free(string);
}

DONNELL_EXPORT void Donnell_GraphicsPrimitives_GetTextLineExtents(DonnellSize *size, char *utf8string, unsigned int pixel_size, DonnellFont req_font) {
    FriBidiString *string;

    if ((!size) || (!utf8string) || (pixel_size < 0)) {
        return;
    }

    string = FriBidiString_ConvertFromUTF8(utf8string);
    FriBidiString_Handle(string);
    FreeType_MeasureAndRender(NULL, size, NULL, string, 0, 0, pixel_size, req_font);
    FriBidiString_Free(string);
}

DONNELL_EXPORT void Donnell_GraphicsPrimitives_DrawText(DonnellImageBuffer *buffer, DonnellPixel *color, char *utf8string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellFont req_font) {
}
