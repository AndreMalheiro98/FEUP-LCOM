#pragma once
#ifndef _VBE_H
#define _VBE_H
#include <stdint.h>
#include <machine/int86.h>
#include "vbeMacros.h"
#include <math.h>
/**
 * @brief  
**/
typedef struct{
  char VbeSignature[4];
  uint16_t VbeVersion;
  phys_bytes OemStringPtr;
  uint32_t Capabilities;
  
}VbeInfoBlock;
//FUNCTIONS
/**
 * @brief  
**/
void * (init_graphics_mode)(uint16_t mode,vbe_mode_info_t *info);
/**
 * @brief  
**/
int (vbe_return_mode_info)(uint16_t mode,vbe_mode_info_t *vmi_p);
/**
 * @brief  
**/
int (vbe_verify_mode)(uint16_t mode);
/**
 * @brief  
**/
void (vg_draw_pixel)(uint16_t x,uint16_t y,uint32_t color,char *end_buffer);
/**
 * @brief  
**/
int (vg_draw_pixmap)(xpm_map_t xpm,int x,int y);
/**
 * @brief  
**/
void (vg_display_pixmap)(xpm_image_t img,int x,int y,char * end_buffer);
/**
 * @brief  
**/
int (pixmap_animation)(xpm_map_t xpm,int x,int y);
/**
 * @brief  
**/
int vbe_get_contr_info(vg_vbe_contr_info_t *vmi_p);
/**
 * @brief  
**/
int get_vres();
/**
 * @brief  
**/
int get_hres();
/**
 * @brief  
**/
void refresh_screen();
/**
 * @brief  
**/
void update_mouse(xpm_image_t mouse_img,int x,int y);
/**
 * @brief  
**/
uint8_t *load_pixmap(xpm_map_t xpm,xpm_image_t *img);
/**
 * @brief  
**/
void free_buffers();
#endif
