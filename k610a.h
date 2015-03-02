#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <errno.h>

#define OFF           0
#define ON            1

#define LATCH_AUTO    0
#define LATCH_MANUAL  1

#define FONT_MINI     0
#define FONT_SMALL    1
#define FONT_MEDIUM   2
#define FONT_LARGE    3

#define MODE_FILL     0
#define MODE_CLEAR    1
#define MODE_INVERT   2
#define MODE_OUTLINE  3
#define MODE_LINE     4

void display_setup();

// General
void display_power(int on);          // 0 = power off, 1 = power on
void display_set_brightness(int val);    // 0 = off, 7 = maximum
void display_reset();
void display_clear();
void display_auto_latch();            /// Update for every draw command
void display_latch();              // Update display on command after a latch command

// Cursor
void display_home();
void display_cr();
void display_lf();
void display_backspace();
void display_tab();
void display_set_cursor(int x, int y);      // x = 0..127, y=0..31

// Text
void display_set_font(int val);          // 0 = mini, 1 = small, 2 = medium, 3 = large
void display_custom_char(int val);          // char 0-15
void display_set_custom_har(int which, char *data);  // which=0..15, data=8 bytes, msb at top

// Drawing
void display_set_pixel(int x, int y, int val);
void display_draw_line(int x1, int y1, int x2, int y2);
void display_draw_box(int mode, int x1, int y1, int x2, int y2);  // mode 0 = outline, 1 = fill, 2 = clear, 3 = invert, 4 = line

void display_draw_bitmap(char *bmp);
