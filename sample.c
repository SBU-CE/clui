#include "clui.h"

/*
int kbhit()
{
    struct termios term;
    tcgetattr(0, &term);

    struct termios term2 = term;
    term2.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &term2);

    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);

    tcsetattr(0, TCSANOW, &term);

    return byteswaiting > 0;
}
*/

int main() {
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


    // change print color 
    // with RGB
    change_color_rgb(150,10,15);
    printf("this text is some color \n");
    reset_color();


    while (true) {
        printf(".");
        flush();
        delay(1000);
        if( is_keyboard_hit() ){
            char c = getch();
            if( c == 27) break;
            printf("\n%c\n", c );
        }
    }


    quit();
    return 0;
} 
