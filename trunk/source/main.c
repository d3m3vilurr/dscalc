#include "common.h"
#include "main.h"

int main(void){

    // initialize
    init();
    
    while(1) {
        // input & process
        if (Stylus.Released) {
            clearButton();
        }
        if (Pad.Released.L || Pad.Released.R) {
            pushed_trig = 0;
        }
        if (Pad.Released.Left || Pad.Released.Y) {
            pushed_left = 0;
        }
        if (Pad.Released.Right || Pad.Released.A) {
            pushed_right = 0;
        }
        if (Pad.Released.Up || Pad.Released.X) {
            pushed_up = 0;
        }
        if (Pad.Released.Down || Pad.Released.B) {
            pushed_down = 0;
        }

        if (Stylus.Held) {
            if (!pushed_stylus) {
                insertString(expr, exprPos, " ", 200);
                expr[exprPos] = getStylusValue();
                insertString(p_expr, printStrPos, " ", 340);
                p_expr[printStrPos] = expr[exprPos];

                if (actionStylus(checkValue(expr[exprPos])))
                    goto LOOP_END;
                
            }
            pushed_stylus = 1;
        }
        
        if (Pad.Held.L || Pad.Held.R) {
            if (!pushed_trig) {
                stylus_page = stylus_page == 1 ? 2 : 1;
                changeLabel(stylus_page);
            }
            pushed_trig = 1;
        }
        
        if (Pad.Held.Left || Pad.Held.Y) {
            if (!pushed_left) {
                moveCursor(-1); // left move
            }
            pushed_left = 1;
        }
        if (Pad.Held.Right || Pad.Held.A) {
            if (!pushed_right) {
                moveCursor(1); // right move
            }
            pushed_right = 1;
        }
        if (Pad.Held.Up || Pad.Held.X) {
            if (!pushed_up) {
                overviewPrevExpr();
            }
            pushed_up = 1;
        }
        if (Pad.Held.Down || Pad.Held.B) {
            if (!pushed_down) {
                overviewNextExpr();
            }
            pushed_down = 1;
        }
        // teardown
        printExpr();
        //moveCursor(1);
        
    LOOP_END:
        PA_WaitForVBL(); // process interrupt
    }
    return 0;
}


