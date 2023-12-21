#include <stdio.h>
#include <stdlib.h>
#include <donnell.h>
#include "test2.h"

#define DEMO_STRING "😀😬😁\nLatin?\n阳光灿烂\nВиняткова\n\nמימין לשמאל\nحتى أطول من اليمين إلى اليسار\n"

int main() {	
	DonnellImageBuffer* buffer;
	DonnellImageBuffer* buffer2;
	DonnellPixel* pixel;
	DonnellPixel* cpixel;
	DonnellRect src;
	DonnellRect dest;
	
	Donnell_Init();
	
	buffer2 = Donnell_ImageBuffer_LoadFromInline(test2);
	
	pixel = Donnell_Pixel_CreateEasy(255, 255, 255, 255);

	cpixel = Donnell_Pixel_CreateEasy(30, 56, 12, 255);

	buffer = Donnell_ImageBuffer_Create(300, 300, 1);
	Donnell_ImageBuffer_Clear(buffer, cpixel);

	Donnell_GraphicsPrimitives_DrawText(buffer, pixel, DEMO_STRING, 0, 0, 16, DONNELL_FONT_OPTIONS_SANS_SERIF);
	
	src.w = 200;
	src.h = 100;
	src.x = 0;
	src.y = 0;
	dest.w = 150;
	dest.h = 150;
	dest.x = 150;
	dest.y = 150;
	Donnell_ImageBuffer_BlendBufferContents(buffer, buffer2, &src, &dest);

	Donnell_ImageBuffer_DumpAsPNG(buffer, "example.png");
	
	Donnell_ImageBuffer_Free(buffer);
	Donnell_ImageBuffer_Free(buffer2);
	Donnell_Pixel_Free(pixel);
	Donnell_Pixel_Free(cpixel);
	
	Donnell_Cleanup();
	return 0;
}
