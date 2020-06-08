#include "clui.h"

int main(){
    
    // initialize library
    init_clui();

    // example of using delay function
    // input is an integer showing time of delay 
    // in milliseconds
    printf("testing delay\nit should take 5 seconds\n");
    delay(5000); // wait for 5 seconds
    
    // get size of terminal with 2 functions for rows and cols
    printf("number of terminal cols: %d\n",  get_window_cols() );
    printf("number of terminal rows: %d\n",  get_window_rows() );

    // change print color to various colors
    change_color(COLOR_ORANGE_1);
    printf("this text is orange 1\n");
    reset_color();

    change_color(COLOR_ORANGE_2);
    printf("this text is orange 2\n");
    reset_color();

    change_color(COLOR_ORANGE_3);
    printf("this text is orange 3\n");
    reset_color();
    

    // use rgb to change color 
    change_color_rgb(150,10,15);
    printf("this text is some color \n");
    reset_color();
    
    // test getch
    while(1==1){
        printf("press esc key to exit\n");
        char c = getch(); // wait for user to enter a chracter
        if(c == 27) break;
    }
    
    // clear screen and exit program
    quit(); 
}
