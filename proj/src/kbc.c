#include <lcom/lcf.h>
#include "kbc.h"
#include <stdint.h>
extern uint32_t number_sysinb_calls;

int (write_command)(uint8_t command){
  uint8_t register_status;
  int number_of_tries=0;
  while(number_of_tries<10)
  {
    if(number_of_tries!=0)
      tickdelay(micros_to_ticks(DELAY_US));
    number_of_tries++;
    if(checkStatusRegister(&register_status)!=0)
      continue;
    if(register_status & KBD_IBF)
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

void (empty_input_buffer)(){
  uint8_t byte;
  uint8_t register_status;
  do{
    if(util_sys_inb(KBD_STAT_REG,&register_status)!=0)
      return ;
    if(register_status & KBD_IBF)
    {
      if(util_sys_inb(KBD_IN_BUF,&byte)!=0)
        return ;  
    }
  }while(register_status & KBD_IBF);
}

int (write_command_byte)(uint8_t command_byte)
{
  int i;
  for(i=0;i<10;i++)
  {
    tickdelay(micros_to_ticks(DELAY_US));
    uint8_t register_status;
    if(checkStatusRegister(&register_status)!=0)
      continue;
    if(register_status & KBD_OBF)
    {
      empty_input_buffer();
      continue;
    }
    else{
      uint32_t convers=command_byte;
      if(sys_outb(KBD_ARGUMENT,convers)!=OK)
      {
        printf("Error writing argument to port 0x60\n");
        continue;
      }
      
      return 0;
      }
  }
  return -1;
}

int checkStatusRegister(uint8_t *reg_status){
  if(util_sys_inb(KBD_STAT_REG,reg_status)!=0){
    printf("Error reading register status\n");
    return -1;
  }
  return 0;
}

int check_data_error(uint8_t reg_status){
  if((reg_status & KBD_TO_ERR) || (reg_status & KBD_PAR_ERR))
  {
    printf("Data error\n");
    return -1;
  }
  return 0;
}

int read_keyboard_data(uint8_t *kb_data){
  uint8_t register_status;
  int number_tries=0;
  while(number_tries<10){
    number_tries++;
    if(checkStatusRegister(&register_status)==-1) 
      continue;
    if((register_status & KBD_OBF) && !(register_status & KBD_AUX))
    {
      if(util_sys_inb(KBD_OUT_BUF,kb_data)!=0)
      {
        printf("Error reading keyboard data from Output Buffer\n");
        continue;
      }
    }
    else
      continue;
    if(check_data_error(register_status)!=0)
      continue;
    return 0;
  }
  
  return -1;
}

int read_mouse_data(uint8_t *mouse_data){
  uint8_t register_status;
  int number_tries=0;
  while(number_tries<10){
    number_tries++;
    if(checkStatusRegister(&register_status)==-1) 
      continue;
    if((register_status & KBD_OBF) && (register_status & KBD_AUX))
    {
      if(util_sys_inb(KBD_OUT_BUF,mouse_data)!=0)
      {
        printf("Error reading mouse data from Output Buffer\n");
        continue;
      }
    }
    else
      continue;
    if(check_data_error(register_status)!=0)
      continue;
    return 0;
  }
  
  return -1;
}
