
CFLAGS=-g -I../include -I../fldvec -I../fld

ALL:	fld_parse.o

clean:
	rm -rf a.out *.o 

distclean:	clean

fld_parse.o: fld_parse.c ../fld/fld.h ../fldvec/fldvec.h fld_parse.h
