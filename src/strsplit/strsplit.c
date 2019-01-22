/*
// @(#) str_split.c - split a "str_t" into fields stored in a "strvec_t"
*/
# include	<string.h>

# include	"error.h"
# include	"str.h"
# include	"strvec.h"
# include	"strsplit.h"


int	str_split (str_t* line, strvec_t* vec, char delim[]) {
	char*	l	= str_storage (line);
	size_t	len	= str_length (line);
	char	fld [len];
	str_t*	field	= 0;
	int	status	= str_Create (&field, len);
	if (status == ok) {
		size_t	i	= 0;
		size_t	j	= 0;
		strvec_clear (vec);
		for (i=0; i <= len; ++i) {	// treat the terminating \0 - l[len] as a delim
			int	ch	= l[i]; // ie strchr (delim, l[len]) != 0
			if (strchr (delim, ch)!=0) {
				str_clear (field);
				FAIL(
					str_appendnchar (field, fld, j)
				);
				FAIL(
					strvec_append (vec, field)
				);
				j	= 0;
			}
			else	{
				fld [j++]	= ch;
			}
		}
		str_Delete (&field);
	}
	return	strvec_last (vec);
}
