#ifndef _MOUSE_H
#define _MOUSE_H
#include "./kbc.h"
#include <stdint.h>
#include "i8042.h"


int (mouse_enable_data_report)(void);
void (mouse_ih)(void);
void createMousePacket(uint8_t array[3],struct packet *pp);
int (mouse_subscribe_interrupts)();
int (mouse_unsubscribe_interrupts)();
int (read_return_from_mouse)();
#endif
