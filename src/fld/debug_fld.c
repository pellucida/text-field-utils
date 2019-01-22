
# include	<stdio.h>

# include	"fld.h"

# if	defined(DEBUG)
void	debug_print_point (char* id, point_t p) {
	fprintf (stderr, "%s: (%c, %ld)\n", id, p.origin, p.offset);
}
void	debug_print_fld (char* id, fld_t f) {
	debug_print_point ("\t", f.start);
	debug_print_point ("\t", f.end);
}
# endif
