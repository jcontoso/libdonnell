#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "donnell.h"
#include "misc.h"
#include "symvis.h"

#include "icons/Error16.h"
#include "icons/Error32.h"
#include "icons/Error64.h"
#include "icons/Error96.h"

#include "icons/Info16.h"
#include "icons/Info32.h"
#include "icons/Info64.h"
#include "icons/Info96.h"

#include "icons/Warning16.h"
#include "icons/Warning32.h"
#include "icons/Warning64.h"
#include "icons/Warning96.h"

#include "gui/ButtonHover.h"
#include "gui/ButtonNormal.h"
#include "gui/ButtonPressed.h"

#include "gui/ButtonHover2x.h"
#include "gui/ButtonNormal2x.h"
#include "gui/ButtonPressed2x.h"

#include "gui/ButtonHover3x.h"
#include "gui/ButtonNormal3x.h"
#include "gui/ButtonPressed3x.h"

DonnellStockImage **stock_images;
unsigned int stock_image_count;

DONNELL_EXPORT DonnellStockImage *Donnell_StockImage_Copy(DonnellStockImage *stock_image) {
    if (!stock_image) {
        return NULL;
    }
    return Donnell_StockImage_Create(stock_image->image, stock_image->name);
}

DONNELL_EXPORT void Donnell_StockImage_Free(DonnellStockImage *stock_image) {
    if (!stock_image) {
        return;
    }

    if (stock_image->alloced_name) {
        free(stock_image->name);
    }
    Donnell_ImageBuffer_Free(stock_image->image);
    free(stock_image);
}

DONNELL_EXPORT DonnellStockImage *Donnell_StockImage_Create(DonnellImageBuffer *image, char *name) {
    DonnellStockImage *stock_image;
    DonnellImageBuffer *dimage;

    if ((!image) || (!name)) {
        return NULL;
    }

    stock_image = malloc(sizeof(DonnellStockImage));
    if (!stock_image) {
        return NULL;
    }

    stock_image->alloced_name = DONNELL_TRUE;
    stock_image->name = strdup(name);
    stock_image->image = Donnell_ImageBuffer_Copy(image);

    return stock_image;
}

