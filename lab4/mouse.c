#include <lcom/lcf.h>
#include "mouse.h"
int hook;
uint8_t data;
bool flag=0;
int (mouse_enable_data_report)(void){
    //Disable data reporting
    int i=0;
    for(i=0;i<3;i++){
        tickdelay(micros_to_ticks(DELAY_US));
        if(write_command(KBC_WRITE_TO_MOUSE)!=0)
        {
            printf("Error writing command 0xD4 to KBC\n");
            continue;
        }

        if(write_command_byte(MOUSE_ENABLE_DATA_REPORT)!=0)
        {
            printf("Error writing command 0xF4 to mouse\n");
            continue;
        }

        //Reading return byte from mouse due to writing byte to mouse
        if(read_return_from_mouse()!=0)
        {
            printf("Error reading answer from mouse\n");
            continue;
        }
        break;
    }
    if(i==3)
        return -1;
    return 0;

}

void (mouse_ih)(void){
    uint8_t byte;
    flag=0;
    if(util_sys_inb(KBD_STAT_REG,&byte)!=0)
    {
        printf("error reading status register\n");
        return ;
    }
    
    if(byte&KBD_OBF && byte&KBD_AUX)
    {
        
        if(util_sys_inb(KBD_OUT_BUF,&data)!=0)
        {
            printf("Error reading mouse data\n");
            return ;
        }
        
        if(byte&KBD_TO_ERR || byte&KBD_PAR_ERR)
        {
            printf("Data has error., discarding\n");
            return ;
        }
        flag=1;
    }
    else if(!(byte & KBD_OBF))
        printf("No data on output buffer\n");
        
    return ;
}

void createMousePacket(uint8_t array[3],struct packet *pp)
{
    pp->bytes[0]=array[0];
    pp->bytes[1]=array[1];
    pp->bytes[2]=array[2];
    pp->rb = array[0] & MOUSE_RB;
    pp->mb = array[0] & MOUSE_MB;
    pp->lb = array[0] & MOUSE_LB;
    pp->x_ov= array[0] & MOUSE_X_OVF;
    pp->y_ov= array[0] & MOUSE_Y_OVF;
    if(array[0] & MOUSE_MSB_X_DELTA)
        pp->delta_x = array[1] | 0xFF00;
    else
        pp->delta_x = array[1];
    if(array[0] & MOUSE_MSB_Y_DELTA)
        pp->delta_y=array[2] | 0xFF00;
    else
        pp->delta_y=array[2]; 
}

int (mouse_subscribe_interrupts)(){
    hook=MOUSE_IRQ;
    if(sys_irqsetpolicy(MOUSE_IRQ,IRQ_REENABLE | IRQ_EXCLUSIVE,&hook)!=OK)
        return -1;
    return 0;
}

int (mouse_unsubscribe_interrupts)(){
    if(sys_irqrmpolicy(&hook)!=OK)
        return -1;
    return 0;
}

int (read_return_from_mouse)(){
    uint8_t mouse_return_value;
    int i=0;
    for(i=0;i<10;i++)
    {
        tickdelay(micros_to_ticks(DELAY_US));
        if(util_sys_inb(KBD_OUT_BUF,&mouse_return_value)!=0)
        {
            printf("Error reading from Output Buffer\n");
            continue;
        }
        else
            break;
    } 
    if(i==10)
        return -1;
    switch(mouse_return_value)
    {
        case MOUSE_ACK:
            printf("Mouse ACKNOWLEDGMENT\t");
            return 0;
        case MOUSE_NACK:
            printf("Mouse NACK error\t");
            return -1;
        case MOUSE_ERROR:
            printf("ERROR \t");
            return -1;
        default:
            printf("Byte read is not a mouse error\t");
            return -1;
    }
    return 0;
}
