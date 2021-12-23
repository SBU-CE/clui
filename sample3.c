#include "clui.h"

int main()
{
    // initialize library
    init_clui();

    int cols =  get_window_cols();
    int rows = get_window_rows();
    int middle = cols / 2;
    cursor_to_pos(0, middle); // set curspor position

    for (int i = 1; i < rows; ++i) {
        // if there is a key? get it
        // if not, do not block and return 0
        int key = nb_getch();
        if(key != 0){
            // a key is pressed
            if(key == 27){ // esc will quit
                break;
            } else {
                printf("%d", key);
            }
        } else { // nothing is pressed
            printf(".");
        }
        cursor_to_pos(i, middle);
        delay(1000);
    }

    quit();
    return 0;
}
