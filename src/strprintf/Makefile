
CFLAGS:=-I../str -I. $(GLIBC_PRINTF) $(_GNU_SOURCE) -Wall

STR_PRINTF_DIR	?= non_glibc_printf

str_printf.o: $(STR_PRINTF_DIR)/str_printf.c str_printf.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o a.out

distclean:	clean
str_printf.o: str_printf.h ../str/str_type.h
