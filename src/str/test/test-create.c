
# include	<stdio.h>

# include	"str.h"


void	debug_str (FILE* output, char* name, str_t* s);
	
int	main () {
	str_t*	shortstr	= 0;
	str_t*	longstr	= 0;
	str_t*	autostr	= str_auto();
	str_t*	conststr	= (str_t*)STR_VOID;
static	char	storage [sizeof(str_t)];
	str_t*	staticstr	= 0;
	
	int	result	= str_Create (&shortstr, 32);
	if (result==0) {
		debug_str (stderr, "shortstr", shortstr);
	}
	result	= str_Create (&longstr, 320);
	if (result==0) {
		debug_str (stderr, "longstr", longstr);
	}
	debug_str (stderr, "autostr", autostr);
	debug_str (stderr, "conststr", conststr);

	result	= str_Init (&staticstr, 0, storage, sizeof(storage), ST_STATIC, 0, 0, 0);
	if (result==0) {
		debug_str (stderr, "staticstr", staticstr);
	}

	str_Delete (&shortstr);
	str_Delete (&longstr);
	str_Delete (&autostr);
	str_Delete (&staticstr);
}

