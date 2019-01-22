
# include	<printf.h>
# include	<stdio.h>
# include	<limits.h>


# include	"str.h"

# include	"str_printf.h"
# if	defined(GLIBC_PRINTF)
//
// Calculate the largest format string "%-*.*s"
// 2^N  <= 10^n ==> n >= log10(2^N) = N*log10(2)
//
# define	PofTen		(100000)
# define	LOG_10_2	(30103)
# define	LOG_10(n)	(((n)*LOG_10_2)+(PofTen-1))/PofTen
/*
// A better approx - not needed here
//
// # define	NUMER	1838395
// # define	DENOM	6107016
// # define	LOG_10(n)	(((n)*NUMER)+DENOM-1)/DENOM
*/

# define	digits10( type)	LOG_10(sizeof(type)*CHAR_BIT)

# define	sizeof_member( type, member)	(sizeof(((type*)(0))->member))
enum	{
	FMT_SIZE	= sizeof("%-") - 1 +
		digits10( sizeof_member( struct printf_info, width)) + sizeof(".") - 1 +
		digits10( sizeof_member( struct printf_info, prec))  + sizeof("s"),
};

static	int	str_fmt_arginfo (const struct printf_info* info, size_t n,
                      int* argtypes, int* size) {
	if (n == 1) {
		argtypes [0]    = PA_POINTER;
        }
        return 1;
}
static	int	str_fmt_print (
		FILE* stream,
		const struct printf_info* info,
		const void* const* args)
{
	str_t*	s	= **(str_t***)(args);
	char*	ss	= str_storage (s);
	int	result	= -1;
	int	width	= info->width;
	int	prec	= info->prec;
	char	fmt [FMT_SIZE];
	int	nw	= 0;
	fmt [nw++]	= '%';
	if (width>0) {
		if (info->left) {
			fmt[nw++]	= '-';
		}
		nw	+= snprintf (&fmt[nw], sizeof(fmt)-nw, "%d", width);
	}
	
	if (prec > 0 && nw < sizeof(fmt)) {
		nw	+= snprintf (&fmt[nw], sizeof(fmt)-nw, ".%d", prec);
	}
	if (nw < sizeof(fmt)-2) {
		fmt [nw++]	= 's';
		fmt [nw++]	= '\0';
		result	= fprintf (stream, fmt, ss);
	}
	return	result;
}

void __attribute__((constructor, visibility("hidden"))) str_fmt_init (void) {
	register_printf_specifier (STR_T_FMT_CHAR[0], str_fmt_print, str_fmt_arginfo);
}

/*
// Extra functions on top of printf extension support for %S
*/
int	str_vprintf (FILE* output, const char* fmt, va_list ap) {
	int	result	= vfprintf (output, fmt, ap);
	return result;
}
int	str_printf (FILE* output, const char* fmt,...) {
	va_list	ap;
	int	result	= 0;
	va_start (ap, fmt);
	result	= str_vprintf (output, fmt, ap);
	va_end (ap);
	return	result;
}
int	str_vSprintf (str_t* buf, const char* fmt, va_list ap) {
	int	result	= 0;
	char*	s	= str_storage (buf);
	strsize_t	size	= str_size (buf);
	va_list	aq;
	va_copy (aq, ap);
	result	= vsnprintf (s, size, fmt, aq);
	va_end (aq);
	if (result > size) {
		str_grow (buf,result);
		result	= str_vSprintf (buf, fmt, ap);
	}
	str_setlength (buf, result);
	return	result;
}	
int	str_Sprintf (str_t* buf, const char* fmt,...) {
	va_list	ap;
	int	result	= 0;
	char*	s	= str_storage (buf);
	strsize_t	size	= str_size (buf);

	va_start (ap, fmt);
	result	= str_vSprintf (buf, fmt, ap);
	va_end (ap);
	return	result;
}
# endif
