#include <dlfcn.h>
#include <ft2build.h>
#include <stdint.h>
#include <stdlib.h>
#include FT_FREETYPE_H
#include "harfbuzz.h"

HarfBuzzLibrary *harfbuzz;

void HarfBuzz_Init(void) {
    harfbuzz = NULL;
    harfbuzz = malloc(sizeof(HarfBuzzLibrary));
    if (!harfbuzz) {
        return;
    }

    harfbuzz->library = dlopen("libharfbuzz.so", RTLD_LAZY);
    if (!harfbuzz->library) {
        free(harfbuzz);
        return;
    }

    harfbuzz->buffer_create = dlsym(harfbuzz->library, "hb_buffer_create");
    harfbuzz->buffer_add = dlsym(harfbuzz->library, "hb_buffer_add_utf32");
    harfbuzz->buffer_guess = dlsym(harfbuzz->library, "hb_buffer_guess_segment_properties");

    harfbuzz->font_create = dlsym(harfbuzz->library, "hb_ft_font_create");
    harfbuzz->font_setup = dlsym(harfbuzz->library, "hb_ft_font_set_funcs");

    harfbuzz->shape = dlsym(harfbuzz->library, "hb_shape");

    harfbuzz->get_glyph_infos = dlsym(harfbuzz->library, "hb_buffer_get_glyph_infos");
    harfbuzz->get_glyph_positions = dlsym(harfbuzz->library, "hb_buffer_get_glyph_positions");
}
