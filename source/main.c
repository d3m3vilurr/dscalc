
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
void initCursor();
void moveCursor();
void initLabel();
void changeLabel();

uint8 pushed_stylus, pushed_trig;
uint8 stylus_page;    // now page
int pushX, pushY;
int cursorX, cursorY;
int cursorPos;

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
    
    cursorX = 0; cursorY = 1; // default input pos
    cursorPos = 0;
    initCursor();
    
    initLabel();
    changeLabel();
    
    while(1) {
        // test case
        //    PA_Print(UP_LCD, " sin(%d) = %f5\n", i, sin(deg2rad(i)));
        // input interrupt
        if (Stylus.Released) {
            clearButton();
        }
        if (Pad.Released.L || Pad.Released.R) {
            pushed_trig = 0;
        }
        
        if (Stylus.Held) {
            if (!pushed_stylus) {
                buf[0] = getStylusValue();
                switch (buf[0]) {
                case ' ':
                    goto LOOP_END;
                case 'E': // ENTER
                    PA_Print(UP_LCD, "enter\n");
                    break;
                case 'b': // BACK-SPACE
                    break;
                case 'd': // DELETE
                    break;
                case 'c': // CLEAR
                    break;
                }
                
                PA_Print(UP_LCD, "%s", buf);
                cursorPos++;
                cursorX += 1;
                if (cursorPos % 32 == 0) {
                    cursorY += 1;
                    cursorX = 0;
                }
                
            }
            pushed_stylus = 1;
        }
        
        if (Pad.Held.L || Pad.Held.R) {
            if (!pushed_trig) {
                stylus_page = stylus_page == 1 ? 2 : 1;
                changeLabel();
            }
            pushed_trig = 1;
        }
        
        // action
        moveCursor();
        
    LOOP_END:
        PA_WaitForVBL(); // process interrupt
    }
    return 0;
}

#define BUTTON_START 107
void initButtonSprites() {
    int i, j;
    
    pushed_stylus = 1;
    PA_LoadSpritePal(
        DOWN_LCD,
        0, // Palette number
        (void*)bbg_Pal
    );
    
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 4; j++) {
            PA_CreateSprite(
                DOWN_LCD, 
                (4 * i) + j + BUTTON_START, // Sprite number
                (void*)button_bg_Sprite,
                OBJ_SIZE_64X64,
                COLOR256,
                0, // Sprite palette number
                i*51, j*48 // button position
            );
        }
    }
}

static const char InputTable1[4][5] = {
  { '7', '8', '9', '+', 'b' },
  { '4', '5', '6', '-', 'd' },
  { '1', '2', '3', '*', 'c' },
  { '0', '.', 'x', '/', 'E' }
};
static const char InputTable2[4][5] = {
  { '(', ')', 'p', 'e', '!' },
  { 's', 'c', 't', 'l', 'r' },
  { 'S', 'C', 'T', 'L', 'd' },
  { '^', 'R', 'f', 'a', 'p' }
};

char getStylusValue() {
    int x = Stylus.X;
    int y = Stylus.Y;
    
    pushX = 2 < x && x < 48 
            ? 0 
            : (52 < x && x < 100 
               ? 1
               : (104 < x && x < 151 
                  ? 2
                  : (155 < x && x < 202
                     ? 3
                     : (206 < x && x < 253
                        ? 4
                        : -1
                       )
                    )
                 )
              );
    pushY = 2 < y && y < 46
            ? 0
            : (50 < y && y < 94 
               ? 1 
               : (98 < y && y < 142 
                  ? 2 
                  : (146 < y && y < 190 
                     ? 3 
                     : -1
                    )
                 )
              );
    
    if (pushX == -1 || pushY == -1) {
        pushX = 0;
        pushY = 0;
        return ' ';
    }
    pushButton();

    // TODO: page select refactoring
    if (stylus_page == 2)
        return InputTable2[pushY][pushX];
    return InputTable1[pushY][pushX];
}

void pushButton() {
    PA_SetSpriteAnim(
        DOWN_LCD,
        (pushX*4)+pushY + BUTTON_START, // sprite number
        1 // set frame is 1
    );
}
void clearButton() {
    PA_SetSpriteAnim(
        DOWN_LCD,
        (pushX*4)+pushY + BUTTON_START, // sprite number
        0 // set frame is 0
    );
    pushed_stylus = false;
}

#define CURSOR_ID 0
void initCursor() {
    pushed_stylus = 1;
    PA_LoadSpritePal(
        UP_LCD,
        1, // Palette number
        (void*)cursor_Pal
    );
    
    PA_CreateSprite(
        UP_LCD, 
        CURSOR_ID, // Sprite number
        (void*)cursor_Sprite,
        OBJ_SIZE_8X8,
        COLOR256,
        1, // Sprite palette number
        0, 8 // button position
    );
    
    PA_StartSpriteAnimEx(
        UP_LCD,
        CURSOR_ID,
        0,
        1,
        2,
        ANIM_UPDOWN,
        -1
    );
}

void moveCursor() {
    PA_SetSpriteXY(
        UP_LCD,
        CURSOR_ID,
        cursorX*8,
        cursorY*8
    );
}

#define LABEL1_START 86
#define LABEL2_START 65
void initLabel() {
    int i, j;
    
    pushed_stylus = 1;
    PA_LoadSpritePal(
        DOWN_LCD,
        2, // Palette number
        (void*)label1_Pal
    );

    PA_LoadSpritePal(
        DOWN_LCD,
        3, // Palette number
        (void*)label2_Pal
    );
    
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 4; j++) {
            /* LABEL 1 */
            PA_CreateSprite(
                DOWN_LCD, 
                (4*i) + j + LABEL1_START, // Sprite number
                (void*)label1_Sprite,
                OBJ_SIZE_64X32,
                COLOR16,
                2, // Sprite palette number
                i*51, (j*48+8) // button position
            );
            PA_SetSpriteAnim(
                DOWN_LCD,
                (4*i) + j + LABEL1_START, // sprite number
                (5*j) + i // set frame is 0
            );
            
            /* LABEL 2 */
            PA_CreateSprite(
                DOWN_LCD, 
                (4*i) + j + LABEL2_START, // Sprite number
                (void*)label2_Sprite,
                OBJ_SIZE_64X32,
                COLOR16,
                3, // Sprite palette number
                i*51, (j*48+8) // button position
            );
            PA_SetSpriteAnim(
                DOWN_LCD,
                (4*i) + j + LABEL2_START, // sprite number
                (5*j) + i // set frame is 0
            );

        }
    } 
}


void changeLabel() {
    int i, j;
    int hidden_page = stylus_page == 1 ? LABEL2_START : LABEL1_START;
    int view_page = stylus_page == 1 ? LABEL1_START : LABEL2_START;
    
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 4; j++) {
            PA_SetSpriteX(
                DOWN_LCD,
                (4*i) + j + hidden_page,
                400
            );

            PA_SetSpriteX(
                DOWN_LCD,
                (4*i) + j + view_page,
                i*51
            );
        }
    }
}