void StockImages_Init() {
    DonnellStockImage *error_16;
    DonnellImageBuffer *error_16_img;
    DonnellStockImage *error_32;
    DonnellImageBuffer *error_32_img;
    DonnellStockImage *error_64;
    DonnellImageBuffer *error_64_img;
    DonnellStockImage *error_96;
    DonnellImageBuffer *error_96_img;
    DonnellStockImage *info_16;
    DonnellImageBuffer *info_16_img;
    DonnellStockImage *info_32;
    DonnellImageBuffer *info_32_img;
    DonnellStockImage *info_64;
    DonnellImageBuffer *info_64_img;
    DonnellStockImage *info_96;
    DonnellImageBuffer *info_96_img;
    DonnellStockImage *warning_16;
    DonnellImageBuffer *warning_16_img;
    DonnellStockImage *warning_32;
    DonnellImageBuffer *warning_32_img;
    DonnellStockImage *warning_64;
    DonnellImageBuffer *warning_64_img;
    DonnellStockImage *warning_96;
    DonnellImageBuffer *warning_96_img;
    DonnellStockImage *bhover;
    DonnellImageBuffer *bhover_img;
    DonnellStockImage *bnormal;
    DonnellImageBuffer *bnormal_img;
    DonnellStockImage *bpressed;
    DonnellImageBuffer *bpressed_img;
    DonnellStockImage *bhover2x;
    DonnellImageBuffer *bhover2x_img;
    DonnellStockImage *bnormal2x;
    DonnellImageBuffer *bnormal2x_img;
    DonnellStockImage *bpressed2x;
    DonnellImageBuffer *bpressed2x_img;
    DonnellStockImage *bhover3x;
    DonnellImageBuffer *bhover3x_img;
    DonnellStockImage *bnormal3x;
    DonnellImageBuffer *bnormal3x_img;
    DonnellStockImage *bpressed3x;
    DonnellImageBuffer *bpressed3x_img;

    stock_image_count = 0;

    error_16_img = Donnell_ImageBuffer_LoadFromInline(Error16);
    error_16 = Donnell_StockImage_Create(error_16_img, DONNELL_STOCK_IMAGE_ERROR_16);
    Donnell_StockImages_Add(error_16);
    Donnell_StockImage_Free(error_16);
    Donnell_ImageBuffer_Free(error_16_img);

    error_32_img = Donnell_ImageBuffer_LoadFromInline(Error32);
    error_32 = Donnell_StockImage_Create(error_32_img, DONNELL_STOCK_IMAGE_ERROR_32);
    Donnell_StockImages_Add(error_32);
    Donnell_StockImage_Free(error_32);
    Donnell_ImageBuffer_Free(error_32_img);

    error_64_img = Donnell_ImageBuffer_LoadFromInline(Error64);
    error_64 = Donnell_StockImage_Create(error_64_img, DONNELL_STOCK_IMAGE_ERROR_64);
    Donnell_StockImages_Add(error_64);
    Donnell_StockImage_Free(error_64);
    Donnell_ImageBuffer_Free(error_64_img);

    error_96_img = Donnell_ImageBuffer_LoadFromInline(Error96);
    error_96 = Donnell_StockImage_Create(error_96_img, DONNELL_STOCK_IMAGE_ERROR_96);
    Donnell_StockImages_Add(error_96);
    Donnell_StockImage_Free(error_96);
    Donnell_ImageBuffer_Free(error_96_img);

    info_16_img = Donnell_ImageBuffer_LoadFromInline(Info16);
    info_16 = Donnell_StockImage_Create(info_16_img, DONNELL_STOCK_IMAGE_INFO_16);
    Donnell_StockImages_Add(info_16);
    Donnell_StockImage_Free(info_16);
    Donnell_ImageBuffer_Free(info_16_img);

    info_32_img = Donnell_ImageBuffer_LoadFromInline(Info32);
    info_32 = Donnell_StockImage_Create(info_32_img, DONNELL_STOCK_IMAGE_INFO_32);
    Donnell_StockImages_Add(info_32);
    Donnell_StockImage_Free(info_32);
    Donnell_ImageBuffer_Free(info_32_img);

    info_64_img = Donnell_ImageBuffer_LoadFromInline(Info64);
    info_64 = Donnell_StockImage_Create(info_64_img, DONNELL_STOCK_IMAGE_INFO_64);
    Donnell_StockImages_Add(info_64);
    Donnell_StockImage_Free(info_64);
    Donnell_ImageBuffer_Free(info_64_img);

    info_96_img = Donnell_ImageBuffer_LoadFromInline(Info96);
    info_96 = Donnell_StockImage_Create(info_96_img, DONNELL_STOCK_IMAGE_INFO_96);
    Donnell_StockImages_Add(info_96);
    Donnell_StockImage_Free(info_96);
    Donnell_ImageBuffer_Free(info_96_img);

    warning_16_img = Donnell_ImageBuffer_LoadFromInline(Warning16);
    warning_16 = Donnell_StockImage_Create(warning_16_img, DONNELL_STOCK_IMAGE_WARNING_16);
    Donnell_StockImages_Add(warning_16);
    Donnell_StockImage_Free(warning_16);
    Donnell_ImageBuffer_Free(warning_16_img);

    warning_32_img = Donnell_ImageBuffer_LoadFromInline(Warning32);
    warning_32 = Donnell_StockImage_Create(warning_32_img, DONNELL_STOCK_IMAGE_WARNING_32);
    Donnell_StockImages_Add(warning_32);
    Donnell_StockImage_Free(warning_32);
    Donnell_ImageBuffer_Free(warning_32_img);

    warning_64_img = Donnell_ImageBuffer_LoadFromInline(Warning64);
    warning_64 = Donnell_StockImage_Create(warning_64_img, DONNELL_STOCK_IMAGE_WARNING_64);
    Donnell_StockImages_Add(warning_64);
    Donnell_StockImage_Free(warning_64);
    Donnell_ImageBuffer_Free(warning_64_img);

    warning_96_img = Donnell_ImageBuffer_LoadFromInline(Warning96);
    warning_96 = Donnell_StockImage_Create(warning_96_img, DONNELL_STOCK_IMAGE_WARNING_96);
    Donnell_StockImages_Add(warning_96);
    Donnell_StockImage_Free(warning_96);
    Donnell_ImageBuffer_Free(warning_96_img);

    bhover_img = Donnell_ImageBuffer_LoadFromInline(ButtonHover);
    bhover = Donnell_StockImage_Create(bhover_img, DONNELL_STOCK_IMAGE_BUTTON_HOVER);
    Donnell_StockImages_Add(bhover);
    Donnell_StockImage_Free(bhover);
    Donnell_ImageBuffer_Free(bhover_img);

    bnormal_img = Donnell_ImageBuffer_LoadFromInline(ButtonNormal);
    bnormal = Donnell_StockImage_Create(bnormal_img, DONNELL_STOCK_IMAGE_BUTTON_NORMAL);
    Donnell_StockImages_Add(bnormal);
    Donnell_StockImage_Free(bnormal);
    Donnell_ImageBuffer_Free(bnormal_img);

    bpressed_img = Donnell_ImageBuffer_LoadFromInline(ButtonPressed);
    bpressed = Donnell_StockImage_Create(bpressed_img, DONNELL_STOCK_IMAGE_BUTTON_PRESSED);
    Donnell_StockImages_Add(bpressed);
    Donnell_StockImage_Free(bpressed);
    Donnell_ImageBuffer_Free(bpressed_img);

    bhover2x_img = Donnell_ImageBuffer_LoadFromInline(ButtonHover2x);
    bhover2x = Donnell_StockImage_Create(bhover2x_img, DONNELL_STOCK_IMAGE_BUTTON_HOVER_2X);
    Donnell_StockImages_Add(bhover2x);
    Donnell_StockImage_Free(bhover2x);
    Donnell_ImageBuffer_Free(bhover2x_img);

    bnormal2x_img = Donnell_ImageBuffer_LoadFromInline(ButtonNormal2x);
    bnormal2x = Donnell_StockImage_Create(bnormal2x_img, DONNELL_STOCK_IMAGE_BUTTON_NORMAL_2X);
    Donnell_StockImages_Add(bnormal2x);
    Donnell_StockImage_Free(bnormal2x);
    Donnell_ImageBuffer_Free(bnormal2x_img);

    bpressed2x_img = Donnell_ImageBuffer_LoadFromInline(ButtonPressed2x);
    bpressed2x = Donnell_StockImage_Create(bpressed2x_img, DONNELL_STOCK_IMAGE_BUTTON_PRESSED_2X);
    Donnell_StockImages_Add(bpressed2x);
    Donnell_StockImage_Free(bpressed2x);
    Donnell_ImageBuffer_Free(bpressed2x_img);

    bhover3x_img = Donnell_ImageBuffer_LoadFromInline(ButtonHover3x);
    bhover3x = Donnell_StockImage_Create(bhover3x_img, DONNELL_STOCK_IMAGE_BUTTON_HOVER_3X);
    Donnell_StockImages_Add(bhover3x);
    Donnell_StockImage_Free(bhover3x);
    Donnell_ImageBuffer_Free(bhover3x_img);

    bnormal3x_img = Donnell_ImageBuffer_LoadFromInline(ButtonNormal3x);
    bnormal3x = Donnell_StockImage_Create(bnormal3x_img, DONNELL_STOCK_IMAGE_BUTTON_NORMAL_3X);
    Donnell_StockImages_Add(bnormal3x);
    Donnell_StockImage_Free(bnormal3x);
    Donnell_ImageBuffer_Free(bnormal3x_img);

    bpressed3x_img = Donnell_ImageBuffer_LoadFromInline(ButtonPressed3x);
    bpressed3x = Donnell_StockImage_Create(bpressed3x_img, DONNELL_STOCK_IMAGE_BUTTON_PRESSED_3X);
    Donnell_StockImages_Add(bpressed3x);
    Donnell_StockImage_Free(bpressed3x);
    Donnell_ImageBuffer_Free(bpressed3x_img);
}

