#include "textutils.h"
#include <fribidi.h>

/* Internal header for FriBidi support */

#ifndef DONNELL_BIDI
#define DONNELL_BIDI

typedef struct {
    FriBidiChar *str;
    FriBidiStrIndex len;
    FriBidiParType direction;
} FriBidiString;

typedef struct {
    FriBidiString **str;
    unsigned int count;
} FriBidiParagraphs;

FriBidiString *FriBidiString_Create(unsigned int size);
FriBidiString *FriBidiString_ConvertFromUTF8(char *string);
char *FriBidiString_ConvertToUTF8(FriBidiString *string);
void FriBidiString_Handle(FriBidiString *string);
void FriBidiString_Free(FriBidiString *string);

FriBidiParagraphs *FriBidiParagraphs_ConvertFromParagraphs(Paragraphs *paragraphs);
void FriBidiParagraphs_Free(FriBidiParagraphs *paragraphs);

#endif
