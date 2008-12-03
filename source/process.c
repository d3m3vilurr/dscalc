// Includes
#include <PA9.h>

// PAGfxConverter Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"
#include "main.h"
#include "common.h"

void init() {
    PA_Init();
    PA_InitVBL();
    PA_InitText(UP_LCD, 0); // On the top screen

    pushed_stylus = 0;
    stylus_page = 1;
    pushX = 0; pushY = 0;
    initButtonSprites();
    
    cursorX = 1; cursorY = 1; // default input pos
    printStrPos = 0; exprPos  = 0;
    count_x = 0;
    result_print = 0;
    
    overview_expr = 0;
    
    initCursor();
    
    initLabel();
    
    memset(expr, 0, 200);
    memset(p_expr, 0, 340);
    
    expr_init();
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
    case VARIABLE:
        count_x++;
        break;
    case FUNCTION:
        addFunction();
        moveCursor(-1);
        break;
    case ACTION:
        runAction();
        moveCursor(-1);
        break;
    }
    changeLabel(1);
    moveCursor(1);
    return 0;
}

void runAction() {
    /* no input except case */
    if (exprPos <= 0) {
        removeString(p_expr, printStrPos-1, 2, 340);
        removeString(expr, exprPos-1, 2, 200);
        return;
    }
    
    if (buf[0] == 'E') { // enter
        removeString(p_expr, printStrPos, 1, 340);
        expr_insert(p_expr);
        if (count_x <= 0) { // expression
//            printResult(eval1(expr));
            result_print = 1;
        } else { // equation
            result_print = 2;
        }
    } else if (buf[0] == 'B') { // backspace
        int v = checkValue(expr[exprPos-1]);
        if (v == FUNCTION) {
            int len = strlen(buf);
            removeString(p_expr, printStrPos-len, len+1, 340);
        } else {
            if (v == VARIABLE) count_x--;
            removeString(p_expr, printStrPos-1, 2, 340);
        }
        moveCursor(-1);
    } else if (buf[0] == 'D') { // delete
        int v = checkValue(expr[exprPos+1]);
        if (v == FUNCTION) {
            int len = strlen(buf);
            removeString(p_expr, printStrPos, len+1, 340);
        } else {
            if (v == VARIABLE) count_x--;
            removeString(p_expr, printStrPos, 2, 340);
        }
    } else if (buf[0] == 'C') { // clear
        memset(expr, '\0', 200);
        memset(p_expr, '\0', 340);
        exprPos = 0;
        printStrPos = 0;
        count_x = 0;
        moveCursor(0);
        return;
    }
    
    /* remove action char */
    removeString(expr, exprPos, 2, 200);

}

uint8 checkValue(const char v) {
    if (v == ' ') return NON_VALUE;
    if (v == '.' || v >= '0' && v <= '9') return NUMBER;
    if (v == 'p' || v == 'e') return CONSTANT;
    if (v == '!' || v == '(' || v == ')'
        || v == '+' || v == '-' || v == '*' || v == '/') return SYMBOL;
    if (v == 'x') return VARIABLE;
    
    
    memset(buf, '\0', 5);

    if (v == 'E' || v == 'B' || v == 'D' || v == 'Z') {
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
    else if (v == 'm') strncpy(buf, "rand", 4);
    return FUNCTION;
}

void addFunction() {
    int size = strlen(buf);
    insertString(expr, exprPos+1, "(", 200);
    p_expr[printStrPos] = '(';
    insertString(p_expr, printStrPos, buf, 340);
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
    int len = move;
    if (move < 0) {
        if (checkValue(expr[exprPos-1]) == FUNCTION) {
            len = strlen(buf) * -1;
        }
    } else if (move > 0) {
        if (checkValue(expr[exprPos]) == FUNCTION) {
            len = strlen(buf);
        }
    }
    printStrPos += len;
    if (move < 0) {
        if (exprPos == 0) return;
        exprPos -= 1;
    } else if (move > 0){
        if (exprPos >= strlen(expr)) return;
        exprPos += 1;
    }
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


void changeLabel(int page) {
    int i, j;
    stylus_page = page;
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
    // DEBUG CODE
    //PA_OutputText(UP_LCD, 1, 20, expr);
    //double ln_10 = log(10)/log(2.7182818284590451);
    //PA_OutputText(UP_LCD, 2, 10, "%d.%d", (int)ln_10, (int)((ln_10-(int)ln_10)*100000000));
    PA_BoxTextNoWrap(UP_LCD, 1, 1, 30, 10, p_expr, 340);
    overviewExpr();
    if (result_print == 1) printResult(eval1(expr));
    else if (result_print == 2) printResult(eval2(expr, var_x));
    
}
void printResult(Num n) {
    char result[100];
    if (n.type) { // float
        snprintf(result, 100, "  = %d.%d", (int)n.v.f, (int)((n.v.f - (int)n.v.f)*1000000000));
    } else { // int
        snprintf(result, 100, "  = %d", n.v.i);
    }
    
    PA_BoxTextNoWrap(UP_LCD, 1, 12, 30, 14, result, 100);
}

void overviewExpr() {
    if (overview_expr == -1) { // view current
        PA_BoxTextNoWrap(UP_LCD, 1, 15, 30, 23, exprs[minPos], 340);
    } else {
        PA_BoxTextNoWrap(UP_LCD, 1, 15, 30, 23, exprs[overview_expr], 340);
    }
}

void expr_init() {
    int i;
    for (i = -10; i < 0; i++) {
        order[10+i] = i;
        memset(exprs[10+i], '\0', 340);
    }
}

void expr_insert(const char *expr) {
    findMinPos();
    strncpy(exprs[minPos], expr, 340);
    order[minPos] = maxOrder+1;
}

void findMinPos() {
    int i, minP = 0, max = order[0];
    
    for (i = 1; i < 10; i++) {
        if (order[minP] > order[i]) minP = i;
        if (max < order[i]) order[i] = max;
    }
    
    minPos = minP;
    maxOrder = max;
}

void overviewPrevExpr() {
    int next = overview_expr - 1;
    if (overview_expr == -1) next = minPos - 1;
    if (next < 0) next = 9;
    if (exprs[next][0] == '\0') return;
    overview_expr = next;
}
void overviewNextExpr() {
    int next = overview_expr + 1;
    if (overview_expr == -1) next = minPos + 1;
    if (next > 9) next = 0;
    if (exprs[next][0] == '\0') return;
    overview_expr = next;
}
