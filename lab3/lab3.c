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
int(kbd_test_scan)() {

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
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
