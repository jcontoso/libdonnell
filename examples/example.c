#include <stdio.h>
#include <stdlib.h>
#include <donnell.h>
#include "test2.h"

#define DEMO_STRING "😀😬😁\nLatin?\n阳光灿烂\nВиняткова\n\nמימין לשמאל\nحتى أطول من اليمين إلى اليسار\n"

int main() {	
	DonnellImageBuffer* buffer;
	DonnellStockImage* stock_image;
	DonnellPixel* pixel;
	DonnellPixel* cpixel;
	DonnellRect src;
	DonnellRect dest;
	
	Donnell_Init();
	
	stock_image = Donnell_StockImages_Load(DONNELL_STOCK_IMAGE_WARNING_64);
	
	pixel = Donnell_Pixel_CreateEasy(255, 255, 255, 255);

	cpixel = Donnell_Pixel_CreateEasy(30, 56, 12, 255);

	buffer = Donnell_ImageBuffer_Create(300, 300, 1);
	Donnell_ImageBuffer_Clear(buffer, cpixel);

	Donnell_GraphicsPrimitives_DrawText(buffer, pixel, DEMO_STRING, 0, 0, 16, DONNELL_FONT_OPTIONS_SANS_SERIF);
	

	dest.w = stock_image->image->width;
	dest.h = stock_image->image->height;
	dest.x = 150;
	dest.y = 150;
	Donnell_ImageBuffer_BlendBufferContents(buffer, stock_image->image, NULL, &dest);

	Donnell_ImageBuffer_DumpAsPNG(buffer, "example.png");
	
	Donnell_ImageBuffer_Free(buffer);
	Donnell_StockImage_Free(stock_image);
	Donnell_Pixel_Free(pixel);
	Donnell_Pixel_Free(cpixel);
	
	Donnell_Cleanup();
	return 0;
}
