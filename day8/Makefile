CC=gcc
CFLAGS =-g3 -Wall -Wextra -pedantic -Og
INCLUDES=-I../lib
LIBS=-L.

all: p1 p2 libaoc.a

p1.o: p1.c
	$(CC) $(CFLAGS) $(INCLUDES) -O -c p1.c

p2.o: p2.c
	$(CC) $(CFLAGS) $(INCLUDES) -O -c p2.c

p1: p1.o libaoc.a
	$(CC) $(CFLAGS) $(LIBS) -o p1 p1.o -laoc

p2: p2.o libaoc.a
	$(CC) $(CFLAGS) $(LIBS) -o p2 p2.o -laoc

aoc.o: 
	$(CC) $(CFLAGS) $(INCLUDES) -c ../lib/aoc.c

libaoc.a: aoc.o
	ar rcs libaoc.a aoc.o

libs: libaoc.a

clean:
	rm -Rf p1 p2 *.o *.a
