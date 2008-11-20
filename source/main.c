
// Includes
#include <PA9.h>

// PAGfxConverter Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"
#include "common.h"

void initButtonSprites();
char getStylusValue();
void clearButton();
void pushButton();

uint8 pushed_stylus;
uint8 stylus_page;    // now page
int pushX, pushY;

static int page = 2;
int main(void){
    char buf[2];
    buf[1] = '\0';

    PA_Init();
    PA_InitVBL();
    
    PA_InitText(UP_LCD, 0); // On the top screen
    PA_Print(UP_LCD, "\n");

    // init
    pushed_stylus = 0;
    stylus_page = 1;
    pushX = 0; pushY = 0;
    initButtonSprites();
    
    while(1) {
        // test case
        //    PA_Print(UP_LCD, " sin(%d) = %f5\n", i, sin(deg2rad(i)));
        // input interrupt
        if (Stylus.Released) {
            clearButton();
        }
        
        if (Stylus.Held) {
            if (!pushed_stylus) {
                buf[0] = getStylusValue();

                if (buf[0] == 'r') {
                    PA_Print(UP_LCD, "add(..) = %d\n", (int)add(10, 20));
                } else {
                    PA_Print(UP_LCD, "%s", buf);
                }
                
            }
            pushed_stylus = 1;
        }
        // action
        
        // loop
        PA_WaitForVBL(); // process interrupt
    }
    return 0;
}
void initButtonSprites() {
    int i, j;
    
    pushed_stylus = 1;
    PA_LoadSpritePal(
        DOWN_LCD,
        0, // Palette number
        (void*)bbg_Pal
    );
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            PA_CreateSprite(
                DOWN_LCD, 
                (4*i) + j, // Sprite number
                (void*)button_bg_Sprite,
                OBJ_SIZE_64X64,
                COLOR256,
                0, // Sprite palette number
                i*64, j*48 // button position
            );    
        }
    }
}

static const char InputTable1[4][4] = {
  { '7', '8', '9', '+' },
  { '4', '5', '6', '-' },
  { '1', '2', '3', '*' },
  { '0', '.', 'r', '/' }
};
static const char InputTable2[4][4] = {
  { '7', '8', '9', '+' },
  { '4', '5', '6', '-' },
  { '1', '2', '3', '*' },
  { '0', '.', 'r', '/' }
};

char getStylusValue() {
    int x = Stylus.X;
    int y = Stylus.Y;
    
    pushX = x < 64 ? 0 : (x < 128 ? 1 : (x < 192 ? 2 : 3));
    pushY = y < 48 ? 0 : (y < 96 ? 1 : (y < 144 ? 2 : 3));
    
    pushButton();

    // TODO: page select refactoring
    if (stylus_page == 2)
        return InputTable2[pushY][pushX];
    return InputTable1[pushY][pushX];
}

void pushButton() {
    PA_SetSpriteAnim(
        DOWN_LCD,
        (pushX*4)+pushY, // sprite number
        1 // set frame is 1
    );
}
void clearButton() {
    PA_SetSpriteAnim(
        DOWN_LCD,
        (pushX*4)+pushY, // sprite number
        0 // set frame is 0
    );
    pushed_stylus = false;
}
