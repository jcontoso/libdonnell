#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>

#include "donnell.h"
#include "misc.h"
#include "symvis.h"

DONNELL_EXPORT DonnellImageBuffer *Donnell_ImageBuffer_Create(unsigned int width, unsigned int height, unsigned int scale) {
    DonnellImageBuffer *buffer;
    unsigned int i;
    unsigned int j;

    buffer = malloc(sizeof(DonnellImageBuffer));
    if (!buffer) {
        return NULL;
    }

    buffer->width = width*scale;
    buffer->height = height*scale;
    buffer->scale = scale;
	
    buffer->pixels = calloc(buffer->height, sizeof(DonnellPixel **));
    if (!buffer->pixels) {
        return NULL;
    }

    for (i = 0; i < buffer->height; i++) {
        buffer->pixels[i] = calloc(buffer->width, sizeof(DonnellPixel *));
        if (!buffer->pixels[i]) {
            return NULL;
        }
        for (j = 0; j < buffer->width; j++) {
            buffer->pixels[i][j] == NULL;
        }
    }

    return buffer;
}

DONNELL_EXPORT void Donnell_ImageBuffer_Clear(DonnellImageBuffer *buffer, DonnellPixel *pixel) {
    unsigned int i;
    unsigned int ci;

    if ((!pixel) || (!buffer)) {
        return;
    }

    for (i = 0; i < buffer->height; i++) {
        for (ci = 0; ci < buffer->width; ci++) {
            Donnell_ImageBuffer_SetPixel(buffer, ci, i, pixel);
        }
    }
}

DONNELL_EXPORT void Donnell_ImageBuffer_Free(DonnellImageBuffer *buffer) {
    unsigned int i;
    unsigned int ci;

    if (!buffer) {
        return;
    }

    for (i = 0; i < buffer->height; i++) {
        for (ci = 0; ci < buffer->width; ci++) {
            if (buffer->pixels[i][ci]) {
                free(buffer->pixels[i][ci]);
            }
        }
        free(buffer->pixels[i]);
    }

    free(buffer->pixels);
    free(buffer);
    buffer = NULL;
}

DONNELL_EXPORT DonnellPixel *Donnell_ImageBuffer_GetPixel(DonnellImageBuffer *buffer, unsigned int x, unsigned int y) {
    if ((y < 0) || (x < 0) || (!buffer)) {
        return NULL;
    }

    if ((y >= buffer->height) || (x >= buffer->width)) {
        return Donnell_Pixel_CreateEasy(0, 0, 0, 255);
    }

    if (!buffer->pixels[y][x]) {
        return Donnell_Pixel_CreateEasy(0, 0, 0, 255);
    }

    return Misc_MemDup(buffer->pixels[y][x], sizeof(DonnellPixel));
}

DONNELL_EXPORT void Donnell_ImageBuffer_SetPixel(DonnellImageBuffer *buffer, unsigned int x, unsigned int y, DonnellPixel *pixel) {
    if ((y < 0) || (x < 0) || (!pixel) || (!buffer)) {
        return;
    }

    if ((y >= buffer->height) || (x >= buffer->width)) {
        return;
    }

    if (buffer->pixels[y][x]) {
        free(buffer->pixels[y][x]);
    }

    buffer->pixels[y][x] = Misc_MemDup(pixel, sizeof(DonnellPixel));
}

DONNELL_EXPORT void Donnell_ImageBuffer_BlendPixel(DonnellImageBuffer *buffer, unsigned int x, unsigned int y, DonnellPixel *pixel) {
    if ((y < 0) || (x < 0) || (!pixel) || (!buffer)) {
        return;
    }

    if ((y >= buffer->height) || (x >= buffer->width)) {
        return;
    }

    if (buffer->pixels[y][x]) {
        DonnellPixel *cpixel;

        cpixel = Donnell_Pixel_Blend(pixel, buffer->pixels[y][x]);

        Donnell_ImageBuffer_SetPixel(buffer, x, y, cpixel);

        free(cpixel);
    } else {
        Donnell_ImageBuffer_SetPixel(buffer, x, y, pixel);
    }
}

