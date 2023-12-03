/* Public API for libdonnell */

#ifndef DONNELL
#define DONNELL

typedef unsigned char DonnellUInt8;
typedef enum
{
    DONNELL_FALSE,
    DONNELL_TRUE
} DonnellBool;

typedef struct {
    unsigned int w;
    unsigned int h;
} DonnellSize;

typedef struct {
    unsigned int w;
    unsigned int h;
    unsigned int x;
    unsigned int y;
} DonnellRect;

typedef struct {
    DonnellUInt8 red;
    DonnellUInt8 green;
    DonnellUInt8 blue;
    DonnellUInt8 alpha;
} DonnellPixel;

typedef struct {
    DonnellPixel ***pixels;
    unsigned int width;
    unsigned int height;
} DonnellImageBuffer;

typedef enum
{
    DONNELL_FONT_SERIF,
    DONNELL_FONT_SANS_SERIF,
    DONNELL_FONT_MONOSPACE,
} DonnellFont;

typedef enum
{
    DONNELL_FONT_STYLE_BOLD = 1 << 0,
    DONNELL_FONT_STYLE_ITALIC = 1 << 1
} DonnellFontStyle;

void Donnell_Init(void);
void Donnell_Cleanup(void);

DonnellPixel *Donnell_Pixel_Create();
DonnellPixel *Donnell_Pixel_CreateEasy(DonnellUInt8 red, DonnellUInt8 green, DonnellUInt8 blue, DonnellUInt8 alpha);
void Donnell_Pixel_Free(DonnellPixel *pixel);

DonnellImageBuffer *Donnell_ImageBuffer_Create(unsigned int width, unsigned int height);
void Donnell_ImageBuffer_SetPixel(DonnellImageBuffer *buffer, unsigned int x, unsigned int y, DonnellPixel *pixel);
void Donnell_ImageBuffer_BlendPixel(DonnellImageBuffer *buffer, unsigned int x, unsigned int y, DonnellPixel *pixel);
void Donnell_ImageBuffer_Clear(DonnellImageBuffer *buffer, DonnellPixel *pixel);
void Donnell_ImageBuffer_DumpAsBitmap(DonnellImageBuffer *buffer, char *name);
void Donnell_ImageBuffer_Free(DonnellImageBuffer *buffer);

void Donnell_GraphicsPrimitives_DrawTextLine(DonnellImageBuffer *buffer, DonnellPixel *color, char *utf8string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellFont req_font, DonnellFontStyle font_style);
void Donnell_GraphicsPrimitives_MeasureTextLine(DonnellSize *size, char *utf8string, unsigned int pixel_size, DonnellFont req_font, DonnellFontStyle font_style);
void Donnell_GraphicsPrimitives_DrawText(DonnellImageBuffer *buffer, DonnellPixel *color, char *utf8string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellFont req_font, DonnellFontStyle font_style);
void Donnell_GraphicsPrimitives_MeasureText(DonnellSize *size, char *utf8string, unsigned int pixel_size, DonnellFont req_font, DonnellFontStyle font_style);

#endif
