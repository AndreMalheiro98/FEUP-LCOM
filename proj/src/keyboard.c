#include <lcom/lcf.h>
#include "keyboard.h"
#include "i8042.h"
#include "kbc.h"
int kbc_hook;
uint8_t bytes[2];
bool kbc_flag , is_two_bytes = 0;

int (kbc_subscribe_interrupts)(uint32_t * mask){ 
  kbc_hook=KBD_IRQ;
  *mask=BIT(kbc_hook);
  if(sys_irqsetpolicy(KBD_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&kbc_hook)!=OK)
    return -1;
  return 0;
}

int (kbc_unsubscribe_interrupts)(){
  if(sys_irqrmpolicy(&kbc_hook)!=OK)
    return -1;
  return 0;
}

void (kbc_ih)()
{
  uint8_t scancode;
  if(read_keyboard_data(&scancode)!=0)
    return;
  if(scancode==KB_TWO_BYTES_SCANCODE_FIRST_BYTE)
  {
    kbc_flag=1;
    bytes[0]=scancode;
  }
  else{
    if(kbc_flag)
    {
      bytes[1]=scancode;
    }
    else
    {
      bytes[0]=scancode;
      kbc_flag=0;
    }
  }
  return;
}

enum KBC_KEY (Verify_Key)(uint8_t scancode) {
  switch (scancode) {
    case ESC_KEY_BREAKCODE:
      return ESC_BC;
      break;
    case R_KEY_MAKECODE:
      return R_MC;
      break;
    case R_KEY_BREAKCODE:
      return R_BC;
      break;
    case SPACE_KEY_BREAKCODE:
      return SPACE_BC;
      break;
    case ENTER_KEY_BREAKCODE:
      return ENTER_BC;
      break;
    default:
      return NO_KEY;
      break;
  }
} 

enum KBC_KEY (get_key_pressed)(bool make, int number_of_bytes, uint8_t * key_codes){
  if(make) return NO_KEY;
  else if(number_of_bytes == 2) return Verify_Key(key_codes[1]);
  else if(number_of_bytes == 1) return Verify_Key(key_codes[0]);
  else  return NO_KEY;
}
