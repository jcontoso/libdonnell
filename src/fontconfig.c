#include <fontconfig/fcfreetype.h>
#include <fontconfig/fontconfig.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <string.h>

#include "bidi.h"
#include "donnell.h"
#include "fontconfig.h"

FcConfig *fontconfig;

void FontConfig_Init(void) {
    FcInit();
    fontconfig = FcInitLoadConfigAndFonts();
}

char *FontConfig_SelectFont(DonnellFont req_font, FriBidiString *string, DonnellFontStyle font_style) {
    FcPattern *pattern;
    FcPattern *font;
    FcResult result;
    FcChar8 *file;
    FcCharSet *char_set;
    char *ret;
    char *font_name;
    unsigned int i;
    int slant;
    int weight;

    char_set = FcCharSetCreate();

    switch (req_font) {
    case DONNELL_FONT_SERIF:
        font_name = "serif";
        break;
    case DONNELL_FONT_MONOSPACE:
        font_name = "monospace";
        break;
    default:
        font_name = "sans-serif";
    }

    if (font_style & DONNELL_FONT_STYLE_ITALIC) {
        slant = FC_SLANT_ITALIC;
    } else {
        slant = FC_SLANT_ROMAN;
    }

    if (font_style & DONNELL_FONT_STYLE_BOLD) {
        weight = FC_WEIGHT_BOLD;
    } else {
        weight = FC_WEIGHT_MEDIUM;
    }

    if (string) {
        for (i = 0; i < string->len; i++) {
            FcCharSetAddChar(char_set, string->str[i]);
        }
    }

    pattern = FcPatternBuild(0, FC_FAMILY, FcTypeString, font_name, FC_CHARSET, FcTypeCharSet, char_set, FC_SLANT, FcTypeInteger, slant, FC_WEIGHT, FcTypeInteger, weight, (char *)0);

    FcConfigSubstitute(fontconfig, pattern, FcMatchPattern);
    FcDefaultSubstitute(pattern);
    font = FcFontMatch(fontconfig, pattern, &result);

    FcPatternGetString(font, FC_FILE, 0, &file);
    ret = strdup((char *)file);

    FcPatternDestroy(font);
    FcPatternDestroy(pattern);
    FcCharSetDestroy(char_set);

    return ret;
}

FT_UInt FontConfig_CharIndex(FT_Face face, FcChar32 chr) {
    return FcFreeTypeCharIndex(face, chr);
}

void FontConfig_Cleanup(void) {
    FcConfigDestroy(fontconfig);
    FcFini();
}
