//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .c, for easier inclusion in a project

#ifdef __cplusplus
extern "C" {
#endif

#include "all_gfx.h"


// Sprite files : 
#include "button_bg.c"
#include "cursor.c"
#include "label1.c"
#include "label2.c"

// Background files : 
#include "bg.c"
PAGfx_struct bg = {(void*)bg_Map, 768, (void*)bg_Tiles, 12160, (void*)bg_Pal, (int*)bg_Info };

// Palette files : 
#include "bbg.pal.c"
#include "cursor.pal.c"
#include "label1.pal.c"
#include "label2.pal.c"
#include "bg.pal.c"


#ifdef __cplusplus
}
#endif

