CC = gcc
CFLAGS = -Wall -g -O0
CFLAGS = -I.

#targets
all: syscpld

syscpld: syscpld.o
	$(CC) -o syscpld syscpld.o

syscpld.o: syscpld.c
	$(CC) $(CFLAGS) -c syscpld.c

clean:
	rm -rf *.o syscpld
