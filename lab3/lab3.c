#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "kbc.h"
int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

uint32_t number_sysinb_calls=0;
extern uint8_t scancode;
extern int timer_tick_counter;

int(kbd_test_scan)() {
  number_sysinb_calls=0;
  if(kbc_subscribe_interrupts()!=0)
  {
    printf("Error subscribing keyboard interrupts\n");
    exit(1);
  }
    
  //u32_t number;
  int r,ipc_status;
  message msg;
  uint32_t mask=BIT(KBD_IRQ);
  int x=1;
  
  while(x)
  {
    if( (r=driver_receive(ANY,&msg,&ipc_status)) !=0 ) 
    {
      printf("driver_receive failed with %d",r);
      continue;
    }
    if(is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source))
      {
        case HARDWARE:
          if(msg.m_notify.interrupts & mask)
          {
            kbc_ih();
            if(scancode==ESC_KEY)
              x=0;
          }
          break;
        default:
          break;
      }
    }
  
  }
  if(kbc_unsubsribe_interrupts()!=0)
  {
    printf("Error unsubscribing keyboard interrupts\n");
    exit(2);
  }
  if(kbd_print_no_sysinb(number_sysinb_calls)!=0)
  {
    printf("Error printing number of sys_inb calls\n");
    exit(3);
  }
  return 0;
}

int(kbd_test_poll)() {
  
  //Writing to input buffer - goal is reading the kbd command byte
  number_sysinb_calls=0;
  uint8_t aux=0x00;
  if(write_to_input_buf(COMMAND_READ_BYTE_COMMAND,aux)!=0)
    return 1;
  //Reading command byte to restore later at the end of function
  
  uint8_t kbc_command_byte;
  if(read_from_output_buffer(&kbc_command_byte)!=0)
    return 1;
  
  //Polling
  while(scancode!=ESC_KEY)
  {
    kbc_ih();
  }


  //End of function. Must enable keyboard interrupts by changing kbd command byte -> set first bit to 1

  uint8_t mask=0x01;
  kbc_command_byte = kbc_command_byte | mask;
  if(write_to_input_buf(COMMAND_WRITE_BYTE_COMMAND,kbc_command_byte)!=0)
  {  
    printf("Error enabling keyboard interrupts\n");
    return 1;
  }

  //Printing number of sys_inb calls

  if(kbd_print_no_sysinb(number_sysinb_calls)!=0)
  {
    printf("Error printing number of sys_inb calls\n");
    exit(3);
  }
  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  //Checking if n is greater than zero
  if(n<=0)
  {
    printf("n must be greater than zero\n");
    return -1;
  }

  number_sysinb_calls=0;

  //Subscribing keyboard interrutps
  if(kbc_subscribe_interrupts()!=0)
  {
    printf("Error subscribing keyboard interrupts\n");
    exit(1);
  }
  
  //Subscribing timer0 interrupts
  uint8_t bit_number;
  if(timer_subscribe_int(&bit_number)==1)
  {
    printf("Error subscribing timer0 interrupts\n");
    return -1;
  }
  
  int kbd_r,kbd_ipc_status,timer_r,timer_ipc_status;
  message kbd_msg,timer_msg;
  uint32_t kbd_mask=BIT(KBD_IRQ);
  uint32_t timer_mask=BIT(bit_number);
  int x=1;
  int max_idle_time=60*n;
  while(x && (timer_tick_counter<max_idle_time))
  {
    if((timer_r=driver_receive(ANY,&timer_msg,&timer_ipc_status)) !=0 )
    {
      printf("timer driver_receive failed with %d\n",timer_r);
      continue;
    }
    if(is_ipc_notify(timer_ipc_status))
    {
      switch (_ENDPOINT_P(timer_msg.m_source))
      {
      case HARDWARE:
        if(timer_msg.m_notify.interrupts & timer_mask)
          timer_int_handler();
        break;
      default:
        break;
      }
    }
    if( (kbd_r=driver_receive(ANY,&kbd_msg,&kbd_ipc_status)) !=0 ) 
    {
      printf("driver_receive failed with %d",kbd_r);
      continue;
    }
    if(is_ipc_notify(kbd_ipc_status)){
      switch(_ENDPOINT_P(kbd_msg.m_source))
      {
        case HARDWARE:
          if(kbd_msg.m_notify.interrupts & kbd_mask)
          {
            kbc_ih();
            if(scancode==ESC_KEY)
              x=0;
          }
          break;
        default:
          break;
      }
    }
  
  }
  if(kbc_unsubsribe_interrupts()!=0)
  {
    printf("Error unsubscribing keyboard interrupts\n");
    exit(2);
  }
  if(kbd_print_no_sysinb(number_sysinb_calls)!=0)
  {
    printf("Error printing number of sys_inb calls\n");
    exit(3);
  }
  return 0;
}
