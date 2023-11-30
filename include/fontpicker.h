#include "donnell.h"

/* Internal functions for font selection */

#ifndef DONNELL_FONTPICKER
#define DONNELL_FONTPICKER

void FontPicker_Init(void);
char *FontPicker_SelectFont(DonnellFont req_font);
void FontPicker_Cleanup(void);

#endif
