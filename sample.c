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
    init_clui();
    while (1==1) {
        //printf("we are not blocking\n");
        //flush();
        printf(".");
        flush();
        delay(1000);
        if( is_keyboard_hit() ){
            char c = getch();
            if( c == 27) break;
            printf("\n%c\n", c );
        }
    }

    return 0;
} 