DONNELL_EXPORT DonnellImageBuffer *Donnell_ImageBuffer_LoadFromInline(char** str) {
    DonnellImageBuffer *buffer;
    char* token;
    char* dstrline;
    char* strline;
	unsigned int w;
	unsigned int h;
	unsigned int i;
	unsigned int j;
	unsigned int s;
	
	if (!str) {
		return NULL;
	}
	
	sscanf(str[0], "%u %u %u", &w, &h, &s);
	
	buffer = Donnell_ImageBuffer_Create(w, h, s);
    for (i = 1; i < h+1; i++) {
		j = 0;		
		dstrline = strdup(str[i]);
		strline = dstrline;
		token = strtok(strline, "|");
   
		while(token) {
 			DonnellPixel* pixel;
			int r;
			int g;
			int b;
			int a;
			sscanf(token, "%02x%02x%02x%02x", &r, &g, &b, &a);
			pixel = Donnell_Pixel_CreateEasy((DonnellUInt8)r, (DonnellUInt8)g, (DonnellUInt8)b, (DonnellUInt8)a);
			Donnell_ImageBuffer_SetPixel(buffer, j, i-1, pixel);
			Donnell_Pixel_Free(pixel);   
			token = strtok(NULL, "|");
			j++;
		}	
		free(dstrline);
	}

	return buffer;
}


DONNELL_EXPORT DonnellImageBuffer *Donnell_ImageBuffer_LoadFromPNG(char* name) {
    DonnellImageBuffer *buffer;
    FILE *file;
    png_structp png;
    png_infop png_info;
	png_byte color_type;
	png_byte bit_depth;
    png_bytep* png_rows;
	unsigned int i;
	unsigned int j;
	
	png_rows = NULL;
	file = fopen(name, "rb");
	
	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_info = png_create_info_struct(png);
	png_init_io(png, file);
	png_read_info(png, png_info);

	buffer = Donnell_ImageBuffer_Create(png_get_image_width(png, png_info), png_get_image_height(png, png_info), 1);
	color_type = png_get_color_type(png, png_info);
	bit_depth  = png_get_bit_depth(png, png_info);

	if(bit_depth == 16) {
		png_set_strip_16(png);
	}
	
	if(color_type == PNG_COLOR_TYPE_PALETTE) {
		png_set_palette_to_rgb(png);
	}
	
	if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
		png_set_expand_gray_1_2_4_to_8(png);
	}
	
	if(png_get_valid(png, png_info, PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(png);
	}

	if(color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE) {
		png_set_filler(png, 0xff, PNG_FILLER_AFTER);
	}
	
	if(color_type == PNG_COLOR_TYPE_GRAY ||  color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
		png_set_gray_to_rgb(png);
	}
	
	png_read_update_info(png, png_info);

	png_rows = malloc(sizeof(png_bytep) * buffer->height);
	for(i = 0; i < buffer->height; i++) {
		png_rows[i] = malloc(png_get_rowbytes(png,png_info));
	}
	png_read_image(png, png_rows);

	for(i = 0; i < buffer->height; i++) {
		for(j = 0; j < buffer->width; j++) {
			DonnellPixel* pixel;
			
			pixel = Donnell_Pixel_CreateEasy(png_rows[i][j * 4], png_rows[i][j * 4 + 1], png_rows[i][j * 4 + 2], png_rows[i][j * 4 + 3]);
			Donnell_ImageBuffer_SetPixel(buffer, j, i, pixel);
			Donnell_Pixel_Free(pixel);
		}
	}	

	png_destroy_read_struct(&png, &png_info, NULL);
 	fclose(file);
 	for(i = 0; i < buffer->height; i++) {
		free(png_rows[i]);
	}
	free(png_rows);
    return buffer;
}

