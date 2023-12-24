#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "donnell.h"
#include "guiprimitives.h"
#include "misc.h"
#include "symvis.h"

DonnellIcon **stock_icons;
DonnellNineSlice **stock_guielems;
unsigned int stock_icons_count;
unsigned int stock_guielems_count;

DONNELL_EXPORT DonnellIcon *Donnell_GuiPrimitives_Icon_Create(DonnellImageBuffer **images, char *name, unsigned int count) {
    DonnellIcon *icon;
    unsigned int i;

    if ((!images) || (count < 1)) {
        return NULL;
    }

    icon = malloc(sizeof(DonnellIcon));
    if (!icon) {
        return NULL;
    }

    icon->alloced_name = DONNELL_TRUE;
    icon->name = strdup(name);
    icon->images = calloc(count, sizeof(DonnellImageBuffer *));
    icon->count = count;
    for (i = 0; i < count; i++) {
        icon->images[i] = Donnell_ImageBuffer_Copy(images[i]);
    }

    return icon;
}

DONNELL_EXPORT DonnellIcon *Donnell_GuiPrimitives_Icon_Copy(DonnellIcon *icon) {
    if (!icon) {
        return NULL;
    }
    return Donnell_GuiPrimitives_Icon_Create(icon->images, icon->name, icon->count);
}

DONNELL_EXPORT void Donnell_GuiPrimitives_Icon_Free(DonnellIcon *icon) {
    unsigned int i;

    if (!icon) {
        return;
    }

    if (icon->alloced_name) {
        free(icon->name);
    }

    for (i = 0; i < icon->count; i++) {
        Donnell_ImageBuffer_Free(icon->images[i]);
    }
    free(icon->images);
    free(icon);
}

DONNELL_EXPORT DonnellIcon *Donnell_GuiPrimitives_StockIcons_Load(char *name) {
    unsigned int i;

    if (!name) {
        return NULL;
    }

    for (i = 0; i < stock_icons_count; i++) {
        if (!strcmp(stock_icons[i]->name, name)) {
            return Donnell_GuiPrimitives_Icon_Copy(stock_icons[i]);
        }
    }

    return NULL;
}

DONNELL_EXPORT void Donnell_GuiPrimitives_StockIcons_Add(DonnellIcon *icon) {
    unsigned int i;

    if (!icon) {
        return;
    }

    for (i = 0; i < stock_icons_count; i++) {
        if (!strcmp(stock_icons[i]->name, icon->name)) {
            return;
        }
    }

    stock_icons_count++;
    if (stock_icons_count == 1) {
        stock_icons = calloc(stock_icons_count, sizeof(DonnellIcon *));
    } else {
        stock_icons = realloc(stock_icons, stock_icons_count * sizeof(DonnellIcon *));
    }

    stock_icons[stock_icons_count - 1] = Donnell_GuiPrimitives_Icon_Copy(icon);
}

DONNELL_EXPORT unsigned int Donnell_GuiPrimitives_Icon_GetBestForSize(DonnellIcon *icon, unsigned int size, unsigned int scale) {
    unsigned int diff;
    unsigned int best;
    unsigned int i;

    if ((!icon) || (size < 1)) {
        return 0;
    }

    size = size * scale;

    best = 0;
    diff = abs(size - icon->images[0]->width);

    for (i = 0; i < icon->count; ++i) {
        unsigned int cdiff;

        cdiff = abs(size - icon->images[i]->width);

        if (cdiff < diff) {
            best = i;
            diff = cdiff;
        }
    }

    return best;
}

DONNELL_EXPORT void Donnell_GuiPrimitives_Icon_Draw(DonnellImageBuffer *buffer, DonnellIcon *icon, unsigned int index, unsigned int size, unsigned int x, unsigned int y) {
    DonnellRect rect;
    DonnellImageBuffer *img;
    DonnellBool scaled;

    if ((!buffer) || (!icon) || (index < 0)) {
        return;
    }

    if (size * buffer->scale != icon->images[index]->width) {
        img = Donnell_ImageBuffer_Scale(icon->images[index], size * buffer->scale, size * buffer->scale, DONNELL_SCALING_ALGORITHM_BILINEAR);
        scaled = DONNELL_TRUE;
    } else {
        img = icon->images[index];
        scaled = DONNELL_FALSE;
    }

    rect.w = img->width;
    rect.h = img->height;
    rect.x = x * buffer->scale;
    rect.y = y * buffer->scale;

    Donnell_ImageBuffer_BlendBufferContents(buffer, img, NULL, &rect);

    if (scaled) {
        Donnell_ImageBuffer_Free(img);
    }
}

