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

void FontConfig_FreeFont(FontConfigFont *font) {
    if (!font) {
        return;
    }

    if (font->font) {
        free(font->font);
    }

    free(font);
}

FontConfigFont *FontConfig_CopyFont(FontConfigFont *font) {
    FontConfigFont *ret;

    ret = malloc(sizeof(FontConfigFont));
    if (!ret) {
        return NULL;
    }

    ret->font = strdup(font->font);
    ret->index = font->index;
    ret->embolden = font->embolden;

    return ret;
}

FontConfigFont *FontConfig_SelectFont(FriBidiString *string, DonnellFontOptions font_options) {
    FcPattern *pattern;
    FcPattern *font;
    FcResult result;
    FcChar8 *file;
    FcCharSet *char_set;
    FcBool embolden;
    FontConfigFont *ret;
    char *font_name;
    unsigned int i;

    if (!fontconfig) {
        return NULL;
    }

    char_set = NULL;
    ret = malloc(sizeof(FontConfigFont));
    if (!ret) {
        return NULL;
    }

    ret->index = 0;

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
        FcPatternAddInteger(pattern, FC_SLANT, FC_SLANT_ITALIC);
    }

    FcConfigSubstitute(fontconfig, pattern, FcMatchPattern);
    FcDefaultSubstitute(pattern);
    font = FcFontMatch(fontconfig, pattern, &result);

    embolden == FcFalse;
    FcPatternGetString(font, FC_FILE, 0, &file);
    FcPatternGetBool(font, FC_EMBOLDEN, 0, &embolden);
    FcPatternGetInteger(font, FC_INDEX, 0, &ret->index);

    if (embolden == FcTrue) {
        ret->embolden = DONNELL_TRUE;
    } else {
        ret->embolden = DONNELL_FALSE;
    }

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
    if (fontconfig) {
        FcConfigDestroy(fontconfig);
    }
    //FcFini();
}