DONNELL_EXPORT void Donnell_ImageBuffer_DumpAsPNG(DonnellImageBuffer *buffer, char *name) {
    FILE *file;
    png_structp png;
    png_infop png_info;
    png_bytep png_rows;
    unsigned int i;
    unsigned int j;

    png_rows = NULL;
    file = fopen(name, "wb");

    png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_info = png_create_info_struct(png);
    png_init_io(png, file);
    png_set_IHDR(png, png_info, buffer->width, buffer->height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(png, png_info);

    png_rows = malloc(sizeof(png_byte) * buffer->width * 4);

    for (i = 0; i < buffer->height; i++) {
        for (j = 0; j < buffer->width; j++) {
            DonnellPixel *pixel;

            pixel = Donnell_ImageBuffer_GetPixel(buffer, j, i);
            png_rows[j * 4] = pixel->red;
            png_rows[j * 4 + 1] = pixel->green;
            png_rows[j * 4 + 2] = pixel->blue;
            png_rows[j * 4 + 3] = pixel->alpha;
            Donnell_Pixel_Free(pixel);
        }
        png_write_row(png, png_rows);
    }

    png_write_end(png, png_info);
    free(png_rows);
	png_destroy_write_struct(&png, &png_info);
    fclose(file);
}

DonnellImageBuffer *ImageBuffer_ScaleNN(DonnellImageBuffer *buffer, unsigned int width, unsigned int height) {
    DonnellImageBuffer *ret;
    double scale_h;
    double scale_w;
    unsigned int closest_h;
    unsigned int closest_w;
    unsigned int i;
    unsigned int j;

    ret = Donnell_ImageBuffer_Create(width, height, 1);
    scale_w = buffer->width / (double)width;
    scale_h = buffer->height / (double)height;

    for (i = 0; i < height; ++i) {
        closest_h = (int)(scale_h * i + 0.5);
        for (j = 0; j < width; ++j) {
            DonnellPixel *pixel;
            unsigned int ch;
            unsigned int cw;

            closest_w = (int)(scale_w * j + 0.5);
            if (closest_h >= buffer->height) {
                ch = buffer->height - 1;
            } else {
                ch = closest_h;
            }

            if (closest_w >= buffer->width) {
                cw = buffer->width - 1;
            } else {
                cw = closest_w;
            }

            pixel = Donnell_ImageBuffer_GetPixel(buffer, cw, ch);

            Donnell_ImageBuffer_SetPixel(ret, j, i, pixel);

            Donnell_Pixel_Free(pixel);
        }
    }

    return ret;
}

float ImageBuffer_ScaleBL_LinearOp(float a, float b, float c) {
    return a + (b - a) * c;
}

float ImageBuffer_ScaleBL_BiLinearOp(float a, float b, float c, float d, float x, float y) {
    return ImageBuffer_ScaleBL_LinearOp(ImageBuffer_ScaleBL_LinearOp(a, b, x), ImageBuffer_ScaleBL_LinearOp(c, d, x), y);
}

DonnellImageBuffer *ImageBuffer_ScaleBL(DonnellImageBuffer *buffer, unsigned int width, unsigned int height) {
    DonnellImageBuffer *ret;
    float scale_h;
    float scale_w;
    unsigned int i;
    unsigned int j;

    ret = Donnell_ImageBuffer_Create(width, height, 1);
    scale_w = buffer->width / (float)width;
    scale_h = buffer->height / (float)height;

    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            float gx;
            float gy;
            int ix;
            int iy;
            DonnellPixel *a;
            DonnellPixel *b;
            DonnellPixel *c;
            DonnellPixel *d;
            DonnellPixel *e;

            gx = j / (float)(width) * (buffer->width - 1);
            gy = i / (float)(height) * (buffer->height - 1);

            ix = (int)gx;
            iy = (int)gy;

            e = Donnell_Pixel_Create();
            a = Donnell_ImageBuffer_GetPixel(buffer, ix, iy);
            b = Donnell_ImageBuffer_GetPixel(buffer, ix + 1, iy);
            c = Donnell_ImageBuffer_GetPixel(buffer, ix, iy + 1);
            d = Donnell_ImageBuffer_GetPixel(buffer, ix + 1, iy + 1);

            e->red = (DonnellUInt8)ImageBuffer_ScaleBL_BiLinearOp(a->red, b->red, c->red, d->red, (gx - ix), (gy - iy));
            e->green = (DonnellUInt8)ImageBuffer_ScaleBL_BiLinearOp(a->green, b->green, c->green, d->green, (gx - ix), (gy - iy));
            e->blue = (DonnellUInt8)ImageBuffer_ScaleBL_BiLinearOp(a->blue, b->blue, c->blue, d->blue, (gx - ix), (gy - iy));
            e->alpha = (DonnellUInt8)ImageBuffer_ScaleBL_BiLinearOp(a->alpha, b->alpha, c->alpha, d->alpha, (gx - ix), (gy - iy));

            Donnell_ImageBuffer_SetPixel(ret, j, i, e);
            
            Donnell_Pixel_Free(a);
            Donnell_Pixel_Free(b);
            Donnell_Pixel_Free(c);
            Donnell_Pixel_Free(d);
            Donnell_Pixel_Free(e);
        }
    }

    return ret;
}

DONNELL_EXPORT DonnellImageBuffer *Donnell_ImageBuffer_Scale(DonnellImageBuffer *buffer, unsigned int width, unsigned int height, DonnellScalingAlgorithm algo) {
    if ((width < 0) || (height < 0) || (!buffer)) {
        return NULL;
    }
    
    switch (algo) {
    case DONNELL_SCALING_ALGORITHM_BILINEAR:
        return ImageBuffer_ScaleBL(buffer, width, height);
        break;
    default:
        return ImageBuffer_ScaleNN(buffer, width, height);
    }
}

