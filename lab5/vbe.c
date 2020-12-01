#include <lcom/lcf.h>
#include "vbe.h"
#include <float.h>
static uint16_t hres,vres;
static char *video_mem;
static uint8_t bytesPerPixel;
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
  bytesPerPixel=ceil(info.BitsPerPixel/8.0);
  unsigned int vram_size=bytesPerPixel*info.XResolution*info.YResolution;
  hres=info.XResolution;
  
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
  if((x+width)>hres || (x+width)<0 || (y+height)>vres || (y+height)<0)
  {
    printf("Error: rectangle drawing would be out of bounds\n");
    return -1;
  }
  for(int i=0;i<height;i++)
  {
   if(vg_draw_line(x,y+i,width,color)!=0)
    return -1;
  }
  return 0;
}

void (vg_draw_pixel)(uint16_t x,uint16_t y,uint32_t color){
  char *pixel_address=(char *)video_mem+(int)((y*hres+x)*(bytesPerPixel));
  for(int j=0;j<bytesPerPixel;j++)
  {
    *pixel_address=color;
    pixel_address++;
    color=color>>8;
  } 
}

int (vg_draw_line)(uint16_t x,uint16_t y,uint16_t len,uint32_t color){
  for(int i=0;i<len;i++)
  {
    vg_draw_pixel(x+i,y,color);
  }
  return 0;
}

