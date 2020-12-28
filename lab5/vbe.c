#include <lcom/lcf.h>
#include "vbe.h"
#include <float.h>
static int hres,vres;
static char *video_mem;
static uint8_t bytesPerPixel;

int get_vres()
{
  return vres;
}

int get_hres(){
  return hres;
}
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

void * (vg_initi)(uint16_t mode,uint8_t *color){
  if(vbe_verify_mode(mode)!=0)
  {
    printf("Mode is not supported\n");
    return NULL;
  }
  vbe_mode_info_t info;
  if(vbe_return_mode_info(mode,&info)!=0)
  {
    printf("Error getting vbe mode info\n");
    return NULL;
  }
  struct minix_mem_range mr;
  unsigned int vram_base=info.PhysBasePtr;
  bytesPerPixel=ceil(info.BitsPerPixel/8.0);
  unsigned int vram_size=bytesPerPixel*info.XResolution*info.YResolution;
  hres=(int)info.XResolution;
  *color=info.RedMaskSize;
  *(color+1)=info.RedFieldPosition;
  *(color+2)=info.GreenMaskSize;
  *(color+3)=info.GreenFieldPosition;
  *(color+4)=info.BlueMaskSize;
  *(color+5)=info.BlueFieldPosition;
  vres=(int)info.YResolution;
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
  mmap_t h;
  if(lm_alloc(sizeof(vbe_mode_info_t),&h)==NULL)
  {
    printf("Error allocating memory for map\n");
    return -1;
  }
  reg86_t aux;
  memset(&aux,0,sizeof(aux));
  aux.intno=VBE_INTNO;
  aux.cx=mode;
  aux.ah=VBE_FUNCTIONS_AH;
  aux.al=VBE_GET_MODE_INFORMATION;
  aux.ax=VBE_GET_MODE;
  aux.es=PB2BASE(h.phys);
  aux.di=PB2OFF(h.phys);
  if(sys_int86(&aux)!=OK)
  {
    printf("Error in int86 function - get mode info\n");
    return -1;
  }
  *vmi_p=*(vbe_mode_info_t *)h.virt;
  lm_free(&h);
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

int (vg_draw_pixmap)(xpm_map_t xpm,int x,int y){
  enum xpm_image_type type= XPM_INDEXED;
  xpm_image_t img;
  uint8_t *sprite= xpm_load(xpm,type,&img);
  if(sprite==NULL)
  {
    printf("Error loading xpm\n");
    return -1;
  }
  if((img.width+x) >hres || (img.width+x)<0 || (img.height+y)>vres || (img.height+y)<0)
  {
    printf("Error:Pixmap out of bounds\n");
    return -1;
  }

  vg_display_pixmap(sprite,img,x,y);  
  return 0;
}

void (vg_display_pixmap)(uint8_t *address,xpm_image_t img,int x,int y){
  uint16_t actX,actY;
  actX=x;
  actY=y;
  for(size_t i=0;i<img.size;i++){
    uint32_t color=*(address);
    vg_draw_pixel(actX,actY,color);
    actX++;
    address++;
    if((actX-x)==img.width)
    {
      actY++;
      actX=x;
    }
  }
}

int vbe_get_contr_info(vg_vbe_contr_info_t *vmi_p){
  mmap_t h;
  if(lm_alloc(sizeof(vbe_mode_info_t),&h)==NULL)
  {
    printf("Error allocating memory for map\n");
    return -1;
  }
  vmi_p->VBESignature[0]='V';
  vmi_p->VBESignature[1]='B';
  vmi_p->VBESignature[2]='E';
  vmi_p->VBESignature[3]='2';
  reg86_t aux;
  memset(&aux,0,sizeof(aux));
  aux.intno=VBE_INTNO;
  aux.ah=VBE_FUNCTIONS_AH;
  aux.ax=0x4F00;
  aux.al=VBE_GET_CTRL_INFORMATION;
  aux.es=PB2BASE(h.phys);
  aux.di=PB2OFF(h.phys);
  if(sys_int86(&aux)!=OK)
  {
    printf("Error in int86 function - get mode info\n");
    return -1;
  }
  //VbeControllerInfo * vbe_block_info=(VbeControllerInfo * ) h.virt;
  void *p=h.virt;
  //memcpy(vmi_p->VbeSignature, vbe_block_info.VbeSignature, 4);
  vmi_p-> VBEVersion[0] = (BCD)*(p+4);
  /*vmi_p->VbeVersion[1] = vbe_block_info.VbeVersion[1];
  vmi_p->TotalMemory = vbe_block_info.TotalMemory * 64; //number of 64kb blocks
  vmi_p->VideoModeList = (uint16_t*) convert_far_ptr(vbe_block_info.videoModePTR, membase_ptr);
  vmi_p->OEMString = (char*) convert_far_ptr(vbe_block_info.OEMStringPTR, membase_ptr);
  vmi_p->OEMVendorNamePtr = (char *) convert_far_ptr(vbe_block_info.OEMVendorNamePTR, membase_ptr);
  vmi_p->OEMProductNamePtr = (char*)convert_far_ptr(vbe_block_info.OEMProductNamePTR, membase_ptr);
  vmi_p->OEMProductRevPtr = (char *) convert_far_ptr(vbe_block_info.OEMProductRevPTR, membase_ptr);*/
  lm_free(&h);
  return 0;
}
