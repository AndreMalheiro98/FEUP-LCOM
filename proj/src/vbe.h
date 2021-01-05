 /**
 * @file mouse.h
 * Mouse
 */
#pragma once
#ifndef _VBE_H
#define _VBE_H
#include <stdint.h>
#include <machine/int86.h>
#include "vbeMacros.h"
#include <math.h>

/** @defgroup vbe VBE
 * @{
 *
 * Functions for using the Video BIOS Extension
 */

/**
 * @brief Information of Video BIOS Extension
**/
typedef struct{
  char VbeSignature[4];
  uint16_t VbeVersion;
  phys_bytes OemStringPtr;
  uint32_t Capabilities;
  
}VbeInfoBlock;

//FUNCTIONS
/**
 * @brief Initializes Graphics
**/
void * (init_graphics_mode)(uint16_t mode,vbe_mode_info_t *info);
/**
 * @brief  Return mode info of Video BIOS Extension 
**/
int (vbe_return_mode_info)(uint16_t mode,vbe_mode_info_t *vmi_p);
/**
 * @brief  Verifies mode of Video BIOS Extension
**/
int (vbe_verify_mode)(uint16_t mode);
/**
 * @brief  Draw a pixel 
**/
void (vg_draw_pixel)(uint16_t x,uint16_t y,uint32_t color,char *end_buffer);
/**
 * @brief Draw a pixmap
**/
int (vg_draw_pixmap)(xpm_image_t img,int x,int y);
/**
 * @brief Display a given pixmap 
**/
void (vg_display_pixmap)(xpm_image_t img,int x,int y,char * end_buffer);
/**
 * @brief Add animation to pixmap
**/
int (pixmap_animation)(xpm_map_t xpm,int x,int y);
/**
 * @brief Get control info for Video BIOS Extension
**/
int vbe_get_contr_info(vg_vbe_contr_info_t *vmi_p);
/**
 * @brief Get screen vertical resolution
**/
int get_vres();
/**
 * @brief  Get screen horizontal resolution
**/
int get_hres();
/**
 * @brief Refresh screen
**/
void refresh_screen();
/**
 * @brief Update mouse coordinates
**/
void update_mouse(xpm_image_t mouse_img,int x,int y);
/**
 * @brief Load saved pixmap
**/
uint8_t *load_pixmap(xpm_map_t xpm,xpm_image_t *img);
/**
 * @brief Free buffers
**/
void free_buffers();
/**
 * @brief Verifies if out of bounds
**/
int out_of_bounds(int16_t leftCornerX,int16_t leftCornerY,int16_t rightCornerX,int16_t rightCornerY);
/**
 * @brief Initializes background buffer
**/
void initBackgroundBuffer(xpm_image_t background_img,int x,int y);
/**
 * @brief Updates game buffer
**/
void updateGameBuffer();
#endif
