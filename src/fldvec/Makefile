
CFLAGS=-g -I../include -I../fld -Wall

ALL:	fldvec.o 

clean:
	rm -rf a.out *.o 

distclean:	clean

fldvec.o: fldvec.c ../fld/fld.h fldvec.h
fldvec.o: fldvec.c ../include/constant.h ../fld/fld.h ../fldvec/fldvec.h
fldvec.o: fldvec.h ../include/constant.h ../fld/fld.h
