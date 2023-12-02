#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
    FreeType_MeasureAndRender(buffer, NULL, color, string, x, y, pixel_size, req_font, false);
    FriBidiString_Free(string);
}

DONNELL_EXPORT void Donnell_GraphicsPrimitives_GetTextLineExtents(DonnellSize *size, char *utf8string, unsigned int pixel_size, DonnellFont req_font) {
    FriBidiString *string;

    if ((!size) || (!utf8string) || (pixel_size < 0)) {
        return;
    }

    string = FriBidiString_ConvertFromUTF8(utf8string);
    FriBidiString_Handle(string);
    FreeType_MeasureAndRender(NULL, size, NULL, string, 0, 0, pixel_size, req_font, false);
    FriBidiString_Free(string);
}

DONNELL_EXPORT void Donnell_GraphicsPrimitives_DrawText(DonnellImageBuffer *buffer, DonnellPixel *color, char *utf8string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellFont req_font) {
	Paragraphs* paragraphs;
	FriBidiParagraphs* fribidi_paragraphs;
	unsigned int i;

    if ((!buffer) || (!color) || (!utf8string) || (x < 0) || (y < 0) || (pixel_size < 0)) {
        return;
    }
    	
	paragraphs = TextUtils_Paragraphs_Create(utf8string);
	fribidi_paragraphs = FriBidiParagraphs_ConvertFromParagraphs(paragraphs);
	
	for (i = 0; i < fribidi_paragraphs->count; i++) {
		FreeType_MeasureAndRender(buffer, NULL, color, fribidi_paragraphs->str[i], x, y, pixel_size, req_font, false);
	
		y += FreeType_MeasureAndRender(NULL, NULL, NULL, NULL, 0, 0, pixel_size, req_font, false);
		printf("%d\n", y);
	}

	FriBidiParagraphs_Free(fribidi_paragraphs);
	TextUtils_Paragraphs_Free(paragraphs);
}
