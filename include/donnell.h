/* Public API for libdonnell */

#ifndef DONNELL
#define DONNELL

typedef unsigned char DonnellUInt8;
typedef unsigned short DonnellUInt16;
typedef unsigned long DonnellUInt32;

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
    unsigned int scale;
} DonnellImageBuffer;

typedef struct {
    DonnellImageBuffer *image;
    char *name;
    DonnellBool alloced_name;
} DonnellStockImage;

typedef struct {
    DonnellImageBuffer **images;
    unsigned int count;
    char *name;
    DonnellBool alloced_name;
} DonnellIcon;

typedef struct {
    DonnellRect top_left;
    DonnellRect top_center;
    DonnellRect top_right;
    DonnellRect center_left;
    DonnellRect center_center;
    DonnellRect center_right;
    DonnellRect bottom_left;
    DonnellRect bottom_center;
    DonnellRect bottom_right;
} DonnellNineSliceRects;

typedef struct {
    DonnellImageBuffer *top_left;
    DonnellImageBuffer *top_center;
    DonnellImageBuffer *top_right;
    DonnellImageBuffer *center_left;
    DonnellImageBuffer *center_center;
    DonnellImageBuffer *center_right;
    DonnellImageBuffer *bottom_left;
    DonnellImageBuffer *bottom_center;
    DonnellImageBuffer *bottom_right;
    unsigned int scale;
    char *name;
    DonnellBool alloced_name;
} DonnellNineSlice;

typedef struct {
    DonnellImageBuffer *img;
    unsigned int scale;
    char *name;
    DonnellBool alloced_name;
} DonnellStockElementStandard;

typedef enum
{
    DONNELL_FONT_OPTIONS_SERIF = 1 << 0,
    DONNELL_FONT_OPTIONS_SANS_SERIF = 1 << 1,
    DONNELL_FONT_OPTIONS_MONOSPACE = 1 << 2,
    DONNELL_FONT_OPTIONS_BOLD = 1 << 3,
    DONNELL_FONT_OPTIONS_ITALIC = 1 << 4
} DonnellFontOptions;

typedef enum
{
    DONNELL_BUTTON_STATE_NORMAL = 1 << 0,
    DONNELL_BUTTON_STATE_HOVER = 1 << 1,
    DONNELL_BUTTON_STATE_PRESSED = 1 << 2,
    DONNELL_BUTTON_STATE_SELECTED = 1 << 3
} DonnellButtonState;

typedef enum
{	
	DONNELL_TITLEBAR_BUTTON_MINIMIZE,
	DONNELL_TITLEBAR_BUTTON_MAXIMIZE,
	DONNELL_TITLEBAR_BUTTON_CLOSE
} DonnellTitlebarButton;

