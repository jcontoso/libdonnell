#include <fontconfig/fontconfig.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <fontconfig/fcfreetype.h>

#include <string.h>

#include "bidi.h"
#include "donnell.h"
#include "fontconfig.h"

FcConfig *fontconfig;

void FontConfig_Init(void) {
    FcInit();
    fontconfig = FcInitLoadConfigAndFonts();
}

void FontConfig_FreeFont(FontConfig_Font *font) {
    free(font->font);
    free(font);
}

FontConfig_Font *FontConfig_SelectFont(FriBidiString *string, DonnellFontOptions font_options) {
    FcPattern *pattern;
    FcPattern *font;
    FcResult result;
    FcChar8 *file;
    FcCharSet *char_set;
    FontConfig_Font *ret;
    char *font_name;
    unsigned int i;
    int slant;
    int weight;

    char_set = NULL;
    ret = malloc(sizeof(FontConfig_Font));
    if (!ret) {
        return NULL;
    }

    if (font_options & DONNELL_FONT_OPTIONS_SERIF) {
        font_name = "serif";
    } else if (font_options & DONNELL_FONT_OPTIONS_MONOSPACE) {
        font_name = "monospace";
    } else {
        font_name = "sans-serif";
    }

    pattern = FcPatternBuild(0, FC_FAMILY, FcTypeString, font_name, (char *)0);

    if (string) {
        char_set = FcCharSetCreate();
        for (i = 0; i < string->len; i++) {
            FcCharSetAddChar(char_set, string->str[i]);
        }
        FcPatternAddCharSet(pattern, FC_CHARSET, char_set);
    }

    if (font_options & DONNELL_FONT_OPTIONS_BOLD) {
        FcPatternAddInteger(pattern, FC_WEIGHT, FC_WEIGHT_BOLD);
    }

    if (font_options & DONNELL_FONT_OPTIONS_ITALIC) {
        FcPatternAddInteger(pattern, FC_SLANT, slant);
    }

    FcConfigSubstitute(fontconfig, pattern, FcMatchPattern);
    FcDefaultSubstitute(pattern);
    font = FcFontMatch(fontconfig, pattern, &result);

    FcPatternGetString(font, FC_FILE, 0, &file);
    FcPatternGetBool(font, FC_EMBOLDEN, 0, &ret->embolden);

    ret->font = strdup((char *)file);

    FcPatternDestroy(font);
    FcPatternDestroy(pattern);
    if (char_set) {
        FcCharSetDestroy(char_set);
    }
    return ret;
}

FT_UInt FontConfig_CharIndex(FT_Face face, FcChar32 chr) {
    return FcFreeTypeCharIndex(face, chr);
}

void FontConfig_Cleanup(void) {
    FcConfigDestroy(fontconfig);
    FcFini();
}