DONNELL_EXPORT DonnellNineSlice *Donnell_GuiPrimitives_StockElements_Load(char *name, unsigned int scale) {
    unsigned int diff;
    unsigned int i;
    int best;

    if (!name) {
        return NULL;
    }

    for (i = 0; i < stock_guielems_count; i++) {
        if ((!strcmp(stock_guielems[i]->name, name)) && (stock_guielems[i]->scale == scale)) {
            return Donnell_GuiPrimitives_NineSlice_Copy(stock_guielems[i]);
        }
    }

    best = -1;
    diff = abs(scale - stock_guielems[0]->scale);
    for (i = 0; i < stock_guielems_count; i++) {
        if (!strcmp(stock_guielems[i]->name, name)) {
			unsigned int cdiff;

			cdiff = abs(scale - stock_guielems[i]->scale);

			if (cdiff < diff) {
				best = i;
				diff = cdiff;
			}
		}
    }

	if (best > 0) {
		return Donnell_GuiPrimitives_NineSlice_Copy(stock_guielems[best]);		
	}
    return NULL;
}

DONNELL_EXPORT void Donnell_GuiPrimitives_StockElements_Add(DonnellNineSlice *elem) {
    unsigned int i;

    if (!elem) {
        return;
    }

    for (i = 0; i < stock_guielems_count; i++) {
        if ((!strcmp(stock_guielems[i]->name, elem->name)) && (stock_guielems[i]->scale == elem->scale)) {
            return;
        }
    }

    stock_guielems_count++;
    if (stock_guielems_count == 1) {
        stock_guielems = calloc(stock_guielems_count, sizeof(DonnellNineSlice *));
    } else {
        stock_guielems = realloc(stock_guielems, stock_guielems_count * sizeof(DonnellNineSlice *));
    }

    stock_guielems[stock_guielems_count - 1] = Donnell_GuiPrimitives_NineSlice_Copy(elem);
}

