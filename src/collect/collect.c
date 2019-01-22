/*
//	@(#) collect.c - collect the values into a list from a specified field where the other fields are otherwise identical
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
static	char	OUTPUT_FIELD_SEPARATOR[2]	= ":";
static	char	OUTPUT_SUBFIELD_SEPARATOR[2]	= ",";
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
		} separators;
	} input;
	struct	{
		struct	{
			char*	field;
			char*	subfield;
		} separators;
		struct	{
			int	unique;
			int	sorted;
			int	empty;
		} options;
	} output;
} config = {
	.input 	= { .separators	= { .field = INPUT_FIELD_SEPARATORS }},
	.output	= { .separators	= { .field = OUTPUT_FIELD_SEPARATOR, .subfield	= OUTPUT_SUBFIELD_SEPARATOR },
		    .options	= { .unique = YES,  .sorted = YES, .empty = NO,  }},
	.program= { .name	=    "collect"},
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
int	summary_append (strvec_t* summary, strvec_t* current, int pivot) {
	int	result	= ok;
	str_t*	pelt	= strvec_at_nocopy (current, pivot);
	if (config.output.options.unique == NO || strvec_find (summary, pelt) < 0) {
		if (str_length (pelt) > 0 || config.output.options.empty == YES)
			result	= strvec_append (summary, pelt);
	}
	return	result;
}
int	summary_initialize (strvec_t* summary, strvec_t* current, int pivot) {
	strvec_clear (summary);
	return	summary_append (summary, current, pivot);
}
	
void	output_fields (FILE* output, strvec_t* current, strvec_t* summary, int pivot) {
	int	i	= 0;
	int	last	= strvec_last (current);
	char*	odelim	= config.output.separators.field;

	for (i=0; i < last; ++i) {
		if (i==last-1) {
			odelim	= "\n";
		}
		if (i==pivot) {	
			char*	sdelim	= config.output.separators.subfield;
			int	j	= 0;
			int	slast	= strvec_last (summary);
			// sort summary
			if (config.output.options.sorted != NO) {
				strvec_sort (summary);
			}
			for (j=0; j < slast; ++j) {
				if (j==slast-1) {
					sdelim	= odelim;
				}
				str_printf (output, "%" STR_T_FMT_CHAR "%s", strvec_at_nocopy (summary, j), sdelim);
			}
		}
		else	{
			str_printf (output, "%" STR_T_FMT_CHAR "%s", strvec_at_nocopy (current, i), odelim);
		}
	}
}
int	strvec_compare_except  (strvec_t* current, strvec_t* next, int except) {
	int	result	= 0;
	int	i	= 0;
	int	j	= strvec_last (current);
	int	l	= strvec_last (next);
	if (j != l) {
		result	= j - l;
	}
	else	while (i!=j) {
		if (i!=except) {
			str_t*	a	= strvec_at_nocopy (current, i);
			str_t*	b	= strvec_at_nocopy (next, i);

			result	= str_compare (a, b);

		}
		if (result != 0) {
			j	= i;
		}
		else	{
				++i;
		}
	}
	return	result;	
}

# define	SWAP( x,y)	do { typeof(x)	tmp	= x; x	= y; y	= tmp; } while (0)

static	void	Usage() {
	fprintf (stderr, "Usage: %s [-i infile] [-o outfile] [-c summary_field] \n"
		  	 "          [-d \"input_field_sep\"] [-O \"output_field_sep\"] [-S \"summary_sep\"]\n"
		  	 "          [-D](duplicates) [-U](unsorted) [-E] (empty fields ok)\n",
		programname() );
	
	exit (EXIT_FAILURE);
}
static	void	process (FILE* input, FILE* output, int pivot) {
	str_t*		line	= 0;
	strvec_t*	current	= 0;
	strvec_t*	next	= 0;
	strvec_t*	summary	= 0;
	int		state	= ok;

	// line 	: used for input
	// current 	: array of fields of current line
	// next	 	: array of fields of next line
	// summary 	: array of collected values of the specified field (pivot)

	FAIL(
		str_Create (&line, 0)
	);
	FAIL(
		strvec_Create (&current, INITIAL_STRVEC_SIZE)
	);
	FAIL(
		strvec_Create (&next,    INITIAL_STRVEC_SIZE)
	);
	FAIL(
		strvec_Create (&summary, INITIAL_STRVEC_SIZE)
	);
	

	while (state != EOF) {
		int	finish		= EOF;
		int	nflds_cur	= 0;
		while (state != finish) {
			if (str_getline (input, line)!=EOF) {
				nflds_cur	= str_split (line, current, config.input.separators.field);
				if (nflds_cur > pivot) {
					FAIL(
						summary_initialize (summary, current, pivot)
					);
					finish	= state;
				}
				else	{	// no pivot field so just output
					output_fields (output, current, summary, pivot);
				}
			}
			else	{
				state	= finish;
			}
		}
		// state == ok  ==> we have a line with pivot field 
		// state == EOF ==> finished and fall through next loop
		finish	= EOF;
		while (state != finish) {
			if (str_getline(input, line)!=EOF) {
				int	nflds_next = str_split (line, next, config.input.separators.field);
				if (nflds_next > pivot) {
					// if current and next are identical(omitting the pivot elt)
					// add the pivot elt to summary
					if (nflds_next==nflds_cur &&
						strvec_compare_except (current, next, pivot) == 0) {
						FAIL( 
							summary_append (summary, next, pivot)
						);
					}
					else	{
					// output current and make next current
						output_fields (output, current, summary, pivot);
						SWAP (current, next);
						FAIL(
							summary_initialize (summary, current, pivot)
						);
					}
				}
				else	{
					// no pivot field in next just output both and start again
					// a the first loop
					output_fields (output, current, summary, pivot);
					output_fields (output, next, summary, pivot);
					finish	= state;
				}
			}
			else	{
				output_fields (output, current, summary, pivot);
				state	= finish;
			}
		}
	}
}
int	main (int argc, char* argv[]) {

	FILE*	input	= stdin;
	FILE*	output	= stdout;
	char*	delims	= ":";
	int	pivot	= 0;
	int	c_flag	= 0;
	int	i_flag	= 0;
	int	o_flag	= 0;
	int	d_flag	= 0;
	int	O_flag	= 0;
	int	S_flag	= 0;
	int	D_flag	= 0;
	int	U_flag	= 0;
	int	E_flag	= 0;
	int	opt	= EOF;
	opterr	= 0;

	setprogramname (argv[0]);
	while ((opt = getopt (argc, argv, "hd:i:o:c:O:S:DUE"))!=EOF) {
	switch (opt) {
		case	'D':
			if (D_flag++) {
				Usage ();
			}
			config.output.options.unique	= NO;
		break;
		case	'U':
			if (U_flag++) {
				Usage ();
			}
			config.output.options.sorted	= NO;
		break;
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
		case	'S':
			if (S_flag++) {
				Usage ();
			}
			config.output.separators.subfield	= optarg;
		break;
		case	'E':
			if (E_flag++) {
				Usage ();
			}
			config.output.options.empty	= YES;
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

