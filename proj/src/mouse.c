#include <lcom/lcf.h>
#include "mouse.h"
#include "kbc.h"
#include "vbe.h"
#include "../Images/aim.xpm"
static Mouse *mouse=NULL;
int hook;
uint8_t mouse_data;
bool mouse_flag=0;

int (mouse_enable_data_report)(void){
    //Enable mouse_data reporting
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
        return 0;
    }
    return -1;
}

void (mouse_ih)(void){
    mouse_flag=0;

    if(read_mouse_data(&mouse_data)==0)
        mouse_flag=1;
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

int (mouse_subscribe_interrupts)(uint32_t *bit_no){
    hook=MOUSE_IRQ;
    *bit_no=BIT(hook);
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
    if(read_mouse_data(&mouse_return_value)!=0)
        return -1;
    switch(mouse_return_value)
    {
        case MOUSE_ACK:
            return 0;
        case MOUSE_NACK:
            printf("Mouse NACK error\t");
            return -1;
        case MOUSE_ERROR:
            printf("ERROR \t");
            return -1;
        default:
            printf("Byte read is not a mouse error %.2x\t",mouse_return_value);
            return -1;
    }
    return 0;
}


int (set_default_minix)(){
    if(write_command(COMMAND_WRITE_BYTE_COMMAND)!=0)
    {
      printf("Error writing command 0x60 to KBC\n");
      return -1;
    }
    uint8_t healthy_kbc_cb=minix_get_dflt_kbc_cmd_byte();
    printf("0x%.2x\t",healthy_kbc_cb);
    if(write_command_byte(healthy_kbc_cb)!=0)
    {
      printf("Error writing minix healthy command byte to Output Buffer\n");
      return -1;
    }
    return 0;
}

int (start_mouse)(uint32_t * mouse_mask){
    // Enabling stream mode
    if(mouse_enable_data_report()!=0)
    {
        printf("Error enabling mouse_data report\n");
        return -1;
    }
    //Subscribing mouse interrupts
    if(mouse_subscribe_interrupts(mouse_mask)==-1)
    {
        printf("Error subscribing mouse interrupts\n");
        return -1;
    }
    printf("Mouse enabled\n");
    return 0;
}

int (disable_mouse)(){
    //Unsubscribing mouse interrupts
    if(mouse_unsubscribe_interrupts()==-1)
    {
        printf("Error unsubscribing mouse interrupts\n");
        return -1;
    }
    empty_input_buffer();
    
    //Disable mouse_data reporting
    if(write_command(KBC_WRITE_TO_MOUSE)!=0)
    {
        printf("Error writing command 0xD4 to KBC\n");
        return -1;
    }

    if(write_command_byte(MOUSE_DISABLE_DATA_REPORT)!=0)
    {
        printf("Error writing command 0xF5 to mouse\n");
        return -1;
    }
    //Reading return byte from mouse due to writing byte to mouse
    if(read_return_from_mouse()!=0)
    {
        printf("Error reading answer from mouse\n");
        return -1;
    }
    printf("Sucessfully disabled mouse\n");
    return 0;
}

Mouse * get_mouse(){
    if(mouse==NULL)
    {
        mouse=(Mouse *)malloc(sizeof(Mouse));
        if(load_pixmap(aim_xpm,&mouse->img)==NULL)
            return NULL;
    }
    return mouse;
}

void eliminate_Mouse(){
    free(mouse);
}

void mouse_update_position(struct packet data){
    mouse->x+=data.delta_x;
    mouse->y-=data.delta_y;
    if((mouse->x+mouse->img.width)>get_hres())
        mouse->x=get_hres()-mouse->img.width;
    else if(mouse->x<0)
        mouse->x=0;

    if((mouse->y+mouse->img.height)>get_vres())
        mouse->y=get_vres()-mouse->img.height;
    else if(mouse->y<0)
        mouse->y=0;
}
