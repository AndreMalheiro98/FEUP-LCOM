#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#define NUMBER_OF_TRIES 2 /* number of tries possible*/
/**
 * @brief  
**/
enum KBC_KEY {
TWO_BYTE = 0,     /* enum Two bytes */
ESC_BC = 1,    /* enum Esc breakcode */
NO_KEY = 2,       /* enum No key pressed or invalid key*/
R_MC = 3,       /* enum R makecode*/
R_BC = 4,       /* enum R breakcode*/
SPACE_BC = 5, /* enum Space breakcode*/
ENTER_BC = 6,  /* enum Enter breakcode*/
A_BC = 7,
B_BC = 8,
C_BC = 9,
D_BC = 10,
E_BC = 11,
F_BC = 12,
G_BC = 13,
H_BC = 14,
I_BC = 15,
J_BC = 16,
K_BC = 17,
L_BC = 18,
M_BC = 19,
N_BC = 20,
O_BC = 21,
P_BC = 22,
Q_BC = 23,
S_BC = 24,
T_BC = 25,
U_BC = 26,
V_BC = 27,
W_BC = 28,
X_BC = 29,
Y_BC = 30,
Z_BC = 31,
};
/**
 * @brief  Keyboard Interrupt handler
**/
void (kbc_ih)();
/**
 * @brief Get the key pressed by the user and return its scancode
**/
enum KBC_KEY (get_key_pressed)(bool make, int number_of_bytes, uint8_t * key_codes);
/**
 * @brief  Verify if key is usable 
**/
enum KBC_KEY (Verify_Key)(uint8_t scancode) ;
/**
 * @brief  Subscribe to keyboard interrupts
**/
int (kbc_subscribe_interrupts)(uint32_t * mask);
/**
 * @brief  Unsubscribe to KeyBoard interrupts
**/
int (kbc_unsubscribe_interrupts)();
    
#endif
