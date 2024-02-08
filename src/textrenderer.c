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
    DonnellSize s;
    DonnellSize c;
    Runs *runs;
    int ret;
    unsigned int i;

    ret = 0;

    if (!string) {
        return text_renderer(buffer, size, color, string, x, y, pixel_size, return_max_asc, font_options);
    }

    if (size) {
        size->w = 0;
        size->h = 0;
    }
    runs = TextUtils_Runs_Create(string);

    for (i = 0; i < runs->count; i++) {
        if (!runs->str[i]) {
            continue;
        }

        if ((!buffer) && (!color) && (size)) {
            text_renderer(buffer, &s, color, runs->str[i], x, y, pixel_size, return_max_asc, font_options);
            size->w += s.w;
            if (s.h > size->h) {
                size->h = s.h;
            }
        } else if ((!buffer) && (!color) && (!size)) {
            int ln;

            ln = text_renderer(buffer, size, color, runs->str[i], x, y, pixel_size, return_max_asc, font_options);
            if (ln > ret) {
                ret = ln;
            }
        } else {
            int p;

            if (!i) {
                TextRenderer_MeasureAndRender(NULL, &c, NULL, string, x, y, pixel_size, DONNELL_TRUE, font_options);
            }

            text_renderer(NULL, &s, NULL, runs->str[i], x, y, pixel_size, DONNELL_TRUE, font_options);

            p = y + (c.h - s.h);
            text_renderer(buffer, size, color, runs->str[i], x, p, pixel_size, return_max_asc, font_options);
            x += s.w;
        }
    }

    TextUtils_Runs_Free(runs);
    return ret;
}

void TextRenderer_Init(void) {
#if (FREETYPE_MINOR >= 12)
    SVG_Init();
#endif
    FontConfig_Init();
    FreeType_Init();

    text_renderer = &FreeType_MeasureAndRender;

    HarfBuzz_Init();
    if (HarfBuzz_GetLibrary()) {
        text_renderer = &HarfBuzz_MeasureAndRender;
    }
}

void TextRenderer_Cleanup(void) {
    FreeType_Cleanup();
    FontConfig_Cleanup();
    HarfBuzz_Cleanup();
#if (FREETYPE_MINOR >= 12)
    SVG_Cleanup();
#endif
}
