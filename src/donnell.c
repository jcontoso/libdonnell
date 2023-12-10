#include "donnell.h"

#include "symvis.h"
#include "textrenderer.h"
#include "bidi.h"
#include "textutils.h"

DONNELL_EXPORT void Donnell_Init(void) {
    TextRenderer_Init();
    return;
    
    FriBidiString* str;
    Runs* runs;
   
    str = FriBidiString_ConvertFromUTF8("😀\n");
    
	FriBidiString_Handle(str);

    runs = TextUtils_Runs_Create(str);
    
    for (int i = 0; i < runs->count; i++) {
		puts(FriBidiString_ConvertToUTF8(runs->str[i]));
	}    
}

DONNELL_EXPORT void Donnell_Cleanup(void) {
    TextRenderer_Cleanup();
}
