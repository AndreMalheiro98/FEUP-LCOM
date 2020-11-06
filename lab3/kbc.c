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

int (write_to_input_buf)(uint8_t command,uint8_t command_byte)
{
  uint8_t register_status;
  int number_of_tries=0;
  while(number_of_tries<10)
  {
    if(number_of_tries!=0)
      tickdelay(micros_to_ticks(DELAY_US));
    number_of_tries++;
    if(util_sys_inb(KBD_STAT_REG,&register_status)!=0)
    {
      printf("Error reading register status\n");
      continue;
    }
    if(register_status & KBD_IN_BUF)
    {
      printf("Status register Input Buffer is full. Can't write at the moment\n");
      continue;
    }
    uint32_t aux;
    aux=command;
    if(sys_outb(KBD_STAT_REG,aux)!=OK)
    {
      printf("Error writing to status register\n");
      continue;
    }
    if(command==COMMAND_WRITE_BYTE_COMMAND)
    {
      uint32_t convers=command_byte;
      if(sys_outb(KBD_ARGUMENT,convers)!=OK)
      {
        printf("Error writing argument to port 0x60\n");
        continue;
      }
      printf("New command byte - 0x%.2x\n",command_byte);
    }
    printf("Returning from write\n");
    return 0;
  }
  return -1;
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
