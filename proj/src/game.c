#include <lcom/lcf.h>
#include "game.h"
#include "../Images/game_background.xpm"
#include <string.h>
#include "../Images/textCharacters/number_0.xpm"
#include "../Images/textCharacters/number_1.xpm"
#include "../Images/textCharacters/number_2.xpm"
#include "../Images/textCharacters/number_3.xpm"
#include "../Images/textCharacters/number_4.xpm"
#include "../Images/textCharacters/number_5.xpm"
#include "../Images/textCharacters/number_6.xpm"
#include "../Images/textCharacters/number_7.xpm"
#include "../Images/textCharacters/number_8.xpm"
#include "../Images/textCharacters/number_9.xpm"

static Game *game;
extern uint8_t bytes[2];
extern bool kbc_flag;
extern uint8_t mouse_data;
extern bool mouse_flag;
extern int timer_tick_counter;
int ticks_between_shot=0;

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
  game->main_menu=NULL;
  game->pause_menu=NULL;
  game->game_mouse=get_mouse();
  if(game->game_mouse==NULL || nu==1)
  {
    printf("Error creating game\n");
    return NULL;
  }
  game->state=STATE_DRAW_MAIN_MENU;
  game->game_mouse->x=get_hres()/2;
  game->game_mouse->y=get_vres()/2;

  game->main_menu=get_menu(0);
  if(game->main_menu==NULL)
    return NULL;
  game->main_menu->x=0;
  game->main_menu->y=0;
  
  game->pause_menu=get_menu(1);
  if(game->pause_menu==NULL)
    return NULL;
  game->pause_menu->x=400;
  game->pause_menu->y=200;

  if(load_pixmap(game_background_xpm,&game->game_background)==NULL)
    return NULL;

  return game;
}

int get_game_state(){
  return game->state;
}

int game_begin(){
  game->disk=create_disk();
  if(game->disk==NULL)
    return -1;
  game->bullet_count=15;
  game->shot_fired=0;
  game->score=0;
  update_background();
  updateGameBuffer();
  draw_score();
  if(draw_screen(game->disk->img,game->disk->x,game->disk->y)==-1)
    return -1;
  return 0;
}

void eliminate_game(){
  eliminate_Mouse();
  delete_menus();
  free_buffers();
  free(game);
}

void update_mouse_coord(struct packet data){
  mouse_update_position(data);
}

int draw_screen(xpm_image_t xpm_image,int x,int y){
  if(vg_draw_pixmap(xpm_image,x,y)!=0)
    return -1;
  draw_mouse();
  return 0;
}

int game_state_machine(){
  switch (game->state)
  {
  case STATE_DRAW_MAIN_MENU:
    if(!draw_screen(game->main_menu->img,game->main_menu->x,game->main_menu->y))
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
    if(game->disk->hit)
      update_background();
    if(game->disk->out_of_bounds)
      reset_disk();
    
    update_game();
    draw_mouse();
    break;
  case STATE_END_GAME:
    
    break;
  case STATE_END_PROGRAM:
    game->state=STATE_EXIT;
    break;
  case STATE_DRAW_PAUSE:
    if(!draw_screen(game->pause_menu->img,game->pause_menu->x,game->pause_menu->y))
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
        game->state=STATE_END_PROGRAM;
      else if(game->game_mouse->y>=342 && game->game_mouse->y<=437) //CHECK IF USER CLICKED ON PLAY 
        game->state=STATE_BEGIN_GAME;
      /*else if(game->game_mouse->y>=482 && game->game_mouse->y<=577) //CHECK IF USER CLICKED ON HIGHSCORE*/
    }
    break;
  case STATE_IN_PAUSE:
    if(game->game_mouse->x>=(game->pause_menu->x+110) && game->game_mouse->x<=(game->pause_menu->x+857)){
      if(game->game_mouse->y>=(game->pause_menu->y+148) && game->game_mouse->y<=(game->pause_menu->y+227))
        game->state=STATE_DURING_GAME;
      else if(game->game_mouse->y>=(game->pause_menu->y+254) && game->game_mouse->y<=(game->pause_menu->y+333))
        game->state=STATE_DRAW_MAIN_MENU;
    } 
  case STATE_DURING_GAME:
    if(!game->shot_fired){
      game->shot_fired=1;
      game->bullet_count--;
      if((game->game_mouse->x+game->game_mouse->img.width/2)>=game->disk->x && (game->game_mouse->x+game->game_mouse->img.width/2)<=(game->disk->x+game->disk->img.width) && (game->game_mouse->y+game->game_mouse->img.height/2)>=game->disk->y && (game->game_mouse->y+game->game_mouse->img.height/2)<=(game->disk->y+game->disk->img.height) && !game->disk->hit)
      {
        game->disk->hit=1;
        game->score++;
      }
      if(game->bullet_count==0)
        game->state=STATE_DRAW_MAIN_MENU;
    }
    break;
  default:
    break;
  }
}

