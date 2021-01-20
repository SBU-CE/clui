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
#include <unistd.h>

/*
 * includes for windows
 */
#if defined(_WIN32) || defined(_WIN64) || defined(WI32)

#define OS_UNIX 0
#include <Windows.h>
#include <conio.h>

/*
 * includes for unix-like systems
 * MacOS or linux
 */
#else

#define OS_UNIX 1
#include <sys/ioctl.h>
#include <termios.h>

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
#define BACKGROUND_DEFAULT 0
#define BACKGROUND_BLUE 1
#define BACKGROUND_CYAN 2
#define BACKGROUND_GREEN 3
#define BACKGROUND_PURPLE 4
#define BACKGROUND_RED 5
#define BACKGROUND_WHITE 6
#define BACKGROUND_YELLOW 7

static void enable_raw_mode() {
#if OS_UNIX
  // struct termios term;
  // tcgetattr(0, &term);
  // term.c_lflag &= ~(ICANON);
  // tcsetattr(0, TCSANOW, &term);
#else

#endif
}

static void disable_raw_mode() {
#if OS_UNIX
  struct termios term;
  tcgetattr(0, &term);
  term.c_lflag |= ICANON | ECHO;
  tcsetattr(0, TCSANOW, &term);
#else

#endif
}

#if 0
/*
 * A cross platform functin to detect
 * whether keyboard is hit
 */
bool is_keyboard_hit()
{
#if OS_UNIX
    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);
    return byteswaiting > 0;
#else
    return kbhit();
#endif
}
#endif

/*
 * Simply clears the terminal screen
 * Uses OS'es command suited for this
 * action to prevent ANSI's problems on
 * different OSes
 */
void clear_screen() {
#if OS_UNIX
  system("clear");
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
void change_color_rgb(int r, int g, int b) {
  if (0 <= r && r <= 255 && 0 <= g && g <= 255 && 0 <= b && b <= 255)
    printf("\033[38;2;%d;%d;%dm", r, g, b);
}

/*
 * NOTE: NEEDS ANSI SUPPORT
 * change color in 8 bit terminal
 */
void change_color(int color) {

  const char *colors[] = {"[0m",    "[0;31m",     "[1;31m",     "[0;32m",
                          "[1;32m", "[0;33m",     "[1;33m",     "[0;34m",
                          "[1;34m", "[0;35m",     "[1;35m",     "[0;36m",
                          "[1;36m", "[38;5;202m", "[38;5;208m", "[38;5;214m"};

  if (color >= 0 && color <= 15) {
    printf("\033");
    printf("%s", colors[color]);
  }
}

void change_background_color(int color) {

  const char *colors[] = {"[40m", "[44m", "[46m", "[42m",
                          "[45m", "[41m", "[47m", "[43m"};

  if (color >= 0 && color <= 7) {
    printf("\033");
    printf("%s", colors[color]);
  }
}

/*
 * NOTE: NEEDS ANSI SUPPORT
 */
void reset_color() { change_color(COLOR_DEFAULT); }

/*
 * the file stdout is line buffered hence
 * if you want to print out to terminal and
 * remain in the same line you should
 * use this function
 */
void flush() {
  fflush(stdout);
  fflush(stderr);
}

/*
 * invoke when you're done with this library
 */
void quit() {
  reset_color();
  disable_raw_mode();
  clear_screen();
  exit(0);
}

static void sigint_handler(int dummy) {
  // to get rid of unused-parameter warning
  (void)dummy;
  quit();
}

/*
 * invoke before using other functions in this
 * library
 */
void init_clui() {
  clear_screen();
  signal(SIGINT, sigint_handler);
  enable_raw_mode();
}

/*
 * since getchar is a nonstandard function
 * this funtion was implemented.
 * Note that windows has this function
 * implicitly
 */
#if OS_UNIX
int getch() {
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

/*
 * a cross platform function to
 * suspends execution for some
 * milli seconds
 */
void delay(size_t milli_seconds) {
#if OS_UNIX
  usleep(1000 * milli_seconds);
#else
  Sleep(milli_seconds);
#endif
}

/*
 * returns the windows rows
 */
int get_window_rows() {
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
int get_window_cols() {
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
 * a type to store window size
 */
typedef struct {
  size_t row;
  size_t col;
} window_size;

/*
 * a C-Style function to get window size
 * this is a common flow in C programming
 * language. Where an int is returned as
 * a status result of function and the
 * actual return is filled in a pointer
 * given as a parameter to the function
 * As a convention zero is known to be the
 * success code of function since it can be
 * used to be placed in if without any changes
 * and if program jumps into the if it means
 * that the function have had a problem.
 * The convention is used here and in the
 * next function too :)
 *
 * The actual output value of this function
 * is stored in the size parameter you give
 * to it as a parameter. Make note that it accepts
 * a pointer to the struct defined above. Hence
 * you have to declare a window_size variable
 * and pass it's address as a pointer to this
 * function.
 */
int get_window_size(window_size *size) {
  if (!size)
    return 1;

  size->col = get_window_cols();
  size->row = get_window_rows();

  return 0;
}

typedef window_size cursor_pos;

/*
 * NOTE: NEEDS ANSI SUPPORT
 * gives the position of the cursor filled in
 * the object where the pos pointer points to.
 * zero is returned on success
 */
int get_cursor_pos(cursor_pos *pos) {
  int y = 0, x = 0;

  char buf[30] = {0};
  int i, pow;
  char ch;

  // asking for position via ANSI
  // escape sequence
  write(1, "\033[6n", 4);

  for (i = 0, ch = 0; ch != 'R'; i++)
    if (!read(0, buf + i, 1))
      return 1;

  if (i < 2)
    return 1;

  // parsing the output
  for (i -= 2, pow = 1; buf[i] != ';'; i--, pow *= 10)
    x = x + (buf[i] - '0') * pow;

  for (i--, pow = 1; buf[i] != '['; i--, pow *= 10)
    y = y + (buf[i] - '0') * pow;

  pos->row = y;
  pos->col = x;

  return 0;
}

/*
 * NOTE: NEEDS ANSI SUPPORT
 * moves cursor up n times
 */
void corsur_up(int n) { printf("\033[%dA", n); }

/*
 * NOTE: NEEDS ANSI SUPPORT
 * moves cursor down n times
 */
void cursor_down(int n) { printf("\033[%dB", n); }
/*
 * NOTE: NEEDS ANSI SUPPORT
 * moves corsur forward n time
 */
void cursor_forward(int n) { printf("\033[%dC", n); }

/*
 * NOTE: NEEDS ANSI SUPPORT
 * moves corsur backwards n time
 */
void cursor_backward(int n) { printf("\033[%dD", n); }

/*
 * NOTE: NEEDS ANSI SUPPORT
 * moves corsur to the given position
 */
void cursor_to_pos(int row, int col) { printf("\033[%d;%dH", row, col); }

void cursor_to_cursor_pos(const cursor_pos *pos) {
  cursor_to_pos(pos->row, pos->col);
}

/*
 * NOTE: NEEDS ANSI SUPPORT
 * saves cursor position for further use
 */
void save_cursor() { printf("\0337"); }

/*
 * NOTE: NEEDS ANSI SUPPORT
 * restors cursor to the last saved
 * position
 */
void restore_cursor() { printf("\0338"); }

/*
 * NOTE: NEEDS ANSI SUPPORT
 * plays beep! :)
 */
void play_beep() { printf("\07"); }

#endif
