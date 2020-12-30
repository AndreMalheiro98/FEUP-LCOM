#ifndef MOUSE_H
#define MOUSE_H
#include <lcom/lcf.h>
#include "kbc.h"
#include "i8042.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct{
    uint8_t x;
    uint8_t y;
    bool lb;
    bool rb;
    bool mb;
}Mouse;



int (mouse_enable_data_report)(void);
void (mouse_ih)(void);
void createMousePacket(uint8_t array[3],struct packet *pp);
int (mouse_subscribe_interrupts)();
int (mouse_unsubscribe_interrupts)();
int (read_return_from_mouse)();
int (read_data_from_mouse)();
int(set_default_minix)();
int (start_mouse)();
int (disable_mouse)();
Mouse * create_mouse();
Mouse * get_mouse();
void eliminate_Mouse();
#endif
