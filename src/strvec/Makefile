
CFLAGS=-g -I../include -I../str -I../strprintf -Wall

strvec.o: strvec.c strvec.h ../str/str.h ../str/str_inline.h ../str/str_io.h ../strprintf/str_printf.h ../str/str_type.h

debug_strvec.o: strvec.c strvec.h ../str/str.h

clean:
	rm -rf a.out *.o 

distclean:	clean
debug_strvec.o: debug_strvec.c ../str/str.h ../str/str_type.h \
  ../str/str_inline.h ../strprintf/str_printf.h ../strvec/strvec.h \
  ../include/constant.h
strvec.o: strvec.c ../include/constant.h ../str/str.h ../str/str_type.h \
  ../str/str_inline.h ../strvec/strvec.h
strvec.o: strvec.h ../include/constant.h ../str/str.h ../str/str_type.h \
  ../str/str_inline.h
