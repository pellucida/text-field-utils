
CFLAGS=-g -I../include -I../fldvec -I../fld -Wall

ALL:	fld_parse.o

clean:
	rm -rf a.out *.o 

distclean:	clean

fld_parse.o: fld_parse.c ../fld/fld.h ../fldvec/fldvec.h fld_parse.h
fld_parse.o: fld_parse.c ../include/constant.h ../include/error.h \
  ../fld/fld.h ../fldvec/fldvec.h ../fldparse/fld_parse.h
fld_parse.o: fld_parse.h ../fld/fld.h ../fldvec/fldvec.h \
  ../include/constant.h
