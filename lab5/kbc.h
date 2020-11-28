#ifndef __KBC_H
#define __KBC_H
  #include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "i8042.h"

  int (kbc_subscribe_interrupts)();
  int (kbc_unsubsribe_interrupts)();
  void (kbc_ih)();
  int (read_from_output_buffer)(uint8_t *read_value);
#endif
