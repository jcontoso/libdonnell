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
} FontConfig_Font;

void FontConfig_Init(void);
FontConfig_Font *FontConfig_SelectFont(FriBidiString *string, DonnellFontOptions font_options);
void FontConfig_FreeFont(FontConfig_Font *font);
FT_UInt FontConfig_CharIndex(FT_Face face, FcChar32 chr);
void FontConfig_Cleanup(void);

#endif