void draw_mouse(){
  update_mouse(game->game_mouse->img,game->game_mouse->x,game->game_mouse->y);
  refresh_screen();
}

void draw_score(){
  //Get score string
  int x=250;
  int xDiff=50;
  int intAux=game->score;
  int final_score=0;
  int number_of_digits=0;
  //Inverting score
  while(1){
    number_of_digits++;
    final_score=final_score*10;
    final_score=final_score+intAux%10;
    intAux=intAux/10;
    if(intAux==0)
      break;
    
  }
  while(number_of_digits>0){
    xpm_image_t number_img;
    parseScoreXpm(&number_img,final_score%10);
    final_score=final_score/10;
    initBackgroundBuffer(number_img,x,0);
    number_of_digits--;
    x+=xDiff;
  }
}

int parseScoreXpm(xpm_image_t *xpm,int i){
  switch (i)
  {
  case 0:
    if(load_pixmap(xpm_0,xpm)==NULL)  
      return -1;
    break;
  case 1:
    if(load_pixmap(xpm_1,xpm)==NULL)  
      return -1;
    break;
  case 2:
    if(load_pixmap(xpm_2,xpm)==NULL)  
      return -1;
    break;
  case 3:
    if(load_pixmap(xpm_3,xpm)==NULL)  
      return -1;
    break;
  case 4:
    if(load_pixmap(xpm_4,xpm)==NULL)  
      return -1;
    break;
  case 5:
    if(load_pixmap(xpm_5,xpm)==NULL)  
      return -1;
    break;
  case 6:
    if(load_pixmap(xpm_6,xpm)==NULL)  
      return -1;
    break;
  case 7:
    if(load_pixmap(xpm_7,xpm)==NULL)  
      return -1;
    break;
  case 8:
    if(load_pixmap(xpm_8,xpm)==NULL)  
      return -1;
    break;
  case 9:
    if(load_pixmap(xpm_9,xpm)==NULL)  
      return -1;
    break;
  default:
    return -1;
  }
  return 0;
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
      game->state=STATE_DURING_GAME;
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

void update_game(){
  updateGameBuffer();
  update_disk_coord();
  
  if(!out_of_bounds(game->disk->x,game->disk->y,game->disk->x+game->disk->img.width,game->disk->y+game->disk->img.height)){
    if(game->disk->hit)
    {
      if(draw_screen(game->disk->splatter_img,game->disk->x,game->disk->y)==-1)
        return ;
    }
    else
      if(draw_screen(game->disk->img,game->disk->x,game->disk->y)==-1)
        return ;
  }
  else
    game->disk->out_of_bounds=1;
}

void handle_timer_interrupts(){
  timer_int_handler();
  game_state_machine();
  if(game->shot_fired && ticks_between_shot<40)
    ticks_between_shot++;
  else if(ticks_between_shot==40)
  {
    game->shot_fired=0;
    ticks_between_shot=0;
  }

}

void update_background(){
  initBackgroundBuffer(game->game_background,0,0);
  draw_score();
}
