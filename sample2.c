#include "clui.h"

int main()
{
    // Initiailize clui
    init_clui();

    printf("Hello my friend\n");
    flush(); // flush the output same as std::flush;

    // Lets save where the cursor is
    // so we can use it later
    save_cursor();

    // capturing windows size
    int row = get_window_rows();
    int cols = get_window_cols();

    // moving cursor to 3 rows before
    // end of the terminal window
    cursor_to_pos(row - 3, 1);

    // starting printing some sort
    // of a way to show a border
    for (int i = 0; i < cols; i++)
        printf("-");
    flush();
    printf("\n");
    printf("I can be your footage in your app\n");
    printf("for example: press ctrl-c to close");

    // moving back the cursor
    restore_cursor();
    printf("\nNoice border :)");
    flush();

    // wait until ctrl-c getting
    // enterred
    while (true)
        ;

    quit();
}
