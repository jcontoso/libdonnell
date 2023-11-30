#include "donnell.h"

#include "fontpicker.h"
#include "freetype.h"
#include "symvis.h"

DONNELL_EXPORT void Donnell_Init(void) {
    FontPicker_Init();
    FreeType_Init();
}

DONNELL_EXPORT void Donnell_Cleanup(void) {
    FreeType_Cleanup();
    FontPicker_Cleanup();
}
