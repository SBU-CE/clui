/*
 * clui library, by nima heydari nasab
 * @radio_nima
 * and some changes by roozbeh sharifnasab
 * @rsharifnasab
 * and parsa noori
 * @parsanoori
 */

#ifndef _CLUI
#define _CLUI

/*
 * common includes
 */
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * includes for windows
 */
#if defined(_WIN32) || defined(_WIN64) || defined(WI32)

#define OS_UNIX 0
#include <Windows.h>
#include <conio.h>

#define _NO_OLDNAMES // for MinGW compatibility
#define getch _getch
#define kbhit _kbhit

/*
 * includes for unix-like systems
 * MacOS or linux
 */
#else

#define OS_UNIX 1
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#endif

/*
 * define colors that can be chosen
 * in change_color function
 */
#define COLOR_DEFAULT 0
#define COLOR_RED 1
#define COLOR_BOLD_RED 2
#define COLOR_GREEN 3
#define COLOR_BOLD_GREEN 4
#define COLOR_YELLOW 5
#define COLOR_BOLD_YELLOW 6
#define COLOR_BLUE 7
#define COLOR_BOLD_BLUE 8
#define COLOR_MAGENTA 9
#define COLOR_BOLD_MAGENTA 10
#define COLOR_CYAN 11
#define COLOR_BOLD_CYAN 12

/*
 * colors added in version2
 * this is use 8 bit terminal
 */
#define COLOR_ORANGE_1 13
#define COLOR_ORANGE_2 14
#define COLOR_ORANGE_3 15

/*
 * define colors that can be chosen
 * in change_background_color function
 */
#define BG_DEFAULT 0
#define BG_BLUE 1
#define BG_CYAN 2
#define BG_GREEN 3
#define BG_PURPLE 4
#define BG_RED 5
#define BG_WHITE 6
#define BG_YELLOW 7

/*
 * the file stdout is line buffered hence
 * if you want to print out to terminal and
 * remain in the same line you should
 * use this function
 */
void flush()
{
    fflush(stdout);
    fflush(stderr);
}

void __enable_raw_mode()
{
#if OS_UNIX
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &term);
#else

#endif
}

void __disable_raw_mode()
{
#if OS_UNIX
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &term);
#else

#endif
}

/*
 * A cross platform functin to detect
 * whether keyboard is hit
 */
bool is_keyboard_hit()
{
#if OS_UNIX
    static struct termios oldt, newt;
    int cnt = 0;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_iflag = 0;     // input mode
    newt.c_oflag = 0;     // output mode
    newt.c_cc[VMIN] = 1;  // minimum time to wait
    newt.c_cc[VTIME] = 1; // minimum characters to wait for
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ioctl(0, FIONREAD, &cnt); // Read count
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100;
    select(STDIN_FILENO + 1, NULL, NULL, NULL, &tv); // A small time delay
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return cnt > 0;
#else
    return kbhit();
#endif
}

/*
 * since getchar is a nonstandard function
 * this funtion was implemented.
 * Note that windows has this function
 * implicitly
 */
#if OS_UNIX
int getch()
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON /* | ECHO */);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}
#endif

int nb_getch(void)
{
    if (is_keyboard_hit())
        return getch();
    else
        return 0;
}

/*
 * Simply clears the terminal screen
 * Uses OS'es command suited for this
 * action to prevent ANSI's problems on
 * different OSes
 */
void clear_screen()
{
#if OS_UNIX
    system("reset");
#else
    system("CLS");
#endif
}

/*
 * NOTE: NEEDS ANSI SUPPORT
 * change output color of terminal
 * by RGB input
 * this will only work for 24-bit terminals
 * tested in linux terminals
 * and new Windows10 cmd and powershell
 */
void change_color_rgb(int r, int g, int b)
{
    if (0 <= r && r <= 255
        && 0 <= g && g <= 255
        && 0 <= b && b <= 255) {
        printf("\033[38;2;%d;%d;%dm", r, g, b);
        flush();
    }
}

/*
 * NOTE: NEEDS ANSI SUPPORT
 * change color in 8 bit terminal
 */
void change_color(int color)
{

    const char* colors[] = { "[0m", "[0;31m", "[1;31m", "[0;32m",
        "[1;32m", "[0;33m", "[1;33m", "[0;34m",
        "[1;34m", "[0;35m", "[1;35m", "[0;36m",
        "[1;36m", "[38;5;202m", "[38;5;208m", "[38;5;214m" };

    if (color >= 0 && color <= 15) {
        printf("\033");
        printf("%s", colors[color]);
    }
    flush();
}

void change_background_color(int color)
{
    const char* colors[] = { "[40m", "[44m", "[46m", "[42m",
        "[45m", "[41m", "[47m", "[43m" };

    if (color >= 0 && color <= 7) {
        printf("\033");
        printf("%s", colors[color]);
    }
    flush();
}

