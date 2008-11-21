//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .h, for easier inclusion in a project

#ifndef ALL_GFX_H
#define ALL_GFX_H

#ifndef PAGfx_struct
    typedef struct{
    void *Map;
    int MapSize;
    void *Tiles;
    int TileSize;
    void *Palette;
    int *Info;
} PAGfx_struct;
#endif


// Sprite files : 
extern const unsigned char button_bg_Sprite[8192] __attribute__ ((aligned (4))) ;  // Pal : bbg_Pal
extern const unsigned char cursor_Sprite[128] __attribute__ ((aligned (4))) ;  // Pal : cursor_Pal
extern const unsigned char label1_Sprite[20480] __attribute__ ((aligned (4))) ;  // Pal : label1_Pal
extern const unsigned char label2_Sprite[20480] __attribute__ ((aligned (4))) ;  // Pal : label2_Pal

// Palette files : 
extern const unsigned short bbg_Pal[51] __attribute__ ((aligned (4))) ;
extern const unsigned short cursor_Pal[2] __attribute__ ((aligned (4))) ;
extern const unsigned short label1_Pal[19] __attribute__ ((aligned (4))) ;
extern const unsigned short label2_Pal[9] __attribute__ ((aligned (4))) ;


#endif

