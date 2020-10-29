 #pragma once
  
 #include <stdbool.h>
 #include <stdint.h>
  
 int kbd_test_scan();
  
/*Tests reading of scancodes via KBD interrupts.
Displays the scancodes received from the keyboard using interrupts.
Exits upon release of the ESC key
Return 0 upon success and non-zero otherwise */

 int kbd_test_poll();

 /*Tests reading of scancodes via polling.
Displays the scancodes received from the keyboard using polling.
Exits upon release of the ESC key
Return 0 upon success and non-zero otherwise */
  
 int kbd_test_timed_scan(uint8_t n);

 /*Tests handling of more than one interrupt.
Similar to kbd_test_scan() except that it should terminate also if 
no scancodes are received for n seconds
Return 0 upon success and non-zero otherwise */

 void kbc_ih(void);
  
 int kbd_print_scancode(bool make, uint8_t size, uint8_t *bytes);
  
 int kbd_print_no_sysinb(uint32_t cnt);