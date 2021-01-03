#ifndef __KEYBOARD_H
#define __KEYBOARD_H

    #define NUMBER_OF_TRIES 2 /* number of tries possible*/

    enum KBC_KEY {
    TWO_BYTE = 0,     /* enum Two bytes */
    ESC_BC = 1,    /* enum Esc breakcode */
    NO_KEY = 2,       /* enum No key pressed or invalid key*/
    R_MC = 3,       /* enum R makecode*/
    R_BC = 4,       /* enum R breakcode*/
    SPACE_BC = 5, /* enum Space breakcode*/
    ENTER_BC = 6  /* enum Enter breakcode*/
    };

    void (kbc_ih)();
    enum KBC_KEY (get_key_pressed)(bool make, int number_of_bytes, uint8_t * key_codes);
    enum KBC_KEY (Verify_Key)(uint8_t scancode) ;
    int (kbc_subscribe_interrupts)(uint32_t * mask);
    int (kbc_unsubscribe_interrupts)();
    
#endif
