#include <lcom/lcf.h>
#include "game.h"
int initGraphics(){
  vbe_mode_info_t mode_info;
  void *video_mem=init_graphics_mode(VBE_MODE_4,&mode_info);
  if(video_mem==NULL)
    return -1;
    printf("fuck yeah");
  if(load_main_menu()!=0)
  {
    vg_exit();
    perror("Error loading main menu");
    return -1;
  }
  return 0;
}

int subscribe_periphericals(){
  if(start_mouse()!=0)
    return -1;
  return 0;
}

int unsubscripe_periphericals(){
  if(disable_mouse()!=0)
    return -1;
  return 0;
}
