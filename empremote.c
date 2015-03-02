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
#include <sched.h>

#include <hijack.h>
#include <linux/empeg.h>

#include "display.h"
#include "serial.h"

#define priority 30

void quit()
{
	printf("Quitting\n");
	serial_close();
	exit(0);
}

int main (void)
{
	char buf;
	int got;
	int buttonDev;
	int valid = 0;

//	struct sched_param  sched_param;
//	int old_policy;
	int iPid;

	unsigned long buttons[2] = { 2, 0 };

	if(fork() != 0)
	{
		return 0;
	}
	else
	{
		display_setup();
		iPid = getpid();

		serial_open();
		signal(SIGINT,quit);

		buttonDev = open("/dev/display",O_WRONLY);

		printf("empRemote started at pid %d\n",iPid);		
		
	
		
		// Set the scheduler to realtime to give us some CPU time

//		old_policy = sched_getscheduler( iPid );
//		sched_getparam(iPid, &sched_param);
	//	sched_param.sched_priority = priority;
//		sched_setscheduler(iPid, SCHED_RR, &sched_param);
//		sched_setparam(iPid, &sched_param);
	
		while(1)
		{
			got = serial_read(&buf,1);
			
			if ( got > 0 )
			{
				valid = 1;
				
				switch ( buf )
				{
				case '+':	// Knob Right
					buttons[1] = IR_KNOB_RIGHT;
					break;
				case '-':	// Knob Left
					buttons[1] = IR_KNOB_LEFT;
					break;
				case 'K':	// Knob Button
					buttons[1] = IR_KNOB_PRESSED;
					break;
				case 'k':
					buttons[1] = IR_KNOB_RELEASED;
					break;
					
				case 'U':	// Up
					buttons[1] = IR_TOP_BUTTON_PRESSED;
					break;
				case 'u':
					buttons[1] = IR_TOP_BUTTON_RELEASED;
					break;
					
				case 'D':	// Down
					buttons[1] = IR_BOTTOM_BUTTON_PRESSED;
					break;
				case 'd':
					buttons[1] = IR_BOTTOM_BUTTON_RELEASED;
					break;
					
				case 'L':	// Left
					buttons[1] = IR_LEFT_BUTTON_PRESSED;
					break;
				case 'l':
					buttons[1] = IR_LEFT_BUTTON_RELEASED;
					break;
					
				case 'R':	// Right
					buttons[1] = IR_RIGHT_BUTTON_PRESSED;
					break;
				case 'r':
					buttons[1] = IR_RIGHT_BUTTON_RELEASED;
					break;

				case 'X':	// Button 11
					buttons[1] = IR_RIO_2_PRESSED;
					break;
				case 'x':
					buttons[1] = IR_RIO_2_RELEASED;
					break;

				case 'V':	// Button 12
					buttons[1] = IR_RIO_INFO_PRESSED;
					break;
				case 'v':
					buttons[1] = IR_RIO_INFO_RELEASED;
					break;
					
				case 'B':	// Button 13
					buttons[1] = IR_RIO_8_PRESSED;
					break;
				case 'b':
					buttons[1] = IR_RIO_8_RELEASED;
					break;

				case 'M':	// Button 21
					buttons[1] = IR_RIO_5_PRESSED;
					break;
				case 'm':
					buttons[1] = IR_RIO_5_RELEASED;
					break;

				case 'C':	// Button 22
					buttons[1] = IR_RIO_VISUAL_PRESSED;
					break;
				case 'c':
					buttons[1] = IR_RIO_VISUAL_RELEASED;
					break;
					
				case 'N':	// Button 23
					buttons[1] = IR_RIO_0_PRESSED;
					break;
				case 'n':
					buttons[1] = IR_RIO_0_RELEASED;
					break;

				case 'q':	// Quit
				case 'Q':
					quit();
					break;
					
				default:
					printf("Unknown key\n");
					valid = 0;
				}
				
				if ( valid )
					ioctl(buttonDev, EMPEG_HIJACK_INJECTBUTTONS, &buttons);
			}
			
			usleep(5000);
		}
			
		serial_close();
		
		return 0;
	}
}
