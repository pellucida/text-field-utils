
#CC=clang
CC=gcc
CFLAGS+=-I../include -DGLIBC_PRINTF=1  -g 
OBJ=str.o  str_io.o

ALL:	$(OBJ)

str.o: str.c str.h str_type.h str_inline.h
	$(CC) -c $(CFLAGS) $<

str_io.o: str_io.c str.h str_type.h str_inline.h str_io.h
	$(CC) -c $(CFLAGS) $<

clean:	
	rm -f *.o a.out

distclean:	clean