void GuiPrimitives_Init() {
    DonnellImageBuffer **images;
    DonnellStockImage *stock16;
    DonnellStockImage *stock32;
    DonnellStockImage *stock64;
    DonnellStockImage *stock96;
    DonnellIcon *stock_icon;
    DonnellStockImage *stock_image;
    DonnellNineSlice *sliced_image;
    DonnellNineSliceRects rects;

    stock_icons_count = 0;
    stock_guielems_count = 0;
    images = calloc(4, sizeof(DonnellImageBuffer *));

    stock16 = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_ERROR_16);
    stock32 = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_ERROR_32);
    stock64 = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_ERROR_64);
    stock96 = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_ERROR_96);
    images[0] = stock16->image;
    images[1] = stock32->image;
    images[2] = stock64->image;
    images[3] = stock96->image;
    stock_icon = Donnell_GuiPrimitives_Icon_Create(images, DONNELL_STOCK_ICON_ERROR, 4);
    Donnell_GuiPrimitives_StockIcons_Add(stock_icon);
    Donnell_StockImage_Free(stock16);
    Donnell_StockImage_Free(stock32);
    Donnell_StockImage_Free(stock64);
    Donnell_StockImage_Free(stock96);
    Donnell_GuiPrimitives_Icon_Free(stock_icon);
    stock16 = stock32 = stock64 = stock96 = NULL;

    stock16 = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_WARNING_16);
    stock32 = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_WARNING_32);
    stock64 = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_WARNING_64);
    stock96 = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_WARNING_96);
    images[0] = stock16->image;
    images[1] = stock32->image;
    images[2] = stock64->image;
    images[3] = stock96->image;
    stock_icon = Donnell_GuiPrimitives_Icon_Create(images, DONNELL_STOCK_ICON_WARNING, 4);
    Donnell_GuiPrimitives_StockIcons_Add(stock_icon);
    Donnell_StockImage_Free(stock16);
    Donnell_StockImage_Free(stock32);
    Donnell_StockImage_Free(stock64);
    Donnell_StockImage_Free(stock96);
    Donnell_GuiPrimitives_Icon_Free(stock_icon);
    stock16 = stock32 = stock64 = stock96 = NULL;

    stock16 = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_INFO_16);
    stock32 = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_INFO_32);
    stock64 = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_INFO_64);
    stock96 = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_INFO_96);
    images[0] = stock16->image;
    images[1] = stock32->image;
    images[2] = stock64->image;
    images[3] = stock96->image;
    stock_icon = Donnell_GuiPrimitives_Icon_Create(images, DONNELL_STOCK_ICON_INFO, 4);
    Donnell_GuiPrimitives_StockIcons_Add(stock_icon);
    Donnell_StockImage_Free(stock16);
    Donnell_StockImage_Free(stock32);
    Donnell_StockImage_Free(stock64);
    Donnell_StockImage_Free(stock96);
    Donnell_GuiPrimitives_Icon_Free(stock_icon);
    stock16 = stock32 = stock64 = stock96 = NULL;
    free(images);

    rects.top_left = (DonnellRect){7, 6, 0, 0};
    rects.center_left = (DonnellRect){7, 15, 0, 6};
    rects.bottom_left = (DonnellRect){7, 6, 0, 21};
    rects.top_center = (DonnellRect){23, 6, 7, 0};
    rects.center_center = (DonnellRect){23, 15, 7, 6};
    rects.bottom_center = (DonnellRect){23, 6, 7, 21};
    rects.top_right = (DonnellRect){7, 6, 30, 0};
    rects.center_right = (DonnellRect){7, 15, 30, 6};
    rects.bottom_right = (DonnellRect){7, 6, 30, 21};

    stock_image = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_BUTTON_NORMAL);
    sliced_image = Donnell_GuiPrimitives_NineSlice_Create(stock_image->image, &rects, 1, DONNELL_STOCK_ELEMENT_BUTTON_NORMAL);
    Donnell_GuiPrimitives_StockElements_Add(sliced_image);
    Donnell_GuiPrimitives_NineSlice_Free(sliced_image);
    Donnell_StockImage_Free(stock_image);

    stock_image = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_BUTTON_HOVER);
    sliced_image = Donnell_GuiPrimitives_NineSlice_Create(stock_image->image, &rects, 1, DONNELL_STOCK_ELEMENT_BUTTON_HOVER);
    Donnell_GuiPrimitives_StockElements_Add(sliced_image);
    Donnell_GuiPrimitives_NineSlice_Free(sliced_image);
    Donnell_StockImage_Free(stock_image);

    stock_image = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_BUTTON_PRESSED);
    sliced_image = Donnell_GuiPrimitives_NineSlice_Create(stock_image->image, &rects, 1, DONNELL_STOCK_ELEMENT_BUTTON_PRESSED);
    Donnell_GuiPrimitives_StockElements_Add(sliced_image);
    Donnell_GuiPrimitives_NineSlice_Free(sliced_image);
    Donnell_StockImage_Free(stock_image);

    rects.top_left = (DonnellRect){14, 12, 0, 0};
    rects.center_left = (DonnellRect){7 * 2, 15 * 2, 0, 6 * 2};
    rects.bottom_left = (DonnellRect){14, 12, 0, 42};
    rects.top_center = (DonnellRect){46, 12, 14, 0};
    rects.center_center = (DonnellRect){46, 30, 14, 12};
    rects.bottom_center = (DonnellRect){46, 12, 14, 42};
    rects.top_right = (DonnellRect){14, 12, 60, 0};
    rects.center_right = (DonnellRect){14, 30, 60, 12};
    rects.bottom_right = (DonnellRect){14, 12, 60, 42};

    stock_image = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_BUTTON_NORMAL_2X);
    sliced_image = Donnell_GuiPrimitives_NineSlice_Create(stock_image->image, &rects, 2, DONNELL_STOCK_ELEMENT_BUTTON_NORMAL);
    Donnell_GuiPrimitives_StockElements_Add(sliced_image);
    Donnell_GuiPrimitives_NineSlice_Free(sliced_image);
    Donnell_StockImage_Free(stock_image);

    stock_image = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_BUTTON_HOVER_2X);
    sliced_image = Donnell_GuiPrimitives_NineSlice_Create(stock_image->image, &rects, 2, DONNELL_STOCK_ELEMENT_BUTTON_HOVER);
    Donnell_GuiPrimitives_StockElements_Add(sliced_image);
    Donnell_GuiPrimitives_NineSlice_Free(sliced_image);
    Donnell_StockImage_Free(stock_image);

    stock_image = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_BUTTON_PRESSED_2X);
    sliced_image = Donnell_GuiPrimitives_NineSlice_Create(stock_image->image, &rects, 2, DONNELL_STOCK_ELEMENT_BUTTON_PRESSED);
    Donnell_GuiPrimitives_StockElements_Add(sliced_image);
    Donnell_GuiPrimitives_NineSlice_Free(sliced_image);
    Donnell_StockImage_Free(stock_image);

    rects.top_left = (DonnellRect){21, 18, 0, 0};
    rects.center_left = (DonnellRect){21, 45, 0, 18};
    rects.bottom_left = (DonnellRect){7 * 3, 6 * 3, 0, 21 * 3};
    rects.top_center = (DonnellRect){23 * 3, 6 * 3, 7 * 3, 0};
    rects.center_center = (DonnellRect){23 * 3, 15 * 3, 7 * 3, 6 * 3};
    rects.bottom_center = (DonnellRect){23 * 3, 6 * 3, 7 * 3, 21 * 3};
    rects.top_right = (DonnellRect){7 * 3, 6 * 3, 30 * 3, 0};
    rects.center_right = (DonnellRect){7 * 3, 15 * 3, 30 * 3, 6 * 3};
    rects.bottom_right = (DonnellRect){7 * 3, 6 * 3, 30 * 3, 21 * 3};

    stock_image = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_BUTTON_NORMAL_3X);
    sliced_image = Donnell_GuiPrimitives_NineSlice_Create(stock_image->image, &rects, 3, DONNELL_STOCK_ELEMENT_BUTTON_NORMAL);
    Donnell_GuiPrimitives_StockElements_Add(sliced_image);
    Donnell_GuiPrimitives_NineSlice_Free(sliced_image);
    Donnell_StockImage_Free(stock_image);

    stock_image = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_BUTTON_HOVER_3X);
    sliced_image = Donnell_GuiPrimitives_NineSlice_Create(stock_image->image, &rects, 3, DONNELL_STOCK_ELEMENT_BUTTON_HOVER);
    Donnell_GuiPrimitives_StockElements_Add(sliced_image);
    Donnell_GuiPrimitives_NineSlice_Free(sliced_image);
    Donnell_StockImage_Free(stock_image);

    stock_image = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_BUTTON_PRESSED_3X);
    sliced_image = Donnell_GuiPrimitives_NineSlice_Create(stock_image->image, &rects, 3, DONNELL_STOCK_ELEMENT_BUTTON_PRESSED);
    Donnell_GuiPrimitives_StockElements_Add(sliced_image);
    Donnell_GuiPrimitives_NineSlice_Free(sliced_image);
    Donnell_StockImage_Free(stock_image);

    rects.top_left = (DonnellRect){7, 6, 0, 0};
    rects.center_left = (DonnellRect){7, 15, 0, 6};
    rects.bottom_left = (DonnellRect){7, 6, 0, 21};
    rects.top_center = (DonnellRect){23, 6, 7, 0};
    rects.center_center = (DonnellRect){23, 15, 7, 6};
    rects.bottom_center = (DonnellRect){23, 6, 7, 21};
    rects.top_right = (DonnellRect){7, 6, 30, 0};
    rects.center_right = (DonnellRect){7, 15, 30, 6};
    rects.bottom_right = (DonnellRect){7, 6, 30, 21};

    stock_image = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_BUTTON_SELECTED_NORMAL);
    sliced_image = Donnell_GuiPrimitives_NineSlice_Create(stock_image->image, &rects, 1, DONNELL_STOCK_ELEMENT_BUTTON_SELECTED_NORMAL);
    Donnell_GuiPrimitives_StockElements_Add(sliced_image);
    Donnell_GuiPrimitives_NineSlice_Free(sliced_image);
    Donnell_StockImage_Free(stock_image);

    stock_image = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_BUTTON_SELECTED_HOVER);
    sliced_image = Donnell_GuiPrimitives_NineSlice_Create(stock_image->image, &rects, 1, DONNELL_STOCK_ELEMENT_BUTTON_SELECTED_HOVER);
    Donnell_GuiPrimitives_StockElements_Add(sliced_image);
    Donnell_GuiPrimitives_NineSlice_Free(sliced_image);
    Donnell_StockImage_Free(stock_image);

    stock_image = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_BUTTON_SELECTED_PRESSED);
    sliced_image = Donnell_GuiPrimitives_NineSlice_Create(stock_image->image, &rects, 1, DONNELL_STOCK_ELEMENT_BUTTON_SELECTED_PRESSED);
    Donnell_GuiPrimitives_StockElements_Add(sliced_image);
    Donnell_GuiPrimitives_NineSlice_Free(sliced_image);
    Donnell_StockImage_Free(stock_image);

    rects.top_left = (DonnellRect){14, 12, 0, 0};
    rects.center_left = (DonnellRect){7 * 2, 15 * 2, 0, 6 * 2};
    rects.bottom_left = (DonnellRect){14, 12, 0, 42};
    rects.top_center = (DonnellRect){46, 12, 14, 0};
    rects.center_center = (DonnellRect){46, 30, 14, 12};
    rects.bottom_center = (DonnellRect){46, 12, 14, 42};
    rects.top_right = (DonnellRect){14, 12, 60, 0};
    rects.center_right = (DonnellRect){14, 30, 60, 12};
    rects.bottom_right = (DonnellRect){14, 12, 60, 42};

    stock_image = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_BUTTON_SELECTED_NORMAL_2X);
    sliced_image = Donnell_GuiPrimitives_NineSlice_Create(stock_image->image, &rects, 2, DONNELL_STOCK_ELEMENT_BUTTON_SELECTED_NORMAL);
    Donnell_GuiPrimitives_StockElements_Add(sliced_image);
    Donnell_GuiPrimitives_NineSlice_Free(sliced_image);
    Donnell_StockImage_Free(stock_image);

    stock_image = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_BUTTON_SELECTED_HOVER_2X);
    sliced_image = Donnell_GuiPrimitives_NineSlice_Create(stock_image->image, &rects, 2, DONNELL_STOCK_ELEMENT_BUTTON_SELECTED_HOVER);
    Donnell_GuiPrimitives_StockElements_Add(sliced_image);
    Donnell_GuiPrimitives_NineSlice_Free(sliced_image);
    Donnell_StockImage_Free(stock_image);

    stock_image = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_BUTTON_SELECTED_PRESSED_2X);
    sliced_image = Donnell_GuiPrimitives_NineSlice_Create(stock_image->image, &rects, 2, DONNELL_STOCK_ELEMENT_BUTTON_SELECTED_PRESSED);
    Donnell_GuiPrimitives_StockElements_Add(sliced_image);
    Donnell_GuiPrimitives_NineSlice_Free(sliced_image);
    Donnell_StockImage_Free(stock_image);

    rects.top_left = (DonnellRect){21, 18, 0, 0};
    rects.center_left = (DonnellRect){21, 45, 0, 18};
    rects.bottom_left = (DonnellRect){7 * 3, 6 * 3, 0, 21 * 3};
    rects.top_center = (DonnellRect){23 * 3, 6 * 3, 7 * 3, 0};
    rects.center_center = (DonnellRect){23 * 3, 15 * 3, 7 * 3, 6 * 3};
    rects.bottom_center = (DonnellRect){23 * 3, 6 * 3, 7 * 3, 21 * 3};
    rects.top_right = (DonnellRect){7 * 3, 6 * 3, 30 * 3, 0};
    rects.center_right = (DonnellRect){7 * 3, 15 * 3, 30 * 3, 6 * 3};
    rects.bottom_right = (DonnellRect){7 * 3, 6 * 3, 30 * 3, 21 * 3};

    stock_image = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_BUTTON_SELECTED_NORMAL_3X);
    sliced_image = Donnell_GuiPrimitives_NineSlice_Create(stock_image->image, &rects, 3, DONNELL_STOCK_ELEMENT_BUTTON_SELECTED_NORMAL);
    Donnell_GuiPrimitives_StockElements_Add(sliced_image);
    Donnell_GuiPrimitives_NineSlice_Free(sliced_image);
    Donnell_StockImage_Free(stock_image);

    stock_image = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_BUTTON_SELECTED_HOVER_3X);
    sliced_image = Donnell_GuiPrimitives_NineSlice_Create(stock_image->image, &rects, 3, DONNELL_STOCK_ELEMENT_BUTTON_SELECTED_HOVER);
    Donnell_GuiPrimitives_StockElements_Add(sliced_image);
    Donnell_GuiPrimitives_NineSlice_Free(sliced_image);
    Donnell_StockImage_Free(stock_image);

    stock_image = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_BUTTON_SELECTED_PRESSED_3X);
    sliced_image = Donnell_GuiPrimitives_NineSlice_Create(stock_image->image, &rects, 3, DONNELL_STOCK_ELEMENT_BUTTON_SELECTED_PRESSED);
    Donnell_GuiPrimitives_StockElements_Add(sliced_image);
    Donnell_GuiPrimitives_NineSlice_Free(sliced_image);
    Donnell_StockImage_Free(stock_image);
}

