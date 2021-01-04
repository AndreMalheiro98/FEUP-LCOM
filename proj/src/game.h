#ifndef _GAME_H
#define _GAME_H
#include "vbe.h"
#include "mouse.h"
#include "timer.h"  
#include "keyboard.h"
#include "menu.h"
#include "disk.h"
typedef struct{
    Mouse *game_mouse;
    int state;
    Menu *main_menu;
    Menu *pause_menu;
    xpm_image_t game_background;
    Disk * disk;
}Game;

#define STATE_DRAW_MAIN_MENU 0
#define STATE_IN_MAIN_MENU 1
#define STATE_BEGIN_GAME 2
#define STATE_INSTRUCTIONS 3
#define STATE_DURING_GAME 4
#define STATE_DRAW_PAUSE 7
#define STATE_IN_PAUSE 8
#define STATE_END_GAME 5
#define STATE_EXIT 6

int game_init_graphics_mode();
int get_game_state();
int subscribe_periphericals(uint32_t * mouse_mask,uint32_t * timer_mask,uint32_t * keyboard_mask);
int unsubscribe_periphericals();
int game_exit_graphic_mode();
Game * create_new_game();
int game_begin();
void eliminate_game();
void update_mouse_coord(struct packet data);
int draw_screen(xpm_image_t xpm_image,int x,int y);
void treat_mouse_click();
int game_state_machine();
void draw_mouse();
void handle_keyboard_interrupts();
void handle_mouse_interrupts();
void update_game();
#endif
