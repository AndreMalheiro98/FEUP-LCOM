#include "kbc.h"
int hook;

void (kbc_ieh)(){ 
  hook=KBC_IRQ;
  sys_irqsetpolicy(KBC_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&hook);
  printf("Subscribed interrupts\n");
}

void (cancel)(){
  sys_irqrmpolicy(&hook);
}

int (aux)(uint8_t *scanccode)
{
  uint32_t number;
  if(sys_inb(KBC_ST_REG,&number)!=OK)
    return -1;
  uint8_t aux=number ;
  uint32_t obf;
  if(sys_inb(OUT_BUF,&obf)!=OK)
    return -1;
  if((BIT(7) & aux))
  {
    printf("Parity  error\n");
    return -1;
  }
  if((BIT(6) & aux))
  {
    printf("Timeout error\n");
    return -1;
  }
  
  *scanccode=obf;
  return 0;

}
