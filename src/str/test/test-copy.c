
# include	<stdio.h>

# include	"str.h"


# define	S10	"0123456789"
# define	S20	S10 S10
# define	S30	S20 S10
# define	S40	S20 S20
# define	S60	S30 S30
# define	S120	S60 S60

void	debug_str (FILE* output, char* name, str_t* s);
	
int	main () {
	str_t*	s1	= 0;
	str_t*	s2	= 0;
	int	result	= str_CreateCstr (&s1, S60);
	if (result==0) {
		debug_str (stderr, "s1", s1);
	}
	result	= str_Copy (&s2, s1);
	if (result==0) {
		debug_str (stderr, "s2", s2);
	}
	result	= str_append (s2, s1);
	if (result==0) {
		debug_str (stderr, "s2", s2);
	}
	result	= str_copy (s1, s2);
	result	= str_append (s2, s1);
	if (result==0) {
		debug_str (stderr, "s2", s2);
	}
	result	= str_append (s1, s1);
	if (result==0) {
		debug_str (stderr, "s1", s1);
	}
}

