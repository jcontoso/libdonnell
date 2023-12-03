#include <stdio.h>
#include <stdlib.h>
#include <donnell.h>

int main() {	
	DonnellImageBuffer* buffer;
	DonnellPixel* pixel;
	DonnellPixel* cpixel;

	Donnell_Init();

	pixel = Donnell_Pixel_CreateEasy(255, 255, 255, 255);

	cpixel = Donnell_Pixel_CreateEasy(30, 56, 12, 255);

	buffer = Donnell_ImageBuffer_Create(300, 300);
	Donnell_ImageBuffer_Clear(buffer, cpixel);

	Donnell_GraphicsPrimitives_DrawTextLine(buffer, pixel, "This is a test", 0,0, 16, DONNELL_FONT_SANS_SERIF, 0);

	Donnell_ImageBuffer_DumpAsBitmap(buffer, "test.bmp");
	Donnell_ImageBuffer_Free(buffer);
	free(pixel);
	free(cpixel);

	Donnell_Cleanup();
	return 0;
}
