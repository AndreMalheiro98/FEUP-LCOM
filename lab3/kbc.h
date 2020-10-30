#ifndef __KBC_H
#define __KBC_H
  #include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "macros.h"

  void (kbc_ieh)();
  void (cancel)();
  int (aux)(uint8_t *scancode);
#endif
