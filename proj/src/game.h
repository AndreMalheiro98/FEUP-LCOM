 /**
 * @file game.h
 * Main class of the project
 */
#ifndef _GAME_H
#define _GAME_H
#include "vbe.h"
#include "mouse.h"
#include "timer.h"  
#include "keyboard.h"
#include "menu.h"
#include "disk.h"
/** @defgroup game Game
 * @{
 *
 * Functions for using the game
 */
/**
 * @brief Main game struct
 */
typedef struct{
    Mouse *game_mouse;
    int state;
    Menu *main_menu;
    Menu *pause_menu;
    xpm_image_t game_background;
    Disk * disk;
    int total_bullet_count;
    bool shot_fired;
    int score;
    xpm_image_t bullet_img;
    int current_bullet_count;
    bool reload;
    xpm_image_t highscore_background;
    char *player_name;
}Game;

#define STATE_DRAW_MAIN_MENU 0
#define STATE_IN_MAIN_MENU 1
#define STATE_BEGIN_GAME 2
#define STATE_INSTRUCTIONS 3
#define STATE_DURING_GAME 4
#define STATE_END_GAME 5
#define STATE_EXIT 6
#define STATE_DRAW_PAUSE 7
#define STATE_IN_PAUSE 8
#define STATE_END_PROGRAM 9
#define STATE_SAVE_SCORE 10

/**
 * @brief Intialize graphics mode
*/
int game_init_graphics_mode();
/**
 * @brief Get current game state
*/
int get_game_state();
/**
 * @brief Subscribe to peripheral
*/
int subscribe_periphericals(uint32_t * mouse_mask,uint32_t * timer_mask,uint32_t * keyboard_mask);
/**
 * @brief  Unsubscribe from peripheral
*/
int unsubscribe_periphericals();
/**
 * @brief Exits game graphics mode
*/
int game_exit_graphic_mode();
/**
 * @brief Creates a new game 
*/
Game * create_new_game();
/**
 * @brief  Begins game
*/
int game_begin();
/**
 * @brief  Eliminates game
*/
void eliminate_game();
/**
 * @brief Updates mouse coordinates
*/
void update_mouse_coord(struct packet data);
/**
 * @brief Draws Screen
*/
int draw_screen(xpm_image_t xpm_image,int x,int y);
/**
 * @brief Treats mouse click accordingly
*/
void treat_mouse_click();
/**
 * @brief Game state machine
*/
int game_state_machine();
/**
 * @brief Draws mouse  
*/
void draw_mouse();
/**
 * @brief Handle interrupts from keyboard
*/
void handle_keyboard_interrupts();
/**
 * @brief Handle interrupts from mouse 
*/
void handle_mouse_interrupts();
/**
 * @brief Updates game
*/
void update_game();
/**
 * @brief  Handle interrupts from timer
*/
void handle_timer_interrupts();
/**
 * @brief  Draws score on the screen
*/
void draw_score(int x,int y);
/**
 * @brief  loads current score xpm img onto game score
*/
int parseScoreXpm(xpm_image_t *xpm,int i);
/**
 * @brief Updates background
*/
void update_background();
/**
 * @brief  Shows name on the screen
*/
void draw_name();
#endif
