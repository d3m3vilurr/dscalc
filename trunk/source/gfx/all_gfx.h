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

// Background files : 
extern const int bg_Info[3]; // BgMode, Width, Height
extern const unsigned short bg_Map[768] __attribute__ ((aligned (4))) ;  // Pal : bg_Pal
extern const unsigned char bg_Tiles[12160] __attribute__ ((aligned (4))) ;  // Pal : bg_Pal
extern PAGfx_struct bg; // background pointer


// Palette files : 
extern const unsigned short bbg_Pal[51] __attribute__ ((aligned (4))) ;
extern const unsigned short cursor_Pal[2] __attribute__ ((aligned (4))) ;
extern const unsigned short label1_Pal[19] __attribute__ ((aligned (4))) ;
extern const unsigned short label2_Pal[32] __attribute__ ((aligned (4))) ;
extern const unsigned short bg_Pal[140] __attribute__ ((aligned (4))) ;


#endif

