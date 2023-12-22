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

DonnellStockImage** stock_images;
unsigned int stock_image_count;

DONNELL_EXPORT DonnellStockImage* Donnell_StockImage_Copy(DonnellStockImage* stock_image) {
	return Donnell_StockImage_Create(stock_image->image, stock_image->name);
}

DONNELL_EXPORT void Donnell_StockImage_Free(DonnellStockImage* stock_image) {
	if (!stock_image) {
		return;
	}
	
	if (stock_image->alloced_name) {
		free(stock_image->name);
	}
	Donnell_ImageBuffer_Free(stock_image->image);
	free(stock_image);
}

DONNELL_EXPORT DonnellStockImage* Donnell_StockImage_Create(DonnellImageBuffer* image, char* name) {
	DonnellStockImage* stock_image;
	DonnellImageBuffer* dimage;
	
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
	DonnellStockImage* error_16;
	DonnellImageBuffer* error_16_img;
	DonnellStockImage* error_32;
	DonnellImageBuffer* error_32_img;
	DonnellStockImage* error_64;
	DonnellImageBuffer* error_64_img;
	DonnellStockImage* error_96;
	DonnellImageBuffer* error_96_img;
	DonnellStockImage* info_16;
	DonnellImageBuffer* info_16_img;
	DonnellStockImage* info_32;
	DonnellImageBuffer* info_32_img;
	DonnellStockImage* info_64;
	DonnellImageBuffer* info_64_img;
	DonnellStockImage* info_96;
	DonnellImageBuffer* info_96_img;
	DonnellStockImage* warning_16;
	DonnellImageBuffer* warning_16_img;
	DonnellStockImage* warning_32;
	DonnellImageBuffer* warning_32_img;
	DonnellStockImage* warning_64;
	DonnellImageBuffer* warning_64_img;
	DonnellStockImage* warning_96;
	DonnellImageBuffer* warning_96_img;

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
}

void StockImages_Cleanup() {
	unsigned int i;

    for (i = 0; i < stock_image_count; i++) {
		Donnell_StockImage_Free(stock_images[i]);
	}
	free(stock_images);
}

DONNELL_EXPORT DonnellStockImage* Donnell_StockImages_Load(char* name) {
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


DONNELL_EXPORT void Donnell_StockImages_Add(DonnellStockImage* stock_image) {
	unsigned int i;
	
	if (!stock_image) {
		return;
	}

    for (i = 0; i < stock_image_count; i++) {
		if (!strcmp(stock_images[i]->name, stock_image->name)) {
			return;
		}
	}
	
	stock_image_count++;
	if(stock_image_count == 1) {
		stock_images = calloc(stock_image_count, sizeof(DonnellStockImage*));
	} else {
		stock_images = realloc(stock_images, stock_image_count * sizeof(DonnellStockImage*));		
	}
	
	stock_images[stock_image_count-1] = Donnell_StockImage_Copy(stock_image);
}
