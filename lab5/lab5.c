// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include "vbe.h"
#include "kbc.h"
// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  void * p;
  if((p=vg_init(mode))==NULL)
    return -1;
  sleep(delay);
  if(vg_exit()!=0)
  {
    printf("Error setting text mode\n");
    return -1;
  }

  return 0;
}
extern bool flag;
extern uint8_t bytes[2];
int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  

  void * p;
  if((p=vg_init(mode))==NULL)
  {
    printf("Error setting mode\n");
    return -1;
  }
  if(vg_draw_rectangle(x,y,width,height,color)!=0)
  {
    printf("Error creating rectangle\n");
  }
  else{
    if(kbc_subscribe_interrupts()!=0)
    {
      printf("Error subscribing keyboard interrupts\n");
      exit(1);
    }
    bool make=0;
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
  }
  
  if(vg_exit()!=0)
  {
    printf("Error setting text mode\n");
    return -1;
  }
  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  /* To be completed */
  printf("%s(0x%03x, %u, 0x%08x, %d): under construction\n", __func__,
         mode, no_rectangles, first, step);

  return 1;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* To be completed */
  printf("%s(%8p, %u, %u): under construction\n", __func__, xpm, x, y);

  return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
