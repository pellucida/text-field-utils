
INCLUDES=../include -I../str -I../strsplit -I../strvec -I../strprintf

STR_PRINTF_O	= ../strprintf/str_printf.o

CFLAGS	= -g -I$(INCLUDES)  -Wall

LDFLAGS	= -g 


OBJ	= uncollect.o ../str/str.o ../str/str_io.o ../strprintf/str_printf.o ../strvec/strvec.o ../strsplit/strsplit.o


HEADERS=str.h str_io.h str_printf.h

uncollect:	$(OBJ)

uncollect.o:	uncollect.c 

clean:
	rm -f *.o a.out

distclean:	clean
	rm -f uncollect
uncollect.o: uncollect.c ../include/error.h ../str/str.h \
  ../str/str_type.h ../str/str_inline.h ../str/str_io.h \
  ../strprintf/str_printf.h ../strvec/strvec.h ../include/constant.h \
  ../strsplit/strsplit.h
