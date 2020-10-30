#include "kbc.h"
int hook;
uint8_t scancode;
int (kbc_subscribe_interrupts)(){ 
  hook=KBD_IRQ;
  if(sys_irqsetpolicy(KBD_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&hook)!=OK)
    return -1;
  return 0;
}

int (kbc_unsubsribe_interrupts)(){
  if(sys_irqrmpolicy(&hook)!=OK)
    return -1;
  return 0;
}

void (kbc_ih)()
{
  uint8_t register_status;
  if(util_sys_inb(KBD_STAT_REG,&register_status)!=0)
  {
    printf("Error reading register status\n");
    return;
  }
  if(register_status & BIT(0))
  {
    if(util_sys_inb(KBD_OUT_BUF,&scancode)!=0)
    {
      printf("Error reading Keyboard Output Buffer\n");
      return;
    }
  }
  else
    printf("No data to read from output buffer\n");
  if((BIT(7) & register_status))
  {
    printf("Parity  error\n");
    return;
  }
  if((BIT(6) & register_status))
  {
    printf("Timeout error\n");
    return;
  }
  uint8_t bytes[2];
  bytes[0]=scancode;
  uint8_t number_of_bytes=1;
  bool make=1;
  if(scancode==KB_TWO_BYTES_SCANCODE_FIRST_BYTE)
  {
    
    if(util_sys_inb(KBD_OUT_BUF,&scancode)!=0)
    {
      printf("Error reading Keyboard Output Buffer\n");
      return;
    }
    bytes[1]=scancode;
    number_of_bytes++;
  }
  if(scancode & BREAKCODE)
      make=0;
  kbd_print_scancode(make,number_of_bytes,bytes);
  return;
}
