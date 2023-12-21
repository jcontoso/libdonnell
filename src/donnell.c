#include <stdio.h>

#include "donnell.h"

#include "bidi.h"
#include "symvis.h"
#include "textrenderer.h"
#include "textutils.h"
#include "stockimages.h"

DONNELL_EXPORT void Donnell_Init(void) {
    TextRenderer_Init();
    StockImages_Init();
}

DONNELL_EXPORT void Donnell_Cleanup(void) {
    TextRenderer_Cleanup();
	StockImages_Cleanup();
}
