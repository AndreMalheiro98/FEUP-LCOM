#include <lcom/lcf.h>
#include "vbe.h"

int (vbe_set_visual_mode)(uint16_t mode){
  switch(mode)
  {
    case VBE_MODE_0:
      break;
    case VBE_MODE_1:
      break;
    case VBE_MODE_2:
      break;
    case VBE_MODE_3:
      break;
    case VBE_MODE_4:
      break;
    default:
      return -1;
  }
  reg86_t aux;
  aux.intno=0x10;
  aux.ah= VBE_FUNCTIONS_AH;
  aux.ax=VBE_SET_MODE;
  aux.bx= BIT(14) | mode;
  if(sys_int86(&aux) != OK){
    printf("VBE set mode failed\n");
    return -1;
  }
  return 0;
}

