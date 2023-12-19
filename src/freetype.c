#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_BITMAP_H
#include FT_LCD_FILTER_H
#include FT_OTSVG_H
#include FT_MODULE_H

#include "bidi.h"
#include "donnell.h"
#include "fontconfig.h"
#include "freetype.h"
#include "svg.h"
#include "textutils.h"

FT_Library freetype;
FT_Error freetype_error;
FT_Int32 flags;

DonnellImageBuffer *FreeType_ConvertToBufferFromRGBABitmap(FT_Bitmap *bitmap, DonnellBool non_ideal_size, unsigned int wanted_size, double w_ratio) {
    DonnellImageBuffer *buffer;
    int x;
    int y;

    buffer = Donnell_ImageBuffer_Create(bitmap->width, bitmap->rows);

    for (y = 0; y < bitmap->rows; y++) {
        for (x = 0; x < bitmap->width; x++) {
            DonnellPixel *pixel;

            pixel = Donnell_Pixel_Create();
            pixel->blue = bitmap->buffer[y * bitmap->pitch + x * 4];
            pixel->green = bitmap->buffer[y * bitmap->pitch + x * 4 + 1];
            pixel->red = bitmap->buffer[y * bitmap->pitch + x * 4 + 2];
            pixel->alpha = bitmap->buffer[y * bitmap->pitch + x * 4 + 3];

            Donnell_ImageBuffer_SetPixel(buffer, x, y, pixel);

            Donnell_Pixel_Free(pixel);
        }
    }

    if (non_ideal_size) {
        DonnellImageBuffer *sbuffer;

        sbuffer = Donnell_ImageBuffer_Scale(buffer, bitmap->width * w_ratio, wanted_size, DONNELL_SCALING_ALGORITHM_BILINEAR);

        Donnell_ImageBuffer_Free(buffer);
        return sbuffer;
    } else {
        return buffer;
    }
}

DonnellImageBuffer *FreeType_ConvertToBufferFromGray8OrMonoBitmap(FT_Bitmap *bitmap, DonnellPixel *color, DonnellBool non_ideal_size, unsigned int wanted_size, FT_Pixel_Mode mode, double w_ratio) {
    DonnellImageBuffer *buffer;
    int x;
    int y;

    buffer = Donnell_ImageBuffer_Create(bitmap->width, bitmap->rows);

    for (y = 0; y < bitmap->rows; y++) {
        for (x = 0; x < bitmap->width; x++) {
            DonnellPixel *pixel;

            pixel = Donnell_Pixel_Create();
            if (mode == FT_PIXEL_MODE_MONO) {
                unsigned char *row;

                row = &bitmap->buffer[y * bitmap->pitch];
                if ((row[x >> 3] & (128 >> (x & 7))) > 0) {
                    pixel->alpha = 255;
                } else {
                    pixel->alpha = 0;
                }
            } else {
                pixel->alpha = bitmap->buffer[y * bitmap->pitch + x];
            }
            pixel->red = color->red;
            pixel->green = color->green;
            pixel->blue = color->blue;

            Donnell_ImageBuffer_SetPixel(buffer, x, y, pixel);

            Donnell_Pixel_Free(pixel);
        }
    }

    if (non_ideal_size) {
        DonnellImageBuffer *sbuffer;
        if (mode == FT_PIXEL_MODE_MONO) {
            sbuffer = Donnell_ImageBuffer_Scale(buffer, bitmap->width * w_ratio, wanted_size, DONNELL_SCALING_ALGORITHM_NEAREST_NEIGHBOR);
        } else {
            sbuffer = Donnell_ImageBuffer_Scale(buffer, bitmap->width * w_ratio, wanted_size, DONNELL_SCALING_ALGORITHM_BILINEAR);
        }
        Donnell_ImageBuffer_Free(buffer);
        return sbuffer;
    } else {
        return buffer;
    }
}

