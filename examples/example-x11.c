#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <donnell.h>
 
Display *display;
Window window;
Pixmap contents;
GC contents_gc;
XEvent event;
int screen;

DonnellImageBuffer* buffer;
DonnellIcon* icon;
DonnellPixel* text_color;
DonnellPixel* bg_color;
DonnellRect button_rect;
DonnellButtonState button_state;
Bool button_pressed;
unsigned int ico_index;

void convert_to_pixmap(Bool first_time) {
    unsigned int j;
    unsigned int i;

	if (first_time == True) {
		for (i = 0; i < buffer->height; i++) {
			for (j = 0; j < buffer->width; j++) {
				DonnellPixel *pixel;

				pixel = Donnell_ImageBuffer_GetPixel(buffer, j, i);			
				XSetForeground(display, contents_gc, ((pixel->red & 0xFF) << 16) + ((pixel->green & 0xFF) << 8) + (pixel->blue & 0xFF));
				XDrawPoint(display, contents, contents_gc, j, i);
				
				Donnell_Pixel_Free(pixel);
			}
		}
	} else {
		for (i = button_rect.y*buffer->scale; i < (button_rect.h+button_rect.y)*buffer->scale; i++) {
			for (j = button_rect.x*buffer->scale; j < (button_rect.w+button_rect.x)*buffer->scale; j++) {
				DonnellPixel *pixel;

				pixel = Donnell_ImageBuffer_GetPixel(buffer, j, i);			
				XSetForeground(display, contents_gc, ((pixel->red & 0xFF) << 16) + ((pixel->green & 0xFF) << 8) + (pixel->blue & 0xFF));
				XDrawPoint(display, contents, contents_gc, j, i);
				
				Donnell_Pixel_Free(pixel);
			}
		}
	}
}

void draw_dialog(DonnellButtonState state, Bool first_time) {	
	if (first_time == True) {
		Donnell_ImageBuffer_Clear(buffer, bg_color);
		Donnell_GraphicsPrimitives_DrawTextLine(buffer, text_color, "This is a dialog.", 55, 24, 14, DONNELL_FONT_OPTIONS_SANS_SERIF);
		Donnell_GuiPrimitives_Icon_Draw(buffer, icon, ico_index, DONNELL_ICON_SIZE_32, 13, 13);
		Donnell_GuiPrimitives_DrawButton(buffer, "OK", &button_rect, text_color, 14, DONNELL_FONT_OPTIONS_SANS_SERIF, state, DONNELL_FALSE);			
	} else {
		Donnell_GraphicsPrimitives_DrawRectangle(buffer, bg_color, &button_rect, DONNELL_FALSE);
		Donnell_GuiPrimitives_DrawButton(buffer, "OK", &button_rect, text_color, 14, DONNELL_FONT_OPTIONS_SANS_SERIF, state, DONNELL_FALSE);			
	}
}
 
void init_dialog_resources() {
	bg_color = Donnell_Pixel_CreateEasy(199, 199, 199, 255);
	text_color = Donnell_Pixel_CreateEasy(0, 0, 0, 255);
	button_rect.w = 62;
	button_rect.h = 27;
	button_rect.x = 160;
	button_rect.y = 59;	
	buffer = Donnell_ImageBuffer_Create(235, 99, 1);
	icon = Donnell_GuiPrimitives_StockIcons_Load(DONNELL_STOCK_ICON_INFO);	
	ico_index = Donnell_GuiPrimitives_Icon_GetBestForSize(icon, DONNELL_ICON_SIZE_32, buffer->scale);
}

void blit_to_window(Pixmap pixmap, Bool first_time) {
	if (first_time == True) {
		XCopyArea(display, contents, window, DefaultGC(display, screen), 0, 0, buffer->width, buffer->height, 0, 0);
	} else {
		XCopyArea(display, contents, window, DefaultGC(display, screen), button_rect.x*buffer->scale, button_rect.y*buffer->scale, button_rect.w*buffer->scale, button_rect.h*buffer->scale, button_rect.x*buffer->scale, button_rect.y*buffer->scale);
	}
	XFlush(display);
}

