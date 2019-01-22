/*
//	@(#) str_io.h
*/
# if	!defined( STR_IO_H)
# define	STR_IO_H	1

# include	<stdio.h>
# include	"str.h"

int	str_getline (FILE* input, str_t* str);
int	str_putstr (FILE* output, str_t* str);
int	str_putline (FILE* output, str_t* str);
# endif
