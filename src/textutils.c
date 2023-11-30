#include <fribidi.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "textutils.h"

/* Various internal text utility functions */

bool TextUtils_IsNewLine(FriBidiChar chr) {
    return chr == '\n' || chr == '\r' || chr == '\f' || chr == '\v';
}

Paragraphs *TextUtils_Paragraphs_Create(char *string) {
    Paragraphs *paragraphs;
    char *dup_string;
    char *paragraph;
    unsigned int i;

    i = 0;
    paragraphs = malloc(sizeof(Paragraphs));
    if (!paragraphs) {
        return NULL;
    }

    paragraphs->count = 0;
    dup_string = strdup(string);
    paragraph = strtok(dup_string, "\n\r\f\v");
    while (paragraph != NULL) {
        paragraphs->count++;
        paragraph = strtok(NULL, "\n\r\f\v");
    }
    free(dup_string);

    paragraphs->str = calloc(paragraphs->count, sizeof(char *));
    if (!paragraphs->str) {
        free(paragraphs);
        return NULL;
    }

    dup_string = strdup(string);
    paragraph = strtok(dup_string, "\n\r\f\v");
    while (paragraph != NULL) {
        paragraphs->str[i] = malloc(strlen(paragraph) + 1);
        strcpy(paragraphs->str[i], paragraph);
        paragraph = strtok(NULL, "\n\r\f\v");
        i++;
    }
    free(dup_string);

    return paragraphs;
}

void TextUtils_Paragraphs_Free(Paragraphs *paragraphs) {
    unsigned int i;

    for (i = 0; i < paragraphs->count; i++) {
        free(paragraphs->str[i]);
    }
    free(paragraphs->str);
    free(paragraphs);
}
