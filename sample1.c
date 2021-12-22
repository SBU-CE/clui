#include "clui.h"

int main()
{
    // initialize library
    init_clui();
    // get size of terminal with 2 functions for rows and cols
    printf("number of terminal cols: %d\n", get_window_cols());
    printf("number of terminal rows: %d\n", get_window_rows());
    printf("cursor pos (x:%d, y:%d)\n", get_cursor_x(), get_cursor_y());

    // change print color
    // with RGB
    change_color_rgb(150, 10, 15);
    printf("this text is red\n");
    reset_color();

    // example of using delay function
    // input is an integer showing time of delay
    // in milliseconds
    printf("testing delay\nit should take 3 seconds\n");
    delay(3000); // wait for 3 seconds

    // demonstrate getch
    printf("press a char!\n");
    printf("or esc to exit\n");

    char t = getch();
    if (t == 27) { // escape
        printf("bye\n");
        play_beep();
        quit();
    } else {
        printf("you entered : <%c>\n", t);
    }

    // let's draw a loading bar
    for (int i = 0; i < 20; ++i) {
        printf(".");
        // flush output (behave same as cout << endl)
        flush();
        // wait for 0.5 between printing dots
        delay(500);
    }

    quit();
    return 0;
}
