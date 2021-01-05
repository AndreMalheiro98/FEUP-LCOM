 /**
 * @file menu.h
 * Menu
 */
#ifndef __MENU_H
#define __MENU_H
/**
 * @brief Menu coordinates and its image
 */ 
typedef struct{
    uint16_t x;
    uint16_t y;
    xpm_image_t img;
}Menu;

/**
 * @brief Get Menu
**/
Menu * get_menu(int x);

/**
 * @brief  Eliminates menu
**/
void delete_menus();

#endif
