#include <lcom/lcf.h>
#include "mouse.h"
int hook;
uint8_t data;

int (mouse_enable_data_report)(void){
    //Disable data reporting
    if(write_command(KBC_WRITE_TO_MOUSE)!=0)
    {
        printf("Error writing command 0xD4 to KBC\n");
        return -1;
    }

    if(write_command_byte(MOUSE_ENABLE_DATA_REPORT)!=0)
    {
        printf("Error writing command 0xF4 to mouse\n");
        return -1;
    }

    //Reading return byte from mouse due to writing byte to mouse
    if(read_return_from_mouse()!=0)
    {
        printf("Error reading answer from mouse\n");
        return -1;
    }
    return 0;
}

void (mouse_ih)(void){
    uint8_t byte;
    
    if(util_sys_inb(KBD_STAT_REG,&byte)!=0)
        return ;
    
    if(byte&KBD_OBF && byte&KBD_AUX)
    {
        if(util_sys_inb(KBD_OUT_BUF,&data)!=0)
        {
            printf("Error reading mouse data\n");
            return ;
        }
    }
    if(byte&KBD_TO_ERR || byte&KBD_PAR_ERR)
    {
        printf("Data has error., discarding\n");
        return ;
    }
    return ;
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
    uint8_t register_status;
    for(i=0;i<10;i++)
    {
        tickdelay(micros_to_ticks(DELAY_US));
        if(util_sys_inb(KBD_STAT_REG,&register_status)!=0)
        {
            printf("Error reading register status\n");
            continue;
        }
        if(register_status & BIT(0) && (register_status & BIT(5)))
        {
            if(util_sys_inb(KBD_OUT_BUF,&mouse_return_value)!=0)
            {
                printf("Error reading from Output Buffer\n");
                continue;
            }
        }
        else
        {
            continue;
        }
        if((BIT(7) & register_status))
        {
            printf("Parity  error\n");
            continue;
        }
        if((BIT(6) & register_status))
        {
            printf("Timeout error\n");
            continue;
        }
        break;
        
    } 
    if(i==11)
        return -1;
    switch(mouse_return_value)
    {
        case MOUSE_ACK:
            printf("Data reporting was successffully disabled\n");
            return 0;
        case MOUSE_NACK:
            printf("NACK error\n");
            return -1;
        case MOUSE_ERROR:
            printf("ERROR \n");
            return -1;
        default:
            printf("Byte read is not a mouse error\n");
            return -1;
    }
    return 0;
}
