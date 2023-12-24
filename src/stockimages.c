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
#include "gui/ButtonSelectedHover.h"
#include "gui/ButtonSelectedNormal.h"
#include "gui/ButtonSelectedPressed.h"
#include "gui/ButtonSelectedHover2x.h"
#include "gui/ButtonSelectedNormal2x.h"
#include "gui/ButtonSelectedPressed2x.h"
#include "gui/ButtonSelectedHover3x.h"
#include "gui/ButtonSelectedNormal3x.h"
#include "gui/ButtonSelectedPressed3x.h"

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
    DonnellStockImage *stock;
    DonnellImageBuffer *stock_img;

    stock_image_count = 0;

    stock_img = Donnell_ImageBuffer_LoadFromInline(Error16);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_ERROR_16);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(Error32);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_ERROR_32);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(Error64);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_ERROR_64);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(Error96);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_ERROR_96);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(Info16);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_INFO_16);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(Info32);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_INFO_32);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(Info64);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_INFO_64);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(Info96);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_INFO_96);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(Warning16);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_WARNING_16);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(Warning32);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_WARNING_32);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(Warning64);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_WARNING_64);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(Warning96);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_WARNING_96);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(ButtonHover);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_BUTTON_HOVER);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(ButtonNormal);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_BUTTON_NORMAL);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(ButtonPressed);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_BUTTON_PRESSED);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(ButtonHover2x);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_BUTTON_HOVER_2X);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(ButtonNormal2x);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_BUTTON_NORMAL_2X);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(ButtonPressed2x);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_BUTTON_PRESSED_2X);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(ButtonHover3x);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_BUTTON_HOVER_3X);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(ButtonNormal3x);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_BUTTON_NORMAL_3X);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(ButtonPressed3x);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_BUTTON_PRESSED_3X);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);
    
    stock_img = Donnell_ImageBuffer_LoadFromInline(ButtonSelectedHover);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_BUTTON_SELECTED_HOVER);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(ButtonSelectedNormal);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_BUTTON_SELECTED_NORMAL);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(ButtonSelectedPressed);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_BUTTON_SELECTED_PRESSED);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(ButtonSelectedHover2x);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_BUTTON_SELECTED_HOVER_2X);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(ButtonSelectedNormal2x);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_BUTTON_SELECTED_NORMAL_2X);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(ButtonSelectedPressed2x);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_BUTTON_SELECTED_PRESSED_2X);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(ButtonSelectedHover3x);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_BUTTON_SELECTED_HOVER_3X);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(ButtonSelectedNormal3x);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_BUTTON_SELECTED_NORMAL_3X);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);

    stock_img = Donnell_ImageBuffer_LoadFromInline(ButtonSelectedPressed3x);
    stock = Donnell_StockImage_Create(stock_img, DONNELL_STOCK_IMAGE_BUTTON_SELECTED_PRESSED_3X);
    Donnell_StockImages_Add(stock);
    Donnell_StockImage_Free(stock);
    Donnell_ImageBuffer_Free(stock_img);
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
