#include "donnell.h"

#include "fontconfig.h"
#include "freetype.h"
#include "harfbuzz.h"
#include "symvis.h"

DONNELL_EXPORT void Donnell_Init(void) {
    FontConfig_Init();
    FreeType_Init();
    HarfBuzz_Init();
}

DONNELL_EXPORT void Donnell_Cleanup(void) {
    FreeType_Cleanup();
    FontConfig_Cleanup();
    HarfBuzz_Cleanup();
}
