#include <lcom/lcf.h>
#include "game.h"
#include "../Images/game_background.xpm"
#include "../Images/main_menu.xpm"
#include "../Images/pause_menu.xpm"
static Game *game;
extern uint8_t bytes[2];
extern bool kbc_flag;
extern uint8_t mouse_data;
extern bool mouse_flag;

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

int subscribe_periphericals(uint32_t * mouse_mask,uint32_t * timer_mask,uint32_t * keyboard_mask){
  if(start_mouse(mouse_mask)!=0)
    return -1;

  if(kbc_subscribe_interrupts(keyboard_mask)!=0)
  {
    printf("Error subscribing mouse interrupts\n");
    return -1;
  }

  uint8_t aux;
  if(timer_subscribe_int(&aux)!=0)
  {
    printf("Error subscribing timer0 interrupts\n");
    return -1;
  }
  *timer_mask=BIT(aux);

  printf("Mouse, timer0 and keyboard successfully enabled\n");
  return 0;
}

int unsubscribe_periphericals(){
  if(disable_mouse()!=0)
    return -1;

  if(kbc_unsubscribe_interrupts()!=0)
  {
    printf("Error unsubscribing keyboard interrupts\n");
    return -1;
  }

  if(timer_unsubscribe_int()!=0){
    printf("Error unsubscribing timer0 interrupts\n");
    return -1;
  }

  printf("Mouse, timer0 and keyboard successfully disabled\n");  
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
  game->state=STATE_DRAW_MAIN_MENU;
  game->game_mouse->x=get_hres()/2;
  game->game_mouse->y=get_vres()/2;
  return game;
}

int get_game_state(){
  return game->state;
}

int game_begin(){
  if(draw_screen(game_background_xpm,0,0)==-1)
    return -1;
  
  return 0;
}

void eliminate_game(){
  eliminate_Mouse();
  free_buffers();
  free(game);
}

void update_mouse_coord(struct packet data){
  mouse_update_position(data);
}

int draw_screen(xpm_map_t xpm_image,int x,int y){
  if(vg_draw_pixmap(xpm_image,x,y)!=0)
    return -1;
  draw_mouse();
  return 0;
}

int game_update(){
  switch (game->state)
  {
  case STATE_DRAW_MAIN_MENU:
    if(!draw_screen(main_menu_xpm,0,0))
      game->state=STATE_IN_MAIN_MENU;
    break;
  case STATE_IN_MAIN_MENU:
    draw_mouse();
    break;
  case STATE_BEGIN_GAME:
    if(!game_begin())
      game->state= STATE_DURING_GAME;
    break;
  case STATE_DURING_GAME:
    draw_mouse();
    break;
  case STATE_END_GAME:
    game->state=STATE_EXIT;
    break;
  case STATE_DRAW_PAUSE:
    if(!draw_screen(pause_menu_xpm,0,0))
      game->state=STATE_IN_PAUSE;
    break;
  case STATE_IN_PAUSE:
    draw_mouse();
    break;
  default:
    break;
  }
  return 0;
}

void treat_mouse_click(){
  switch (game->state)
  {
  case STATE_IN_MAIN_MENU:
    if(game->game_mouse->x>=295 && game->game_mouse->x<=857){ 
      if(game->game_mouse->y>=622 && game->game_mouse->y<=717)  //CHECK IF USER CLICKED ON EXIT
        game->state=STATE_END_GAME;
      else if(game->game_mouse->y>=342 && game->game_mouse->y<=437) //CHECK IF USER CLICKED ON PLAY 
        game->state=STATE_BEGIN_GAME;
      /*else if(game->game_mouse->y>=482 && game->game_mouse->y<=577) //CHECK IF USER CLICKED ON HIGHSCORE*/
    }
    break;
  case STATE_IN_PAUSE:
    if(game->game_mouse->x>=110 && game->game_mouse->x<=857){
      if(game->game_mouse->y>=148 && game->game_mouse->y<=227)
        game->state=STATE_BEGIN_GAME;
      else if(game->game_mouse->y>=254 && game->game_mouse->y<=333)
        game->state=STATE_DRAW_MAIN_MENU;
    } 
  default:
    break;
  }
}

void draw_mouse(){
  update_mouse(game->game_mouse->img,game->game_mouse->x,game->game_mouse->y);
  refresh_screen();
}

void handle_keyboard_interrupts(){
  static bool make = 0;
  static int i = 0;
  kbc_ih();
  enum KBC_KEY pressed_key=NO_KEY;
  if(kbc_flag==1)
  {
    if(bytes[1]&BREAKCODE)
      make=0;
    else
      make=1;
    if(i==2)
    {
      pressed_key=get_key_pressed(make,2,bytes);
      kbc_flag=0;
      i=0;
    }
  }
  else
  {
    i=0;
    if(bytes[0]&BREAKCODE)
      make=0;
    else
      make=1;
    pressed_key=get_key_pressed(make,1,bytes);
    kbc_flag=0;
  }
  switch (pressed_key)
  {
  case ESC_BC:
    if(game->state==STATE_DURING_GAME)
      game->state=STATE_DRAW_PAUSE;
    else if(game->state==STATE_IN_PAUSE)
      game->state=STATE_BEGIN_GAME;
    break;
  default:
    break;
  }
}

void handle_mouse_interrupts(){
  static uint8_t mouse_dados[3];
  static uint8_t mouse_bytes=0;
  mouse_ih();
  if(mouse_flag)
  {
    if(mouse_data & BIT(3) && mouse_bytes==0)
      mouse_dados[0]=mouse_data;
    else
      mouse_dados[mouse_bytes]=mouse_data;
    mouse_bytes++;
  }
  else
    return ;
  
  if(mouse_bytes==3)
  {
    struct packet pacote_dados;
    createMousePacket(mouse_dados,&pacote_dados);
    mouse_bytes=0;
    mouse_update_position(pacote_dados);
    
    if(pacote_dados.bytes[0] & MOUSE_LB)
      treat_mouse_click();
  }
}
