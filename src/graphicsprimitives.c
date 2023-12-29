#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "bidi.h"
#include "donnell.h"
#include "symvis.h"
#include "textrenderer.h"
#include "textutils.h"

DONNELL_EXPORT void Donnell_GraphicsPrimitives_DrawTextLine(DonnellImageBuffer *buffer, DonnellPixel *color, char *utf8string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellFontOptions font_options) {
    FriBidiString *string;

    if ((!buffer) || (!color) || (!utf8string) || (x < 0) || (y < 0) || (pixel_size < 0)) {
        return;
    }

    string = FriBidiString_ConvertFromUTF8(utf8string);
    FriBidiString_Handle(string);
    TextRenderer_MeasureAndRender(buffer, NULL, color, string, x * buffer->scale, y * buffer->scale, pixel_size * buffer->scale, DONNELL_FALSE, font_options);
    FriBidiString_Free(string);
}

DONNELL_EXPORT void Donnell_GraphicsPrimitives_MeasureTextLine(DonnellSize *size, char *utf8string, unsigned int pixel_size, DonnellFontOptions font_options, unsigned int pixel_scale) {
    FriBidiString *string;

    if ((!size) || (!utf8string) || (pixel_size < 0)) {
        return;
    }

    string = FriBidiString_ConvertFromUTF8(utf8string);
    FriBidiString_Handle(string);
    TextRenderer_MeasureAndRender(NULL, size, NULL, string, 0, 0, pixel_size * pixel_scale, DONNELL_FALSE, font_options);
    FriBidiString_Free(string);
}

unsigned int GetLongestWidth(FriBidiParagraphs *fribidi_paragraphs, unsigned int pixel_size, FriBidiParType dir, DonnellFontOptions font_options) {
    DonnellSize size;
    unsigned int longest;
    unsigned int i;

    longest = 0;

    for (i = 0; i < fribidi_paragraphs->count; i++) {
        FriBidiString *string;

        string = fribidi_paragraphs->str[i];

        if (string->direction == dir) {
            TextRenderer_MeasureAndRender(NULL, &size, NULL, string, 0, 0, pixel_size, DONNELL_FALSE, font_options);

            if (longest < size.w) {
                longest = size.w;
            }
        }
    }

    return longest;
}

int GetFirstNDNParagraph(FriBidiParagraphs *fribidi_paragraphs) {
    int i;

    for (i = 0; i < fribidi_paragraphs->count; i++) {
        FriBidiString *string;

        string = fribidi_paragraphs->str[i];

        if (string->direction != FRIBIDI_PAR_ON) {
            return i;
        }
    }

    return -1;
}