void GuiPrimitives_Cleanup() {
    unsigned int i;

    for (i = 0; i < stock_icons_count; i++) {
        Donnell_GuiPrimitives_Icon_Free(stock_icons[i]);
    }
    free(stock_icons);

    for (i = 0; i < stock_guielems_count; i++) {
        Donnell_GuiPrimitives_NineSlice_Free(stock_guielems[i]);
    }
    free(stock_guielems);
}

DONNELL_EXPORT DonnellNineSlice *Donnell_GuiPrimitives_NineSlice_Create(DonnellImageBuffer *image, DonnellNineSliceRects *rects, unsigned int scale, char *name) {
    DonnellNineSlice *ns_image;
    unsigned int i;

    if ((!image) || (!rects)) {
        return NULL;
    }

    ns_image = malloc(sizeof(DonnellNineSlice));
    if (!ns_image) {
        return NULL;
    }
    ns_image->scale = scale;

    ns_image->top_left = Donnell_ImageBuffer_Crop(image, &rects->top_left, DONNELL_FALSE);
    ns_image->top_center = Donnell_ImageBuffer_Crop(image, &rects->top_center, DONNELL_FALSE);
    ns_image->top_right = Donnell_ImageBuffer_Crop(image, &rects->top_right, DONNELL_FALSE);
    ns_image->center_left = Donnell_ImageBuffer_Crop(image, &rects->center_left, DONNELL_FALSE);
    ns_image->center_center = Donnell_ImageBuffer_Crop(image, &rects->center_center, DONNELL_FALSE);
    ns_image->center_right = Donnell_ImageBuffer_Crop(image, &rects->center_right, DONNELL_FALSE);
    ns_image->bottom_left = Donnell_ImageBuffer_Crop(image, &rects->bottom_left, DONNELL_FALSE);
    ns_image->bottom_center = Donnell_ImageBuffer_Crop(image, &rects->bottom_center, DONNELL_FALSE);
    ns_image->bottom_right = Donnell_ImageBuffer_Crop(image, &rects->bottom_right, DONNELL_FALSE);

    if (name) {
        ns_image->alloced_name = DONNELL_TRUE;
        ns_image->name = strdup(name);
    } else {
        ns_image->alloced_name = DONNELL_FALSE;
        ns_image->name = "";
    }

    return ns_image;
}