DONNELL_EXPORT DonnellImageBuffer *Donnell_ImageBuffer_Crop(DonnellImageBuffer *buffer, DonnellRect *rect, DonnellBool ignore_xy) {
	DonnellImageBuffer* cbuffer;
	DonnellImageBuffer* rbuffer;
    unsigned int i;
    unsigned int j;
	DonnellBool scaled;
	
	if ((!buffer) || (!rect)) {
		return NULL;
	}
	
	scaled = DONNELL_FALSE;
	cbuffer = buffer;
	
	if (rect->w > buffer->width) {
		cbuffer = Donnell_ImageBuffer_Scale(buffer, rect->w, buffer->height, DONNELL_SCALING_ALGORITHM_BILINEAR);		
		scaled = DONNELL_TRUE;
	}
	
	if (rect->h > buffer->height)  {
		unsigned int w;
		
		w = cbuffer->width;
		if (scaled) {
			Donnell_ImageBuffer_Free(cbuffer);
		}
		cbuffer = Donnell_ImageBuffer_Scale(buffer, w, rect->h, DONNELL_SCALING_ALGORITHM_BILINEAR);		
	}
	
	rbuffer = Donnell_ImageBuffer_Create(rect->w, rect->h, 1);
	
    for (i = 0; i < rect->h; ++i) {
        for (j = 0; j < rect->w; ++j) {
            DonnellPixel *pixel;

			if (ignore_xy) {
				pixel = Donnell_ImageBuffer_GetPixel(cbuffer, j, i);				
			} else {
				pixel = Donnell_ImageBuffer_GetPixel(cbuffer, j + rect->x, i + rect->y);
			}

            Donnell_ImageBuffer_SetPixel(rbuffer, j, i, pixel);

            Donnell_Pixel_Free(pixel);
        }
    }	
   
   	if (scaled) {
		Donnell_ImageBuffer_Free(cbuffer);
	} 
    return rbuffer;
}

DONNELL_EXPORT DonnellImageBuffer *Donnell_ImageBuffer_Copy(DonnellImageBuffer *src) {
	DonnellImageBuffer* ret;
    unsigned int i;
    unsigned int j;
    
	if (!src) {
		return NULL;
	}
	
	ret = Donnell_ImageBuffer_Create(src->width, src->height, src->scale);
	
    for (i = 0; i < src->height; ++i) {
        for (j = 0; j < src->width; ++j) {
            DonnellPixel *pixel;

            pixel = Donnell_ImageBuffer_GetPixel(src, j, i);

            Donnell_ImageBuffer_SetPixel(ret, j, i, pixel);

            Donnell_Pixel_Free(pixel);
        }
    }
    
    return ret;
}

DONNELL_EXPORT void Donnell_ImageBuffer_BlendBufferContents(DonnellImageBuffer *buffer, DonnellImageBuffer *cbuffer, DonnellRect *srect, DonnellRect *drect) {
	DonnellImageBuffer* sbuffer;
	DonnellImageBuffer* dbuffer;
	DonnellRect s;
	DonnellRect d;
    unsigned int i;
    unsigned int j;
    
	if ((!buffer) || (!cbuffer)) {
		return;
	}

	if (!srect) {
		s.w = cbuffer->width;
		s.h = cbuffer->height;
		s.x = 0;
		s.y = 0;
		srect = &s;
	}
	
	if (!drect) {
		d.w = buffer->width;
		d.h = buffer->height;
		d.x = 0;
		d.y = 0;
		drect = &d;
	}
			
	sbuffer = Donnell_ImageBuffer_Crop(cbuffer, srect, DONNELL_FALSE);
	dbuffer = Donnell_ImageBuffer_Crop(sbuffer, drect, DONNELL_TRUE);
	
    for (i = 0; i < drect->h; ++i) {
        for (j = 0; j < drect->w; ++j) {
            DonnellPixel *pixel;

            pixel = Donnell_ImageBuffer_GetPixel(dbuffer, j, i);

            Donnell_ImageBuffer_BlendPixel(buffer, drect->x + j, drect->y + i, pixel);

            Donnell_Pixel_Free(pixel);
        }
    }
    
    Donnell_ImageBuffer_Free(dbuffer);
    Donnell_ImageBuffer_Free(sbuffer);
}
