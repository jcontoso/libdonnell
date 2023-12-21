#include <stdio.h>
#include <stdlib.h>
#include <donnell.h>

int main() {	
	DonnellImageBuffer* buffer;
	DonnellImageBuffer* scaled_buffer;
	
	Donnell_Init();

	buffer = Donnell_ImageBuffer_LoadFromPNG("examples/test.png");
	scaled_buffer = Donnell_ImageBuffer_Scale(buffer, 300, 300, DONNELL_SCALING_ALGORITHM_BILINEAR);
	Donnell_ImageBuffer_DumpAsPNG(scaled_buffer, "example2.png");
	
	Donnell_ImageBuffer_Free(buffer);
	Donnell_Cleanup();
	return 0;
}
