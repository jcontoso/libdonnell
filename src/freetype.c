#include <ft2build.h>
#include FT_FREETYPE_H
#include "freetype.h"
#include "bidi.h"
#include "fontpicker.h"
#include "textutils.h"
#include "donnell.h"

FT_Library freetype;
FT_Error freetype_error;

void FreeType_Init(void) {
	FT_Init_FreeType(&freetype);
}

void FreeType_Cleanup(void) {
	FT_Done_FreeType(freetype);
}

void FreeType_CopyToBuffer(DonnellImageBuffer* buffer, DonnellPixel* color, FT_Bitmap* bitmap, unsigned int a, unsigned int b) {
	for (int y = 0; y < bitmap->rows; y++) {
		for (int x = 0; x < bitmap->width; x++)	{
			DonnellPixel* pixel;
			
			pixel = Donnell_Pixel_Create();
			pixel->alpha = bitmap->buffer[y * bitmap->pitch + x];
			pixel->red = color->red;
			pixel->green = color->green;
			pixel->blue = color->blue;
			
			Donnell_ImageBuffer_BlendPixel(buffer, x + a, y + b, pixel);
			
			free(pixel);
		}
	}
}

/* 
 * If buffer, color and size are all NULL, this function will simply return the advance amount for new lines.
 * If buffer and color are both NULL, this function will calculate the text extents. 
 * If size is NULL, this function will render text to the buffer.
*/

int FreeType_MeasureAndRender(DonnellImageBuffer* buffer, DonnellSize* size, DonnellPixel* color, FriBidiString* string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellFont req_font) {
	FT_Face face;
	char* font_file;
	unsigned int i;
	int val;
	
	if (size) {
		size->h = 0;
		size->w = 0;
	}

	font_file = FontPicker_SelectFont(req_font);
	FT_New_Face(freetype, font_file, 0, &face);	
	free(font_file);

	FT_Set_Pixel_Sizes(face, pixel_size, pixel_size);
	FT_Select_Charmap(face , ft_encoding_unicode);	
	
	if (!string) {
		val = face->size->metrics.height/64;
		FT_Done_Face(face);	
		return val;
	}
	
	if (!size)  {
		DonnellSize csize;
	
		FreeType_MeasureAndRender(NULL, &csize, NULL, string, x, y, pixel_size, req_font);	
		for (i = 0; i < string->len; i++) {		
			FT_UInt  glyph_index;

			if (TextUtils_IsNewLine(string->str[i])) {
				continue;
			}
		
			glyph_index = FT_Get_Char_Index(face, string->str[i]);
		
			freetype_error = FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER);
			if (freetype_error) {
				continue;
			}
			
			FreeType_CopyToBuffer(buffer, color, &face->glyph->bitmap, x + face->glyph->bitmap_left, csize.h + y - face->glyph->bitmap_top);
			
			x += face->glyph->advance.x >> 6;
			y += face->glyph->advance.y >> 6;
		}
	} else {
		for (i = 0; i < string->len; i++) {		
			FT_Vector kerning;
			FT_UInt  glyph_index;
				
			glyph_index = FT_Get_Char_Index(face, string->str[i]);		
			freetype_error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);	
			if (freetype_error) {
				continue;
			}
				
			if (i == 0) {
				size->w += face->glyph->advance.x >> 6;	
			} else {
				FT_Get_Kerning(face, FT_Get_Char_Index(face, string->str[i-1]), glyph_index, FT_KERNING_DEFAULT, &kerning) ;
				size->w += (face->glyph->advance.x + kerning.x) >> 6;	
			}
			
			if (size->h < ((face->glyph->metrics.height + face->glyph->advance.y) >> 6)) {
				size->h = (face->glyph->metrics.height + face->glyph->advance.y) >> 6;	
			}
		}
	}
	
    FT_Done_Face(face);	
    return 0;
}
