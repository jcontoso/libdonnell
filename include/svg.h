#include <stdlib.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#ifdef HAS_NANOSVG
#include <nanosvg.h>
#include <nanosvgrast.h>
#endif

#if (FREETYPE_MINOR >= 12)
#ifndef DONNELL_SVG
#define DONNELL_SVG

typedef void *SVGHandle;
typedef void *SVGSurface;
typedef void *SVGCtx;

typedef enum
{
    SVG_UNIT_PERCENT,
    SVG_UNIT_PX,
    SVG_UNIT_EM,
    SVG_UNIT_EX,
    SVG_UNIT_IN,
    SVG_UNIT_CM,
    SVG_UNIT_MM,
    SVG_UNIT_PT,
    SVG_UNIT_PC
} SVGUnit;

typedef struct {
    double length;
    SVGUnit unit;
} SVGLength;

typedef struct {
    double x;
    double y;
    double width;
    double height;
} SVGRect;

typedef struct {
    int width;
    int height;
    double em;
    double ex;
} SVGDimensionData;

typedef struct {
    double xx;
    double yx;
    double xy;
    double yy;
    double x0;
    double y0;
} SVGMatrix;

typedef struct {
    SVGSurface *rec_surface;
    double x;
    double y;
} SVGRenderer;

typedef enum
{
    CONTENT_COLOR = 0x1000,
    CONTENT_ALPHA = 0x2000,
    CONTENT_COLOR_ALPHA = 0x3000
} SVGCtxContent;

typedef enum
{
    FORMAT_INVALID = -1,
    FORMAT_ARGB32 = 0,
    FORMAT_RGB24 = 1,
    FORMAT_A8 = 2,
    FORMAT_A1 = 3,
    FORMAT_RGB16_565 = 4
} SVGCtxFormat;

typedef SVGHandle (*SVGHandleCreate)(unsigned char *, size_t, void **);
typedef void (*SVGHandleGetIntrinsicDimensions)(SVGHandle, DonnellBool *, SVGLength *, DonnellBool *, SVGLength *, DonnellBool *, SVGRect *);
typedef void (*SVGHandleRenderLayer)(SVGHandle, SVGCtx, const char *, const SVGRect *, void ***);
typedef void (*SVGHandleRenderCSub)(SVGHandle, SVGCtx, const char *);
typedef void (*SVGHandleDestroy)(SVGHandle);

typedef SVGSurface (*SVGCtxCreateRecordingSurface)(SVGCtxContent, const SVGRect *);
typedef SVGSurface (*SVGCtxGetRecordingSurfaceExtents)(SVGSurface, double *, double *, double *, double *);
typedef void (*SVGCtxDestroySurface)(SVGSurface);

typedef SVGSurface (*SVGCtxCreateImageSurface)(unsigned char *, SVGCtxFormat, int, int, int);

typedef SVGCtx (*SVGCtxCreate)(SVGSurface);
typedef void (*SVGCtxFlushSurface)(SVGSurface);
typedef void (*SVGCtxScale)(SVGCtx, double, double);
typedef void (*SVGCtxTransform)(SVGCtx, const SVGMatrix *);
typedef void (*SVGCtxTranslate)(SVGCtx, double, double);
typedef void (*SVGCtxSetSrcSurface)(SVGCtx, SVGSurface, double, double);
typedef void (*SVGCtxPaint)(SVGCtx);
typedef void (*SVGCtxDestroy)(SVGCtx);

typedef struct {
    SVGHandleCreate handle_create;
    SVGHandleGetIntrinsicDimensions handle_getidim;
    SVGHandleRenderLayer handle_render_layer;
    SVGHandleRenderCSub handle_render_csub;
    SVGHandleDestroy handle_destroy;

    SVGCtxCreateImageSurface ctx_create_img_surface;
    SVGCtxCreateRecordingSurface ctx_create_rec_surface;
    SVGCtxGetRecordingSurfaceExtents ctx_get_rec_surface_extents;
    SVGCtxDestroySurface ctx_destroy_surface;
    SVGCtxCreate ctx_create;
    SVGCtxScale ctx_scale;
    SVGCtxTransform ctx_transform;
    SVGCtxTranslate ctx_translate;
    SVGCtxPaint ctx_paint;
    SVGCtxFlushSurface ctx_flush_surface;
    SVGCtxSetSrcSurface ctx_set_src;
    SVGCtxDestroy ctx_destroy;

    void *library;
} SVGLibrary;

#ifdef HAS_NANOSVG
typedef struct {
    DonnellUInt32 cookie;
    NSVGimage *svg;
    float scale;
    DonnellUInt16 glyph_id_start;
    DonnellUInt16 glyph_id_end;
    float x_ofs;
    float y_ofs;
    FT_Error error;
} NanoSVGRenderer;
#endif

void SVG_Init(void);
void SVG_Cleanup(void);
SVGLibrary *SVG_GetLibrary(void);

FT_Error SVGRenderer_Init(FT_Pointer *svg);
void SVGRenderer_Free(FT_Pointer *svg);
FT_Error SVGRenderer_PresetSlot(FT_GlyphSlot slot, FT_Bool cache, FT_Pointer svg);
FT_Error SVGRenderer_Render(FT_GlyphSlot slot, FT_Pointer *svg);

FT_Error NanoSVGRenderer_Init(FT_Pointer *svg);
void NanoSVGRenderer_Free(FT_Pointer *svg);
FT_Error NanoSVGRenderer_PresetSlot(FT_GlyphSlot slot, FT_Bool cache, FT_Pointer svg);
FT_Error NanoSVGRenderer_Render(FT_GlyphSlot slot, FT_Pointer *svg);

#endif
#endif
