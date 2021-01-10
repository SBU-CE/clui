#include "clui.h"

int main()
{
    // initialize library
    init_clui();

    // example of using delay function
    // input is an integer showing time of delay
    // in milliseconds
    printf("testing delay\nit should take 3 seconds\n");
    delay(3000); // wait for 3 seconds

    // get size of terminal with 2 functions for rows and cols
    printf("number of terminal cols: %d\n", get_window_cols());
    printf("number of terminal rows: %d\n", get_window_rows());

    // change print color
    // with RGB
    change_color_rgb(150, 10, 15);
    printf("this text is red\n");
    reset_color();

    printf("press any key to see what happen!\n");
    printf("and press esc for exit\n");

    // loop of our demo progrma
    while (true) {
        //here we show that this is non-blocking
        // we print and get character in same while
        printf(".");
        // flush the stdout (behave same as cout << endl)
        flush();

        // wait for 0.1 second in every iteration
        delay(100);

        // we only get character if a key is pressed
        // if no key is pressed, we continue the while
        if (is_keyboard_hit()) {
            char t = getch();
            if (t == 27)
                break;
            else printf("<%c>", t);
        }
    }

    quit();
    return 0;
}