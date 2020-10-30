#include <lcom/lcf.h>

#include <stdint.h>
extern uint32_t number_sysinb_calls;
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL){
    return 1;
  }
  else{
    *lsb = val;
    return 0;
  }
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  
  //printf("%s is not yet implemented!\n", _func_);
  if (msb == NULL){
    return 1;
  }
  else{
    *msb = val>>8;
    return 0;
  }
}

int (util_sys_inb)(int port, uint8_t *value) {
  
  //printf("%s is not yet implemented!\n", __func__);
  u32_t number;
 
  if(sys_inb(port,&number)==EINVAL)
    return 1;
  *value=(uint8_t) number;
  number_sysinb_calls++;
  return 0;
}
