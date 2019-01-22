
# include	<unistd.h>
# include	<stdio.h>
# include	<stdlib.h>

# include	"constant.h"
# include	"str.h"
# include	"str_io.h"

void    debug_str (FILE* output, char* name, str_t* s);

static	str_t *	literal	= str_literal ("This is an example");
static	str_t*	long_literal	= str_literal_long ("This is a longer example");

int	main (int argc, char* argv[]) {
	FILE*	output	= stdout;
	FILE*	input	= stdin;
static	str_t	static_str[] = str_literal ("Static_test");

	str_putline (output, literal);
	debug_str (output, "literal", literal);
	fputc ('\n', output);

	str_putline (output, long_literal);
	debug_str (output, "long_literal", long_literal);
	fputc ('\n', output);

	str_putline (output, static_str);
	debug_str (output, "static_str", static_str);
	
}

