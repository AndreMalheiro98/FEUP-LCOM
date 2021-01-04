#ifndef __DISK_H
#define __DISK_H

/**
 * @brief  
**/
typedef struct{
    uint16_t x;
    uint16_t y;
    uint8_t xVar;
    uint8_t yVar;
    uint16_t target_x;
    uint16_t target_y;
    xpm_image_t img;
    xpm_image_t splatter_img;
    bool kill;
}Disk;

Disk * create_disk();
void reset_disk();
void update_disk_coord();
#endif
