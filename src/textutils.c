#include <fribidi.h>
#include <unictype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textutils.h"
#include "bidi.h"

/* Various internal text utility functions */

DonnellBool TextUtils_IsNewLine(FriBidiChar chr) {
    return chr == '\n' || chr == '\r' || chr == '\f' || chr == '\v';
}

DonnellBool TextUtils_IsNewLineChar(char chr) {
    return chr == '\n' || chr == '\r' || chr == '\f' || chr == '\v';
}

Paragraphs *TextUtils_Paragraphs_Create(char *original_string) {
    Paragraphs *paragraphs;
    char *string;
    char *start;
    char *end;
    unsigned int i;

    i = 0;

    string = malloc(strlen(original_string) + strlen("\n") + 1);
    if (!string) {
        return NULL;
    }
    strcpy(string, original_string);
    strcat(string, "\n");
    start = end = string;

    paragraphs = malloc(sizeof(Paragraphs));
    if (!paragraphs) {
        free(string);
        return NULL;
    }

    while (end = strpbrk(start, "\n\r\f\v")) {
        i++;

        if (i == 1) {
            paragraphs->str = malloc(sizeof(char *));
            if (!paragraphs->str) {
                free(string);
                free(paragraphs);
                return NULL;
            }
        } else {
            paragraphs->str = realloc(paragraphs->str, i * sizeof(char *));
            if (!paragraphs->str) {
                free(string);
                free(paragraphs);
                return NULL;
            }
        }

        paragraphs->str[i - 1] = strndup(start, (size_t)(end - start));
        start = end + 1;
    }
    paragraphs->count = i;

    free(string);
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

unsigned int Runs_GetCount(FriBidiString *str) {
	const uc_script_t *iscript;	
	const uc_script_t *script;	
	unsigned int i;
	unsigned int j;
	unsigned int c;
    
    c = 0;
    
    for (i = 0; i < str->len; i += j + 1) {
		j = 0;
		iscript = uc_script(str->str[i + j]);
		script = uc_script(str->str[i + j]);
		
		while (!strcmp(iscript->name, script->name)) {	
			j++;
			script = uc_script(str->str[i + j]);
		}
		c++;
	}
			   
	return c;
}

Runs *TextUtils_Runs_Create(FriBidiString *str) {
	Runs* runs;
	const uc_script_t *iscript;	
	const uc_script_t *script;	
	unsigned int i;
	unsigned int j;
 	unsigned int c;
  	DonnellBool jc;
 
	if (!str) {
		return NULL;
	}
	
	c = 0;
	
    runs = malloc(sizeof(Runs));
    if (!runs) {
        return NULL;
    }
     
    for (i = 0; i < str->len; i += j - 1) {
		j = 0;
		
		if (TextUtils_IsNewLine(str->str[i + j])) {
			i++;
		}

		if(!i) {
			runs->str = calloc(c+1, sizeof(FriBidiString*));
			if (!runs->str) {
				free(runs);
				return NULL;
			}
		} else {
			runs->str = realloc(runs->str, (c+1)*sizeof(FriBidiString*));
			if (!runs->str) {
				free(runs);
				return NULL;
			}			
		}

		script = uc_script(str->str[i + j]);
		iscript = uc_script(str->str[i + j]);
		
		if(!script) {
			puts("something has gone horribly wrong 1");
		}
	
		if(!iscript) {
			puts("something has gone horribly wrong 2");
		}
			
		while(!strcmp(script->name, iscript->name)) {
			jc = DONNELL_FALSE;
			if (TextUtils_IsNewLine(str->str[i + j])) {
				script = uc_script(str->str[i + j - 1]);
				j++;
				jc = DONNELL_TRUE;
				continue;
			}
			
			script = uc_script(str->str[i + j]);
	
			if(!script) {
				script = uc_script(str->str[i + j - 1]);
			}
						
			if ((jc) || (!j)) {
				runs->str[c] = FriBidiString_Create(1);
				if (!runs->str[c]) {
					free(runs->str);
					free(runs);
					return NULL;
				}				
			} else {
				runs->str[c]->str = realloc(runs->str[c]->str, (j+1)*sizeof(FriBidiChar));
				if (!runs->str[c]) {
					free(runs->str);
					free(runs);
					return NULL;
				}					
			}
			
			if (jc) {
				runs->str[c]->str[j-1] = str->str[i + j - 1];
				runs->str[c]->len = j;
			} else {
				runs->str[c]->str[j] = str->str[i + j];	
				runs->str[c]->len = j;
			}
			
			j++;
		}
		c++;
	}
	   
	runs->count = c;
	return runs;
}

