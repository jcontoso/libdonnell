#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <donnell.h>
 
typedef struct motif_hints
{
    unsigned long   flags;
    unsigned long   functions;
    unsigned long   decorations;
    long            inputMode;
    unsigned long   status;
} motif_hints;

Display *display;
Window window;
Pixmap contents;
GC contents_gc;
XEvent event;
int screen;

DonnellImageBuffer* buffer;
DonnellPixel* bg_color;
DonnellPixel* border_color;
DonnellStockElementStandard *titlebar_background;
DonnellRect rect;
DonnellSize size;

void convert_to_pixmap() {
    unsigned int j;
    unsigned int i;

	for (i = 0; i < buffer->height; i++) {
		for (j = 0; j < buffer->width; j++) {
			DonnellPixel *pixel;

			pixel = Donnell_ImageBuffer_GetPixel(buffer, j, i);			
			XSetForeground(display, contents_gc, ((pixel->red & 0xFF) << 16) + ((pixel->green & 0xFF) << 8) + (pixel->blue & 0xFF));
			XDrawPoint(display, contents, contents_gc, j, i);
			
			Donnell_Pixel_Free(pixel);
		}
	}
}

void draw() {	
	Donnell_ImageBuffer_Clear(buffer, border_color);
	Donnell_GraphicsPrimitives_DrawRectangle(buffer, bg_color, &rect, DONNELL_FALSE);
	Donnell_GuiPrimitives_StandardStockElement_Draw(buffer, titlebar_background, &rect, DONNELL_TRUE, DONNELL_FALSE);
	Donnell_GraphicsPrimitives_DrawTextLine(buffer, bg_color, "Titlebar", 7, 10, 11, DONNELL_FONT_OPTIONS_SANS_SERIF);
	Donnell_GraphicsPrimitives_DrawTextLine(buffer, border_color, "Titlebar", 7, 9, 11, DONNELL_FONT_OPTIONS_SANS_SERIF);
	Donnell_GuiPrimitives_DrawTitlebarButton(buffer, 139, 4, DONNELL_TITLEBAR_BUTTON_MINIMIZE, DONNELL_BUTTON_STATE_NORMAL);
	Donnell_GuiPrimitives_DrawTitlebarButton(buffer, 158, 4, DONNELL_TITLEBAR_BUTTON_MAXIMIZE, DONNELL_BUTTON_STATE_NORMAL);
	Donnell_GuiPrimitives_DrawTitlebarButton(buffer, 177, 4, DONNELL_TITLEBAR_BUTTON_CLOSE, DONNELL_BUTTON_STATE_NORMAL);
}
 
void init_resources() {
	bg_color = Donnell_Pixel_CreateEasy(255, 255, 255, 255);
	border_color = Donnell_Pixel_CreateEasy(109, 109, 109, 255);
	buffer = Donnell_ImageBuffer_Create(200, 200, 1);
	rect.h = rect.w = (buffer->width/buffer->scale)-2;
	rect.y = rect.x = 1;
	titlebar_background = Donnell_GuiPrimitives_StandardStockElements_Load(DONNELL_STOCK_ELEMENT_CAPTION_BACKGROUND, buffer->scale);
}

void blit_to_window() {
	XCopyArea(display, contents, window, DefaultGC(display, screen), 0, 0, buffer->width, buffer->height, 0, 0);
	XFlush(display);
}

int main(void) {
	XSizeHints* size_hints;
	motif_hints hints;
	Atom prop;
	
	Donnell_Init();
	
	init_resources();
	draw();
	
    display = XOpenDisplay(NULL);
    screen = DefaultScreen(display);
	
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, buffer->width, buffer->height, 0, BlackPixel(display, screen), WhitePixel(display, screen));
    XSelectInput(display, window, ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask | ButtonReleaseMask | PointerMotionMask);
        
    size_hints = XAllocSizeHints();
    size_hints->flags = PResizeInc | PMaxSize | PMinSize | PWinGravity;
    size_hints->width_inc = 0;
    size_hints->height_inc = 0;
    size_hints->max_width = buffer->width;
    size_hints->max_height = buffer->height;
    size_hints->min_width = buffer->width;
    size_hints->min_height = buffer->height;
	size_hints->win_gravity = CenterGravity;
    XSetWMNormalHints(display, window, size_hints);
    XFree(size_hints);
	
	prop = XInternAtom(display, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(display, window, &prop, 1);

	hints.flags = 2;
	hints.decorations = 0;
	prop = XInternAtom(display, "_MOTIF_WM_HINTS", True);
	XChangeProperty(display, window, prop, prop, 32, PropModeReplace, (unsigned char *)&hints, 5);
	
    XMapWindow(display, window);
	XMoveWindow(display, window, (DisplayWidth(display, screen) - buffer->width)/2, (DisplayHeight(display, screen) - buffer->height)/2);

    contents = XCreatePixmap(display, window, buffer->width, buffer->height, DefaultDepth(display, screen));
	contents_gc = XCreateGC(display, contents, 0, NULL);
	convert_to_pixmap();
			
    for (;;) {
        XNextEvent(display, &event);

		switch(event.type) {
			case Expose:
				blit_to_window();
				break;
				break;
			case ClientMessage:
				goto end;
				break;
		}
    }
 
end:
	XDestroyWindow(display, window);
	XFreePixmap(display, contents);
	XFreeGC(display, contents_gc);
    XCloseDisplay(display);
	Donnell_ImageBuffer_Free(buffer);
	Donnell_GuiPrimitives_StandardStockElement_Free(titlebar_background);
	Donnell_Pixel_Free(bg_color);
	Donnell_Pixel_Free(border_color);	
	Donnell_Cleanup();
	
    return 0;
 }