DONNELL_EXPORT void Donnell_GuiPrimitives_NineSlice_Free(DonnellNineSlice *image) {
    if (!image) {
        return;
    }

    if (image->alloced_name) {
        free(image->name);
    }

    Donnell_ImageBuffer_Free(image->top_left);
    Donnell_ImageBuffer_Free(image->top_center);
    Donnell_ImageBuffer_Free(image->top_right);
    Donnell_ImageBuffer_Free(image->center_left);
    Donnell_ImageBuffer_Free(image->center_center);
    Donnell_ImageBuffer_Free(image->center_right);
    Donnell_ImageBuffer_Free(image->bottom_left);
    Donnell_ImageBuffer_Free(image->bottom_center);
    Donnell_ImageBuffer_Free(image->bottom_right);

    free(image);
}

DONNELL_EXPORT DonnellNineSlice *Donnell_GuiPrimitives_NineSlice_Copy(DonnellNineSlice *image) {
    DonnellNineSlice *ns_image;

    if (!image) {
        return NULL;
    }

    ns_image = malloc(sizeof(DonnellNineSlice));
    if (!ns_image) {
        return NULL;
    }

    ns_image->scale = image->scale;
    if (image->name) {
        ns_image->alloced_name = DONNELL_TRUE;
        ns_image->name = strdup(image->name);
    }

    ns_image->top_left = Donnell_ImageBuffer_Copy(image->top_left);
    ns_image->top_center = Donnell_ImageBuffer_Copy(image->top_center);
    ns_image->top_right = Donnell_ImageBuffer_Copy(image->top_right);
    ns_image->center_left = Donnell_ImageBuffer_Copy(image->center_left);
    ns_image->center_center = Donnell_ImageBuffer_Copy(image->center_center);
    ns_image->center_right = Donnell_ImageBuffer_Copy(image->center_right);
    ns_image->bottom_left = Donnell_ImageBuffer_Copy(image->bottom_left);
    ns_image->bottom_center = Donnell_ImageBuffer_Copy(image->bottom_center);
    ns_image->bottom_right = Donnell_ImageBuffer_Copy(image->bottom_right);

    return ns_image;
}

