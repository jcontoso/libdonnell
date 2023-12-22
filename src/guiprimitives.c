#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "donnell.h"
#include "misc.h"
#include "symvis.h"
#include "guiprimitives.h"

DonnellIcon** stock_icons;
unsigned int stock_icons_count;

DONNELL_EXPORT DonnellIcon* Donnell_GuiPrimitives_Icon_Create(DonnellImageBuffer** images, char* name, unsigned int count) {
	DonnellIcon* icon;
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
	icon->images = calloc(count, sizeof(DonnellImageBuffer*));
	icon->count = count;
    for (i = 0; i < count; i++) {
		icon->images[i] = Donnell_ImageBuffer_Copy(images[i]);
	}
		
	return icon;
}

DONNELL_EXPORT DonnellIcon* Donnell_GuiPrimitives_Icon_Copy(DonnellIcon* icon) {
	if (!icon) {
		return NULL;
	}
	return Donnell_GuiPrimitives_Icon_Create(icon->images, icon->name, icon->count);
}


DONNELL_EXPORT void Donnell_GuiPrimitives_Icon_Free(DonnellIcon* icon) {
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

DONNELL_EXPORT DonnellIcon* Donnell_GuiPrimitives_StockIcons_Load(char* name) {
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


DONNELL_EXPORT void Donnell_GuiPrimitives_StockIcons_Add(DonnellIcon* icon) {
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
	if(stock_icons_count == 1) {
		stock_icons = calloc(stock_icons_count, sizeof(DonnellIcon*));
	} else {
		stock_icons = realloc(stock_icons, stock_icons_count * sizeof(DonnellIcon*));		
	}
	
	stock_icons[stock_icons_count-1] = Donnell_GuiPrimitives_Icon_Copy(icon);
}

DONNELL_EXPORT unsigned int Donnell_GuiPrimitives_Icon_GetBestForSize(DonnellImageBuffer *buffer, DonnellIcon* icon, unsigned int size) {
	unsigned int diff;
 	unsigned int best;
 	unsigned int i;

	if ((!buffer) || (!icon) || (size < 1)) {
		return 0;
	}

	size = size*buffer->scale;
			
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

DONNELL_EXPORT void Donnell_GuiPrimitives_Icon_Draw(DonnellImageBuffer *buffer, DonnellIcon* icon, unsigned int index, unsigned int size, unsigned int x, unsigned int y) {
	DonnellRect rect;
    DonnellImageBuffer* img;
    DonnellBool scaled;
    
	if ((!buffer) || (!icon) || (index < 0)) {
		return;
	}

	if (size*buffer->scale != icon->images[index]->width) {
		img = Donnell_ImageBuffer_Scale(icon->images[index], size*buffer->scale, size*buffer->scale, DONNELL_SCALING_ALGORITHM_BILINEAR);
		scaled = DONNELL_TRUE;
	} else {
		img = icon->images[index];
		scaled = DONNELL_FALSE;
	}
	
	rect.w = img->width;
	rect.h = img->height;
	rect.x = x*buffer->scale;
	rect.y = y*buffer->scale;

	Donnell_ImageBuffer_BlendBufferContents(buffer, img, NULL, &rect);
	
	if (scaled) {
		Donnell_ImageBuffer_Free(img);
	}
}


void StockIcons_Init() {
	DonnellImageBuffer** images;
	DonnellStockImage* error16;
	DonnellStockImage* error32;
	DonnellStockImage* error64;
	DonnellStockImage* error96;
	DonnellIcon* 	   error_icon;
	
	stock_icons_count = 0;
	
	images = calloc(4, sizeof(DonnellImageBuffer*));
	error16 = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_ERROR_16);
	error32 = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_ERROR_32);
	error64 = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_ERROR_64);
	error96 = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_ERROR_96);
	images[0] = error16->image;
	images[1] = error32->image;
	images[2] = error64->image;
	images[3] = error96->image;
	error_icon = Donnell_GuiPrimitives_Icon_Create(images, DONNELL_STOCK_ICON_ERROR, 4);
	Donnell_GuiPrimitives_StockIcons_Add(error_icon);
	Donnell_StockImage_Free(error16);
	Donnell_StockImage_Free(error32);
	Donnell_StockImage_Free(error64);
	Donnell_StockImage_Free(error96);
	Donnell_GuiPrimitives_Icon_Free(error_icon);
	
	free(images);
}

void StockIcons_Cleanup() {
	unsigned int i;

    for (i = 0; i < stock_icons_count; i++) {
		Donnell_GuiPrimitives_Icon_Free(stock_icons[i]);
	}
	free(stock_icons);
}
