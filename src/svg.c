#include <dlfcn.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OTSVG_H

#include "donnell.h"
#include "svg.h"

SVGLibrary *svg_library;

void SVG_Init(void) {
    char *error;

    error = NULL;
    svg_library = NULL;

    svg_library = malloc(sizeof(SVGLibrary));
    if (!svg_library) {
        return;
    }

    svg_library->library = dlopen("librsvg-2.so.2", RTLD_LAZY);
    if (!svg_library->library) {
        free(svg_library);
        svg_library = NULL;
        return;
    }

    svg_library->handle_create = dlsym(svg_library->library, "rsvg_handle_new_from_data");
    svg_library->handle_getidim = dlsym(svg_library->library, "rsvg_handle_get_intrinsic_dimensions");
    svg_library->handle_render_layer = dlsym(svg_library->library, "rsvg_handle_render_layer");
    error = dlerror();
    if ((error) || (!svg_library->handle_render_layer)) {
        svg_library->handle_render_layer = NULL;
        error = NULL;
    }
    svg_library->handle_render_csub = dlsym(svg_library->library, "rsvg_handle_render_cairo_sub");
    svg_library->handle_destroy = dlsym(svg_library->library, "rsvg_handle_free");

    svg_library->ctx_create_img_surface = dlsym(svg_library->library, "cairo_image_surface_create_for_data");
    svg_library->ctx_create_rec_surface = dlsym(svg_library->library, "cairo_recording_surface_create");
    svg_library->ctx_create = dlsym(svg_library->library, "cairo_create");
    svg_library->ctx_scale = dlsym(svg_library->library, "cairo_scale");
    svg_library->ctx_transform = dlsym(svg_library->library, "cairo_transform");
    svg_library->ctx_translate = dlsym(svg_library->library, "cairo_translate");
    svg_library->ctx_set_src = dlsym(svg_library->library, "cairo_set_source_surface");
    svg_library->ctx_paint = dlsym(svg_library->library, "cairo_paint");
    svg_library->ctx_flush_surface = dlsym(svg_library->library, "cairo_surface_flush");

    svg_library->ctx_get_rec_surface_extents = dlsym(svg_library->library, "cairo_recording_surface_ink_extents");
    svg_library->ctx_destroy = dlsym(svg_library->library, "cairo_destroy");
    svg_library->ctx_destroy_surface = dlsym(svg_library->library, "cairo_surface_destroy");

    error = dlerror();
    if (error) {
        free(svg_library);
        svg_library = NULL;
        return;
    }
}

SVGLibrary *SVG_GetLibrary(void) {
    return svg_library;
}

void SVG_Cleanup(void) {
    if (!svg_library) {
        return;
    }

    dlclose(svg_library->library);
    free(svg_library);
}

FT_Error SVGRenderer_Init(FT_Pointer *svg) {
    *svg = malloc(sizeof(SVGRenderer));
    return FT_Err_Ok;
}

void SVGRenderer_Free(FT_Pointer *svg) {
    free(*svg);
}

FT_Error SVGRenderer_Render(FT_GlyphSlot slot, FT_Pointer *svg) {
    SVGRenderer *renderer;
    SVGSurface *surface;
    SVGCtx *ctx;

    renderer = *(SVGRenderer **)svg;

    surface = svg_library->ctx_create_img_surface(slot->bitmap.buffer, FORMAT_ARGB32, (int)slot->bitmap.width, (int)slot->bitmap.rows, slot->bitmap.pitch);

    ctx = svg_library->ctx_create(surface);

    svg_library->ctx_translate(ctx, -renderer->x, -renderer->y);

    svg_library->ctx_set_src(ctx, renderer->rec_surface, 0.0, 0.0);
    svg_library->ctx_paint(ctx);

    svg_library->ctx_flush_surface(surface);

    slot->bitmap.pixel_mode = FT_PIXEL_MODE_BGRA;
    slot->bitmap.num_grays = 256;
    slot->format = FT_GLYPH_FORMAT_BITMAP;

    svg_library->ctx_destroy_surface(surface);
    svg_library->ctx_destroy(ctx);
    svg_library->ctx_destroy_surface(renderer->rec_surface);

    return FT_Err_Ok;
}

