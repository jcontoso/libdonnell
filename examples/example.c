#include <stdio.h>
#include <stdlib.h>
#include <donnell.h>
#include "test2.h"

#define DEMO_STRING "😀😬😁\nLatin?\n阳光灿烂\nВиняткова\n\nמימין לשמאל\nحتى أطول من اليمين إلى اليسار\n"

int main() {	
	DonnellImageBuffer* buffer;
	DonnellIcon* stock_image;
	DonnellPixel* textcolor;
	DonnellPixel* bg;
	unsigned int index;
	
	Donnell_Init();
	
	stock_image = Donnell_GuiPrimitives_StockIcons_Load(DONNELL_STOCK_ICON_ERROR);
	 
	textcolor = Donnell_Pixel_CreateEasy(0, 0, 0, 255);

	bg = Donnell_Pixel_CreateEasy(199, 199, 199, 255);

	buffer = Donnell_ImageBuffer_Create(350, 58, 1);
	Donnell_ImageBuffer_Clear(buffer, bg);

	Donnell_GraphicsPrimitives_DrawText(buffer, textcolor, "Unable to create an OpenGL context.", 55, 23, 14, DONNELL_FONT_OPTIONS_SANS_SERIF);

	index = Donnell_GuiPrimitives_Icon_GetBestForSize(buffer, stock_image, 32);
	Donnell_GuiPrimitives_Icon_Draw(buffer, stock_image, index, 32, 13, 13);

	Donnell_ImageBuffer_DumpAsPNG(buffer, "example.png");
	
	Donnell_ImageBuffer_Free(buffer);
	Donnell_GuiPrimitives_Icon_Free(stock_image);
	Donnell_Pixel_Free(textcolor);
	Donnell_Pixel_Free(bg);
	
	Donnell_Cleanup();
	return 0;
}
