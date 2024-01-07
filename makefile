CC = gcc
CFLAGS = -W -Wall -Werror `pkg-config --cflags glib-2.0 gtk+-3.0`

unimage : zpixel.o arbrePixel.o pile/stack2_x86_64.o interface.o main.o
	$(CC) zpixel.o pile/stack2_x86_64.o arbrePixel.o interface.o  main.o -o unimage -lm `pkg-config --libs glib-2.0 gtk+-3.0`

zpixel.o : zpixel.c zpixel.h
	$(CC) -c $(CFLAGS) zpixel.c

main.o : main.c pile/stack2.h
	$(CC) -c $(CFLAGS) main.c

arbrePixel.o : arbrePixel.c arbrePixel.h pile/stack2.h
	$(CC) -c $(CFLAGS) arbrePixel.c

interface.o : interface.c interface.h
	$(CC) -c $(CFLAGS) interface.c

clear: 
	rm -f zpixel.o main.o arbrePixel.o interface.o unimage