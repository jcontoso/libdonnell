#include <fribidi.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "bidi.h"
#include "harfbuzz.h"
#include "textutils.h"

/* Internal functions for FriBidi support */

char *FriBidiString_ConvertToUTF8(FriBidiString *string) {
    char *result;

    if (!string) {
        return NULL;
    }

    result = malloc(((string->len * sizeof(char)) * 4) + 1);
    if (!result) {
        return NULL;
    }

    fribidi_unicode_to_charset(FRIBIDI_CHAR_SET_UTF8, string->str, string->len, result);
    return result;
}

FriBidiString *FriBidiString_Create(unsigned int size) {
    FriBidiString *result;

    if (size <= 0) {
        return NULL;
    }

    result = malloc(sizeof(FriBidiString));
    if (!result) {
        return NULL;
    }

    result->str = calloc(size, sizeof(FriBidiChar));
    if (!result->str) {
        free(result);
        return NULL;
    }

    result->len = size;

    return result;
}

FriBidiString *FriBidiString_Copy(FriBidiString *string) {
    FriBidiString *result;
	unsigned int i;
	
	result = FriBidiString_Create(string->len);
	result->direction = string->direction;
	
    for (i = 0; i < string->len; i++) {
		result->str[i] = string->str[i];
    }
    
    return result;
}

DonnellBool FriBidiString_Compare(FriBidiString *a, FriBidiString *b) {
	unsigned int i;
	
	if (a->len != b->len) {
		return DONNELL_FALSE;
	}
	
    for (i = 0; i <a->len ; i++) {
		if (a->str[i] != b->str[i]) {
			return DONNELL_FALSE;
		}
    }
    
    return DONNELL_TRUE;
}

FriBidiString *FriBidiString_ConvertFromUTF8(char *string) {
    FriBidiString *result;

    if (!string) {
        return NULL;
    }

    result = malloc(sizeof(FriBidiString));
    if (!result) {
        return NULL;
    }

    result->str = calloc(strlen(string) + 1, sizeof(FriBidiChar));
    if (!result->str) {
        free(result);
        return NULL;
    }

    result->len = fribidi_charset_to_unicode(FRIBIDI_CHAR_SET_UTF8, string, strlen(string), result->str);
    return result;
}

void FriBidiString_Handle(FriBidiString *string) {
    FriBidiCharType *types;
    FriBidiParType direction;
    FriBidiLevel *levels;
    FriBidiLevel max_level;
    FriBidiLevel start;
    FriBidiLevel end;
    FriBidiArabicProp *props;
    unsigned int i;
    unsigned int c;

    if (!string) {
        return;
    }

    direction = FRIBIDI_PAR_LTR;
    types = NULL;
    levels = NULL;

    props = calloc(string->len + 1, sizeof(FriBidiArabicProp));
    if (!props) {
        return;
    }
    levels = calloc(string->len + 1, sizeof(FriBidiLevel));
    if (!levels) {
        free(props);
        return;
    }
    types = calloc(string->len + 1, sizeof(FriBidiCharType));
    if (!types) {
        free(props);
        free(levels);
        return;
    }

    fribidi_get_bidi_types(string->str, string->len, types);
    string->direction = fribidi_get_par_direction(types, string->len);

    max_level = fribidi_get_par_embedding_levels(types, string->len, &direction, levels);
    if (!HarfBuzz_GetLibrary()) {
        fribidi_get_joining_types(string->str, string->len, props);
        fribidi_join_arabic(types, string->len, levels, props);
        fribidi_shape(FRIBIDI_FLAGS_DEFAULT | FRIBIDI_FLAGS_ARABIC, levels, string->len, props, string->str);
    }

    for (end = 0, start = 0; end < string->len; end++) {
        if (TextUtils_IsNewLine(string->str[end]) || end == string->len - 1) {
            max_level = fribidi_reorder_line(FRIBIDI_FLAGS_DEFAULT | FRIBIDI_FLAGS_ARABIC, types, end - start + 1, start, direction, levels, string->str, NULL);
            start = end + 1;
        }
    }

    for (i = 0, c = 0; i < string->len; i++) {
        if (string->str[i] != FRIBIDI_CHAR_FILL) {
            string->str[c++] = string->str[i];
        }
    }
    string->len = c;

    free(levels);
    free(props);
    free(types);
}

void FriBidiString_Free(FriBidiString *string) {
    if (!string) {
        return;
    }

    if (string->str) {
        free(string->str);
    }

    free(string);
}

FriBidiParagraphs *FriBidiParagraphs_ConvertFromParagraphs(Paragraphs *paragraphs) {
    FriBidiParagraphs *fr_paragraphs;
    unsigned int i;

    if (!paragraphs) {
        return NULL;
    }

    fr_paragraphs = malloc(sizeof(FriBidiParagraphs));
    if (!fr_paragraphs) {
        return NULL;
    }
    fr_paragraphs->count = paragraphs->count;
    fr_paragraphs->str = calloc(paragraphs->count, sizeof(FriBidiString *));

    for (i = 0; i < paragraphs->count; i++) {
        fr_paragraphs->str[i] = FriBidiString_ConvertFromUTF8(paragraphs->str[i]);
        FriBidiString_Handle(fr_paragraphs->str[i]);
    }

    return fr_paragraphs;
}

void FriBidiParagraphs_Free(FriBidiParagraphs *paragraphs) {
    unsigned int i;

    if (!paragraphs) {
        return;
    }

    if (paragraphs->str) {
        for (i = 0; i < paragraphs->count; i++) {
            FriBidiString_Free(paragraphs->str[i]);
        }
        free(paragraphs->str);
    }
    free(paragraphs);
}
