#ifndef MOUSE_H
#define MOUSE_H

typedef struct{
    uint16_t x;
    uint16_t y;
    xpm_image_t img;
}Mouse;
#include "i8042.h"

int (mouse_enable_data_report)(void);
void (mouse_ih)(void);
void createMousePacket(uint8_t array[3],struct packet *pp);
int (mouse_subscribe_interrupts)(uint32_t *bit_no);
int (mouse_unsubscribe_interrupts)();
int (read_return_from_mouse)();
int(set_default_minix)();
int (start_mouse)(uint32_t * mouse_mask);
int (disable_mouse)();
Mouse * get_mouse();
void eliminate_Mouse();
void mouse_update_position(struct packet data);

#endif
