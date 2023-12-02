#include <stdio.h>
#include <stdlib.h>
#include <donnell.h>

int main() {	
	DonnellImageBuffer* buffer;
	DonnellPixel* pixel;
	DonnellPixel* cpixel;
	DonnellSize rect;

	Donnell_Init();

	pixel = Donnell_Pixel_CreateEasy(255, 255, 255, 255);

	cpixel = Donnell_Pixel_CreateEasy(30, 56, 12, 255);

	buffer = Donnell_ImageBuffer_Create(300, 300);
	Donnell_ImageBuffer_Clear(buffer, cpixel);

	Donnell_GraphicsPrimitives_DrawText(buffer, pixel, "Latin\n日本語\n한글\n台灣\n大陆\nСлава Україні\nمیک ڈونلڈز سیگا گیمز لائن\nܐܠܦ ܒܝܬ ܣܘܪܝܝܐ \nᏣᎳᎩ ᎦᏬᏂᎯᏍᏗ\n", 0, 0, 16, DONNELL_FONT_SANS_SERIF, DONNELL_FONT_STYLE_ITALIC | DONNELL_FONT_STYLE_BOLD);

	Donnell_ImageBuffer_DumpAsBitmap(buffer, "test.bmp");
	Donnell_ImageBuffer_Free(buffer);
	free(pixel);
	free(cpixel);

	Donnell_Cleanup();
	return 0;
}