DONNELL_EXPORT void Donnell_GuiPrimitives_NineSlice_Draw(DonnellImageBuffer *buffer, DonnellNineSlice *image, DonnellRect *rect) {
    DonnellRect i_rect;
    DonnellImageBuffer *i_img;
    unsigned int h_stretch;
    unsigned int w_stretch;

    if ((!image) || (!buffer) || (!rect)) {
        return;
    }

    h_stretch = rect->h * buffer->scale - image->top_left->height - image->bottom_left->height;
    w_stretch = rect->w * buffer->scale - image->top_left->width - image->top_right->width;

    i_rect.w = image->top_left->width;
    i_rect.h = image->top_left->height;
    i_rect.x = rect->x * buffer->scale;
    i_rect.y = rect->y * buffer->scale;
    Donnell_ImageBuffer_BlendBufferContents(buffer, image->top_left, NULL, &i_rect);

    i_img = Donnell_ImageBuffer_Scale(image->center_left, image->center_left->width, h_stretch, DONNELL_SCALING_ALGORITHM_NEAREST_NEIGHBOR);
    i_rect.w = i_img->width;
    i_rect.h = i_img->height;
    i_rect.x = rect->x * buffer->scale;
    i_rect.y = rect->y * buffer->scale + image->top_left->height;
    Donnell_ImageBuffer_BlendBufferContents(buffer, i_img, NULL, &i_rect);
    Donnell_ImageBuffer_Free(i_img);

    i_rect.w = image->bottom_left->width;
    i_rect.h = image->bottom_left->height;
    i_rect.x = rect->x * buffer->scale;
    i_rect.y = rect->y * buffer->scale + image->top_left->height + h_stretch;
    Donnell_ImageBuffer_BlendBufferContents(buffer, image->bottom_left, NULL, &i_rect);

    i_img = Donnell_ImageBuffer_Scale(image->top_center, w_stretch, image->top_center->height, DONNELL_SCALING_ALGORITHM_NEAREST_NEIGHBOR);
    i_rect.w = i_img->width;
    i_rect.h = i_img->height;
    i_rect.x = rect->x * buffer->scale + image->top_left->width;
    i_rect.y = rect->y * buffer->scale;
    Donnell_ImageBuffer_BlendBufferContents(buffer, i_img, NULL, &i_rect);
    Donnell_ImageBuffer_Free(i_img);

    i_img = Donnell_ImageBuffer_Scale(image->center_center, w_stretch, h_stretch, DONNELL_SCALING_ALGORITHM_NEAREST_NEIGHBOR);
    i_rect.w = i_img->width;
    i_rect.h = i_img->height;
    i_rect.x = rect->x * buffer->scale + image->top_left->width;
    i_rect.y = rect->y * buffer->scale + image->top_center->height;
    Donnell_ImageBuffer_BlendBufferContents(buffer, i_img, NULL, &i_rect);
    Donnell_ImageBuffer_Free(i_img);

    i_img = Donnell_ImageBuffer_Scale(image->bottom_center, w_stretch, image->bottom_center->height, DONNELL_SCALING_ALGORITHM_NEAREST_NEIGHBOR);
    i_rect.w = i_img->width;
    i_rect.h = i_img->height;
    i_rect.x = rect->x * buffer->scale + image->top_left->width;
    i_rect.y = rect->y * buffer->scale + image->top_center->height + h_stretch;
    Donnell_ImageBuffer_BlendBufferContents(buffer, i_img, NULL, &i_rect);
    Donnell_ImageBuffer_Free(i_img);

    i_rect.w = image->top_right->width;
    i_rect.h = image->top_right->height;
    i_rect.x = rect->x * buffer->scale + image->top_right->width + w_stretch;
    i_rect.y = rect->y * buffer->scale;
    Donnell_ImageBuffer_BlendBufferContents(buffer, image->top_right, NULL, &i_rect);

    i_img = Donnell_ImageBuffer_Scale(image->center_right, image->center_right->width, h_stretch, DONNELL_SCALING_ALGORITHM_NEAREST_NEIGHBOR);
    i_rect.w = i_img->width;
    i_rect.h = i_img->height;
    i_rect.x = rect->x * buffer->scale + image->top_right->width + w_stretch;
    i_rect.y = rect->y * buffer->scale + image->top_right->height;
    Donnell_ImageBuffer_BlendBufferContents(buffer, i_img, NULL, &i_rect);
    Donnell_ImageBuffer_Free(i_img);

    i_rect.w = image->bottom_right->width;
    i_rect.h = image->bottom_right->height;
    i_rect.x = rect->x * buffer->scale + image->top_right->width + w_stretch;
    i_rect.y = rect->y * buffer->scale + image->top_right->height + h_stretch;
    Donnell_ImageBuffer_BlendBufferContents(buffer, image->bottom_right, NULL, &i_rect);
}

