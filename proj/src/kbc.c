#include <lcom/lcf.h>
#include "kbc.h"
extern uint32_t number_sysinb_calls;

int hook;
uint8_t bytes[2];
bool flag , is_two_bytes = 0;
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
uint8_t discard_mc(void) {

  uint8_t useless;
  uint8_t i = NUMBER_OF_TRIES;
  while (i > 0) {
    util_sys_inb(OUTPUT_B, &useless);
    i--;
    if (useless & BIT(7)) 
      return useless;
  }

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
enum KBC_KEY (check_key)(uint8_t scancode, bool is_two_bytes) {

  if (is_two_bytes) {

    switch(scancode) {
      default:
      return NO_KEY;
      break;
    }
  } else {

    switch (scancode) {
      case ESC_BREAKCODE:
        return ESC_BC;
        break;
      case W_MAKECODE:
        return W_MC;
        break;
      case S_MAKECODE:
        return S_MC;
        break;
      case A_MAKECODE:
        return A_MC;
        break;
      case D_MAKECODE:
        return D_MC;
        break;
      case W_BREAKCODE:
        return W_BC;
        break;
      case S_BREAKCODE:
        return S_BC;
        break;
      case A_BREAKCODE:
        return A_BC;
        break;
      case D_BREAKCODE:
        return D_BC;
        break;
      case SPACE_BREAKCODE:
        return SPACE_BC;
        break;
      case ENTER_BREAKCODE:
        return ENTER_BC;
        break;
      default:
        return NO_KEY;
        break;
    }
  } 
}
enum KBC_KEY (get_key_pressed)(void){
  uint8_t stat;
  if(read_from_output_buffer(&stat) != 0){
    return NO_KEY;
  }
  uint8_t discarded = discard_mc();
  if (discarded != 0) 
    scancode = discarded;

}

int (write_command)(uint8_t command){
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
    
    printf("Returning from write\n");
    return 0;
  }
  return -1;
}

int (write_command_byte)(uint8_t command_byte)
{
  
  tickdelay(micros_to_ticks(DELAY_US));
  uint8_t register_status;
  if(util_sys_inb(KBD_STAT_REG,&register_status)!=OK)
  {
    printf("Error reading register status\n");
  }
  if(register_status & KBD_OBF)
  {
    uint8_t aux;
    if(read_from_output_buffer(&aux)!=0)
      printf("Error - 0x%.2x\n",register_status);
  }
  else{
    uint32_t convers=command_byte;
    if(sys_outb(KBD_ARGUMENT,convers)!=OK)
    {
      printf("Error writing argument to port 0x60\n");
      return -1;
    }
    printf("New command byte - 0x%.2x\n",command_byte);
  }
    
  
  return 0;
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
