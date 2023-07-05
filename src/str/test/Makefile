
#CC=clang
CC=gcc

CFLAGS	= -I.. -I../../include -g -Wall
LDFLAGS	= -g
test-cstr-auto:	../str.o debug-str.o ../str_io.o test-cstr-auto.o
test-str_literal:	../str.o debug-str.o ../str_io.o test-str_literal.o
test-str_getline:	../str.o ../str_getline.o test-str_io.o
test-copy:	../str.o debug-str.o  test-copy.o
test-create:	../str.o debug-str.o  test-create.o

clean:
	rm -f *.o a.out 

test-cstr-auto.o: test-cstr-auto.c ../str.h \
 ../str_type.h ../str_inline.h ../str_io.h ../str.h
