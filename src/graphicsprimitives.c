#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "bidi.h"
#include "donnell.h"
#include "freetype.h"
#include "harfbuzz.h"
#include "symvis.h"
#include "textutils.h"

void MeasureAndRenderTextLine(DonnellImageBuffer *buffer, DonnellSize *size, DonnellPixel *color, FriBidiString *string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellFont req_font, FT_Bool return_max_asc, DonnellFontStyle font_style) {
    if (HarfBuzz_GetLibrary()) {
        HarfBuzz_MeasureAndRender(buffer, size, color, string, x, y, pixel_size, req_font, return_max_asc, font_style);
    } else {
        FreeType_MeasureAndRender(buffer, size, color, string, x, y, pixel_size, req_font, return_max_asc, font_style);
    }
}

DONNELL_EXPORT void Donnell_GraphicsPrimitives_DrawTextLine(DonnellImageBuffer *buffer, DonnellPixel *color, char *utf8string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellFont req_font, DonnellFontStyle font_style) {
    FriBidiString *string;

    if ((!buffer) || (!color) || (!utf8string) || (x < 0) || (y < 0) || (pixel_size < 0)) {
        return;
    }

    string = FriBidiString_ConvertFromUTF8(utf8string);
    if (HarfBuzz_GetLibrary()) {
        FriBidiString_Handle(string, DONNELL_FALSE);
    } else {
        FriBidiString_Handle(string, DONNELL_TRUE);
    }
    MeasureAndRenderTextLine(buffer, NULL, color, string, x, y, pixel_size, req_font, 0, font_style);
    FriBidiString_Free(string);
}

DONNELL_EXPORT void Donnell_GraphicsPrimitives_MeasureTextLine(DonnellSize *size, char *utf8string, unsigned int pixel_size, DonnellFont req_font, DonnellFontStyle font_style) {
    FriBidiString *string;

    if ((!size) || (!utf8string) || (pixel_size < 0)) {
        return;
    }

    string = FriBidiString_ConvertFromUTF8(utf8string);
    if (HarfBuzz_GetLibrary()) {
        FriBidiString_Handle(string, DONNELL_FALSE);
    } else {
        FriBidiString_Handle(string, DONNELL_TRUE);
    }
    MeasureAndRenderTextLine(NULL, size, NULL, string, 0, 0, pixel_size, req_font, 0, font_style);
    FriBidiString_Free(string);
}

unsigned int GetLongestWidth(FriBidiParagraphs *fribidi_paragraphs, unsigned int pixel_size, DonnellFont req_font, FriBidiParType dir, DonnellFontStyle font_style) {
    DonnellSize size;
    unsigned int longest;
    unsigned int i;

    longest = 0;

    for (i = 0; i < fribidi_paragraphs->count; i++) {
        FriBidiString *string;

        string = fribidi_paragraphs->str[i];

        if (string->direction == dir) {
            MeasureAndRenderTextLine(NULL, &size, NULL, string, 0, 0, pixel_size, req_font, 0, font_style);

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

void MeasureAndRender(DonnellImageBuffer *buffer, DonnellSize *csize, DonnellPixel *color, char *utf8string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellFont req_font, DonnellFontStyle font_style) {
    Paragraphs *paragraphs;
    FriBidiParagraphs *fribidi_paragraphs;
    DonnellSize size;
    unsigned int longest_rtl_width;
    unsigned int longest_ltr_width;
    unsigned int longest_on_width;
    unsigned int longest_ndn_width;
    unsigned int longest_width;
    unsigned int line_height;
    unsigned int last_ndn_paragraph;
    unsigned int first_ndn_paragraph;
    unsigned int i;
    unsigned int t;

    longest_rtl_width = 0;

    line_height = FreeType_MeasureAndRender(NULL, NULL, NULL, NULL, 0, 0, pixel_size, req_font, 0, font_style);

    paragraphs = TextUtils_Paragraphs_Create(utf8string);
    fribidi_paragraphs = FriBidiParagraphs_ConvertFromParagraphs(paragraphs);

    longest_rtl_width = GetLongestWidth(fribidi_paragraphs, pixel_size, req_font, FRIBIDI_PAR_RTL, font_style);
    longest_ltr_width = GetLongestWidth(fribidi_paragraphs, pixel_size, req_font, FRIBIDI_PAR_LTR, font_style);
    longest_on_width = GetLongestWidth(fribidi_paragraphs, pixel_size, req_font, FRIBIDI_PAR_ON, font_style);

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
                    if (fribidi_paragraphs->str[t]->direction == FRIBIDI_PAR_RTL) {
                        goto RTL_RENDER;
                    } else {
                        goto LTR_RENDER;
                    }
                }
                break;
            case FRIBIDI_PAR_RTL:
            RTL_RENDER:
                MeasureAndRenderTextLine(NULL, &size, NULL, string, 0, 0, pixel_size, req_font, 0, font_style);
                MeasureAndRenderTextLine(buffer, NULL, color, fribidi_paragraphs->str[i], longest_width - size.w + x, y, pixel_size, req_font, 0, font_style);
                last_ndn_paragraph = i;
                break;
            default:
            LTR_RENDER:
                MeasureAndRenderTextLine(buffer, NULL, color, fribidi_paragraphs->str[i], x, y, pixel_size, req_font, 0, font_style);
                MeasureAndRenderTextLine(NULL, &size, NULL, string, 0, 0, pixel_size, req_font, 0, font_style);
                last_ndn_paragraph = i;
            }
        }

        y += line_height;
    }

    if (csize) {
        MeasureAndRenderTextLine(NULL, &size, NULL, fribidi_paragraphs->str[i - 1], 0, 0, pixel_size, req_font, 0, font_style);
        csize->h = y - line_height + size.h;
    }

    FriBidiParagraphs_Free(fribidi_paragraphs);
    TextUtils_Paragraphs_Free(paragraphs);
}

DONNELL_EXPORT void Donnell_GraphicsPrimitives_DrawText(DonnellImageBuffer *buffer, DonnellPixel *color, char *utf8string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellFont req_font, DonnellFontStyle font_style) {
    if ((!buffer) || (!color) || (!utf8string) || (x < 0) || (y < 0) || (pixel_size < 0)) {
        return;
    }

    MeasureAndRender(buffer, NULL, color, utf8string, x, y, pixel_size, req_font, font_style);
}

DONNELL_EXPORT void Donnell_GraphicsPrimitives_MeasureText(DonnellSize *size, char *utf8string, unsigned int pixel_size, DonnellFont req_font, DonnellFontStyle font_style) {
    if ((!size) || (!utf8string) || (pixel_size < 0)) {
        return;
    }

    MeasureAndRender(NULL, size, NULL, utf8string, 0, 0, pixel_size, req_font, font_style);
}
