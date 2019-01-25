
INCLUDES=../include -I../str -I../strsplit -I../strvec -I../strprintf

STR_PRINTF_O	= ../strprintf/str_printf.o

CFLAGS	= -g -I$(INCLUDES) 

LDFLAGS	= -g 


OBJ	= uncollect.o ../str/str.o ../str/str_io.o ../strprintf/str_printf.o ../strvec/strvec.o ../strsplit/strsplit.o


HEADERS=str.h str_io.h str_printf.h

uncollect:	$(OBJ)

uncollect.o:	uncollect.c 

clean:
	rm -f *.o a.out

distclean:	clean
	rm -f uncollect