DONNELL_EXPORT void Donnell_GuiPrimitives_DrawButton(DonnellImageBuffer *buffer, char *text, DonnellRect *rect, DonnellPixel *color, unsigned int text_size, DonnellFontOptions text_font_options, DonnellButtonState state) {
    DonnellSize ctext_size;
    DonnellRect dest_rect;
    DonnellRect src1_rect;
    DonnellRect src2_rect;
    DonnellRect draw_rect;
    DonnellNineSlice *button_img;
    unsigned int w;
    unsigned int h;

    if ((!buffer) || (!rect) || (!color)) {
        return;
    }

    w = rect->w;
    h = rect->h;
    button_img = NULL;

    if ((state & DONNELL_BUTTON_STATE_NORMAL) && (state & DONNELL_BUTTON_STATE_SELECTED)) {
        button_img = Donnell_GuiPrimitives_StockElements_Load(DONNELL_STOCK_ELEMENT_BUTTON_SELECTED_NORMAL, buffer->scale);
    } else if ((state & DONNELL_BUTTON_STATE_HOVER) && (state & DONNELL_BUTTON_STATE_SELECTED)) {
        button_img = Donnell_GuiPrimitives_StockElements_Load(DONNELL_STOCK_ELEMENT_BUTTON_SELECTED_HOVER, buffer->scale);
    } else if ((state & DONNELL_BUTTON_STATE_PRESSED) && (state & DONNELL_BUTTON_STATE_SELECTED)) {
        button_img = Donnell_GuiPrimitives_StockElements_Load(DONNELL_STOCK_ELEMENT_BUTTON_SELECTED_PRESSED, buffer->scale);
    } else if (state & DONNELL_BUTTON_STATE_NORMAL) {
        button_img = Donnell_GuiPrimitives_StockElements_Load(DONNELL_STOCK_ELEMENT_BUTTON_NORMAL, buffer->scale);
    } else if (state & DONNELL_BUTTON_STATE_HOVER) {
        button_img = Donnell_GuiPrimitives_StockElements_Load(DONNELL_STOCK_ELEMENT_BUTTON_HOVER, buffer->scale);
    } else if (state & DONNELL_BUTTON_STATE_PRESSED) {
        button_img = Donnell_GuiPrimitives_StockElements_Load(DONNELL_STOCK_ELEMENT_BUTTON_PRESSED, buffer->scale);
    }

    src1_rect.w = w * buffer->scale;
    src1_rect.h = h * buffer->scale;
    src1_rect.x = 0;
    src1_rect.y = 0;

    if (text) {
        Donnell_GraphicsPrimitives_MeasureTextLine(&ctext_size, text, text_size, text_font_options, buffer->scale);

        src2_rect.w = ctext_size.w;
        src2_rect.h = ctext_size.h;
        src2_rect.x = 0;
        src2_rect.y = 0;

        Donnell_Rect_Center(&dest_rect, &src1_rect, &src2_rect);

        if (src1_rect.w  < ctext_size.w) {
            w = (ctext_size.w + dest_rect.x/buffer->scale * 2);
        }

        if (src1_rect.h < ctext_size.h) {
            h = (ctext_size.h + dest_rect.y/buffer->scale * 2);
        }
    }

    draw_rect.w = w;
    draw_rect.h = h;
    draw_rect.x = rect->x;
    draw_rect.y = rect->y;

    Donnell_GuiPrimitives_NineSlice_Draw(buffer, button_img, &draw_rect);

    if (text) {
        Donnell_GraphicsPrimitives_DrawTextLine(buffer, color, text, rect->x + dest_rect.x/buffer->scale, rect->y + dest_rect.y/buffer->scale-1, text_size, text_font_options);
    }

    if (button_img) {
        Donnell_GuiPrimitives_NineSlice_Free(button_img);
    }
}
