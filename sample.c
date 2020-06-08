#include "clui.h"

int main(){
    init_clui(); // initialize the library

    change_color(COLOR_ORANGE_1); 
    printf("this text is orange 1\n");     reset_color(); 

    change_color(COLOR_ORANGE_2); 
    printf("this text is orange 2\n");     reset_color(); 
    
    change_color(COLOR_ORANGE_3); 
    printf("this text is orange 3\n");     reset_color(); 
        
    change_color_rgb(150,10,15); 
    printf("this text is TEXT \n"); 
    reset_color(); 

    
    printf("press any key to exit\n");
    getch(); // wait for user to enter a chracter

    quit(); // clear screen and exit program
}