void MeasureAndRender(DonnellImageBuffer *buffer, DonnellSize *csize, DonnellPixel *color, char *utf8string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellFontOptions font_options) {
    Paragraphs *paragraphs;
    FriBidiParagraphs *fribidi_paragraphs;
    DonnellSize size;
    unsigned int longest_rtl_width;
    unsigned int longest_ltr_width;
    unsigned int longest_on_width;
    unsigned int longest_ndn_width;
    unsigned int longest_width;
    unsigned int last_ndn_paragraph;
    unsigned int first_ndn_paragraph;
    unsigned int i;
    unsigned int line_height;
    int t;

    longest_rtl_width = 0;
    t = -1;
    x = buffer->scale * x;
    y = buffer->scale * y;

    paragraphs = TextUtils_Paragraphs_Create(utf8string);
    fribidi_paragraphs = FriBidiParagraphs_ConvertFromParagraphs(paragraphs);

    longest_rtl_width = GetLongestWidth(fribidi_paragraphs, pixel_size * buffer->scale, FRIBIDI_PAR_RTL, font_options);
    longest_ltr_width = GetLongestWidth(fribidi_paragraphs, pixel_size * buffer->scale, FRIBIDI_PAR_LTR, font_options);
    longest_on_width = GetLongestWidth(fribidi_paragraphs, pixel_size * buffer->scale, FRIBIDI_PAR_ON, font_options);

    if (longest_rtl_width > longest_ltr_width) {
        longest_ndn_width = longest_rtl_width;
    } else {
        longest_ndn_width = longest_ltr_width;
    }

    if (longest_ndn_width > longest_on_width) {
        longest_width = longest_ndn_width;
    } else {
        longest_width = longest_on_width;
    }

    first_ndn_paragraph = GetFirstNDNParagraph(fribidi_paragraphs);

    if (csize) {
        csize->w = longest_width;
    }

    for (i = 0; i < fribidi_paragraphs->count; i++) {
        FriBidiString *string;
        line_height = TextRenderer_MeasureAndRender(NULL, NULL, NULL, NULL, 0, 0, pixel_size * buffer->scale, DONNELL_FALSE, font_options);

        string = fribidi_paragraphs->str[i];

        if (buffer) {
            switch (string->direction) {
            case FRIBIDI_PAR_ON:
                if (!i) {
                    if (first_ndn_paragraph >= 0) {
                        t = first_ndn_paragraph;
                        goto DN_RENDER;
                    } else {
                        goto LTR_RENDER;
                    }
                } else {
                    t = last_ndn_paragraph;
                DN_RENDER:
                    if (t < 0) {
                        goto LTR_RENDER;
                    }
                    if (fribidi_paragraphs->str[t]->direction == FRIBIDI_PAR_RTL) {
                        goto RTL_RENDER;
                    } else {
                        goto LTR_RENDER;
                    }
                }
                break;
            case FRIBIDI_PAR_RTL:
            RTL_RENDER:
                TextRenderer_MeasureAndRender(NULL, &size, NULL, string, 0, 0, pixel_size * buffer->scale, DONNELL_FALSE, font_options);
                TextRenderer_MeasureAndRender(buffer, NULL, color, fribidi_paragraphs->str[i], (longest_width - size.w + x), y, pixel_size * buffer->scale, DONNELL_FALSE, font_options);
                last_ndn_paragraph = i;
                break;
            default:
            LTR_RENDER:
                TextRenderer_MeasureAndRender(buffer, NULL, color, fribidi_paragraphs->str[i], x, y, pixel_size * buffer->scale, DONNELL_FALSE, font_options);
                TextRenderer_MeasureAndRender(NULL, &size, NULL, string, 0, 0, pixel_size * buffer->scale, DONNELL_FALSE, font_options);
                last_ndn_paragraph = i;
            }
        }

        y += line_height;
    }

    if (csize) {
        TextRenderer_MeasureAndRender(NULL, &size, NULL, fribidi_paragraphs->str[i - 1], 0, 0, pixel_size * buffer->scale, DONNELL_FALSE, font_options);
        csize->h = y - line_height + size.h;
    }

    FriBidiParagraphs_Free(fribidi_paragraphs);
    TextUtils_Paragraphs_Free(paragraphs);
}

DONNELL_EXPORT void Donnell_GraphicsPrimitives_DrawText(DonnellImageBuffer *buffer, DonnellPixel *color, char *utf8string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellFontOptions font_options) {
    if ((!buffer) || (!color) || (!utf8string) || (x < 0) || (y < 0) || (pixel_size < 0)) {
        return;
    }

    MeasureAndRender(buffer, NULL, color, utf8string, x, y, pixel_size, font_options);
}

DONNELL_EXPORT void Donnell_GraphicsPrimitives_MeasureText(DonnellSize *size, char *utf8string, unsigned int pixel_size, DonnellFontOptions font_options, unsigned int pixel_scale) {
    if ((!size) || (!utf8string) || (pixel_size < 0)) {
        return;
    }

    MeasureAndRender(NULL, size, NULL, utf8string, 0, 0, pixel_size * pixel_scale, font_options);
}


DONNELL_EXPORT void Donnell_GraphicsPrimitives_DrawRectangle(DonnellImageBuffer *buffer, DonnellPixel *color, DonnellRect *rect, DonnellBool blend) {
	unsigned int x;
	unsigned int y;
	unsigned int w;
	unsigned int h;
	unsigned int i;
	unsigned int j;

	if ((!buffer) || (!color) || (!rect)) {
        return;
    }
	
	x = rect->x*buffer->scale;
	w = rect->w*buffer->scale;
	y = rect->y*buffer->scale;
	h = rect->h*buffer->scale;
	
    for (i = 0; i < h; ++i) {
        for (j = 0; j < w; ++j) {
			if (blend) {
				Donnell_ImageBuffer_BlendPixel(buffer, x+j, y+i, color);
			} else {
				Donnell_ImageBuffer_SetPixel(buffer, x+j, y+i, color);			
			}
        }
    }
}
