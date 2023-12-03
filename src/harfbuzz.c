#include <dlfcn.h>
#include <stdint.h>
#include <stdlib.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_BITMAP_H
#include FT_LCD_FILTER_H

#include "bidi.h"
#include "donnell.h"
#include "fontconfig.h"
#include "freetype.h"
#include "harfbuzz.h"

HarfBuzzLibrary *harfbuzz;

FT_Library hb_freetype;
FT_Error hb_freetype_error;
FT_Error hb_lcd_error;

void HarfBuzz_Init(void) {
    char *error;

    error = NULL;
    harfbuzz = NULL;
    harfbuzz = malloc(sizeof(HarfBuzzLibrary));
    if (!harfbuzz) {
        return;
    }

    harfbuzz->library = dlopen("libharfbuzz.so", RTLD_LAZY);
    if (!harfbuzz->library) {
        free(harfbuzz);
        harfbuzz = NULL;
        return;
    }

    harfbuzz->buffer_create = dlsym(harfbuzz->library, "hb_buffer_create");
    harfbuzz->buffer_add = dlsym(harfbuzz->library, "hb_buffer_add_utf32");
    harfbuzz->buffer_add_utf8 = dlsym(harfbuzz->library, "hb_buffer_add_utf8");
    harfbuzz->buffer_guess = dlsym(harfbuzz->library, "hb_buffer_guess_segment_properties");
    harfbuzz->buffer_destroy = dlsym(harfbuzz->library, "hb_buffer_destroy");

    harfbuzz->font_create = dlsym(harfbuzz->library, "hb_ft_font_create");
    harfbuzz->font_setup = dlsym(harfbuzz->library, "hb_ft_font_set_funcs");
    harfbuzz->font_destroy = dlsym(harfbuzz->library, "hb_buffer_destroy");

    harfbuzz->shape = dlsym(harfbuzz->library, "hb_shape");

    harfbuzz->get_glyph_infos = dlsym(harfbuzz->library, "hb_buffer_get_glyph_infos");
    harfbuzz->get_glyph_positions = dlsym(harfbuzz->library, "hb_buffer_get_glyph_positions");

    error = dlerror();
    if (error) {
        free(harfbuzz);
        harfbuzz = NULL;
        return;
    }

    hb_freetype = *FreeType_GetLibrary();
    hb_lcd_error = FreeType_GetLCDError();
}

HarfBuzzLibrary *HarfBuzz_GetLibrary(void) {
    return harfbuzz;
}

void HarfBuzz_Cleanup(void) {
    if (!harfbuzz) {
        return;
    }

    dlclose(harfbuzz->library);
    free(harfbuzz);
}

void HarfBuzz_MeasureAndRender(DonnellImageBuffer *buffer, DonnellSize *size, DonnellPixel *color, FriBidiString *string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellFont req_font, FT_Bool return_max_asc, DonnellFontStyle font_style) {
    FT_Face face;
    FT_Int32 flags;
    FontConfig_Font *font_file;
    HarfBuzzBuffer harfbuzz_buffer;
    HarfBuzzFont harfbuzz_font;
    HarfBuzzGlyphPos *harfbuzz_pos;
    HarfBuzzGlyphInfo *harfbuzz_info;
    unsigned int glyph_count;
    unsigned int i;
    int val;

    if (size) {
        size->h = 0;
        size->w = 0;
    }

    if (hb_lcd_error) {
        flags = FT_LOAD_RENDER;
    } else {
        flags = FT_LOAD_RENDER | FT_LOAD_TARGET_LCD;
    }

    harfbuzz_buffer = harfbuzz->buffer_create();
    harfbuzz->buffer_add(harfbuzz_buffer, string->str, string->len, 0, string->len);
    harfbuzz->buffer_guess(harfbuzz_buffer);

    char *asda;

    font_file = FontConfig_SelectFont(req_font, string, font_style);
    FT_New_Face(hb_freetype, font_file->font, 0, &face);

    FT_Set_Pixel_Sizes(face, pixel_size, pixel_size);
    FT_Select_Charmap(face, ft_encoding_unicode);

    harfbuzz_font = harfbuzz->font_create(face, NULL);
    harfbuzz->font_setup(harfbuzz_font);

    harfbuzz->shape(harfbuzz_font, harfbuzz_buffer, NULL, 0);

    harfbuzz_info = harfbuzz->get_glyph_infos(harfbuzz_buffer, &glyph_count);
    harfbuzz_pos = harfbuzz->get_glyph_positions(harfbuzz_buffer, &glyph_count);

    if (!size) {
        DonnellSize csize;

        HarfBuzz_MeasureAndRender(NULL, &csize, NULL, string, x, y, pixel_size, req_font, 1, font_style);
        for (i = 0; i < glyph_count; i++) {
            hb_freetype_error = FT_Load_Glyph(face, harfbuzz_info[i].codepoint, flags);
            if (hb_freetype_error) {
                continue;
            }

            if (hb_lcd_error) {
                FreeType_CopyToBuffer(buffer, color, &face->glyph->bitmap, x + face->glyph->bitmap_left + (harfbuzz_pos[i].x_offset / 64), csize.h + y - face->glyph->bitmap_top + (harfbuzz_pos[i].y_offset / 64));
            } else {
                FreeType_CopyToBufferLCD(buffer, color, &face->glyph->bitmap, x + face->glyph->bitmap_left + (harfbuzz_pos[i].x_offset / 64), csize.h + y - face->glyph->bitmap_top + (harfbuzz_pos[i].y_offset / 64));
            }

            x += harfbuzz_pos[i].x_advance / 64;
            y += harfbuzz_pos[i].y_advance / 64;
        }
    } else {
        unsigned int max_ascent;
        unsigned int max_descent;

        max_ascent = 0;
        max_descent = 0;

        for (i = 0; i < string->len; i++) {
            FT_Vector kerning;
            int calc_height;

            hb_freetype_error = FT_Load_Glyph(face, harfbuzz_info[i].codepoint, FT_LOAD_NO_BITMAP);
            if (hb_freetype_error) {
                continue;
            }

            if (i == 0) {
                size->w += harfbuzz_pos[i].x_advance / 64;
            } else {
                FT_Get_Kerning(face, harfbuzz_info[i - 1].codepoint, harfbuzz_info[i].codepoint, FT_KERNING_DEFAULT, &kerning);
                size->w += ((harfbuzz_pos[i].x_advance / 64) - kerning.x) >> 6;
            }

            if ((face->glyph->metrics.height >> 6) - (face->glyph->bitmap_top + (harfbuzz_pos[i].y_offset / 64))) {
                max_descent = (face->glyph->metrics.height >> 6) - (face->glyph->bitmap_top + (harfbuzz_pos[i].y_offset / 64));
            }

            calc_height = face->glyph->bitmap_top + (harfbuzz_pos[i].y_offset / 64);
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
}
