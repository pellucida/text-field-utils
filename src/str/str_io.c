/*
//	@(#) str_getline.c
*/
# include	<stdio.h>
# include	<stdlib.h>

# include	"constant.h"
# include	"str.h"
# include	"str_io.h"


int	str_getline (FILE* input, str_t* s) {
	int	result	= EOF;
	char*	buf	= 0;
	size_t	size	= 0;
	ssize_t	nread	= 0;

	nread	= getline (&buf, &size, input);
	if (nread > 0) {
		str_setlength (s, 0);
		if (buf[nread-1] == '\n') {
			buf [nread-1]	= '\0';
			--nread;
		}
		str_appendnchar (s, buf, nread);
	
		result	= ok;
	}
	if (buf) {
		free (buf);
	}
	return	result;
}	
int	str_putstr (FILE* output, str_t* s) {
	int	nwrite	= fwrite (str_storage (s), 1, str_length(s), output);
	return	nwrite;
}
int	str_putline (FILE* output, str_t* s) {
	int	nwrite	= str_putstr (output, s);
	fputc ('\n', output);
	return	nwrite+1;
}
