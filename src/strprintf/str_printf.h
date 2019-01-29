/*
//	@(#) str_printf.h - glibc user supplied extensions to printf
*/

# if	!defined( STR_PRINTF_H)
# define	STR_PRINTF_H

# include	<stdio.h>

# include	"str_type.h"

# define        STR_T_FMT_CHAR  "T"
# define        STR_T_FMT  "%" STR_T_FMT_CHAR

// Functions using glibc printf extension interface
# if	defined(_GNU_SOURCE)

int     str_vprintf (FILE* __restrict output, const char* __restrict fmt, _G_va_list ap)
        __attribute__ ((__format__ (__printf__, 2, 0)));

int     str_printf (FILE* output, const char*__restrict  fmt,...)
        __attribute__ ((__format__ (__printf__, 2, 3)));

int     str_vSprintf (str_t* __restrict buf, const char* __restrict  fmt, _G_va_list ap)
        __attribute__ ((__format__ (__printf__, 2, 0)));

int     str_Sprintf (str_t* __restrict buf, const char* __restrict fmt,...)
        __attribute__ ((__format__ (__printf__, 2, 3)));
# else
int     str_vprintf (FILE* output, const char* fmt, va_list ap);
int     str_printf (FILE* output, const char* fmt,...);
int     str_vSprintf (str_t* buf, const char* fmt, va_list ap);
int     str_Sprintf (str_t*  buf, const char* fmt,...);
int     str_printf_str (FILE* output, str_t* fmt,...);
# endif
# endif
