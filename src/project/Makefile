
INCLUDES=../include -I../str -I../strsplit -I../strvec -I../strprintf \
	-I../fld -I../fldvec -I../fldparse

STR_PRINTF_O	= ../strprintf/str_printf.o

CFLAGS	= -g -I$(INCLUDES) 

LDFLAGS	= -g 


OBJ	= project.o ../str/str.o ../str/str_io.o ../strprintf/str_printf.o \
	../strvec/strvec.o ../strsplit/strsplit.o ../fldparse/fld_parse.o ../fldvec/fldvec.o


project:	$(OBJ)


clean:
	rm -f *.o a.out

distclean:	clean
	rm -f project

project.o: project.c  \
 ../str/str.h ../str/str_type.h ../str/str_inline.h ../str/str_io.h \
 ../str/str.h ../strprintf/str_printf.o ../strvec/strvec.h ../strsplit/strsplit.h ../fld/fld.h \
 ../fldvec/fldvec.h ../fldparse/fld_parse.h
