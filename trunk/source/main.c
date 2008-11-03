
// Includes
#include <PA9.h>

// PAGfxConverter Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"
#include "common.h"

int main(void){
    int i;

    PA_Init();
    PA_InitVBL();
    
    PA_InitText(UP_LCD, 0); // On the top screen
    PA_Print(UP_LCD, "\n");

    // test case
    for (i = 0; i <= 180; i++ ) {
        PA_Print(UP_LCD, " sin(%d) = %f5\n", i, sin(deg2rad(i)));
        PA_WaitForVBL(); // process interrupt
    }
    
    while(1)
    {
        // input interrupt
        
        // action
        
        // loop
        PA_WaitForVBL(); // process interrupt
    }
    return 0;
}
