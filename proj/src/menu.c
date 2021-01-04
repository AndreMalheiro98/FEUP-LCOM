#include <lcom/lcf.h>
#include "menu.h"
#include "vbe.h"
#include "../Images/main_menu.xpm"
#include "../Images/pause_menu.xpm"
static Menu * main_menu=NULL;
static Menu * pause_menu=NULL;

Menu * get_menu(int x){
    if(x==0){
        if(main_menu==NULL){
            main_menu=(Menu *)malloc(sizeof(Menu));
            if(load_pixmap(main_menu_xpm,&main_menu->img)==NULL)
                return NULL;  
        }
        return main_menu;
    }else if(x==1){
        if(pause_menu==NULL){
            pause_menu=(Menu *)malloc(sizeof(Menu));
            if(load_pixmap(pause_menu_xpm,&pause_menu->img)==NULL)
                return NULL;  
        }
        return pause_menu;
    }else
        return NULL;
}

void delete_menus(){
    free(pause_menu);
    free(main_menu);
}

