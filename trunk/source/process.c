// Includes
#include <PA9.h>

// PAGfxConverter Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"
#include "main.h"
#include "queue.h"

void init() {
    PA_Init();
    PA_InitVBL();
    PA_InitText(UP_LCD, 0); // On the top screen

    pushed_stylus = 0;
    stylus_page = 1;
    pushX = 0; pushY = 0;
    initButtonSprites();
    
    cursorX = 1; cursorY = 1; // default input pos
    cursorPos = 0; printStrPos = 0; exprPos  = 0;
    initCursor();
    
    initLabel();
    
    memset(expr, 0, 200);
    memset(p_expr, 0, 340);
    
    queue_init();
}

void insertString(char *target, const int pos, const char *text, const int limit) {
    int i, len = strlen(text);
    memmove(target+pos+len, target+pos, limit - pos - 2);
    for (i = 0; i < len; i++) {
        target[pos+i] = text[i];
    }
}
int removeString(char *target, int pos, int size, int limit) {
    if (pos < 0) pos = 0;
    if (size == 0) return 0;
    memmove(target+pos, target+pos+size, limit - pos);
    return size;
}


uint8 actionStylus(const uint8 type) {
    switch (type) {
    case NON_VALUE:
        return 1;
    case NUMBER:
        break;
    case FUNCTION:
        addFunction();
        break;
    case ACTION:
        runAction();
        moveCursor(-1);
        break;
    }
    moveCursor(1);
    return 0;
}

void runAction() {
    if (buf[0] == 'E') {
    } else if (buf[0] == 'B') {
    } else if (buf[0] == 'D') {
    } else if (buf[0] == 'C') {
    }
}

uint8 checkValue(const char v) {
    if (v == ' ') return NON_VALUE;
    if (v == '.' || v >= '0' && v <= '9') return NUMBER;
    if (v == 'p' || v == 'e') return CONSTANT;
    if (v == '!' || v == '(' || v == ')'
        || v == '+' || v == '-' || v == '*' || v == '/') return SYMBOL;
    
    memset(buf, '\0', 5);

    if (v == 'E' || v == 'B' || v == 'D') {
        if (v == 'E')      strncpy(buf, "ENT", 3);  // ENTER
        else if (v == 'B') strncpy(buf, "BSP", 3);  // BACKSPACE
        else if (v == 'D') strncpy(buf, "DEL", 3);  // DELETE
        else if (v == 'Z') strncpy(buf, "CLR", 3);  // CLEAR
        return ACTION;
    }
    
    if (v == 'L') strncpy(buf, "ln", 2);
    else if (v == 'R') strncpy(buf, "root", 4);
    else if (v == 's') strncpy(buf, "sin", 3);
    else if (v == 'c') strncpy(buf, "cos", 3);
    else if (v == 't') strncpy(buf, "tan", 3);
    else if (v == 'S') strncpy(buf, "sec", 3);
    else if (v == 'C') strncpy(buf, "csc", 3);
    else if (v == 'T') strncpy(buf, "cot", 3);
    else if (v == 'l') strncpy(buf, "log", 3);
    else if (v == 'a') strncpy(buf, "abs", 3);
    else if (v == 'r') strncpy(buf, "rad", 3);
    else if (v == 'd') strncpy(buf, "deg", 3);
    return FUNCTION;
}

void addFunction() {
    int size = strlen(buf);
    insertString(expr, exprPos, "(", 200);
    p_expr[printStrPos] = '(';
    insertString(p_expr, printStrPos, buf, 340);
    //printStrPos+=size-1;
    exprPos++;
    moveCursor(size+1);
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

char getStylusValue() {
    int x = Stylus.X;
    int y = Stylus.Y;
    
    pushX = 2 < x && x < 48 ? 0 : (52 < x && x < 100 ? 1 : (104 < x && x < 151 ? 2 : (155 < x && x < 202 ? 3 : (206 < x && x < 253 ? 4 : -1 ))));
    
    pushY = 2 < y && y < 46 ? 0 : (50 < y && y < 94 ? 1 : (98 < y && y < 142 ? 2 : (146 < y && y < 190 ? 3 : -1)));
    
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
        8, 8 // button position
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

void moveCursor(int move) {
    if (move < 0) {
        if (exprPos == 0) return;
        exprPos -= 1;
    } else if (move > 0){
        if (exprPos >= strlen(expr)) return;
        exprPos += 1;
    }
    printStrPos += move; // TYPE SIZE ADD
    printStrPos = printStrPos < 0 ? 0 : printStrPos;
    exprPos = exprPos < 0 ? 0 : exprPos;
    
    cursorX = 1 + (printStrPos % 30);
    cursorY = 1 + printStrPos / 30;
    cursorY = cursorY <= 0 ? 1 : cursorY;
    cursorX = cursorX <= 0 ? 1 : cursorX;
    
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
                400, (j*48+8) // button position
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

void printExpr() {
    PA_ClearTextBg(UP_LCD);
    PA_BoxTextNoWrap(UP_LCD, 1, 1, 30, 18, p_expr, 340);
}
