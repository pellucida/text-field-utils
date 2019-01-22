
CFLAGS:=-I../str -I. $(GLIBC_PRINTF) $(_GNU_SOURCE)

STR_PRINTF_DIR	?= non_glibc_printf

str_printf.o: $(STR_PRINTF_DIR)/str_printf.c str_printf.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o a.out
