#include "textrenderer.h"
#include "bidi.h"
#include "donnell.h"
#include "fontconfig.h"
#include "freetype.h"
#include "harfbuzz.h"
#include "svg.h"
#include "textutils.h"

TextRendererFunction text_renderer;

int TextRenderer_MeasureAndRender(DonnellImageBuffer *buffer, DonnellSize *size, DonnellPixel *color, FriBidiString *string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellBool return_max_asc, DonnellFontOptions font_options) {
    return text_renderer(buffer, size, color, string, x, y, pixel_size, return_max_asc, font_options);
}

void TextRenderer_Init(void) {
    SVG_Init();
    FontConfig_Init();
    FreeType_Init();

    text_renderer = &FreeType_MeasureAndRender;

    /*HarfBuzz_Init();*/
    if (HarfBuzz_GetLibrary()) {
        text_renderer = &HarfBuzz_MeasureAndRender;
    }
}

void TextRenderer_Cleanup(void) {
    FreeType_Cleanup();
    FontConfig_Cleanup();
    HarfBuzz_Cleanup();
    SVG_Cleanup();
}
