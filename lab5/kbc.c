#include "kbc.h"


int hook;
uint8_t bytes[2];
bool flag;
int poll_helper;
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
  uint8_t scancode;
  if(read_from_output_buffer(&scancode)!=0)
    return;
  else
    poll_helper=1;
  if(scancode==KB_TWO_BYTES_SCANCODE_FIRST_BYTE)
  {
    flag=1;
    bytes[0]=scancode;
  }
  else{
    if(flag)
    {
      bytes[1]=scancode;
    }
    else
    {
      bytes[0]=scancode;
      flag=0;
    }
  }
  return;
}

int (read_from_output_buffer)(uint8_t *read_value)
{ 
  uint8_t register_status;
  int number_of_tries=0;
  
  while(number_of_tries<10)
  {
    number_of_tries++;
    if(util_sys_inb(KBD_STAT_REG,&register_status)!=0)
    {
      printf("Error reading register status\n");
      continue;
    }
    if(register_status & BIT(0) && !(register_status & BIT(5)))
    {
      if(util_sys_inb(KBD_OUT_BUF,read_value)!=0)
      {
        printf("Error reading from Output Buffer\n");
        continue;
      }
    }
    else
    {
      continue;
    }
    if((BIT(7) & register_status))
    {
      printf("Parity  error\n");
      continue;
    }
    if((BIT(6) & register_status))
    {
      printf("Timeout error\n");
      continue;
    }
    return 0;
  }
  return -1;
}
