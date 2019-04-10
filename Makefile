CC=gcc
CFLAGS=-I.

trosh: trosh.c
	$(CC) trosh.c -o trosh
