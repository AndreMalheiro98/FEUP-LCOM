#ifndef _VBE_H
#define _VBE_H
#include <stdint.h>
#include <machine/int86.h>
#include "macros.h"
#include <math.h>
//FUNCTIONS
void * (vg_initi)(uint16_t mode,uint8_t *color);
int (vbe_return_mode_info)(uint16_t mode,vbe_mode_info_t *vmi_p);
int (vbe_verify_mode)(uint16_t mode);
int (vg_draw_rectangle)(uint16_t x,uint16_t y,uint16_t width, uint16_t height,uint32_t color);
int (vg_draw_line)(uint16_t x,uint16_t y,uint16_t len,uint32_t color);
void (vg_draw_pixel)(uint16_t x,uint16_t y,uint32_t color);
int (vg_draw_pixmap)(xpm_map_t xpm,int x,int y);
void (vg_display_pixmap)(uint8_t *address,xpm_image_t img,int x,int y);
int (pixmap_animation)(xpm_map_t xpm,int x,int y);
int get_vres();
int get_hres();
#endif