/*
 * NOTE: NEEDS ANSI SUPPORT
 */
void reset_color()
{
    change_color(COLOR_DEFAULT);
}

/*
 * invoke when you're done with this library
 */
void quit()
{
    reset_color();
    __disable_raw_mode();
    clear_screen();
    exit(0);
}

static void sigint_handler(int dummy)
{
    // to get rid of unused-parameter warning
    (void)dummy;
    quit();
}

/*
 * library
 */
void init_clui()
{
    clear_screen();
    signal(SIGINT, sigint_handler);
    __enable_raw_mode();
    flush();
}

/*
 * a cross platform function to
 * suspends execution for some
 * milli seconds
 */
void delay(size_t milli_seconds)
{
    flush();
#if OS_UNIX
    usleep(1000 * milli_seconds);
#else
    Sleep(milli_seconds);
#endif
}

/*
 * returns the windows rows
 */
int get_window_rows()
{
#if OS_UNIX
    struct winsize max;
    ioctl(0, TIOCGWINSZ, &max);
    return (int)max.ws_row;
#else
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return (int)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
#endif
}

/*
 * returns the window cols
 */
int get_window_cols()
{
#if OS_UNIX
    struct winsize max;
    ioctl(0, TIOCGWINSZ, &max);
    return (int)max.ws_col;
#else
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return (int)(csbi.srWindow.Right - csbi.srWindow.Left + 1);
#endif
}
/*
 * NOTE: NEEDS ANSI SUPPORT
 * gives the x of the cursor
 * linux version might be a liitle slow
 */
int get_cursor_x()
{
    flush();
    int x = 0;
#if OS_UNIX
    const char* const cmd = "IFS=\";\" read -sdR -p $'\"'\\E[6n'\"' ROW COL;\
                             echo \"${ROW#*[}\" > /tmp/cursor_row";

    char cmd_bash[200];
    sprintf(cmd_bash, "bash -c '%s' ", cmd);
    int status = system(cmd_bash);
    if (status != 0) {
        return 0;
    }
    FILE* fp = fopen("/tmp/cursor_row", "r");
    if (fp == NULL) {
        return 0;
    }
    fscanf(fp, "%d", &x);
    fclose(fp);
#else
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    x = csbi.dwCursorPosition.Y + 1;
#endif
    return x;
}

/*
 * NOTE: NEEDS ANSI SUPPORT
 * gives the x of the cursor
 * linux version might be a little slow!
 */
int get_cursor_y()
{
    flush();
    int y = 0;
#if OS_UNIX
    const char* const cmd = "IFS=\";\" read -sdR -p $'\"'\\E[6n'\"' ROW COL;\
                             echo \"${COL#*[}\" > /tmp/cursor_col";

    char cmd_bash[200];
    sprintf(cmd_bash, "bash -c '%s' ", cmd);
    int status = system(cmd_bash);
    if (status != 0) {
        return 0;
    }
    FILE* fp = fopen("/tmp/cursor_col", "r");
    if (fp == NULL) {
        return 0;
    }
    fscanf(fp, "%d", &y);
    fclose(fp);
#else
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    y = csbi.dwCursorPosition.X + 1;
#endif
    return y;
}

/*
 * NOTE: NEEDS ANSI SUPPORT
 * moves cursor up n times
 */
// this define will solve backward compability issue
#define corsur_up cursor_up
void cursor_up(int n)
{
    printf("\033[%dA", n);
    flush();
}

/*
 * NOTE: NEEDS ANSI SUPPORT
 * moves cursor down n times
 */
void cursor_down(int n)
{
    printf("\033[%dB", n);
    flush();
}
/*
 * NOTE: NEEDS ANSI SUPPORT
 * moves cursor forward n time
 */
void cursor_forward(int n)
{
    printf("\033[%dC", n);
    flush();
}

/*
 * NOTE: NEEDS ANSI SUPPORT
 * moves cursor backwards n time
 */
void cursor_backward(int n)
{
    printf("\033[%dD", n);
    flush();
}

/*
 * NOTE: NEEDS ANSI SUPPORT
 * moves cursor to the given position
 */
void cursor_to_pos(int row, int col)
{
    printf("\033[%d;%dH", row, col);
    flush();
}

/*
 * NOTE: NEEDS ANSI SUPPORT
 * saves cursor position for further use
 */
void save_cursor()
{
    printf("\0337");
    flush();
}

/*
 * NOTE: NEEDS ANSI SUPPORT
 * restors cursor to the last saved
 * position
 */
void restore_cursor()
{
    printf("\0338");
    flush();
}

/*
 * NOTE: NEEDS ANSI SUPPORT
 * plays beep! :)
 */
void play_beep()
{
    printf("\07");
    flush();
}

#endif
