#pragma once
#ifndef _VBE_H
#define _VBE_H
#include <stdint.h>
#include <machine/int86.h>
#include "vbeMacros.h"
#include <math.h>

typedef struct{
  char VbeSignature[4];
  uint16_t VbeVersion;
  phys_bytes OemStringPtr;
  uint32_t Capabilities;
  
}VbeInfoBlock;
//FUNCTIONS
void * (init_graphics_mode)(uint16_t mode,vbe_mode_info_t *info);
int (vbe_return_mode_info)(uint16_t mode,vbe_mode_info_t *vmi_p);
int (vbe_verify_mode)(uint16_t mode);
void (vg_draw_pixel)(uint16_t x,uint16_t y,uint32_t color,char *end_buffer);
int (vg_draw_pixmap)(xpm_map_t xpm,int x,int y);
void (vg_display_pixmap)(xpm_image_t img,int x,int y,char * end_buffer);
int (pixmap_animation)(xpm_map_t xpm,int x,int y);
int vbe_get_contr_info(vg_vbe_contr_info_t *vmi_p);
int get_vres();
int get_hres();
void refresh_screen();
void update_mouse(xpm_image_t mouse_img,int x,int y);
uint8_t *load_pixmap(xpm_map_t xpm,xpm_image_t *img);
void free_buffers();
#endif