Bool update_dialog(Bool pressed, unsigned int x, unsigned int y) {
	if (((x >= 160*buffer->scale) && (x <= 222*buffer->scale)) && ((y >= 59*buffer->scale) && (y <= (59+27)*buffer->scale))) {
		if (pressed == True) {
			button_state = DONNELL_BUTTON_STATE_PRESSED  | DONNELL_BUTTON_STATE_SELECTED;	
		} else {
			button_state = DONNELL_BUTTON_STATE_HOVER  | DONNELL_BUTTON_STATE_SELECTED;
		}
	} else  {
		button_state = DONNELL_BUTTON_STATE_NORMAL | DONNELL_BUTTON_STATE_SELECTED;
	}	
	
	draw_dialog(button_state, False);
	convert_to_pixmap(False);
	blit_to_window(contents, False);	
	
	if (button_state & DONNELL_BUTTON_STATE_PRESSED) {
		return True;
	} else {
		return False;
	}
}

int main(void) {
	XSizeHints* size_hints;
	Atom allowed_actions;
	Atom type;
	Atom type_dialog;
	Atom close;
	Atom delete;
	
	Donnell_Init();
	
	button_pressed = False;
	button_state = DONNELL_BUTTON_STATE_NORMAL | DONNELL_BUTTON_STATE_SELECTED;
	init_dialog_resources();
	draw_dialog(button_state, True);
	
    display = XOpenDisplay(NULL);
    screen = DefaultScreen(display);
	
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, buffer->width, buffer->height, 0, BlackPixel(display, screen), WhitePixel(display, screen));
    XSelectInput(display, window, ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask | ButtonReleaseMask | PointerMotionMask);
    
    XStoreName(display, window, "Information");
    
    size_hints = XAllocSizeHints();
    size_hints->flags = PResizeInc | PMaxSize | PMinSize;
    size_hints->width_inc = 0;
    size_hints->height_inc = 0;
    size_hints->max_width = buffer->width;
    size_hints->max_height = buffer->height;
    size_hints->min_width = buffer->width;
    size_hints->min_height = buffer->height;
    XSetWMNormalHints(display, window, size_hints);
    XFree(size_hints);
    
	allowed_actions = XInternAtom(display, "_NET_WM_ALLOWED_ACTIONS", False);
	close = XInternAtom(display, " _NET_WM_ACTION_CLOSE", False);
	XChangeProperty(display, window, allowed_actions, XA_ATOM, 32, PropModeReplace, (unsigned char*)&close,  1);
   
	type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
    type_dialog = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DIALOG", False);
    XChangeProperty(display, window, type, XA_ATOM, 32, PropModeReplace, (unsigned char*)&type_dialog , 1);
 
	delete = XInternAtom(display, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(display, window, &delete, 1);
	
    XMapWindow(display, window);

    contents = XCreatePixmap(display, window, buffer->width, buffer->height, DefaultDepth(display, screen));
	contents_gc = XCreateGC(display, contents, 0, NULL);
	convert_to_pixmap(True);
			
    for (;;) {
        XNextEvent(display, &event);

		switch(event.type) {
			case Expose:
				blit_to_window(contents, True);
				break;
			case MotionNotify:
				update_dialog(button_pressed, event.xbutton.x, event.xbutton.y);
       			break;
			case ButtonPress:	
				if (event.xbutton.button == 1)  {
					button_pressed = True;
					update_dialog(button_pressed, event.xbutton.x, event.xbutton.y);
				}
				break;
			case ButtonRelease:	
				if (event.xbutton.button == 1)  {
					Bool ret;
					ret = update_dialog(button_pressed, event.xbutton.x, event.xbutton.y);

					if (ret == True) {
						usleep(5);
						goto end;
					}	
					button_pressed = False;
				}
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
	Donnell_GuiPrimitives_Icon_Free(icon);
	Donnell_Pixel_Free(text_color);
	Donnell_Pixel_Free(bg_color);
	Donnell_Cleanup();
	
    return 0;
 }
