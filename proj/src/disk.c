#include <lcom/lcf.h>
#include "disk.h"
#include "vbe.h"
#include "../Images/disk.xpm"
#include "../Images/diskHit.xpm"

static Disk *disk =NULL;

Disk * create_disk(){
    if(disk==NULL)
    {
        disk=(Disk *) malloc(sizeof(Disk));
        if(load_pixmap(disk_xpm,&disk->img)==NULL)
            return NULL;
        if(load_pixmap(disk_hit_xpm,&disk->splatter_img)==NULL)
            return NULL;
    }
    reset_disk();
    return disk;
}

void reset_disk(){
    disk->hit=0;
    disk->out_of_bounds=0;
    disk->x=rand() % (get_hres() - disk->img.width);
    disk->y=rand() % (get_vres() - disk->img.height);

    if(disk->x >(get_hres()/2))
        disk->xVar=-3;
    else
        disk->xVar=3;
    if(disk->y>(get_vres()/2))
        disk->yVar=-3;
    else
        disk->yVar=3; 
}

void update_disk_coord(){
    disk->x+=disk->xVar;
    disk->y+=disk->yVar;
}