void FreeType_CopyToBuffer(DonnellImageBuffer *buffer, DonnellPixel *color, FT_Bitmap *bitmap, unsigned int a, unsigned int b, DonnellBool non_ideal_size, unsigned int wanted_size, double w_ratio) {
    DonnellImageBuffer *cbuffer;
    DonnellPixel *pixel;
    DonnellRect dst;
    int x;
    int y;

    switch (bitmap->pixel_mode) {
    case FT_PIXEL_MODE_BGRA:
        cbuffer = FreeType_ConvertToBufferFromRGBABitmap(bitmap, non_ideal_size, wanted_size, w_ratio);

        dst.w = cbuffer->width;
        dst.h = cbuffer->height;
        dst.x = a;
        dst.y = b;

        Donnell_ImageBuffer_BlendBufferContents(buffer, cbuffer, NULL, &dst);
        Donnell_ImageBuffer_Free(cbuffer);
        break;
    case FT_PIXEL_MODE_MONO:
    case FT_PIXEL_MODE_GRAY:
        if (!non_ideal_size) {
            for (y = 0; y < bitmap->rows; y++) {
                for (x = 0; x < bitmap->width; x++) {
                    pixel = Donnell_Pixel_Create();
                    if (bitmap->pixel_mode == FT_PIXEL_MODE_MONO) {
                        unsigned char *row;

                        row = &bitmap->buffer[y * bitmap->pitch];
                        if ((row[x >> 3] & (128 >> (x & 7))) > 0) {
                            pixel->alpha = 255;
                        } else {
                            pixel->alpha = 0;
                        }
                    } else {
                        pixel->alpha = bitmap->buffer[y * bitmap->pitch + x];
                    }
                    pixel->red = color->red;
                    pixel->green = color->green;
                    pixel->blue = color->blue;
                    Donnell_ImageBuffer_BlendPixel(buffer, x + a, y + b, pixel);
                    Donnell_Pixel_Free(pixel);
                }
            }
        } else {
            cbuffer = FreeType_ConvertToBufferFromGray8OrMonoBitmap(bitmap, color, non_ideal_size, wanted_size, bitmap->pixel_mode, w_ratio);

            dst.w = cbuffer->width;
            dst.h = cbuffer->height;
            dst.x = a;
            dst.y = b;

            Donnell_ImageBuffer_BlendBufferContents(buffer, cbuffer, NULL, &dst);
            Donnell_ImageBuffer_Free(cbuffer);
        }
        break;
    }
}

void FreeType_Init(void) {
    FT_Init_FreeType(&freetype);
    flags = FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL;

    if (SVG_GetLibrary()) {
        SVG_RendererHooks funcs;

        funcs.init_svg = (SVG_Lib_Init_Func)SVGRenderer_Init;
        funcs.free_svg = (SVG_Lib_Free_Func)SVGRenderer_Free;
        funcs.render_svg = (SVG_Lib_Render_Func)SVGRenderer_Render;
        funcs.preset_slot = (SVG_Lib_Preset_Slot_Func)SVGRenderer_PresetSlot;

        FT_Property_Set(freetype, "ot-svg", "svg-hooks", &funcs);
    } else {
#if ((FREETYPE_MINOR >= 13) && (FREETYPE_PATCH >= 1))
        cflags |= FT_LOAD_NO_SVG;
#endif
    }
}

void FreeType_Cleanup(void) {
    FT_Done_FreeType(freetype);
}

FT_Library *FreeType_GetLibrary(void) {
    return &freetype;
}

FT_Int32 FreeType_GetFlags(void) {
    return flags;
}

/*
 * If buffer, color and size are all NULL, this function will simply return the advance amount for new lines.
 * If buffer and color are both NULL, this function will calculate the text extents.
 * If size is NULL, this function will render text to the buffer.
 */

