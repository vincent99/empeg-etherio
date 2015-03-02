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

#include "serial.h"
#include "k610a.h"

#define GRAY_THRESHOLD    2

#define MIN_LINE_LEN_V    2
#define MIN_LINE_LEN_H    0

#define RAW_W             128
#define RAW_H             32
#define RAW_PPB           2                     // Pixels per byte
#define RAW_ROWBYTES      RAW_W/RAW_PPB         // Number of bytes per row
#define RAW_BITMAP_SIZE   RAW_ROWBYTES * RAW_H

#define getRawPx(bmp,x,y)  ( (( *( bmp + RAW_ROWBYTES*y + x/RAW_PPB ) & ( x % 2 == 0 ? 0x0F : 0xF0 ) ) ) >> ( x % 2 == 0 ? 0 : 4 ) )

#define EMPEG_W           128
#define EMPEG_H           32
#define EMPEG_PPB         8                     // Pixels per byte
#define EMPEG_ROWS        EMPEG_W               // Number of rows
#define EMPEG_COLS        EMPEG_H/EMPEG_PPB     // Number of columns
#define EMPEG_BITMAP_SIZE EMPEG_ROWS * EMPEG_COLS

#define getEmpegPx(bmp,x,y)  (( *( bmp + EMPEG_ROWBYTES*y + x/EMPEG_PPB ) & ( 1 << ( x % EMPEG_PPB ) )) > 0 ? 1 : 0 )

static int quitTime = 0;

static char frameBmp[ EMPEG_BITMAP_SIZE ];
static char *frame  = frameBmp;

static char rawBmp[ RAW_BITMAP_SIZE ];
static char *raw     = rawBmp;

void rawToDisplay(char *raw, char *dipslay)
{
  int x, y, i, j;
  char b;

  for ( x = 0 ; x < EMPEG_W ; x++ )
  {
    for ( i = 0 ; i < EMPEG_H/8; i++ )
    {
      b = 0;
      for ( j = 0 ; j < 8 ; j++ )
      {
        y = 8*i+j;
        if ( getRawPx(raw,x,y) > GRAY_THRESHOLD )
        {
          b |= 1 << j;
        }
      }

      *(dipslay + EMPEG_COLS*x + i) = b;
    }
  }
}

void quit()
{
  printf("Quitting\n");
  quitTime = 1;
}

int main (void)
{
  int     i = 0;
  time_t  last_time, cur_time;
  int     frame_count = 0;
  int     screen;

  printf("Starting up...\n");

  if(fork() != 0)
  {
    return 0;
  }

  for ( i = 0 ; i < EMPEG_BITMAP_SIZE ; i++ )
  {
    frameBmp[i] = 0;
  }

  display_setup();
  serial_open();
  signal(SIGINT,quit);

  printf("Ready\n");

  screen = open("/proc/empeg_screen.raw",O_RDONLY);

  while( ! quitTime )
  {
    frame_count++;
    lseek(screen, 0, SEEK_SET);
    read(screen, raw, RAW_BITMAP_SIZE);
    rawToDisplay(raw, frame);

    renderBitmap();

    last_time = cur_time;
    time(&cur_time);
    if ( last_time != cur_time )
    {
      printf("%d fps\n", frame_count);
      frame_count = 0;
    }
  }

  close(screenDev);
  serial_close();
  return(0);
}

void renderBitmap(void)
{
  display_draw_bitmap(frame);
}
