
CFLAGS=-g -I../include  -Wall

ALL:	debug_fld.o 

clean:
	rm -rf a.out *.o 

distclean:	clean

debug_fld.o: debug_fld.c fld.h
debug_fld.o: debug_fld.c ../fld/fld.h
fld.o: fld.h
