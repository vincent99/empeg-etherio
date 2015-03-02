CFLAGS	:= -Wall -g -O2
LDLIBS	:= -lm
INC			:= -I../

CC    	:= /usr/local/armtools-empeg/bin/arm-empeg-linux-gcc
STRIP 	:= /usr/local/armtools-empeg/bin/arm-empeg-linux-strip
AR			:= /usr/local/armtools-empeg/bin/arm-empeg-linux-ar
RANLIB	:= /usr/local/armtools-empeg/bin/arm-empeg-linux-ranlib


#-----------------------

all: empvfd empremote

empvfd: k610a serial.c serial.h empvfd.c
	$(CC) $(CFLAGS) $(INC) -c empvfd.c k610a.o
	$(CC) $(LDLIBS) -o empvfd empvfd.o k610a.o

empremote: empremote.c serial.h serial.c
	$(CC) $(CFLAGS) $(INC) -c empremote.c
	$(CC) $(LDLIBS) -o empremote empremote.o

k610a: serial.c serial.h k610a.h k610a.c
	$(CC) $(CFLAGS) $(INC) -fPIC -c k610a.c -o k610a.o

clean:
	rm *.o empvfd empremote
