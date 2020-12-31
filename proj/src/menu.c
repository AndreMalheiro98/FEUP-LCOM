#include <lcom/lcf.h>
#include "menu.h"
#include "../Images/menu_background.XPM"
int load_main_menu(){
    //xpm_image_t menu_img;
    /*if(load_pixmap(main_menu_xpm, &menu_img)==NULL)
    {
        printf("Error loading main menu pixmap\n");
        return -1;
    }*/
    vg_draw_pixmap(main_menu_xpm,0,0);
    return 0;
}
