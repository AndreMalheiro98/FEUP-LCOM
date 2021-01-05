 /**
 * @file mouse.h
 * Mouse
 */
#ifndef MOUSE_H
#define MOUSE_H
#include "i8042.h"
/** @defgroup mouse Mouse
 * @{
 *
 * Functions for using the mouse
 */
/**
 * @brief Mouse coordinates and its image 
**/
typedef struct{
    int16_t x;
    int16_t y;
    xpm_image_t img;
}Mouse;
/**
 * @brief  Enables mouse data report
**/
int (mouse_enable_data_report)(void);
/**
 * @brief Mouse interrupt Handler
**/
void (mouse_ih)(void);
/**
 * @brief Retrieves a scancode when the interrupt is generated
**/
void createMousePacket(uint8_t array[3],struct packet *pp);
/**
 * @brief  Subscribes to the interrupts from the mouse
**/
int (mouse_subscribe_interrupts)(uint32_t *bit_no);
/**
 * @brief  Unsubscribes from the interrupts of the mouse
**/
int (mouse_unsubscribe_interrupts)();
/**
 * @brief  Reads upon return from the mouse
**/
int (read_return_from_mouse)();
/**
 * @brief Sets minix default KBC command byte
**/
int(set_default_minix)();
/**
 * @brief Initialize mouse
**/
int (start_mouse)(uint32_t * mouse_mask);
/**
 * @brief Disables mouse
**/
int (disable_mouse)();
/**
 * @brief Returns mouse class instance if it exists, else, creates one and returns it
**/
Mouse * get_mouse();
/**
 * @brief  
**/
void eliminate_Mouse();
/**
 * @brief  Updates mouse position on the screen
**/
void mouse_update_position(struct packet data);

#endif
