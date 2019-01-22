
# include	<stdio.h>
# include	"str.h"
# include	"str_printf.h"
# include	"strvec.h"

void	debug_strvec (FILE* output, char* label, strvec_t* vec) {
	int	i	= 0;
	fprintf (output, "strvec:\t%s\n", label);
	fprintf (output,"\tsize\t= %lu\n", strvec_size (vec));
	fprintf (output,"\tlast\t= %lu\n", strvec_last (vec));
	for (i=0; i < strvec_last (vec); ++i) {
		str_printf (output,"\tvec[%d]\t= '%" STR_T_FMT_CHAR "'\n", i, vec->vec[i]);
	}
}	
