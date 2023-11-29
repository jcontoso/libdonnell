#include <stdlib.h>
#include <string.h>
#include "misc.h"

void* Misc_MemDup(void* mem, size_t size) { 
   void* dmem;
   
   dmem = malloc(size);

   if(dmem) {
       memcpy(dmem, mem, size); 
   }

   return dmem;
}
