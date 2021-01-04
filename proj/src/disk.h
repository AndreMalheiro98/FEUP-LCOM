#ifndef __DISK_H
#define __DISK_H

/**
 * @brief  
**/
typedef struct{
    int16_t x;
    int16_t y;
    int8_t xVar;
    int8_t yVar;
    xpm_image_t img;
    xpm_image_t splatter_img;
    bool hit;
    bool out_of_bounds;
}Disk;

Disk * create_disk();
void reset_disk();
void update_disk_coord();
#endif
