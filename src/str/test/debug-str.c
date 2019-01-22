
# include	<stdio.h>

# include	"str.h"


static	char*	storetype[]	= {
	[ST_HEAP]	= "heap",
	[ST_AUTO]	= "alloca",
	[ST_STATIC]	= "static",
	[ST_CONST]	= "const"

};
static	char*	strtype[]	= {
	[T_SHORT] = "short",
	[T_LONG] = "long"
};

void	debug_str (FILE* output, char* name, str_t* s) {

	if (s) {
		int	short_long	= ((s)->n.flags & MASK_TYPE);	
		int	base_store	=
				(((s)->n.flags & MASK_BASE) >> SHIFT_BASE);

		int	ext_store	=
				(((s)->n.flags & MASK_EXT) >> SHIFT_EXT);

		fprintf (output, "%s:\n\tlength\t= %d\n", name, s->n.length);
		fprintf (output, "\tflags\t= %x\t", s->n.flags);
		fprintf (output, "%d|%d|%d", ext_store, base_store, short_long);
		fprintf (output, "\t%s|%s|%s\n", 
			storetype[ext_store],
			storetype[base_store],
			strtype [short_long]);

		if (short_long == T_SHORT) {
			fprintf (output, "\tstr[]\t= \"");
			fwrite (s->n.str, s->n.length, 1, output);
			fprintf (output, "\"\n");
		} 
		else	{
			fprintf (output, "\tsize\t= %d\n", s->x.size);
			fprintf (output, "\tstr[]\t= \"");
			fwrite (s->x.str, s->x.length, 1, output);
			fprintf (output, "\"\n");
		}
	}
}
