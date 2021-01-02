#ifndef _GAME_H
#define _GAME_H
#include "vbe.h"
#include "mouse.h"
#include "timer.h"  
#include "keyboard.h"
typedef struct{
    Mouse *game_mouse;
    int state;
}Game;

#define STATE_DRAW_MAIN_MENU 0
#define STATE_IN_MAIN_MENU 1
#define STATE_BEGIN_GAME 2
#define STATE_INSTRUCTIONS 3
#define STATE_DURING_GAME 4
#define STATE_END_GAME 5
#define STATE_EXIT 6

int game_init_graphics_mode();
int get_game_state();
int subscribe_periphericals(uint32_t * mouse_mask,uint32_t * timer_mask);
int unsubscribe_periphericals();
int game_exit_graphic_mode();
Game * create_new_game();
int game_begin();
void eliminate_game();
void update_mouse_coord(struct packet data);
int draw_screen(xpm_map_t xpm_image);
void treat_mouse_click();
int game_update();
void draw_mouse();
void treat_key_board_keys();
#endif