void StockImages_Cleanup() {
    unsigned int i;

    for (i = 0; i < stock_image_count; i++) {
        Donnell_StockImage_Free(stock_images[i]);
    }
    free(stock_images);
}

DONNELL_EXPORT DonnellStockImage *Donnell_StockImages_Load(char *name) {
    unsigned int i;

    if (!name) {
        return NULL;
    }

    for (i = 0; i < stock_image_count; i++) {
        if (!strcmp(stock_images[i]->name, name)) {
            return Donnell_StockImage_Copy(stock_images[i]);
        }
    }

    return NULL;
}

DONNELL_EXPORT void Donnell_StockImages_Add(DonnellStockImage *stock_image) {
    unsigned int i;

    if (!stock_image) {
        return;
    }

    for (i = 0; i < stock_image_count; i++) {
        if (!strcmp(stock_images[i]->name, stock_image->name)) {
            Donnell_StockImage_Free(stock_images[i]);
            stock_images[i] = Donnell_StockImage_Copy(stock_image);
            return;
        }
    }

    stock_image_count++;
    if (stock_image_count == 1) {
        stock_images = calloc(stock_image_count, sizeof(DonnellStockImage *));
    } else {
        stock_images = realloc(stock_images, stock_image_count * sizeof(DonnellStockImage *));
    }

    stock_images[stock_image_count - 1] = Donnell_StockImage_Copy(stock_image);
}
