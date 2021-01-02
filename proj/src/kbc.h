#ifndef __KBC_H
#define __KBC_H
  #include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "i8042.h"
#define NUMBER_OF_TRIES 2 /* number of tries possible*/
enum KBC_KEY {
  TWO_BYTE = 0,     /* enum Two bytes */
  ESC_BC = 1,    /* enum Esc breakcode */
  NO_KEY = 2,       /* enum No key pressed or invalid key*/
  R_MC = 3,       /* enum A makecode*/
  R_BC = 4,       /* enum R breakcode*/
  SPACE_BC = 5, /* enum Space breakcode*/
  ENTER_BC = 6  /* enum Enter breakcode*/
};


int (kbc_subscribe_interrupts)();
int (kbc_unsubsribe_interrupts)();
void (kbc_ih)();
int (write_command)(uint8_t command);
int (write_command_byte)(uint8_t command_byte);
int (read_from_output_buffer)(uint8_t *read_value);
#endif
