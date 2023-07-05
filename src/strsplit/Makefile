
CFLAGS=-g -I../include -I../str -I../strvec -I../strprintf -Wall
LDFLAGS=-g
strsplit.o: strsplit.c strsplit.h ../strvec/strvec.h ../str/str.h ../str/str_inline.h ../str/str_io.h ../strprintf/str_printf.h ../str/str_type.h

clean:
	rm -f a.out *.o 

distclean:	clean
strsplit.o: strsplit.c ../include/error.h ../str/str.h ../str/str_type.h \
  ../str/str_inline.h ../strvec/strvec.h ../include/constant.h \
  strsplit.h
strsplit.o: strsplit.h ../str/str.h ../str/str_type.h ../str/str_inline.h \
  ../strvec/strvec.h ../include/constant.h
