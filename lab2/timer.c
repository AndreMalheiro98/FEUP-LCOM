#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
  if(timer<0 || timer>2)
    exit(-1);
  uint8_t port;
  if(timer==0)
    port=TIMER_0;
  else if(timer==1)
    port=TIMER_1;
  else
    port=TIMER_2;

  u32_t READ_BACK=TIMER_RB_CMD | TIMER_RB_COUNT_ |TIMER_RB_SEL(timer);
  if(sys_outb(port,READ_BACK)!=OK)
    return 1;
  if(util_sys_inb(port,st)!=0)
    return 1;
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
