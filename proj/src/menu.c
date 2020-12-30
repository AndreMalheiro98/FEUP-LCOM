#include <lcom/lcf.h>
#include "menu.h"

int load_main_menu(){
    if(vg_draw_pixmap(main_menu_xpm,0,0)!=0)
        return -1;
    return 0;
}
