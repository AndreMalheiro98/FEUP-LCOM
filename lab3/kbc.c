#include "kbc.h"
extern uint32_t number_sysinb_calls;
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
  if(read_from_output_buffer(&scancode)!=0)
    return;
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

int (write_to_input_buf)(uint8_t command_byte)
{
  uint8_t register_status;
  if(util_sys_inb(KBD_STAT_REG,&register_status)!=0)
  {
    printf("Error reading register status\n");
    return -1;
  }
  if(register_status & KBD_IN_BUF)
  {
    printf("Status register Input Buffer is full. Can't write at the moment\n");
    return -1;
  }
  uint32_t aux;
  aux=command_byte;
  if(sys_outb(KBD_STAT_REG,aux)!=OK)
  {
    printf("Error writing to status register\n");
    return -1;
  }
  return 0;
}

int (read_from_output_buffer)(uint8_t *read_value)
{ 
  uint8_t register_status;
  if(util_sys_inb(KBD_STAT_REG,&register_status)!=0)
  {
    printf("Error reading register status\n");
    return -1;
  }
  if(register_status & BIT(0))
  {
    if(util_sys_inb(KBD_OUT_BUF,read_value)!=0)
    {
      printf("Error reading from Output Buffer\n");
      return -1;
    }
  }
  else
    printf("No data to read from output buffer\n");
  if((BIT(7) & register_status))
  {
    printf("Parity  error\n");
    return -1;
  }
  if((BIT(6) & register_status))
  {
    printf("Timeout error\n");
    return -1;
  }
  return 0;
}