FT_Error SVGRenderer_PresetSlot(FT_GlyphSlot slot, FT_Bool cache, FT_Pointer svg) {
    FT_SVG_Document document;
    FT_Size_Metrics metrics;
    FT_UShort units_per_em;
    FT_UShort end_glyph;
    FT_UShort start_glyph;
    SVGCtx ctx;
    SVGMatrix matrix;
    SVGHandle handle;
    SVGLength w;
    SVGLength h;
    SVGRect viewbox;
    SVGDimensionData dimension;
    DonnellBool has_w;
    DonnellBool has_h;
    DonnellBool has_viewbox;
    SVGRenderer *renderer;
    SVGRenderer crenderer;
    double x;
    double y;
    double xx;
    double xy;
    double yx;
    double yy;
    double x0;
    double y0;
    double width;
    double height;
    double x_svg;
    double y_svg;
    double dtemp;
    float metrics_width;
    float metrics_height;
    float hbearing_x;
    float hbearing_y;
    float vbearing_x;
    float vbearing_y;
    float ftemp;
    char *id;
    char str[32];

    document = (FT_SVG_Document)slot->other;
    metrics = document->metrics;
    units_per_em = document->units_per_EM;
    end_glyph = document->end_glyph_id;
    start_glyph = document->start_glyph_id;

    if (cache) {
        renderer = *(SVGRenderer **)svg;
    } else {
        renderer = &crenderer;
    }

    handle = svg_library->handle_create(document->svg_document, document->svg_document_length, NULL);
    svg_library->handle_getidim(handle, &has_w, &w, &has_h, &h, &has_viewbox, &viewbox);

    if (has_viewbox) {
        dimension.width = (int)viewbox.width;
        dimension.height = (int)viewbox.height;
    } else if ((has_w) && (has_h)) {
        dimension.width = (int)w.length;
        dimension.height = (int)h.length;
        if ((w.length) && (h.length)) {
            dimension.width = units_per_em;
            dimension.height = units_per_em;
        }
    } else {
        dimension.width = units_per_em;
        dimension.height = units_per_em;
    }

    x_svg = (double)metrics.x_ppem / dimension.width;
    y_svg = (double)metrics.y_ppem / dimension.height;

    renderer->rec_surface = svg_library->ctx_create_rec_surface(CONTENT_COLOR_ALPHA, NULL);
    ctx = svg_library->ctx_create(renderer->rec_surface);

    xx = (double)document->transform.xx / (1 << 16);
    xy = -(double)document->transform.xy / (1 << 16);
    yx = -(double)document->transform.yx / (1 << 16);
    yy = (double)document->transform.yy / (1 << 16);
    x0 = (double)document->delta.x / 64 * dimension.width / metrics.x_ppem;
    y0 = -(double)document->delta.y / 64 * dimension.height / metrics.y_ppem;

    matrix.xx = xx;
    matrix.yx = yx;
    matrix.xy = xy;
    matrix.yy = yy;
    matrix.x0 = x0;
    matrix.y0 = y0;

    svg_library->ctx_scale(ctx, x_svg, y_svg);
    svg_library->ctx_transform(ctx, &matrix);

    if (start_glyph < end_glyph) {
        snprintf(str, sizeof(str), "#glyph%u", slot->glyph_index);
        id = str;
    } else {
        id = NULL;
    }

    if (svg_library->handle_render_layer) {
        SVGRect viewport;

        viewport.x = 0;
        viewport.y = 0;
        viewport.width = (double)dimension.width;
        viewport.height = (double)dimension.height;

        svg_library->handle_render_layer(handle, ctx, id, &viewport, NULL);
    } else {
        svg_library->handle_render_csub(handle, ctx, id);
    }

    svg_library->ctx_get_rec_surface_extents(renderer->rec_surface, &x, &y, &width, &height);

    renderer->x = x;
    renderer->y = y;
    slot->bitmap_left = (FT_Int)renderer->x;
    slot->bitmap_top = (FT_Int)-renderer->y;
    dtemp = ceil(height);
    slot->bitmap.rows = (unsigned int)dtemp;
    dtemp = ceil(width);
    slot->bitmap.width = (unsigned int)dtemp;
    slot->bitmap.pitch = (int)slot->bitmap.width * 4;
    slot->bitmap.pixel_mode = FT_PIXEL_MODE_BGRA;
    metrics_width = (float)width;
    metrics_height = (float)height;
    hbearing_x = (float)renderer->x;
    hbearing_y = (float)-renderer->y;
    vbearing_x = slot->metrics.horiBearingX / 64.0f - slot->metrics.horiAdvance / 64.0f / 2;
    vbearing_y = (slot->metrics.vertAdvance / 64.0f - slot->metrics.height / 64.0f) / 2;
    ftemp = roundf(metrics_width * 64);
    slot->metrics.width = (FT_Pos)ftemp;
    ftemp = roundf(metrics_height * 64);
    slot->metrics.height = (FT_Pos)ftemp;
    slot->metrics.horiBearingX = (FT_Pos)(hbearing_x * 64);
    slot->metrics.horiBearingY = (FT_Pos)(hbearing_y * 64);
    slot->metrics.vertBearingX = (FT_Pos)(vbearing_x * 64);
    slot->metrics.vertBearingY = (FT_Pos)(vbearing_y * 64);
    if (slot->metrics.vertAdvance == 0) {
        slot->metrics.vertAdvance = (FT_Pos)(metrics_height * 1.2f * 64);
    }

    if (cache) {
        svg_library->ctx_destroy(ctx);
        svg_library->handle_destroy(handle);
        return FT_Err_Ok;
    }

    svg_library->ctx_destroy_surface(renderer->rec_surface);
    svg_library->ctx_destroy(ctx);
    svg_library->handle_destroy(handle);
    return FT_Err_Ok;
}
