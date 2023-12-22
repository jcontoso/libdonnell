#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>  
#include <png.h>
  
char *RemoveExtension(char* myStr) {
    char *retStr;
    char *lastExt;
    if (myStr == NULL) return NULL;
    if ((retStr = malloc (strlen (myStr) + 1)) == NULL) return NULL;
    strcpy (retStr, myStr);
    lastExt = strrchr (retStr, '.');
    if (lastExt != NULL)
        *lastExt = '\0';
    return retStr;
}

void LoadAndConvert(char* name, char* out_name, unsigned int scale) {
    FILE *file;
    FILE *out_file;
    char* header_name;
    png_structp png;
    png_infop png_info;
	png_byte color_type;
	png_byte bit_depth;
    png_bytep* png_rows;
	unsigned int width;
	unsigned int height;
	unsigned int i;
	unsigned int j;
	
	png_rows = NULL;
	file = fopen(name, "rb");
	out_file = fopen(out_name, "a");
	
	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_info = png_create_info_struct(png);
	png_init_io(png, file);
	png_read_info(png, png_info);

	header_name = RemoveExtension(basename(out_name));
	fprintf(out_file, "static char *%s[] = {\n", header_name);
	free(header_name);
	
	width      = png_get_image_width(png, png_info);
	height     = png_get_image_height(png, png_info);
  	color_type = png_get_color_type(png, png_info);
	bit_depth  = png_get_bit_depth(png, png_info);

	fprintf(out_file, "	\"%u %u %u\",\n", width, height, scale);

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

	png_rows = malloc(sizeof(png_bytep) * height);
	for(i = 0; i < height; i++) {
		png_rows[i] = malloc(png_get_rowbytes(png,png_info));
	}
	png_read_image(png, png_rows);

	for(i = 0; i < height; i++) {
		fprintf(out_file, "	\"", width, height);

		for(j = 0; j < width; j++) {
			fprintf(out_file, "%02x%02x%02x%02x|", png_rows[i][j * 4], png_rows[i][j * 4 + 1], png_rows[i][j * 4 + 2], png_rows[i][j * 4 + 3]);
		}
		fprintf(out_file, "\",\n", width, height);
	}	
	fputs("};", out_file);

	png_destroy_read_struct(&png, &png_info, NULL);
 	fclose(file);
 	fclose(out_file);
 	for(i = 0; i < height; i++) {
		free(png_rows[i]);
	}
	free(png_rows);
}

int main(int argc, char *argv[]) { 
	char* ifile;
	char* ofile;
    unsigned int overwrite; 
	unsigned int scale;
    int opt; 
    
    ifile = NULL;
    ofile = NULL;
    overwrite = 0;
    scale = 1;
    
    while((opt = getopt(argc, argv, "hri:o:s:")) != -1)  {  
        switch(opt)  {  
            case 'h':  
				puts("-h: Display help");
				puts("-r: Overwrite the output file");
				puts("-i: Input file name");
				puts("-o: Output file name");
				puts("-s: Scale (integer)");				
				exit(0);
                break;  
            case 'r':  
				overwrite = 1;
                break;  
            case 'i':  
				ifile = strdup(optarg);
                break;  
            case 'o':  
				ofile = strdup(optarg);
                break;  
            case 's':  
				scale = atoi(optarg);
                break;  
       }
    }  
    
    if (!ifile) {
		puts("No filename supplied");
		exit(1);
	}
	
	if (!ofile) {
		char* cfn;
		char* dfn;
		
		dfn = dirname(ifile);
		cfn = RemoveExtension(basename(ifile));
		ofile = malloc(strlen(dfn)+strlen("/")+strlen(cfn)+strlen(".h")+1);
		strcpy(ofile, dfn);
		strcat(ofile, "/");
		strcat(ofile, cfn);
		strcat(ofile, ".h");				
		free(cfn);
	}

	if (access(ifile, F_OK) != 0) {
		puts("Cannot access the input image");
		exit(1);
	}

	if (access(ofile, F_OK) == 0) {
		if (!overwrite) {
			puts("Output image already exists");
			exit(1);
		} else {
			remove(ofile);
		}
	}

	LoadAndConvert(ifile, ofile, scale);
	puts("Conversion successful");

	if (ifile) {
		free(ifile);
	}
	
	if (ofile) {
		free(ofile);
	}
    return 0; 
} 
