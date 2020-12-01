#ifndef _VBE_H
#define _VBE_H
#include <stdint.h>
#include <machine/int86.h>
#include "macros.h"
#include <math.h>
//FUNCTIONS
void * (vg_init)(uint16_t mode);
int (vbe_return_mode_info)(uint16_t mode,vbe_mode_info_t *vmi_p);
int (vbe_verify_mode)(uint16_t mode);
int (vg_draw_rectangle)(uint16_t x,uint16_t y,uint16_t width, uint16_t height,uint32_t color);
int (vg_draw_line)(uint16_t x,uint16_t y,uint16_t len,uint32_t color);
void (vg_draw_pixel)(uint16_t x,uint16_t y,uint32_t color);
#endif
