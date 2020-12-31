#include <lcom/lcf.h>
#include "game.h"
static Game *game;
int game_init_graphics_mode(){
  vbe_mode_info_t mode_info;
  void *video_mem=init_graphics_mode(VBE_MODE_4,&mode_info);
  if(video_mem==NULL)
    return -1;
    
  return 0;
}

int game_exit_graphic_mode(){
  if(vg_exit()!=0)
  {
    printf("Error setting text mode\n");
    return -1;
  }
  return 0;
}

int subscribe_periphericals(){
  if(start_mouse()!=0)
    return -1;
  return 0;
}

int unsubscribe_periphericals(){
  if(disable_mouse()!=0)
    return -1;
    
  return 0;
}

Game * create_new_game(){
  bool nu=0;
  if(game==NULL)
    game=(Game *)malloc(sizeof(Game));
  else
    nu=1;
  game->game_mouse=get_mouse();
  if(game->game_mouse==NULL || nu==1)
  {
    printf("Error creating game\n");
    return NULL;
  }
  game->game_mouse->x=get_hres()/2;
  game->game_mouse->y=get_vres()/2;
  return game;
}

void eliminate_game(){
  eliminate_Mouse();
  free(game);
}

void update_mouse_coord(struct packet data){
  
}

int draw_screen(){
  
  return 0;
}

int draw_menu(){
  if(load_main_menu()!=0)
    return -1;
  update_mouse(game->game_mouse->img,game->game_mouse->x,game->game_mouse->y);
  refresh_screen();
  return 0;
}
