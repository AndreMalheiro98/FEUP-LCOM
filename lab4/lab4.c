// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}
extern uint8_t data;
extern bool flag;
#include "mouse.h"
#include "kbc.h"
int (mouse_test_packet)(uint32_t cnt) {
  // Enabling stream mode
  if(mouse_enable_data_report()!=0)
  {
    printf("Error enabling data report\n");
    return -1;
  }
  else
    printf("Data report enabled successfully\n");
  //Subscribing mouse interrupts
  if(mouse_subscribe_interrupts()==-1)
  {
    printf("Error subscribing mouse interrupts\n");
    return -1;
  }
  
  int ipc_status,r;
  message msg;
  uint32_t mask=BIT(MOUSE_IRQ);
  
  int bytes=0;
  uint8_t dados[3];
  for(int i=0;i<3;i++)
    dados[i]=0;
  while(cnt>0)
  {
    if( (r=driver_receive(ANY,&msg,&ipc_status)) !=0 )
    {
      printf("Driver receive failed with %d\n",r);
      continue;
    }
    if(is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source))
      {
        case HARDWARE:
          if(msg.m_notify.interrupts & mask)
          {
              mouse_ih();

              if(flag)
              {
                if(data & BIT(3) && bytes==0)
                  dados[0]=data;
                else
                  dados[bytes]=data;
                bytes++;
              }
              else
                continue;
              
              if(bytes==3)
              {
                struct packet pacote_dados;
                createMousePacket(dados,&pacote_dados);
                mouse_print_packet(&pacote_dados);
                cnt--;
                bytes=0;
              }
            
          }
        default:
          break;
      }
    }
  }

  //Unsubscribing mouse interrupts
  if(mouse_unsubscribe_interrupts()==-1)
  {
    printf("Error unsubscribing mouse interrupts\n");
    return -1;
  }

  //Disable data reporting
  if(write_command(KBC_WRITE_TO_MOUSE)!=0)
  {
    printf("Error writing command 0xD4 to KBC\n");
    return -1;
  }

  if(write_command_byte(MOUSE_DISABLE_DATA_REPORT)!=0)
  {
    printf("Error writing command 0xF5 to mouse\n");
    return -1;
  }

  //Reading return byte from mouse due to writing byte to mouse
  if(read_return_from_mouse()!=0)
  {
    printf("Error reading answer from mouse\n");
    return -1;
  }
  else
    printf("Data report successfully disabled\n");
  
  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    /* To be completed */
    printf("%s(%u): under construction\n", __func__, idle_time);
    return 1;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* To be completed */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
