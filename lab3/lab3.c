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
extern uint8_t bytes[2];
extern bool flag;
extern int timer_tick_counter;
int(kbd_test_scan)() {
  number_sysinb_calls=0;
  bool make=0;
  flag =0;
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
  int i=0;
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
            i++;
            kbc_ih();
            if(flag==1)
            {
              if(bytes[1]&BREAKCODE)
                make=0;
              else
                make=1;
              if(i==2)
              {
                kbd_print_scancode(make,2,bytes);
                flag=0;
                i=0;
              }
              if(bytes[1]==ESC_KEY)
                x=0;
            }
            else
            {
              i=0;
              if(bytes[0]&BREAKCODE)
                make=0;
              else
                make=1;
              kbd_print_scancode(make,1,bytes);
              flag=0;
              if(bytes[0]==ESC_KEY)
                x=0;
            }
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
  number_sysinb_calls=0;
  flag =0;
  bool make=0;
  //Writing to input buffer - goal is reading the kbd command byte
  
  if(write_command(COMMAND_READ_BYTE_COMMAND)!=0)
    return 1;
  //Reading command byte to restore later at the end of function
  
  uint8_t kbc_command_byte;
  if(read_from_output_buffer(&kbc_command_byte)!=0)
    return 1;
  printf("Original command byte - 0x%.2x\n",kbc_command_byte);
  //Polling
  int x=1;
  int i=0;
  while(x)
  {
    i++;
    kbc_ih();
    if(flag==1)
    {
      if(bytes[0]&BREAKCODE)
        make=0;
      else
        make=1;
      if(i==2)
      {
        kbd_print_scancode(make,2,bytes);
        flag=0;
        i=0;
      }
      if(bytes[1]==ESC_KEY)
        x=0;
    }
    else
    {
      i=0;
      if(bytes[0]&BREAKCODE)
        make=0;
      else
        make=1;
      kbd_print_scancode(make,1,bytes);
      flag=0;
      if(bytes[0]==ESC_KEY)
        x=0;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }


  //End of function. Must enable keyboard interrupts by changing kbd command byte -> set first bit to 1

  uint8_t mask=0x01;
  kbc_command_byte = kbc_command_byte | mask;

  
  if(write_command(COMMAND_WRITE_BYTE_COMMAND)!=0)
  {  
    printf("Error preparing port 0x60\n");
    return 1;
  }

  if(write_command_byte(kbc_command_byte)!=0)
  {
    printf("Error setting bit 0 to 1\n");
    return 1;
  }
  tickdelay(micros_to_ticks(DELAY_US));
 /* if(write_command(COMMAND_READ_BYTE_COMMAND)!=0)
  {
    printf("Error writing to input buffer\n");
    return 1;
  }
  //Reading command byte to restore later at the end of function
  
  tickdelay(micros_to_ticks(DELAY_US));
  
  if(read_from_output_buffer(&helper)!=0)
  {
    printf("Error reading from output buffern\n");
    continue;
  }
  printf("Checking command byte - 0x%.2x\n",helper);
  }while(helper!=kbc_command_byte);*/
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
  flag =0;
  bool make=0;
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
  
  int r,ipc_status;
  message msg;
  uint32_t kbd_mask=BIT(KBD_IRQ);
  uint32_t timer_mask=BIT(bit_number);
  int x=1;
  int max_idle_time=60*n;
  int i=0;
  while(x && (timer_tick_counter<max_idle_time))
  {
    if((r=driver_receive(ANY,&msg,&ipc_status)) !=0 )
    {
      printf("timer driver_receive failed with %d\n",r);
      continue;
    }
    if(is_ipc_notify(ipc_status))
    {
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE:
        if(msg.m_notify.interrupts & timer_mask)
        {
          timer_int_handler();
        }
        if(msg.m_notify.interrupts & kbd_mask)
        {
          i++;
          timer_tick_counter=0;
          kbc_ih();
          if(flag==1)
          {
            if(bytes[0]&BREAKCODE)
              make=0;
            else
              make=1;
            if(i==2)
            {
              kbd_print_scancode(make,2,bytes);
              flag=0;
              i=0;
            }
            if(bytes[1]==ESC_KEY)
              x=0;
          }
          else
          {
            i=0;
            if(bytes[0]&BREAKCODE)
              make=0;
            else
              make=1;
            kbd_print_scancode(make,1,bytes);
            flag=0;
            if(bytes[0]==ESC_KEY)
              x=0;
          }
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
  if(timer_unsubscribe_int()!=0)
  {
    printf("Error unsubscribing timer 0 interrupts\n");
    return -1;
  }
  if(kbd_print_no_sysinb(number_sysinb_calls)!=0)
  {
    printf("Error printing number of sys_inb calls\n");
    exit(3);
  }
  return 0;
}
