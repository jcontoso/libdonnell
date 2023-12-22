/* Public API for libdonnell */

#ifndef DONNELL
#define DONNELL

typedef unsigned char DonnellUInt8;
typedef unsigned short DonnellUInt16;
typedef enum {
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
    unsigned int scale;
} DonnellImageBuffer;

typedef struct {
	DonnellImageBuffer* image;
	char* name;
	DonnellBool alloced_name;
} DonnellStockImage;

typedef struct {
	DonnellImageBuffer** images;
	unsigned int count;
	char* name;
	DonnellBool alloced_name;
} DonnellIcon;

typedef enum {
    DONNELL_FONT_OPTIONS_SERIF = 1 << 0,
    DONNELL_FONT_OPTIONS_SANS_SERIF = 1 << 1,
    DONNELL_FONT_OPTIONS_MONOSPACE = 1 << 2,
    DONNELL_FONT_OPTIONS_BOLD = 1 << 3,
    DONNELL_FONT_OPTIONS_ITALIC = 1 << 4
} DonnellFontOptions;

typedef enum {
    DONNELL_SCALING_ALGORITHM_NEAREST_NEIGHBOR,
    DONNELL_SCALING_ALGORITHM_BILINEAR,
} DonnellScalingAlgorithm;

#define DONNELL_ICON_SIZE_16 16
#define DONNELL_ICON_SIZE_32 32
#define DONNELL_ICON_SIZE_64 64
#define DONNELL_ICON_SIZE_96 96

#define DONNELL_STOCK_IMAGE_ERROR_16 "error-16"
#define DONNELL_STOCK_IMAGE_ERROR_32 "error-32"
#define DONNELL_STOCK_IMAGE_ERROR_64 "error-64"
#define DONNELL_STOCK_IMAGE_ERROR_96 "error-96"

#define DONNELL_STOCK_IMAGE_INFO_16 "info-16"
#define DONNELL_STOCK_IMAGE_INFO_32 "info-32"
#define DONNELL_STOCK_IMAGE_INFO_64 "info-64"
#define DONNELL_STOCK_IMAGE_INFO_96 "info-96"

#define DONNELL_STOCK_IMAGE_WARNING_16 "warning-16"
#define DONNELL_STOCK_IMAGE_WARNING_32 "warning-32"
#define DONNELL_STOCK_IMAGE_WARNING_64 "warning-64"
#define DONNELL_STOCK_IMAGE_WARNING_96 "warning-96"

#define DONNELL_STOCK_ICON_ERROR "error"
#define DONNELL_STOCK_ICON_WARNING "warning"
#define DONNELL_STOCK_ICON_INFO "info"


void Donnell_Init(void);
void Donnell_Cleanup(void);

DonnellPixel *Donnell_Pixel_Create();
DonnellPixel *Donnell_Pixel_CreateEasy(DonnellUInt8 red, DonnellUInt8 green, DonnellUInt8 blue, DonnellUInt8 alpha);
DonnellPixel *Donnell_Pixel_Blend(DonnellPixel *a, DonnellPixel *b);
void Donnell_Pixel_Free(DonnellPixel *pixel);

DonnellStockImage* Donnell_StockImage_Create(DonnellImageBuffer* image, char* name);
DonnellStockImage* Donnell_StockImage_Copy(DonnellStockImage* stock_image);
void Donnell_StockImage_Free(DonnellStockImage* stock_image);
void Donnell_StockImages_Add(DonnellStockImage* stock_image);
DonnellStockImage* Donnell_StockImages_Load(char* name);

DonnellImageBuffer *Donnell_ImageBuffer_Create(unsigned int width, unsigned int height, unsigned int scale);
DonnellImageBuffer *Donnell_ImageBuffer_LoadFromPNG(char* name);
DonnellImageBuffer *Donnell_ImageBuffer_LoadFromInline(char** str);
DonnellImageBuffer *Donnell_ImageBuffer_Copy(DonnellImageBuffer *src);
void Donnell_ImageBuffer_SetPixel(DonnellImageBuffer *buffer, unsigned int x, unsigned int y, DonnellPixel *pixel);
DonnellPixel *Donnell_ImageBuffer_GetPixel(DonnellImageBuffer *buffer, unsigned int x, unsigned int y);
void Donnell_ImageBuffer_BlendPixel(DonnellImageBuffer *buffer, unsigned int x, unsigned int y, DonnellPixel *pixel);
void Donnell_ImageBuffer_Clear(DonnellImageBuffer *buffer, DonnellPixel *pixel);
void Donnell_ImageBuffer_DumpAsPNG(DonnellImageBuffer *buffer, char *name);
DonnellImageBuffer *Donnell_ImageBuffer_Scale(DonnellImageBuffer *buffer, unsigned int width, unsigned int height, DonnellScalingAlgorithm algo);
DonnellImageBuffer *Donnell_ImageBuffer_Crop(DonnellImageBuffer *buffer, DonnellRect *rect, DonnellBool ignore_xy);
void Donnell_ImageBuffer_BlendBufferContents(DonnellImageBuffer *buffer, DonnellImageBuffer *cbuffer, DonnellRect *srect, DonnellRect *drect);
void Donnell_ImageBuffer_Free(DonnellImageBuffer *buffer);

void Donnell_GraphicsPrimitives_DrawTextLine(DonnellImageBuffer *buffer, DonnellPixel *color, char *utf8string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellFontOptions font_options);
void Donnell_GraphicsPrimitives_MeasureTextLine(DonnellSize *size, char *utf8string, unsigned int pixel_size, DonnellFontOptions font_options, unsigned int pixel_scale);
void Donnell_GraphicsPrimitives_DrawText(DonnellImageBuffer *buffer, DonnellPixel *color, char *utf8string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellFontOptions font_options);
void Donnell_GraphicsPrimitives_MeasureText(DonnellSize *size, char *utf8string, unsigned int pixel_size, DonnellFontOptions font_options, unsigned int pixel_scale);

DonnellIcon* Donnell_GuiPrimitives_Icon_Create(DonnellImageBuffer** images, char* name, unsigned int count);
DonnellIcon* Donnell_GuiPrimitives_Icon_Copy(DonnellIcon* icon);
void Donnell_GuiPrimitives_Icon_Free(DonnellIcon* icon);
unsigned int Donnell_GuiPrimitives_Icon_GetBestForSize(DonnellImageBuffer *buffer, DonnellIcon* icon, unsigned int size);
void Donnell_GuiPrimitives_Icon_Draw(DonnellImageBuffer *buffer, DonnellIcon* icon, unsigned int index, unsigned int size, unsigned int x, unsigned int y);

DonnellIcon* Donnell_GuiPrimitives_StockIcons_Load(char* name);
#endif
