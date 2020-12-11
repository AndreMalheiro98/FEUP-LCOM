#include <lcom/lcf.h>
#include "vbe.h"
int initMenu(uint16_t mode){
  void  *video_addr;
  vbe_mode_info_t info;
  if((video_addr=init_graphics_mode(mode,&info))==NULL)
  {
    printf("Error setting graphics mode\n");
    return -1;
  }
  if(vg_draw_rectangle(0,0,info.XResolution,ifo.YResolution,))
  return 0;
}
