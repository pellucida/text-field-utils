
#CC=clang
CC=gcc
CFLAGS+=-I../include -DGLIBC_PRINTF=1  -g  -Wall
OBJ=str.o  str_io.o

ALL:	$(OBJ)

str.o: str.c str.h str_type.h str_inline.h
	$(CC) -c $(CFLAGS) $<

str_io.o: str_io.c str.h str_type.h str_inline.h str_io.h
	$(CC) -c $(CFLAGS) $<

clean:	
	rm -f *.o a.out

distclean:	clean
str.o: str.c ../include/constant.h ../str/str.h ../str/str_type.h \
  ../str/str_inline.h
str.o: str.h ../str/str_type.h ../str/str_inline.h
str_inline.o: str_inline.h ../str/str_type.h
str_io.o: str_io.c ../include/constant.h ../str/str.h ../str/str_type.h \
  ../str/str_inline.h ../str/str_io.h
str_io.o: str_io.h ../str/str.h ../str/str_type.h ../str/str_inline.h
str_type.o: str_type.h
