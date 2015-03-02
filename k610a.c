#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#include <hijack.h>
#include <linux/empeg.h>

#include "k610a.h"

#define K610A

#define SCREEN_W             128
#define SCREEN_H             32
#define SCREEN_PPB           8                           // Pixels per byte
#define SCREEN_ROWBYTES      SCREEN_W/SCREEN_PPB         // Number of bytes per row
#define SCREEN_COLBYTES      SCREEN_H/SCREEN_PPB         // Number of bytes per col
#define SCREEN_BITMAP_SIZE   SCREEN_COLBYTES * SCREEN_W

#define ESCAPE  0x1B
#define BUSY_NORMAL 5100
#define BUSY_SHORT 1

//*********** General commands
void display_power(int on)
{
  char cmd = ( on ? 0x4F : 0x46 );
  char command[2] = { ESCAPE, cmd };

  serial_write(command,2);
  usleep(BUSY_SHORT);
}

void display_reset()
{
  char command[2] = { ESCAPE, 0x52 };
  serial_write(command,2);
  usleep(6100);
}

void display_clear()
{
  display_draw_box(MODE_CLEAR,0,0,SCREEN_W-1,SCREEN_H-1);
}

void display_auto_latch()
{
  char command[2] = { ESCAPE, 0x41 };
  serial_write(command,2);
  usleep(BUSY_SHORT);
}

void display_latch()
{
  char command[2] = { ESCAPE, 0x4D };
  serial_write(command,2);
  usleep(BUSY_SHORT);
}

// Set the brightness of the display (0-7)
void display_set_brightness(int val)
{
  char command[3] = { ESCAPE, 0x4C, val % 8 };
  serial_write(command,3);
  usleep(BUSY_SHORT);
}

//*********** Cursor

void display_home()
{
  display_set_cursor(0,0);
}

void display_set_cursor(int x, int y)
{
  char command[3] =  {  0x10, x, y  };
  serial_write(command,3);
  usleep(BUSY_SHORT);
}


//***********  Text

// 0 = mini, 1 = small, 2 = medium, 3 = large
void display_set_font(int val)
{
  char command[1] = { 0x1C + (val % 4) };
  serial_write(command,1);
  usleep(BUSY_SHORT);
}

// char 0-15
void display_custom_char(int val)
{
  char command[1] = { (val % 0x10) };
  serial_write(command,1);
  usleep(BUSY_NORMAL);
}

// which=0..15, data=8 bytes, msb at top
void display_set_custom_char(int which, char *data)
{
  char command[10] = { 0x1A, (which % 0x10), data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7] };
  serial_write(command,10);
  usleep(32100);
}


//***********  Drawing

void display_set_pixel(int x, int y, int val)
{
  display_set_cursor(x,y);
  char command[1] = { 0x11 + ( val > 0 ? 0 : 1 ) };
  serial_write(command,1);
  usleep(BUSY_NORMAL);
}

void display_draw_box(int mode, int x1, int y1, int x2, int y2)  // mode 0 = outline, 1 = fill, 2 = clear, 3 = invert, 4 = line
{
  char command[5] =  { 0x00, x1, y1, x2, y2  };

  switch ( mode )
  {
    case MODE_FILL:     command[0] = 0x13; break;
    case MODE_CLEAR:    command[0] = 0x14; break;
    case MODE_INVERT:   command[0] = 0x15; break;
    case MODE_OUTLINE:  command[0] = 0x16; break;
    case MODE_LINE:     command[0] = 0x17; break;
    default:            command[0] = 0x13;
  }

  serial_write(command,5);
  usleep(BUSY_NORMAL);
}

void display_draw_line(int x1, int y1, int x2, int y2)
{
  display_draw_box(MODE_LINE,x1,y1,x2,y2);
}


void display_draw_bitmap(char *bmp)
{
  int i;
  char command[3] =  {  0x18, 0x00, 0x80  };

  for ( i = 0 ; i < 4 ; i++ )
  {
    display_set_cursor(0,i*8);
    serial_write(command,3);
    usleep(50);
    serial_write(bmp,128);
    bmp += 128;
    usleep(BUSY_NORMAL);
  }
}

void display_reset()
{
  display_reset();
}
