#include <string.h>
#include <fontconfig/fontconfig.h>
#include "fontpicker.h"
#include "donnell.h"

FcConfig* fontconfig;

void FontPicker_Init(void) {
	FcInit();	
	fontconfig = FcInitLoadConfigAndFonts(); 		
}

char* FontPicker_SelectFont(DonnellFont req_font) {
	FcPattern* pattern;
	FcPattern* font;
	FcResult result;
	FcChar8* file; 
	char* ret;
	
	switch(req_font) {
		case DONNELL_FONT_SERIF:
			pattern = FcNameParse((const FcChar8*)"serif");
			break;
		case DONNELL_FONT_MONOSPACE:
			pattern = FcNameParse((const FcChar8*)"monospace");
			break;
		default:
			pattern = FcNameParse((const FcChar8*)"sans-serif");
	}		
	
	FcConfigSubstitute(fontconfig, pattern, FcMatchPattern);
	FcDefaultSubstitute(pattern);
	font = FcFontMatch(fontconfig, pattern, &result);
		
	FcPatternGetString(font, FC_FILE, 0, &file);
	ret = strdup((char*)file);
	
	FcPatternDestroy(font);
	FcPatternDestroy(pattern);
	
	return ret;
}

void FontPicker_Cleanup(void) {
	FcConfigDestroy(fontconfig);
	FcFini();
}



