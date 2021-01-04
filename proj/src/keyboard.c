#include <lcom/lcf.h>
#include "keyboard.h"
#include "i8042.h"
#include "kbc.h"
int kbc_hook;
uint8_t bytes[2];
bool kbc_flag , is_two_bytes = 0;

int (kbc_subscribe_interrupts)(uint32_t * mask){ 
  kbc_hook=KBD_IRQ;
  *mask=BIT(kbc_hook);
  if(sys_irqsetpolicy(KBD_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&kbc_hook)!=OK)
    return -1;
  return 0;
}

int (kbc_unsubscribe_interrupts)(){
  if(sys_irqrmpolicy(&kbc_hook)!=OK)
    return -1;
  return 0;
}

void (kbc_ih)()
{
  uint8_t scancode;
  if(read_keyboard_data(&scancode)!=0)
    return;
  if(scancode==KB_TWO_BYTES_SCANCODE_FIRST_BYTE)
  {
    kbc_flag=1;
    bytes[0]=scancode;
  }
  else{
    if(kbc_flag)
    {
      bytes[1]=scancode;
    }
    else
    {
      bytes[0]=scancode;
      kbc_flag=0;
    }
  }
  return;
}

enum KBC_KEY (Verify_Key)(uint8_t scancode) {
  switch (scancode) {
    case ESC_KEY_BREAKCODE:
      return ESC_BC;
      break;
    case R_KEY_MAKECODE:
      return R_MC;
      break;
    case R_KEY_BREAKCODE:
      return R_BC;
      break;
    case SPACE_KEY_BREAKCODE:
      return SPACE_BC;
      break;
    case ENTER_KEY_BREAKCODE:
      return ENTER_BC;
      break;
    case A_KEY_BREAKCODE:
      return A_BC;
      break;
    case B_KEY_BREAKCODE:
      return B_BC;
      break;
    case C_KEY_BREAKCODE:
      return C_BC;
      break;
    case D_KEY_BREAKCODE:
      return D_BC;
      break;
    case E_KEY_BREAKCODE:
      return E_BC;
      break;
    case F_KEY_BREAKCODE:
      return F_BC;
      break;
    case G_KEY_BREAKCODE:
      return G_BC;
      break;
    case H_KEY_BREAKCODE:
      return H_BC;
      break;
    case I_KEY_BREAKCODE:
      return I_BC;
      break;
    case J_KEY_BREAKCODE:
      return J_BC;
      break;
    case K_KEY_BREAKCODE:
      return K_BC;
      break;
    case L_KEY_BREAKCODE:
      return L_BC;
      break;
    case M_KEY_BREAKCODE:
      return M_BC;
      break;
    case N_KEY_BREAKCODE:
      return N_BC;
      break;
    case O_KEY_BREAKCODE:
      return O_BC;
      break;
    case P_KEY_BREAKCODE:
      return P_BC;
      break;
    case Q_KEY_BREAKCODE:
      return Q_BC;
      break;
    case S_KEY_BREAKCODE:
      return  S_BC; 
      break;
    case T_KEY_BREAKCODE:
      return T_BC;
      break;
    case U_KEY_BREAKCODE:
      return U_BC;
      break;
    case V_KEY_BREAKCODE:
      return V_BC;
      break;
    case W_KEY_BREAKCODE:
      return W_BC;
      break;
    case X_KEY_BREAKCODE:
      return X_BC;
      break;
    case Y_KEY_BREAKCODE:
      return Y_BC;
      break;
    case Z_KEY_BREAKCODE:
      return Z_BC;
      break;
    default:
      return NO_KEY;
      break;
  }
} 

enum KBC_KEY (get_key_pressed)(bool make, int number_of_bytes, uint8_t * key_codes){
  if(make) return NO_KEY;
  else if(number_of_bytes == 2) return Verify_Key(key_codes[1]);
  else if(number_of_bytes == 1) return Verify_Key(key_codes[0]);
  else  return NO_KEY;
}
