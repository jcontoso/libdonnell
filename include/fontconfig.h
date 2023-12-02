#include <fontconfig/fontconfig.h>
#include <fontconfig/fcfreetype.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "bidi.h"
#include "donnell.h"

/* Internal functions for font selection */

#ifndef DONNELL_FONTCONFIG
#define DONNELL_FONTCONFIG

void FontConfig_Init(void);
char *FontConfig_SelectFont(DonnellFont req_font, FriBidiString* string, DonnellFontStyle font_style);
FT_UInt FontConfig_CharIndex(FT_Face face, FcChar32 chr);
void FontConfig_Cleanup(void);

#endif
