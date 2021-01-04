#ifndef MOUSE_H
#define MOUSE_H
#include "i8042.h"
/**
 * @brief  
**/
typedef struct{
    int16_t x;
    int16_t y;
    xpm_image_t img;
}Mouse;
/**
 * @brief  
**/
int (mouse_enable_data_report)(void);
/**
 * @brief  
**/
void (mouse_ih)(void);
/**
 * @brief  
**/
void createMousePacket(uint8_t array[3],struct packet *pp);
/**
 * @brief  
**/
int (mouse_subscribe_interrupts)(uint32_t *bit_no);
/**
 * @brief  
**/
int (mouse_unsubscribe_interrupts)();
/**
 * @brief  
**/
int (read_return_from_mouse)();
/**
 * @brief  
**/
int(set_default_minix)();
/**
 * @brief  
**/
int (start_mouse)(uint32_t * mouse_mask);
/**
 * @brief  
**/
int (disable_mouse)();
/**
 * @brief  
**/
Mouse * get_mouse();
/**
 * @brief  
**/
void eliminate_Mouse();
/**
 * @brief  
**/
void mouse_update_position(struct packet data);

#endif
