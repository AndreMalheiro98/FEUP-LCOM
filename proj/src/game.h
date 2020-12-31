#ifndef _GAME_H
#define _GAME_H
#include "vbe.h"
#include "menu.h"
#include "mouse.h"
#include "kbc.h"
#include "timer.h"  

typedef struct{
    Mouse *game_mouse;

}Game;
int game_init_graphics_mode();
int subscribe_periphericals(uint32_t * mouse_mask,uint32_t * timer_mask);
int unsubscribe_periphericals();
int game_exit_graphic_mode();
Game * create_new_game();
void eliminate_game();
void update_mouse_coord(struct packet data);
int draw_screen();
int draw_menu();
#endif
