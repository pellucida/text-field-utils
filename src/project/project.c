/*
// @(#) project.c 
*/

# include	<unistd.h>

# include	"constant.h"
# include	"error.h"

# include	"str.h"
# include	"str_io.h"
# include	"str_printf.h"
# include	"strvec.h"
# include	"strsplit.h"
# include	"fld.h"
# include	"fldvec.h"
# include	"fld_parse.h"

static  char    INPUT_FIELD_SEPARATORS[2]       = ":";
static  char    OUTPUT_FIELD_SEPARATOR[2]       = ":";

static  struct  {
        struct  {
                char*   name;
        } program;
        struct  {
                struct  {
                        char*   field;
                } separators;
        } input;
        struct  {
                struct  {
                        char*   field;
                } separators;
	} output;
}	config	= {
        .input  = { .separators = { .field = INPUT_FIELD_SEPARATORS }},
        .output = { .separators = { .field = OUTPUT_FIELD_SEPARATOR }},
	.program = { .name = "project" },
};
static  inline  void    setprogramname (char* name) {
        config.program.name     = name;
}
static  inline  char*   programname (void) {
        char*   result  = config.program.name;
        char*   t       = strrchr (result, '/');
        if (t) {
                result  = t+1;
        }
        return  result;
}
/* -----------------
*/
static  void    Usage() {
        fprintf (stderr, "Usage: %s -f field_list [-o outfile] "
			 "[-d \"input_field_separators\"]\n"
			 "          [-O \"output_field_separator\"] infiles...\n",
                programname() );

        exit (EXIT_FAILURE);
}


void process_line (FILE* output, fldvec_t* fldlist,  strvec_t* fldsread) {
        int	nranges 	= fldvec_last (fldlist);
        fldnr_t	nfldsread	= strvec_last (fldsread);  // Number of fields in line
        int	i       = 0;
	fldnr_t	total	= 0;	// printed.
	char*	odelim	= config.output.separators.field;
        for (i=0; i < nranges; ++i) {
                int     j       = 0;
                fld_t   f       = fldvec_at (fldlist, i);

		// Calculate the actual fields availble for printing
		interval_t	interval;
		int	nsteps	= fld_interval (&interval, f, nfldsread);

		fldnr_t	x	= 0;	// !!field number rebias 1..n to 0..n-1!!
		for (j=0, x = interval.start-1; j < nsteps; ++j, x += interval.step) {

                	if (total == 0 ) { // first field
				str_printf (output, "%" STR_T_FMT_CHAR, strvec_at (fldsread, x));
			}
			else	{
				str_printf (output, "%s%" STR_T_FMT_CHAR, odelim, strvec_at (fldsread, x));
			}
			++total;
                }
        }
	if (total) {
		fputc ('\n', output);
	}
}
void     process_file (FILE* input, FILE* output, fldvec_t* fldlist) {
        str_t*    	line   		= 0;
	strvec_t*	fldsread	= 0;
        FAIL( 
		str_Create (&line, 0)
	);
	FAIL(
        	strvec_Create (&fldsread, 0)
	);

        while (str_getline (input, line) == ok) {
		str_split (line, fldsread, config.input.separators.field);
                process_line (output, fldlist, fldsread);
        }
}
void	process_field_list (fldvec_t* fldlist, char fieldspec[]) {
	fld_parse (fldlist, fieldspec);
}

static	FILE*	mustopen (char* path, char* rw) {
	FILE*	result	= fopen (path, rw);
	if (!result) {
		fprintf (stderr, "ERROR: %s - Cannot open file '%s'\n", programname(), path);
		exit (EXIT_FAILURE);
	}
	return	result;
}
int	main (int argc, char* argv[]) {
        FILE*   output  = stdout;
        int     d_flag  = 0;
        int     O_flag  = 0;
        int     f_flag  = 0;
	char*	fieldspec	= 0;
        int     o_flag  = 0;

	int	opt	= EOF;
	opterr	= 0;

	setprogramname (argv[0]);
        while ((opt = getopt (argc, argv, "f:o:d:O:"))!=EOF) {
        switch (opt) {
		case	'f':
			if (f_flag++) {
				Usage ();
			}
			fieldspec	= optarg;
		break;
		case	'o':
                        if (o_flag++) {
                                Usage ();
                        }
                        output  = mustopen (optarg, "w");
                break;
		case	'd':
                        if (d_flag++) {
                                Usage ();
                        }
                        config.input.separators.field   = optarg;
                break;
		case    'O':
                        if (O_flag++) {
                                Usage ();
                        }
                        config.output.separators.field  = optarg;
                break;
		case	'?':
			Usage ();
		break;
		}
	}
	if (f_flag) {
		fldvec_t*	fldlist	= 0;
		int	i	= optind;
		FILE*   input   = stdin;
		FAIL(
			fldvec_Create (&fldlist, 0)
		);
		process_field_list (fldlist, fieldspec);
		if (optind < argc) for (i=optind; i < argc; ++i) {
			input	= mustopen (argv[i], "r");
			process_file (input, output, fldlist);
			fclose (input);
		}
		else	{
			process_file (input, output, fldlist);
		}
	}
	else	{
		Usage ();
	}
}