typedef enum
{
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
#define DONNELL_STOCK_IMAGE_BUTTON_HOVER "button-hover"
#define DONNELL_STOCK_IMAGE_BUTTON_NORMAL "button-normal"
#define DONNELL_STOCK_IMAGE_BUTTON_PRESSED "button-pressed"
#define DONNELL_STOCK_IMAGE_BUTTON_HOVER_2X "button-hover-2x"
#define DONNELL_STOCK_IMAGE_BUTTON_NORMAL_2X "button-normal-2x"
#define DONNELL_STOCK_IMAGE_BUTTON_PRESSED_2X "button-pressed-2x"
#define DONNELL_STOCK_IMAGE_BUTTON_HOVER_3X "button-hover-3x"
#define DONNELL_STOCK_IMAGE_BUTTON_NORMAL_3X "button-normal-3x"
#define DONNELL_STOCK_IMAGE_BUTTON_PRESSED_3X "button-pressed-3x"
#define DONNELL_STOCK_IMAGE_BUTTON_SELECTED_HOVER "button-selected-hover"
#define DONNELL_STOCK_IMAGE_BUTTON_SELECTED_NORMAL "button-selected-normal"
#define DONNELL_STOCK_IMAGE_BUTTON_SELECTED_PRESSED "button-selected-pressed"
#define DONNELL_STOCK_IMAGE_BUTTON_SELECTED_HOVER_2X "button-selected-hover-2x"
#define DONNELL_STOCK_IMAGE_BUTTON_SELECTED_NORMAL_2X "button-selected-normal-2x"
#define DONNELL_STOCK_IMAGE_BUTTON_SELECTED_PRESSED_2X "button-selected-pressed-2x"
#define DONNELL_STOCK_IMAGE_BUTTON_SELECTED_HOVER_3X "button-selected-hover-3x"
#define DONNELL_STOCK_IMAGE_BUTTON_SELECTED_NORMAL_3X "button-selected-normal-3x"
#define DONNELL_STOCK_IMAGE_BUTTON_SELECTED_PRESSED_3X "button-selected-pressed-3x"
#define DONNELL_STOCK_IMAGE_CAPTION_BACKGROUND "caption-background"
#define DONNELL_STOCK_IMAGE_CAPTION_BACKGROUND_2X "caption-background-2x"
#define DONNELL_STOCK_IMAGE_CAPTION_BACKGROUND_3X "caption-background-3x"
#define DONNELL_STOCK_IMAGE_CAPTION_CLOSE_PRESSED "caption-close-pressed"
#define DONNELL_STOCK_IMAGE_CAPTION_CLOSE_PRESSED_2X "caption-close-pressed-2x"
#define DONNELL_STOCK_IMAGE_CAPTION_CLOSE_PRESSED_3X "caption-close-pressed-3x"
#define DONNELL_STOCK_IMAGE_CAPTION_CLOSE_NORMAL "caption-close-normal"
#define DONNELL_STOCK_IMAGE_CAPTION_CLOSE_NORMAL_2X "caption-close-normal-2x"
#define DONNELL_STOCK_IMAGE_CAPTION_CLOSE_NORMAL_3X "caption-close-normal-3x"
#define DONNELL_STOCK_IMAGE_CAPTION_CLOSE_HOVER "caption-close-hover"
#define DONNELL_STOCK_IMAGE_CAPTION_CLOSE_HOVER_2X "caption-close-hover-2x"
#define DONNELL_STOCK_IMAGE_CAPTION_CLOSE_HOVER_3X "caption-close-hover-3x"
#define DONNELL_STOCK_IMAGE_CAPTION_MAXIMIZE_PRESSED "caption-maximize-pressed"
#define DONNELL_STOCK_IMAGE_CAPTION_MAXIMIZE_PRESSED_2X "caption-maximize-pressed-2x"
#define DONNELL_STOCK_IMAGE_CAPTION_MAXIMIZE_PRESSED_3X "caption-maximize-pressed-3x"
#define DONNELL_STOCK_IMAGE_CAPTION_MAXIMIZE_NORMAL "caption-maximize-normal"
#define DONNELL_STOCK_IMAGE_CAPTION_MAXIMIZE_NORMAL_2X "caption-maximize-normal-2x"
#define DONNELL_STOCK_IMAGE_CAPTION_MAXIMIZE_NORMAL_3X "caption-maximize-normal-3x"
#define DONNELL_STOCK_IMAGE_CAPTION_MAXIMIZE_HOVER "caption-maximize-hover"
#define DONNELL_STOCK_IMAGE_CAPTION_MAXIMIZE_HOVER_2X "caption-maximize-hover-2x"
#define DONNELL_STOCK_IMAGE_CAPTION_MAXIMIZE_HOVER_3X "caption-maximize-hover-3x"
#define DONNELL_STOCK_IMAGE_CAPTION_MINIMIZE_PRESSED "caption-minimize-pressed"
#define DONNELL_STOCK_IMAGE_CAPTION_MINIMIZE_PRESSED_2X "caption-minimize-pressed-2x"
#define DONNELL_STOCK_IMAGE_CAPTION_MINIMIZE_PRESSED_3X "caption-minimize-pressed-3x"
#define DONNELL_STOCK_IMAGE_CAPTION_MINIMIZE_NORMAL "caption-minimize-normal"
#define DONNELL_STOCK_IMAGE_CAPTION_MINIMIZE_NORMAL_2X "caption-minimize-normal-2x"
#define DONNELL_STOCK_IMAGE_CAPTION_MINIMIZE_NORMAL_3X "caption-minimize-normal-3x"
#define DONNELL_STOCK_IMAGE_CAPTION_MINIMIZE_HOVER "caption-minimize-hover"
#define DONNELL_STOCK_IMAGE_CAPTION_MINIMIZE_HOVER_2X "caption-minimize-hover-2x"
#define DONNELL_STOCK_IMAGE_CAPTION_MINIMIZE_HOVER_3X "caption-minimize-hover-3x"
#define DONNELL_STOCK_ELEMENT_BUTTON_HOVER "button-hover"
#define DONNELL_STOCK_ELEMENT_BUTTON_NORMAL "button-normal"
#define DONNELL_STOCK_ELEMENT_BUTTON_PRESSED "button-pressed"
#define DONNELL_STOCK_ELEMENT_BUTTON_SELECTED_HOVER "button-selected-hover"
#define DONNELL_STOCK_ELEMENT_BUTTON_SELECTED_NORMAL "button-selected-normal"
#define DONNELL_STOCK_ELEMENT_BUTTON_SELECTED_PRESSED "button-selected-pressed"
#define DONNELL_STOCK_ELEMENT_CAPTION_BACKGROUND "caption-background"
#define DONNELL_STOCK_ELEMENT_CAPTION_CLOSE_BUTTON_HOVER "caption-close-hover"
#define DONNELL_STOCK_ELEMENT_CAPTION_MINIMIZE_BUTTON_HOVER "caption-minimize-hover"
#define DONNELL_STOCK_ELEMENT_CAPTION_MAXIMIZE_BUTTON_HOVER "caption-maximize-hover"
#define DONNELL_STOCK_ELEMENT_CAPTION_CLOSE_BUTTON_NORMAL "caption-close-normal"
#define DONNELL_STOCK_ELEMENT_CAPTION_MINIMIZE_BUTTON_NORMAL "caption-minimize-normal"
#define DONNELL_STOCK_ELEMENT_CAPTION_MAXIMIZE_BUTTON_NORMAL "caption-maximize-normal"
#define DONNELL_STOCK_ELEMENT_CAPTION_CLOSE_BUTTON_PRESSED "caption-close-pressed"
#define DONNELL_STOCK_ELEMENT_CAPTION_MINIMIZE_BUTTON_PRESSED "caption-minimize-pressed"
#define DONNELL_STOCK_ELEMENT_CAPTION_MAXIMIZE_BUTTON_PRESSED "caption-maximize-pressed"
#define DONNELL_STOCK_ELEMENT_TITLEBAR_BACKGROUND DONNELL_STOCK_ELEMENT_CAPTION_BACKGROUND
#define DONNELL_STOCK_ICON_ERROR "error"
#define DONNELL_STOCK_ICON_WARNING "warning"
#define DONNELL_STOCK_ICON_INFO "info"

void Donnell_Init(void);
unsigned int Donnell_GetVersionMajor(void);
unsigned int Donnell_GetVersionMinor(void);
void Donnell_Cleanup(void);

void Donnell_Rect_Center(DonnellRect *dest, DonnellRect *src1, DonnellRect *src2);

DonnellPixel *Donnell_Pixel_Create();
DonnellPixel *Donnell_Pixel_CreateEasy(DonnellUInt8 red, DonnellUInt8 green, DonnellUInt8 blue, DonnellUInt8 alpha);
DonnellPixel *Donnell_Pixel_Blend(DonnellPixel *a, DonnellPixel *b);
void Donnell_Pixel_Free(DonnellPixel *pixel);

DonnellStockImage *Donnell_StockImage_Create(DonnellImageBuffer *image, char *name);
DonnellStockImage *Donnell_StockImage_Copy(DonnellStockImage *stock_image);
void Donnell_StockImage_Free(DonnellStockImage *stock_image);
void Donnell_StockImages_Add(DonnellStockImage *stock_image);
DonnellStockImage *Donnell_StockImages_Load(char *name);

DonnellImageBuffer *Donnell_ImageBuffer_Create(unsigned int width, unsigned int height, unsigned int scale);
DonnellImageBuffer *Donnell_ImageBuffer_LoadFromPNG(char *name);
DonnellImageBuffer *Donnell_ImageBuffer_LoadFromInline(char **str);
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
void Donnell_GraphicsPrimitives_DrawRectangle(DonnellImageBuffer *buffer, DonnellPixel *color, DonnellRect *rect, DonnellBool blend);

DonnellIcon *Donnell_GuiPrimitives_Icon_Create(DonnellImageBuffer **images, char *name, unsigned int count);
DonnellIcon *Donnell_GuiPrimitives_Icon_Copy(DonnellIcon *icon);
void Donnell_GuiPrimitives_Icon_Free(DonnellIcon *icon);
unsigned int Donnell_GuiPrimitives_Icon_GetBestForSize(DonnellIcon *icon, unsigned int size, unsigned int scale);
void Donnell_GuiPrimitives_Icon_Draw(DonnellImageBuffer *buffer, DonnellIcon *icon, unsigned int index, unsigned int size, unsigned int x, unsigned int y);

DonnellIcon *Donnell_GuiPrimitives_StockIcons_Load(char *name);
void Donnell_GuiPrimitives_StockIcons_Add(DonnellIcon *icon);

DonnellNineSlice *Donnell_GuiPrimitives_StockElements_Load(char *name, unsigned int scale);
void Donnell_GuiPrimitives_StockElements_Add(DonnellNineSlice *elem);

DonnellNineSlice *Donnell_GuiPrimitives_NineSlice_Create(DonnellImageBuffer *image, DonnellNineSliceRects *rects, unsigned int scale, char *name);
void Donnell_GuiPrimitives_NineSlice_Free(DonnellNineSlice *image);
DonnellNineSlice *Donnell_GuiPrimitives_NineSlice_Copy(DonnellNineSlice *image);
void Donnell_GuiPrimitives_NineSlice_Draw(DonnellImageBuffer *buffer, DonnellNineSlice *image, DonnellRect *rect);

DonnellStockElementStandard *Donnell_GuiPrimitives_StandardStockElement_Create(DonnellImageBuffer *image, unsigned int scale, char *name);
void Donnell_GuiPrimitives_StandardStockElement_Free(DonnellStockElementStandard *image);
DonnellStockElementStandard *Donnell_GuiPrimitives_StandardStockElement_Copy(DonnellStockElementStandard *image);
void Donnell_GuiPrimitives_StandardStockElement_Draw(DonnellImageBuffer *buffer, DonnellStockElementStandard *elem, DonnellRect *rect, DonnellBool stretch_w, DonnellBool stretch_h);

void Donnell_GuiPrimitives_StandardStockElements_Add(DonnellStockElementStandard *elem);
DonnellStockElementStandard *Donnell_GuiPrimitives_StandardStockElements_Load(char *name, unsigned int scale);

void Donnell_GuiPrimitives_DrawButton(DonnellImageBuffer *buffer, char *text, DonnellRect *rect, DonnellPixel *color, unsigned int text_size, DonnellFontOptions text_font_options, DonnellButtonState state, DonnellBool text_stretch);
void Donnell_GuiPrimitives_DrawTitlebarButton(DonnellImageBuffer *buffer, unsigned int x, unsigned int y, DonnellTitlebarButton button, DonnellButtonState state);

#endif
