
# include	<unistd.h>
# include	<stdio.h>
# include	<stdlib.h>

# include	"constant.h"
# include	"str.h"
# include	"str_io.h"



static	void	filecopy (FILE* input, FILE* output, str_t* line) {
	while (str_getline (input, line) == ok) {
		str_putline (output, line);
	}
}
int	main (int argc, char* argv[]) {
	FILE*	output	= stdout;
	FILE*	input	= stdin;
	int	opt	= EOF;
	int	A_flag	= 0;
	str_t*	line	= 0;

	while ((opt = getopt (argc, argv, "Ai:o:")) != EOF) {
		switch (opt) {
		case	'A':
			A_flag	= 1;
		break;
		case	'i':
			input	= fopen (optarg, "r");
		break;
		case	'o':
			output	= fopen (optarg, "w");
		break;
		}
	}
	if (A_flag) {
		line	= str_auto();
		filecopy (input, output, line);
		str_Delete (&line);
	}
	else	{
		str_Create (&line, 0);
		filecopy (input, output, line);
		str_Delete (&line);
	}
}

