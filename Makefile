CC = gcc
CFlags = -Wall -std=c99 

memgrind: mymalloc.h mymalloc.c memgrind.c	
	$(CC) $(CFlags) -o $@ $^

correctnesstesting: mymalloc.h mymalloc.c correctnesstesting.c	
	$(CC) $(CFlags) -o $@ $^