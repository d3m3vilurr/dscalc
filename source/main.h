#ifndef __MAIN_H__
#define __MAIN_H__

#include <PA9.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UP_LCD 1
#define DOWN_LCD 0

#define BG_0 0
#define BG_1 1
#define BG_2 2
#define BG_3 3

#define COLOR256 1
#define COLOR16 0

#define MAXCOL 32
#define MAXROW 24

typedef struct number{
        int num_flag;   // 0 ÀÌ¸é Á¤¼ö, 1ÀÌ¸é ½Ç¼ö
        union type{
                int i;
                float f;
        }t;
}number;


/*typedef struct number {
    uint8 type;   // 0: int 1: float
    union value {
        int i;
        float f;
    } v;
} Num;*/
#define Num number 

uint8 pushed_stylus, pushed_trig, pushed_left, pushed_right, pushed_up, pushed_down;
uint8 stylus_page;    // now page
int pushX, pushY;
int cursorX, cursorY;
int printStrPos, exprPos, inputPos;
int count_x;
int overview_expr;
int minPos, maxOrder, result_print, inputed_x, on_graph, var;
Num var_x[2], result;
char exprs[10][340];
uint8 order[10];

static int EVAL_ERR;
static int page = 2;

char expr[200], p_expr[340], buf[5];
char inputValue[100];

enum {NUMBER, FUNCTION, CONSTANT, SYMBOL, ACTION, NON_VALUE, VARIABLE, GRAPHIC};

static const char InputTable1[4][5] = {
  { '7', '8', '9', '+', 'B' },
  { '4', '5', '6', '-', 'D' },
  { '1', '2', '3', '*', 'Z' },
  { '0', '.', 'x', '/', 'E' }
};
static const char InputTable2[4][5] = {
  { '(', ')', 'p', 'e', '!' },
  { 's', 'c', 't', 'l', 'r' },
  { 'S', 'C', 'T', 'L', 'd' },
  { '^', 'R', 'm', 'a', 'P' }
};

void init();
void initButtonSprites();
void initCursor();
void initLabel();

uint8 actionStylus(const uint8);
uint8 actionStylus2(const uint8);
void runAction();
void runAction2();

void insertString(char*, int, const char*, int);
int removeString(char*, int, int, int);
uint8 checkValue(const char);
void addFunction();
char getStylusValue();
void clearButton();
void pushButton();
void changeLabel(int);
void printExpr();
void printResult(Num);
void printInputStr();
void printGraph();
void moveCursor(int);
void overviewExpr();
void overviewNextExpr();
void overviewPrevExpr();

void hiddenStripe();
void expr_init();
void expr_insert(const char *);
void findMinPos();
#ifdef __cplusplus
}
#endif

#endif
