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
int subscribe_periphericals();
int unsubscribe_periphericals();
int game_exit_graphic_mode();
Game * create_new_game();
void eliminate_game();
#endif
