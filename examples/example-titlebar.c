#include <stdio.h>
#include <stdlib.h>
#include <donnell.h>

int main() {	
	DonnellImageBuffer* buffer;
	DonnellPixel* bg_color;
	DonnellPixel* border_color;
	DonnellStockElementStandard *titlebar_background;
	DonnellRect rect;
	DonnellSize size;
	Donnell_Init();

	bg_color = Donnell_Pixel_CreateEasy(255, 255, 255, 255);
	border_color = Donnell_Pixel_CreateEasy(109, 109, 109, 255);

	buffer = Donnell_ImageBuffer_Create(200, 200, 2);
	
	// border and background
	Donnell_ImageBuffer_Clear(buffer, border_color);
	rect.h = rect.w = (buffer->width/buffer->scale)-2;
	rect.y = rect.x = 1;
	Donnell_GraphicsPrimitives_DrawRectangle(buffer, bg_color, &rect, DONNELL_FALSE);

	// titlebar bg
	titlebar_background = Donnell_GuiPrimitives_StandardStockElements_Load(DONNELL_STOCK_ELEMENT_CAPTION_BACKGROUND, buffer->scale);
	Donnell_GuiPrimitives_StandardStockElement_Draw(buffer, titlebar_background, &rect, DONNELL_TRUE, DONNELL_FALSE);
	
	// text
	Donnell_GraphicsPrimitives_DrawTextLine(buffer, bg_color, "Titlebar", 7, 10, 11, DONNELL_FONT_OPTIONS_SANS_SERIF);
	Donnell_GraphicsPrimitives_DrawTextLine(buffer, border_color, "Titlebar", 7, 9, 11, DONNELL_FONT_OPTIONS_SANS_SERIF);
	
	Donnell_ImageBuffer_DumpAsPNG(buffer, "titlebar.png");
	
	Donnell_ImageBuffer_Free(buffer);
	Donnell_GuiPrimitives_StandardStockElement_Free(titlebar_background);
	Donnell_Pixel_Free(bg_color);
	Donnell_Pixel_Free(border_color);	
	Donnell_Cleanup();
	
	return 0;
}