int FreeType_MeasureAndRender(DonnellImageBuffer *buffer, DonnellSize *size, DonnellPixel *color, FriBidiString *string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellBool return_max_asc, DonnellFontOptions font_options) {
    FT_Face face;
    FT_Int32 cflags;
    FontConfig_Font *font_file;
    DonnellBool non_ideal_size;
    unsigned int i;
    unsigned int best;
    int val;
    double w_ratio;
    double h_ratio;

    non_ideal_size = DONNELL_FALSE;
    w_ratio = 1;
    h_ratio = 1;

    if (size) {
        size->h = 0;
        size->w = 0;
    }

    font_file = FontConfig_SelectFont(string, font_options);
    if (!font_file) {
        return 0;
    }

    FT_New_Face(freetype, font_file->font, font_file->index, &face);

    cflags = flags;
    if ((size) && (!FT_HAS_COLOR(face))) {
        cflags &= ~FT_LOAD_RENDER;
        cflags |= FT_LOAD_NO_BITMAP;
    }

    if (FT_HAS_COLOR(face)) {
        cflags |= FT_LOAD_COLOR;
    }

    freetype_error = FT_Set_Pixel_Sizes(face, 0, pixel_size);
    if (freetype_error) {
        unsigned int diff;

        best = 0;
        diff = abs(pixel_size - face->available_sizes[0].width);

        for (i = 0; i < face->num_fixed_sizes; ++i) {
            unsigned int cdiff;

            cdiff = abs(pixel_size - face->available_sizes[i].width);

            if (cdiff < diff) {
                best = i;
                diff = cdiff;
            }
        }

        if (!return_max_asc) {
            h_ratio = (double)pixel_size / (double)face->available_sizes[best].height;
            w_ratio = h_ratio;
        }

        FT_Select_Size(face, best);
        non_ideal_size = DONNELL_TRUE;
    }

    FT_Select_Charmap(face, ft_encoding_unicode);

    if (!string) {
        val = face->size->metrics.height / 64 * h_ratio;
        FT_Done_Face(face);
        return val;
    }

    if (!size) {
        DonnellSize csize;

        FreeType_MeasureAndRender(NULL, &csize, NULL, string, x, y, pixel_size, DONNELL_TRUE, font_options);
        for (i = 0; i < string->len; i++) {
            FT_UInt glyph_index;

            if (TextUtils_IsNewLine(string->str[i])) {
                continue;
            }

            glyph_index = FontConfig_CharIndex(face, string->str[i]);

            freetype_error = FT_Load_Glyph(face, glyph_index, cflags);

            if (freetype_error) {
                continue;
            }

            if (font_file->embolden) {
                FT_Bitmap emboldened;

                FT_Bitmap_New(&emboldened);
                FT_Bitmap_Copy(freetype, &face->glyph->bitmap, &emboldened);
                FT_Bitmap_Embolden(freetype, &emboldened, 1 << 6, 1 << 6);
                FreeType_CopyToBuffer(buffer, color, &emboldened, x + face->glyph->bitmap_left, csize.h + y - face->glyph->bitmap_top, non_ideal_size, pixel_size, h_ratio);
                FT_Bitmap_Done(freetype, &emboldened);
            } else {
                FreeType_CopyToBuffer(buffer, color, &face->glyph->bitmap, x + face->glyph->bitmap_left, csize.h + y - face->glyph->bitmap_top, non_ideal_size, pixel_size, h_ratio);
            }

            x += (int)(face->glyph->advance.x * w_ratio) >> 6;
            y += (int)(face->glyph->advance.y * h_ratio) >> 6;
        }
    } else {
        unsigned int max_ascent;
        unsigned int max_descent;

        max_ascent = 0;
        max_descent = 0;

        for (i = 0; i < string->len; i++) {
            FT_Vector kerning;
            FT_UInt glyph_index;
            int calc_height;

            glyph_index = FontConfig_CharIndex(face, string->str[i]);
            freetype_error = FT_Load_Glyph(face, glyph_index, cflags);
            if (freetype_error) {
                continue;
            }

            if (i == 0) {
                size->w += (face->glyph->advance.x >> 6) * w_ratio;
            } else {
                FT_Get_Kerning(face, FontConfig_CharIndex(face, string->str[i - 1]), glyph_index, FT_KERNING_DEFAULT, &kerning);
                size->w += ((face->glyph->advance.x + kerning.x) >> 6) * w_ratio;
            }

            if (((int)(face->glyph->metrics.height * h_ratio) >> 6) - (int)(face->glyph->bitmap_top * h_ratio) > max_descent) {
                max_descent = ((int)(face->glyph->metrics.height * h_ratio) >> 6) - (int)(face->glyph->bitmap_top * h_ratio);
            }

            calc_height = (int)(face->glyph->bitmap_top * h_ratio);
            if (calc_height < 0) {
                calc_height = 0;
            }

            if (calc_height > max_ascent) {
                max_ascent = calc_height;
            }
        }

        if (return_max_asc) {
            size->h = max_ascent;
        } else {
            size->h = max_ascent + max_descent;
        }
    }

    FontConfig_FreeFont(font_file);
    FT_Done_Face(face);
    return 0;
}
