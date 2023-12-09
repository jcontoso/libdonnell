#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_BITMAP_H
#include FT_LCD_FILTER_H

#include "bidi.h"
#include "donnell.h"
#include "fontconfig.h"
#include "freetype.h"
#include "textutils.h"

FT_Library freetype;
FT_Error freetype_error;
FT_Int32 flags;

DonnellImageBuffer *FreeType_ConvertToBufferFromRGBABitmap(FT_Bitmap *bitmap, DonnellBool non_ideal_size, unsigned int wanted_size) {
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
		
        sbuffer = Donnell_ImageBuffer_Scale(buffer, wanted_size, wanted_size, DONNELL_SCALING_ALGORITHM_BILINEAR);
        
        Donnell_ImageBuffer_Free(buffer);
        return sbuffer;
    } else {
        return buffer;
    }
}

DonnellImageBuffer *FreeType_ConvertToBufferFromGray8Bitmap(FT_Bitmap *bitmap, DonnellPixel* color, DonnellBool non_ideal_size, unsigned int wanted_size) {
    DonnellImageBuffer *buffer;
    int x;
    int y;

    buffer = Donnell_ImageBuffer_Create(bitmap->width, bitmap->rows);

    for (y = 0; y < bitmap->rows; y++) {
        for (x = 0; x < bitmap->width; x++) {
            DonnellPixel *pixel;

            pixel = Donnell_Pixel_Create();
			pixel->alpha = bitmap->buffer[y * bitmap->pitch + x];
			pixel->red = color->red;
			pixel->green = color->green;
			pixel->blue = color->blue;		
						
            Donnell_ImageBuffer_SetPixel(buffer, x, y, pixel);

            Donnell_Pixel_Free(pixel);
        }
    }

    if (non_ideal_size) {
		DonnellImageBuffer *sbuffer;
        sbuffer = Donnell_ImageBuffer_Scale(buffer, wanted_size, wanted_size, DONNELL_SCALING_ALGORITHM_BILINEAR);
        Donnell_ImageBuffer_Free(buffer);
        return sbuffer;
    } else {
        return buffer;
    }
}

void FreeType_CopyToBuffer(DonnellImageBuffer *buffer, DonnellPixel *color, FT_Bitmap *bitmap, unsigned int a, unsigned int b, DonnellBool non_ideal_size, unsigned int wanted_size) {
    DonnellImageBuffer *cbuffer;
    DonnellPixel *pixel;
    DonnellRect dst;
    int x;
    int y;

	switch(bitmap->pixel_mode) {
		case FT_PIXEL_MODE_BGRA:
			cbuffer = FreeType_ConvertToBufferFromRGBABitmap(bitmap, non_ideal_size, wanted_size);
	
			dst.w = cbuffer->width;
			dst.h = cbuffer->height;
			dst.x = a;
			dst.y = b;

			Donnell_ImageBuffer_BlendBufferContents(buffer, cbuffer, NULL, &dst);
			Donnell_ImageBuffer_Free(cbuffer);
			break;
		case FT_PIXEL_MODE_GRAY:
			if (!non_ideal_size) {
			    for (y = 0; y < bitmap->rows; y++) {
					for (x = 0; x < bitmap->width; x++) {
					    pixel = Donnell_Pixel_Create();
						pixel->alpha = bitmap->buffer[y * bitmap->pitch + x];
						pixel->red = color->red;
						pixel->green = color->green;
						pixel->blue = color->blue;		
			            Donnell_ImageBuffer_BlendPixel(buffer, x + a, y + b, pixel);
						Donnell_Pixel_Free(pixel);				
					}
				}
			} else {
				cbuffer = FreeType_ConvertToBufferFromGray8Bitmap(bitmap, color, non_ideal_size, wanted_size);
		
				dst.w = cbuffer->width;
				dst.h = cbuffer->height;
				dst.x = a;
				dst.y = b;

				Donnell_ImageBuffer_BlendBufferContents(buffer, cbuffer, NULL, &dst);
				Donnell_ImageBuffer_Free(cbuffer);				
			}
			break;

	}

	/*
    for (y = 0; y < bitmap->rows; y++) {
        for (x = 0; x < bitmap->width; x++) {
            DonnellPixel *pixel;
			char* cbuffer;
			
            pixel = Donnell_Pixel_Create();
			cbuffer = bitmap->buffer;
			
            switch (bitmap->pixel_mode) {
            case FT_PIXEL_MODE_LCD:
                pixel->alpha = 255;
                pixel->red = bitmap->buffer[y * bitmap->pitch + x * 3];
                pixel->green = bitmap->buffer[y * bitmap->pitch + x * 3 + 1];
                pixel->blue = bitmap->buffer[y * bitmap->pitch + x * 3 + 2];
                break;
            case FT_PIXEL_MODE_MONO:
				cbuffer = &bitmap->buffer[y*bitmap->pitch];
				if (((cbuffer[x >> 3] & (1 << (x & 7))) != 0) == 1) {
					pixel->alpha = 255;
				}	else {
					pixel->alpha = 0;
			
				}
                pixel->red = color->red;
                pixel->green = color->green;
                pixel->blue = color->blue;
				break;
            default:
                pixel->alpha = bitmap->buffer[y * bitmap->pitch + x];
                pixel->red = color->red;
                pixel->green = color->green;
                pixel->blue = color->blue;
            }


        }
    }*/
}

void FreeType_Init(void) {
    FT_Init_FreeType(&freetype);
    flags = FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL;
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

FreeTypeCopyToBufferFunction *FreeType_GetBufferCopyFunction(void) {
    return NULL;
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

    freetype_error = FT_Set_Pixel_Sizes(face, pixel_size, pixel_size);
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
            w_ratio = (double)pixel_size / (double)face->available_sizes[best].width;
            h_ratio = (double)pixel_size / (double)face->available_sizes[best].height;
        }

        FT_Select_Size(face, best);
        non_ideal_size = DONNELL_TRUE;
    }

    FT_Select_Charmap(face, ft_encoding_unicode);

    if (!string) {
        val = face->size->metrics.height / 64;
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

            FreeType_CopyToBuffer(buffer, color, &face->glyph->bitmap, x + face->glyph->bitmap_left, csize.h + y - face->glyph->bitmap_top, non_ideal_size, pixel_size);

            if (non_ideal_size) {
                x += (int)(face->glyph->advance.x * w_ratio) >> 6;
                y += (int)(face->glyph->advance.y * h_ratio) >> 6;
            } else {
                x += face->glyph->advance.x >> 6;
                y += face->glyph->advance.y >> 6;
            }
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
                size->w += (int)(face->glyph->advance.x * w_ratio) >> 6;
            } else {
                FT_Get_Kerning(face, FontConfig_CharIndex(face, string->str[i - 1]), glyph_index, FT_KERNING_DEFAULT, &kerning);
                size->w += ((int)(face->glyph->advance.x * w_ratio) - (int)(kerning.x * w_ratio)) >> 6;
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
