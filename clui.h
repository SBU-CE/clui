/*
 * clui library, by nima heydari nasab 
 * @radio_nima
 * and some changes by roozbeh sharifnasab
 * @rsharifnasab
 */


#ifndef _CLUI
#define _CLUI

/*
 * common includes
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/*
 * includes for unix and linux 
 */
#ifdef __unix__
    #define OS_UNIX 1
    #include <termios.h>
    #include <unistd.h>

/*
 * includes for windows
 */
#elif defined(_WIN32) || defined(_WIN64) || defined(WI32)
    #define OS_UNIX 0
    #include <conio.h>

/*
 * includes for unknown operation system
 * typically mac os 
 * it will hopefully work if unknown operation system is posix
 * I've tested on MacOS
 */
#else
    #define OS_UNIX 2
    #include <termios.h>
    #include <unistd.h>

#endif


/*
 * define colors that can be chosen
 * in change_color function
 */
#define COLOR_DEFAULT      0
#define COLOR_RED          1
#define COLOR_BOLD_RED     2
#define COLOR_GREEN        3
#define COLOR_BOLD_GREEN   4
#define COLOR_YELLOW       5
#define COLOR_BOLD_YELLOW  6
#define COLOR_BLUE         7
#define COLOR_BOLD_BLUE    8
#define COLOR_MAGENTA      9
#define COLOR_BOLD_MAGENTA 10
#define COLOR_CYAN         11
#define COLOR_BOLD_CYAN    12

/*
 * colors added in version2
 * this is use 8 bit terminal
 */
#define COLOR_ORANGE_1    13
#define COLOR_ORANGE_2    14
#define COLOR_ORANGE_3    15



void clear_screen(){
    #if OS_UNIX
    printf("\033c"); //for ansi terminal
    #else
    system("CLS"); //for windows
    #endif
}

/*
 * change output color of terminal 
 * by RGB input 
 * this will only work for 24-bit terminals
 * tested in linux terminals
 * and new Windows10 cmd and powershell
 */
void change_color_rgb(int r, int g, int b){
    printf("\033[38;2;%d;%d;%dm",r,g,b);
}

/*
 * change color in 8 bit terminal
 */
void change_color(int color) {
    printf("\033");
     
    const char *colors[] = {
        "[0m",
        "[0;31m",
        "[1;31m",
        "[0;32m",
        "[1;32m",
        "[0;33m",
        "[1;33m",
        "[0;34m",
        "[1;34m",
        "[0;35m",
        "[1;35m",
        "[0;36m",
        "[1;36m",
        
        "[38;5;202m",
        "[38;5;208m",
        "[38;5;214m" 
    };
    if (color >= 0 && color <= 15) {
        printf("%s", colors[color]);
    }
}


void reset_color(){
    change_color(COLOR_DEFAULT);
}


void quit() {
    reset_color();
    clear_screen();
    exit(0);
}


void sigint_handler(int dummy) {
    quit();
}



void init_clui(){
    clear_screen();
    signal(SIGINT, sigint_handler);
    #if OS_UNIX == 2 
        //printf("warning, using on unkown operation system\n");
    #endif      
}


#if OS_UNIX 
int getch(){
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}
#endif

//TODO: add other functions and make them platform independent

#endif
