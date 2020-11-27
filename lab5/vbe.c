#include <lcom/lcf.h>
#include "vbe.h"
uint16_t hres,vres;
void *video_mem;
uint8_t bitsPerPixel;
int (vbe_verify_mode)(uint16_t mode)
{
  switch(mode)
  {
    case VBE_MODE_0:
      break;
    case VBE_MODE_1:
      break;
    case VBE_MODE_2:
      break;
    case VBE_MODE_3:
      break;
    case VBE_MODE_4:
      break;
    default:
      return -1;
  }
  return 0;
}

void * (vg_init)(uint16_t mode){
  if(vbe_verify_mode(mode)!=0)
  {
    printf("Mode is not supported\n");
    return NULL;
  }
  vbe_mode_info_t info;
  if(vbe_get_mode_info(mode,&info)!=0)
  {
    printf("Error getting vbe mode info\n");
    return NULL;
  }
  struct minix_mem_range mr;
  unsigned int vram_base=info.PhysBasePtr;
  unsigned int vram_size=info.BitsPerPixel*info.XResolution*info.YResolution;
  hres=info.XResolution;
  bitsPerPixel=info.BitsPerPixel;
  vres=info.YResolution;
  mr.mr_base=(phys_bytes) vram_base;
  mr.mr_limit=vram_base+vram_size;

  if(sys_privctl(SELF,SYS_PRIV_ADD_MEM,&mr)!=OK)
  {
    panic("sys_privctl ADD MEM failed\n");
    return NULL;
  }
  
  video_mem=vm_map_phys(SELF,(void *) mr.mr_base,vram_size);
  
  if(video_mem==MAP_FAILED)
  {
    panic("Couldn't map video memory\n");
    return NULL;
  }

  reg86_t aux;
  memset(&aux,0,sizeof(aux));
  aux.intno=VBE_INTNO;
  aux.ax=VBE_SET_MODE;
  aux.bx= BIT(14) | mode;
  aux.ah=VBE_FUNCTIONS_AH;
  aux.al=VBE_SET_MODE_FUNCTION;
  if(sys_int86(&aux) != OK){
    printf("VBE set mode failed\n");
    return NULL;
  }
  return video_mem;
}

int (vbe_return_mode_info)(uint16_t mode,vbe_mode_info_t *vmi_p){
  if(vbe_verify_mode(mode)!=0)
  {
    printf("Mode is not supported\n");
    return -1;
  }
  return 0;
}

int (vg_draw_rectangle)(uint16_t x,uint16_t y,uint16_t width, uint16_t height,uint32_t color){
  if(height>vres || width>hres || width<0 || height<0)
  {
    printf("Error: height or width values are out of bounds\n");
    return -1;
  }
  for(int i=0;i<height;i++)
  {
    
   if(vg_draw_line(x,y+i,width,color)!=0)
    return -1;
  }
  return 0;
}

int (vg_draw_line)(uint16_t x,uint16_t y,uint16_t len,uint32_t color){
  int bytes_per_pixel=bitsPerPixel/8;
  
  char *pixel_address=(char *)video_mem+((y*hres+x)*(bytes_per_pixel));
  
  for(int i=0;i<len;i++)
  {
    uint32_t color_aux=color;
    for(int j=0;j<bytes_per_pixel;j++)
    {
      *pixel_address=color_aux;
      pixel_address++;
      color_aux=color_aux>>8;
    }  
  }
  return 0;
}


