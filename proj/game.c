#include <lcom/lcf.h>
#include "game.h"
int initMenu(uint16_t mode){
  void  *video_addr;
  vbe_mode_info_t info;
  if((video_addr=init_graphics_mode(mode,&info))==NULL)
  {
    printf("Error setting graphics mode\n");
    return -1;
  }
  uint32_t base_color=0x07E0;
  if(vg_draw_rectangle(0,0,info.XResolution,info.YResolution,base_color)!=0)
    return -1;
  uint16_t y=0,x;
  uint16_t height=info.YResolution/5;
  uint16_t width=info.XResolution/2;
  uint32_t rec_color=0x07FF;
  for(int i=0;i<3;i++){
    y+=height;
    x=info.XResolution/4; 
    if(vg_draw_rectangle(x,y,width,height,rec_color)!=0)
      return -1;
  }
  return 0;
}
