// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

static int print_usage() {
  printf("Usage: ./lcom_run proj\n");

  return 1;
}
#include "game.h"

int(proj_main_loop)(int argc, char *argv[]) {
  /* 
   * Substitute the code below by your own
   */

  //
  // if you're interested, try to extend the command line options so that the usage becomes:
  // <mode - hex> <minix3 logo  - true|false> <grayscale - true|false> <delay (secs)>
  //
  /*bool const minix3_logo = true;
  bool const grayscale = false;*/
 // uint16_t const delay = 600;
  //uint16_t mode;
  srand(time(NULL));
  if (argc !=0)
    return print_usage();
/*
  // parse mode info (it has some limitations for the sake of simplicity)
  if (sscanf(argv[0], "%hx", &mode) != 1) {
    printf("%s: invalid mode (%s)\n", __func__, argv[0]);

    return print_usage();
  }*/

  //Enter graphic mode
  if( game_init_graphics_mode()!=0)
  {
    game_exit_graphic_mode();
    return -1;
  }

  //Create game
  Game *current_game=create_new_game();
  if(current_game==NULL){
    game_exit_graphic_mode();
    return -1;
  }
  
  //Subscribe to periphericals
  uint32_t mouse_mask,timer_mask,keyboard_mask;
  if(subscribe_periphericals(&mouse_mask,&timer_mask,&keyboard_mask)!=0){
    game_exit_graphic_mode();
    return -1;
  }
  
  //main cycle for interrupts
  message msg;
  int r,ipc_status;
  while(get_game_state()!=STATE_EXIT){
    if((r=driver_receive(ANY,&msg,&ipc_status)) !=0){
      printf("Driver receive failed with %d",r);
      continue;
    }
    if(is_ipc_notify(ipc_status)){
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE:
        if(msg.m_notify.interrupts & mouse_mask){ //Mouse interrupts
          handle_mouse_interrupts();
        }
        
        if(msg.m_notify.interrupts & keyboard_mask){//keyboard interrupts)
          handle_keyboard_interrupts();
        }
        
        if(msg.m_notify.interrupts & timer_mask){//timer interrupts
          handle_timer_interrupts();
        }
        
        break;
      
      default:
        break;
      }
    }
  }
  
  //Unsubscribing periphericals
  if(unsubscribe_periphericals()!=0)
    return -1;

  //Exiting graphic mode
  if(game_exit_graphic_mode()!=0)
    return -1; 

  //Eliminating game and whatnot - avoiding memory leaks by freeing allocated mem
  eliminate_game();

  printf("Exit\n");
  //return proj_demo(mode, minix3_logo, grayscale, delay);
  return 0;
}
