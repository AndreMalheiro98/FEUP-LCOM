 /**
 * @file disk.h
 * Disk
 */

#ifndef __DISK_H
#define __DISK_H
/** @defgroup disc Disc
 * @{
 *
 * Functions for using the target
 */
/**
 * @brief Target struct 
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
/**
 * @brief Creates target and returns it
**/
Disk * create_disk();
/**
 * @brief Resets target
**/
void reset_disk();
/**
 * @brief Updates target coordinates
**/
void update_disk_coord();
#endif
