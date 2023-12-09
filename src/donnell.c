#include "donnell.h"

#include "symvis.h"
#include "textrenderer.h"
#include "bidi.h"
#include "textutils.h"

DONNELL_EXPORT void Donnell_Init(void) {
    TextRenderer_Init();
    TextUtils_Runs_Create(FriBidiString_ConvertFromUTF8("😀😬😁😂😃😄😅😆\nThis is the journey into sound\n阳光灿烂\nВиняткова технологія\n\nמימין לשמאל.\nحتى أطول من اليمين إلى اليسار\n"));
}

DONNELL_EXPORT void Donnell_Cleanup(void) {
    TextRenderer_Cleanup();
}
