#include <lcom/lcf.h>
#include "timer.h"

#include <stdint.h>

#include "i8254.h"
uint32_t global_int_counter=0;
int var=TIMER0_IRQ;
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
  uint8_t defaul;
  if(timer_get_conf(timer,&defaul)!=0)
    return 1;
  uint8_t contr=defaul & 0xF;
  freq= freq | ((u32_t) contr);
  if(timer==0)
    timer=TIMER_0;
  else if(timer==1)
    timer=TIMER_1;
  else
    timer=TIMER_2;
  if(sys_outb(timer,freq)!=OK)
    return 1;
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  var=TIMER0_IRQ;
  *bit_no=(uint8_t )var;
  if(sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&var)!=OK)
    return 1;
  return 0;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  if(sys_irqrmpolicy(&var)!=OK)
    return 1;
  return 0;
}

void (timer_int_handler)() {
  /* To be implemented by the students */ 
  global_int_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  
  //printf("%s is not yet implemented!\n", __func__);
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
  
  union timer_status_field_val value;
  switch (field)
  {
  case tsf_all:
    value.byte=st;
    break;

  case tsf_initial:
    value.in_mode=(TIMER_LSB_MSB & st)>>4;

  case tsf_mode:
    value.count_mode= (TIMER_SQR_WAVE & st) >>1;
    break;
  case tsf_base:
    value.bcd=st & TIMER_BCD;
    break;

  default:
    perror("Error on timer_status_field");
    return 1;
  }
  //printf("%s is not yet implemented!\n", __func__);
  if(timer_print_config(timer,field,value)!=0)
  {
    perror("Erron on printing timer config");
    return 1;
  }
  return 0;
}
