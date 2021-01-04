#ifndef __MENU_H
#define __MENU_H

typedef struct{
    uint16_t x;
    uint16_t y;
    xpm_image_t img;
}Menu;

/**
 * @brief  
**/
Menu * get_menu(int x);

/**
 * @brief  
**/
void delete_menus();

#endif
