#include <fribidi.h>

/* Internal header for various text utility functions */

#ifndef DONNELL_TEXTUTILS
#define DONNELL_TEXTUTILS

#include "donnell.h"

typedef struct {
    char **str;
    unsigned int count;
} Paragraphs;

DonnellBool TextUtils_IsNewLine(FriBidiChar chr);
Paragraphs *TextUtils_Paragraphs_Create(char *string);
void TextUtils_Paragraphs_Free(Paragraphs *paragraphs);

/*
#include "bidi.h"

typedef struct {
    FriBidiString **str;
    unsigned int count;
} Runs;

Runs *TextUtils_Runs_Create(FriBidiString *str);
*/

#endif
