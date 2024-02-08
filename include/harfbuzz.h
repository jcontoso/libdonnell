#include <ft2build.h>
#include FT_FREETYPE_H

#include "bidi.h"
#include "donnell.h"
#include "fontconfig.h"
#include "freetype.h"

#ifndef DONNELL_HARFBUZZ
#define DONNELL_HARFBUZZ

typedef void *HarfBuzzBuffer;
typedef void *HarfBuzzFont;
typedef void *HarfBuzzBlob;
typedef void *HarfBuzzFace;

typedef union {
    FT_UInt32 u32;
    FT_Int32 i32;
    FT_UInt16 u16[2];
    FT_Int16 i16[2];
    unsigned char u8[4];
    char i8[4];
} HarfBuzzVarInt;

typedef struct {
    FT_Int32 x_advance;
    FT_Int32 y_advance;
    FT_Int32 x_offset;
    FT_Int32 y_offset;
    HarfBuzzVarInt var;
} HarfBuzzGlyphPos;

typedef struct {
    FT_UInt32 codepoint;
    FT_UInt32 mask;
    FT_UInt32 cluster;
    HarfBuzzVarInt var1;
    HarfBuzzVarInt var2;
} HarfBuzzGlyphInfo;

typedef enum {
    HB_DIRECTION_INVALID = 0,
    HB_DIRECTION_LTR = 4,
    HB_DIRECTION_RTL,
    HB_DIRECTION_TTB,
    HB_DIRECTION_BTT
} HarfBuzzDirection;

typedef HarfBuzzBuffer (*HarfBuzzBufferCreate)(void);
typedef void (*HarfBuzzBufferAdd)(HarfBuzzBuffer, const FT_UInt32 *, int, unsigned int, int);
typedef void (*HarfBuzzBufferAddUTF8)(HarfBuzzBuffer, const char *, int, unsigned int, int);
typedef void (*HarfBuzzBufferDestroy)(HarfBuzzBuffer);
typedef void (*HarfBuzzBufferGuess)(HarfBuzzBuffer);
typedef void (*HarfBuzzBufferSetDirecton)(HarfBuzzBuffer, HarfBuzzDirection);

typedef HarfBuzzBlob (*HarfBuzzBlobCreate)(const char *);

typedef HarfBuzzFace (*HarfBuzzFaceCreate)(HarfBuzzBlob, unsigned int);

typedef HarfBuzzFont (*HarfBuzzFontCreate)(FT_Face, void *);
typedef HarfBuzzFont (*HarfBuzzFontCreateFromFace)(HarfBuzzFace);
typedef void (*HarfBuzzFontSetup)(HarfBuzzFont);
typedef void (*HarfBuzzFontDestroy)(HarfBuzzFont);
typedef void (*HarfBuzzFontSetupOT)(HarfBuzzFont);

typedef void (*HarfBuzzShape)(HarfBuzzFont, HarfBuzzBuffer, void *, unsigned int);

typedef HarfBuzzGlyphInfo *(*HarfBuzzGetGlyphInfos)(HarfBuzzBuffer, unsigned int *);
typedef HarfBuzzGlyphPos *(*HarfBuzzGetGlyphPositions)(HarfBuzzBuffer, unsigned int *);

typedef struct {
    HarfBuzzBufferCreate buffer_create;
    HarfBuzzBufferAdd buffer_add;
    HarfBuzzBufferAddUTF8 buffer_add_utf8;
    HarfBuzzBufferGuess buffer_guess;
    HarfBuzzBufferDestroy buffer_destroy;
    HarfBuzzBufferSetDirecton buffer_set_direction;

    HarfBuzzBlobCreate blob_create;

    HarfBuzzFaceCreate face_create;

    HarfBuzzFontCreate font_create;
    HarfBuzzFontCreateFromFace font_create_from_face;
    HarfBuzzFontSetup font_setup;
    HarfBuzzFontSetupOT font_setup_ot;
    HarfBuzzFontDestroy font_destroy;

    HarfBuzzShape shape;

    HarfBuzzGetGlyphInfos get_glyph_infos;
    HarfBuzzGetGlyphPositions get_glyph_positions;

    void *library;
} HarfBuzzLibrary;

void HarfBuzz_Init(void);
int HarfBuzz_MeasureAndRender(DonnellImageBuffer *buffer, DonnellSize *size, DonnellPixel *color, FriBidiString *string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellBool return_max_asc, DonnellFontOptions font_options);
void HarfBuzz_Cleanup(void);
HarfBuzzLibrary *HarfBuzz_GetLibrary(void);

#endif
