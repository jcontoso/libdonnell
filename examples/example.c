#include <stdio.h>
#include <stdlib.h>
#include <donnell.h>

#define DEMO_STRING "😀😬😁\nLatin\n阳光灿烂\nВиняткова\n\nמימין לשמאל\nحتى أطول من اليمين إلى اليسار\n"

int main() {	
	DonnellImageBuffer* buffer;
	DonnellImageBuffer* downscaled;
	DonnellImageBuffer* upscaled;
	DonnellPixel* pixel;
	DonnellPixel* cpixel;

	Donnell_Init();

	pixel = Donnell_Pixel_CreateEasy(255, 255, 255, 255);

	cpixel = Donnell_Pixel_CreateEasy(30, 56, 12, 255);

	buffer = Donnell_ImageBuffer_Create(300, 300);
	Donnell_ImageBuffer_Clear(buffer, cpixel);

	Donnell_GraphicsPrimitives_DrawText(buffer, pixel, DEMO_STRING, 0, 0, 16, DONNELL_FONT_OPTIONS_SANS_SERIF);
	
	downscaled = Donnell_ImageBuffer_Scale(buffer, 120, 120, DONNELL_SCALING_ALGORITHM_NEAREST_NEIGHBOR);
	upscaled = Donnell_ImageBuffer_Scale(buffer, 640, 640, DONNELL_SCALING_ALGORITHM_NEAREST_NEIGHBOR);
	
	Donnell_ImageBuffer_DumpAsBitmap(buffer, "example.bmp");
	Donnell_ImageBuffer_DumpAsBitmap(downscaled, "example_downsized.bmp");
	Donnell_ImageBuffer_DumpAsBitmap(upscaled, "example_upsized.bmp");
	
	Donnell_ImageBuffer_Free(buffer);
	Donnell_Pixel_Free(pixel);
	Donnell_Pixel_Free(cpixel);
	Donnell_ImageBuffer_Free(downscaled);
	Donnell_ImageBuffer_Free(upscaled);

	Donnell_Cleanup();
	return 0;
}
