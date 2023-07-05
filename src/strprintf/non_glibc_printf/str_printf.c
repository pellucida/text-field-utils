/*
//	@(#) str_printf_non_glibc.c
*/

# include	<stdarg.h>
# include	<stdio.h>
# include	<string.h>
# include	<alloca.h>
# include	"str.h"
# include	"str_printf.h"

/*
//	Use printf to print usual formats and
//	locate and rewrite the str_t* format %STR_T_FMT_CHAR (%T) to %s
//	and use printf.
//
//	GLIBC: can install own extension to the standard printf
*/


# define	PRINTF_FMT_CHAR	"scdiouxXeEfFgGaAp" "n" "m"

static	const	char	fmt_letters []	= "%" PRINTF_FMT_CHAR STR_T_FMT_CHAR "\0";

int	str_vSprintf (str_t* __restrict ss, const char* __restrict fmt, va_list ap) {
	size_t	count	= 0;
	char* 	t	= (char*) fmt;

	char	fmt_copy [strlen(t)+1]; // copy parts of fmt into here 
	str_clear (ss);
	while (*t) {
		if (*t=='%') {
			char*	q	= strpbrk (t+1, fmt_letters);
			if (q) {
				if (*q == '%') {
					str_appendchar (ss, '%');
					++count;
				}
				else if (*q == STR_T_FMT_CHAR[0]) {	/* Our extension */
					int	width_precision [2];	/* for %*[.*]S */
					int	nwidth_prec	 = 0;
					int	i	= 0;
					char*	p	= t;
					while (p < q) {
						if (*p == '*'){
							width_precision [nwidth_prec++]	= va_arg(ap, int);
						}
						fmt_copy [i++]	= *p++;
					}
					fmt_copy [i++]	= 's'; /* change STR_T_FMT_CHAR:'T' to 's' */
					fmt_copy [i++]	= '\0';

					str_t*	s = va_arg (ap, str_t*);
					char*	cstr	= str_storage (s);
					size_t	nw	= 0;
					switch (nwidth_prec) { //Dry run
					case	0:
						nw	= snprintf (0, 0, fmt_copy, cstr);
					break;
					case	1:
						nw	= snprintf (0, 0, fmt_copy, width_precision [0], cstr);
					break;
					case	 2:
						nw	= snprintf (0,0, fmt_copy, width_precision [0], width_precision [1], cstr);
					break;
					}
					{	//For real
					char	buf [nw+1];
					switch (nwidth_prec) {
					case	0:
						nw	= snprintf (buf, sizeof(buf), fmt_copy, cstr);
					break;
					case	1:
						nw	= snprintf (buf, sizeof(buf), fmt_copy, width_precision [0], cstr);
					break;
					case	 2:
						nw	= snprintf (buf, sizeof(buf), fmt_copy, width_precision [0], width_precision [1], cstr);
					break;
					}
					count	+= nw;
					str_appendnchar (ss, buf, nw);
					}
				}
				else	{
					va_list	aq;
					size_t	nw	= 0;
					memcpy (fmt_copy, t, q - t+1);
					fmt_copy [q-t+1]	= '\0';

					va_copy (aq, ap);
					nw = vsnprintf (0, 0,  fmt_copy, aq);
					va_end (aq);

					{
					char	buf [nw+1];
					nw = vsnprintf (buf, sizeof(buf),  fmt_copy, ap);
					count	+= nw;
					str_appendnchar (ss, buf, nw);
					}
				}
				t	= q+1;
			}
		}
		else	{
			str_appendchar (ss, *t);
			++count;
			++t;
		}
	}
	return	count;
}
	

int	str_Sprintf (str_t* __restrict ss, const char* __restrict fmt, ...) {
	va_list	ap;
	va_start (ap, fmt);

	int	count	= str_vSprintf  (ss, fmt, ap);
	va_end (ap);
	return	count;
}

int	str_printf (FILE* output, const char* __restrict fmt, ...) {
	va_list	ap;
	va_start (ap, fmt);
	int	count	= 0;
	str_t*	buffer	= str_auto();
	count	= str_vSprintf (buffer, fmt, ap);
	fwrite (str_storage(buffer), 1, count, output);
	str_Delete (&buffer);
	
	va_end (ap);
	return count;
}
int	str_vprintf (FILE* output, const char* __restrict fmt, va_list ap) {
	int	count	= 0;
	str_t*	buffer	= str_auto();
	count	= str_vSprintf (buffer, fmt, ap);
	fwrite (str_storage(buffer), 1, count, output);
	str_Delete (&buffer);
	return count;
}
int	str_printf_str (FILE* output, str_t* fmt, ...) {
	va_list ap;
        va_start (ap, fmt);
        int     count   = 0;
        str_t*  buffer  = str_auto();
        count   = str_vSprintf (buffer, str_storage (fmt), ap);
        fwrite (str_storage(buffer), 1, count, output);
        str_Delete (&buffer);
	return	count;
}
