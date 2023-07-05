/*
//	@(#) uncollect.c 
*/
# include	<unistd.h>

# include	"error.h"
# include	"str.h"
# include	"str_io.h"
# include	"str_printf.h"
# include	"strvec.h"
# include	"strsplit.h"

/*
// Store global setting
*/
enum	{
	INITIAL_STRVEC_SIZE	= 64,
};
static	char	INPUT_FIELD_SEPARATORS[2]	= ":";
static	char	INPUT_SUBFIELD_SEPARATOR[2]	= ",";
static	char	OUTPUT_FIELD_SEPARATOR[2]	= ":";
enum	{
	YES	= 'Y',
	NO	= 0,
};

static	struct	{
	struct	{
		char*	name;
	} program;
	struct	{
		struct	{
			char*	field;
			char*	subfield;
		} separators;
	} input;
	struct	{
		struct	{
			char*	field;
		} separators;
	} output;
} config = {
	.input 	= { .separators	= { .field = INPUT_FIELD_SEPARATORS, .subfield = INPUT_SUBFIELD_SEPARATOR }},
	.output	= { .separators	= { .field = OUTPUT_FIELD_SEPARATOR }},
	.program= { .name	=    "uncollect"},
};

static	inline	void	setprogramname (char* name) {
	config.program.name	= name;
}
static	inline	char*	programname (void) {
	char*	result	= config.program.name;
	char*	t	= strrchr (result, '/');
	if (t) {
		result	= t+1;
	}
	return	result;
}
/* -----------------
*/
// Build a format string from the fixed fields leaving a % spec for
// collected field
//
void	prepare (str_t* fmt, strvec_t* current, int pivot) {
	int	last	= strvec_last (current);
	char*	odelim	= config.output.separators.field;
	str_t*	fldfmt	= str_auto ();
	int	j	= 0;
	for (j=0; j < last; ++j) {
		if (j+1==last) {
			odelim	= "\n";
		}
		if (j==pivot) {
			str_Sprintf (fldfmt, "%%" STR_T_FMT_CHAR "%s", odelim);
		}	
		else {
			str_t*	fldstr	= strvec_at_nocopy (current, j);
			str_Sprintf (fldfmt, "%" STR_T_FMT_CHAR "%s",  fldstr, odelim);
		}
		str_append (fmt, fldfmt);
	}
	str_Delete (&fldfmt);	// Does nothing if no heap storage
}
	
void	output_fields (FILE* output, strvec_t* current, strvec_t* summary, int pivot) {
	int	last	= strvec_last (current);
	str_t*	fmt	= str_auto ();
	prepare (fmt, current, pivot);
	if (last < pivot) {
		str_printf_str (output, fmt);
	}
	else {
		int	nsf	= strvec_last (summary);
		int	i	= 0;
		for (i=0; i < nsf; ++i) {
			str_t*	subfld	= strvec_at_nocopy (summary, i);
			str_printf_str (output, fmt, subfld);
		}	
	}
	str_Delete (&fmt);	// Does nothing if no heap storage
}

static	void	Usage() {
	fprintf (stderr, "Usage: %s [-i infile] [-o outfile] [-c summary_field] \n"
		  	 "          [-d \"input_field_sep\"] [-O \"output_field_sep\"] [-s \"summary_sep\"]\n",
		programname() );
	
	exit (EXIT_FAILURE);
}
static	void	process (FILE* input, FILE* output, int pivot) {
	str_t*		line	= 0;
	strvec_t*	current	= 0;
	strvec_t*	summary	= 0;
	int		state	= ok;
	int		finish	= EOF;

	// line 	: used for input
	// current 	: array of fields of current line
	// summary 	: array of collected values of the specified field (pivot)

	FAIL(
		str_Create (&line, 0)
	);
	FAIL(
		strvec_Create (&current, INITIAL_STRVEC_SIZE)
	);
	FAIL(
		strvec_Create (&summary, INITIAL_STRVEC_SIZE)
	);
	
	while (state != finish) {
		if (str_getline (input, line)!=EOF) {
			// Ignore empty lines
			if (str_length (line) > 0) {
				str_split (line, current, config.input.separators.field);
				str_split (strvec_at (current, pivot), summary, config.input.separators.subfield);
				output_fields (output, current, summary, pivot);
			}
		}
		else	{
			state	= finish;
		}
	}
}
int	main (int argc, char* argv[]) {

	FILE*	input	= stdin;
	FILE*	output	= stdout;
	int	pivot	= 0;
	int	c_flag	= 0;
	int	i_flag	= 0;
	int	o_flag	= 0;
	int	d_flag	= 0;
	int	O_flag	= 0;
	int	s_flag	= 0;
	int	opt	= EOF;
	opterr	= 0;

	setprogramname (argv[0]);
	while ((opt = getopt (argc, argv, "hd:i:o:c:O:s:"))!=EOF) {
	switch (opt) {
		case	'i':
			if (i_flag++) {
				Usage ();
			}
			input	= fopen (optarg, "r");
			if (!input) {
				fprintf (stderr, "ERROR: %s - Cannot open file '%s'\n", programname(), optarg);
				exit (EXIT_FAILURE);
			}
		break;
		case	'o':
			if (o_flag++) {
				Usage ();
			}
			output	= fopen (optarg, "w");
			if (!output) {
				fprintf (stderr, "ERROR: %s - Cannot open file '%s'\n", programname(), optarg);
				exit (EXIT_FAILURE);
			}
		break;
		case	'c':
			if (c_flag++) {
				Usage ();
			}
			pivot	= strtoul (optarg, 0, 10); 
			if (pivot > 0) {
				pivot	= pivot - 1;
			}
		break;
		case	'd':
			if (d_flag++) {
				Usage ();
			}
			config.input.separators.field	= optarg;
		break;
		case	'O':
			if (O_flag++) {
				Usage ();
			}
			config.output.separators.field	= optarg;
		break;
		case	's':
			if (s_flag++) {
				Usage ();
			}
			config.input.separators.subfield = optarg;
		break;
		case 'h':
		case '?':
		default:
			Usage ();
		break;
		}
	}
	process (input, output, pivot);
}

