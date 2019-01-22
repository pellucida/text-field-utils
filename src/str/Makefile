
#CC=clang
CC=gcc
CFLAGS+=-I../include -DGLIBC_PRINTF=1  -g 
OBJ=str.o  str_io.o

ALL:	$(OBJ)

str.o:	str.c str.h
	$(CC) -c $(CFLAGS) $<

str_io.o:	str_io.c str_io.h
	$(CC) -c $(CFLAGS) $<

clean:	
	rm -f *.o a.out
