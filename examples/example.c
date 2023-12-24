#include <stdio.h>
#include <stdlib.h>
#include <donnell.h>

int main() {	
	DonnellImageBuffer* buffer;
	DonnellIcon* icon;
	DonnellPixel* text_color;
	DonnellPixel* bg_color;
	DonnellRect button_rect;
	unsigned int index;

	Donnell_Init();

	bg_color = Donnell_Pixel_CreateEasy(199, 199, 199, 255);
	text_color = Donnell_Pixel_CreateEasy(0, 0, 0, 255);
	button_rect.w = 62;
	button_rect.h = 27;
	button_rect.x = 160;
	button_rect.y = 59;
	
	icon = Donnell_GuiPrimitives_StockIcons_Load(DONNELL_STOCK_ICON_WARNING);	

	buffer = Donnell_ImageBuffer_Create(235, 99, 1);
	Donnell_ImageBuffer_Clear(buffer, bg_color);
	index = Donnell_GuiPrimitives_Icon_GetBestForSize(icon, DONNELL_ICON_SIZE_32, buffer->scale);

	Donnell_GraphicsPrimitives_DrawTextLine(buffer, text_color, "Sample text!", 55, 24, 12, DONNELL_FONT_OPTIONS_SANS_SERIF);
	Donnell_GuiPrimitives_Icon_Draw(buffer, icon, index, DONNELL_ICON_SIZE_32, 13, 13);
	Donnell_GuiPrimitives_DrawButton(buffer, "OK", &button_rect, text_color, 12, DONNELL_FONT_OPTIONS_SANS_SERIF, DONNELL_BUTTON_STATE_NORMAL);
	
	Donnell_ImageBuffer_DumpAsPNG(buffer, "example.png");
	
	Donnell_ImageBuffer_Free(buffer);
	Donnell_GuiPrimitives_Icon_Free(icon);
	Donnell_Pixel_Free(text_color);
	Donnell_Pixel_Free(bg_color);
	Donnell_Cleanup();
	
	return 0;
}
