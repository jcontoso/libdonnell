#include <fontconfig/fontconfig.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <fontconfig/fcfreetype.h>

#include "bidi.h"
#include "donnell.h"

/* Internal functions for font selection */

#ifndef DONNELL_FONTCONFIG
#define DONNELL_FONTCONFIG

typedef struct {
    char *font;
    int index;
    DonnellBool embolden;
} FontConfigFont;

void FontConfig_Init(void);
FontConfigFont *FontConfig_SelectFont(FriBidiString *string, DonnellFontOptions font_options);
FontConfigFont *FontConfig_CopyFont(FontConfigFont *font);
void FontConfig_FreeFont(FontConfigFont *font);
FT_UInt FontConfig_CharIndex(FT_Face face, FcChar32 chr);
void FontConfig_Cleanup(void);

#endif
