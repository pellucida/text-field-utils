/*
//	@(#) error.h
//
// Primative exception/error handling.
// Fail and exit if function doesn't return 'ok'
*/
# if	!defined(ERROR_H)
# define	ERROR_H	1

# include	<stdio.h>
# include	<stdlib.h>

# if	!defined(NO_FAIL_MACRO)

static  void inline     _FAIL (int retval, int sense, char* file, int line, char* msg) {
        if ((sense == 0 && retval != 0) || (sense == 1 && retval == 0)) {
                fprintf (stderr, "FATAL: %s(%d) %s\n", file, line, msg);
                exit (EXIT_FAILURE);
        }
}
/* Need macro to expand FILE and LINE macros in place and stringize 
// offending function.
*/
#	define        FAIL(v)  _FAIL((v), 0, __FILE__, __LINE__, #v )
#	define        FALSE(v) _FAIL((v), 1, __FILE__, __LINE__, #v )

# else
#	define     FAIL(v) (v)
#	define     FALSE(v) (v)
# endif

# endif
