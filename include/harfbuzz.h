#include <ft2build.h>
#include FT_FREETYPE_H

#ifndef DONNELL_HARFBUZZ
#define DONNELL_HARFBUZZ

typedef void *HarfBuzzBuffer;
typedef HarfBuzzBuffer (*HarfBuzzBufferCreate)(void);
typedef void (*HarfBuzzBufferAdd)(HarfBuzzBuffer, const FT_UInt32 *, int, unsigned int, int);

typedef void (*HarfBuzzBufferGuess)(HarfBuzzBuffer);

typedef void *HarfBuzzFont;
typedef HarfBuzzFont (*HarfBuzzFontCreate)(FT_Face, void *);
typedef void (*HarfBuzzFontSetup)(HarfBuzzFont);

typedef void (*HarfBuzzShape)(HarfBuzzFont, HarfBuzzBuffer, void *, unsigned int);

typedef struct {
    FT_Int32 x_advance;
    FT_Int32 y_advance;
    FT_Int32 x_offset;
    FT_Int32 y_offset;
} HarfBuzzGlyphPos;

typedef struct {
    FT_UInt32 codepoint;
    FT_UInt32 cluster;
} HarfBuzzGlyphInfo;

typedef HarfBuzzGlyphInfo *(*HarfBuzzGetGlyphInfos)(HarfBuzzBuffer, unsigned int *);
typedef HarfBuzzGlyphPos *(*HarfBuzzGetGlyphPositions)(HarfBuzzBuffer, unsigned int *);

typedef struct {
    HarfBuzzBufferCreate buffer_create;
    HarfBuzzBufferAdd buffer_add;
    HarfBuzzBufferGuess buffer_guess;

    HarfBuzzFontCreate font_create;
    HarfBuzzFontSetup font_setup;

    HarfBuzzShape shape;

    HarfBuzzGetGlyphInfos get_glyph_infos;
    HarfBuzzGetGlyphPositions get_glyph_positions;

    void *library;
} HarfBuzzLibrary;

#endif
