# include	<stdio.h>
# include	<stdlib.h>
# include	<ctype.h>

# include	"constant.h"
# include	"error.h"

# include	"fld.h"
# include	"fldvec.h"
# include	"fld_parse.h"

static	void	fld_listerr_print (char s[], size_t position) {
	fprintf (stderr, "ERROR: field list syntax:\n");
	fprintf (stderr, "       %s\n", s);
	fprintf (stderr, "       %*s%c\n", (int)position, "", '^');
}
// look for an integer (*np) in s starting at (*ip) update (*ip) to index
// the character after the end of the integer.
static	int	get_number (char s[], size_t* ip, fldnr_t* np) {
	int	result	= err;
	size_t	i	= *ip;
	if (isdigit (s[i])) {
		char*	nextp	= 0;
		fldnr_t	n	= strtol (&s[i], &nextp, 10);
		if (n > 0) {
			*np	= n;
			i	= nextp - s;
			*ip	= i;
			result	= ok;

		}
	}
	return	result;
}

// get field expression = number | END | END SUB n
// starting from s[*ip] update (*ip)

static	int	get_expression (char s[], size_t* ip, point_t* fp) {
	int	result	= err;
	size_t	i	= *ip;
	if (s[i] == CHAR_END) {
		if (s[i+1] == CHAR_SUB) {
			fldnr_t	n	= 0;
			size_t	j	= i+2;
			if (get_number (s, &j, &n)==ok) {
				fp->origin	= ORG_END;
				fp->offset	= -n;
				*ip	= j;
				result	= ok;
			}
			else	{
				fld_listerr_print (s, j);
			}
		}
		else	{
			fp->origin	= ORG_END;
			fp->offset	= 0;
			*ip	= i+1;
			result	= ok;
		}
	}
	else if (isdigit(s[i]) && s[i] != '0') {
		fldnr_t	n	= 0;
		size_t	j	= i;
		if (get_number (s, &j, &n)==ok) {
			fp->origin	= ORG_BEG;
			fp->offset	= n;
			*ip	= j;
			result	= ok;
		}
	}
	else	{
		fld_listerr_print (s, i);
	}
	return	result;
}

// get range expression = field expression | field expression RANGE field expression
//
static	int	get_range (char s[], size_t* ip, fld_t* rp) {
	int	result	= err;
	size_t	i	= *ip;
	if (get_expression (s, &i, &rp->start) == ok) {
		if (s[i] == CHAR_RANGE) {
			size_t	j	= i+1;
			if (get_expression (s, &j, &rp->end) == ok) {
				*ip	= j;
				result	= ok;
			}
		}
		else if (s[i] == CHAR_LIST || s[i] == '\0'){
			rp->end	= rp->start;
			*ip	= i;
			result	= ok;
		}
		else	{
			fld_listerr_print (s, i);
		}
	}
	return	result;
}

// get range list = expresion range | expression range LIST range list
// 
static	int	get_rangelist (char s[], size_t* ip, fldvec_t* rl) {
	int	result	= err;
	fld_t	f;
	size_t	i	= *ip;
	if (get_range (s, &i, &f) == ok) {
		FAIL(
			fldvec_append (rl, f)
		);
		if (s[i] == CHAR_LIST) {
			size_t	j	= i+1;
			result	= get_rangelist (s, &j, rl);
			*ip	= j;
		}
		else if (s[i] == '\0') { 
			result	= ok;
			*ip	= i;
		}
		else	{
			fld_listerr_print (s, i);
		}
	}
	return	result;
}
int	fld_parse (fldvec_t* fieldlist, char fieldspec[]){
	size_t	i	= 0;
	return	get_rangelist (fieldspec, &i, fieldlist);
}
		
// Return the number of steps from start..end 
// 0 - mean empty interval

# define	max(x,y)	((x)>=(y)?(x):(y))
# define	min(x,y)	((x)<=(y)?(x):(y))

int	fld_interval (interval_t* ivp, fld_t rp, fldnr_t nfr) {
	int	nsteps	= 0;
	fldnr_t	step	= 0;
	fldnr_t	start	= ((rp.start.origin == ORG_END) ? nfr : 0) + rp.start.offset;
	fldnr_t	end	= ((rp.end.origin == ORG_END) ? nfr : 0) + rp.end.offset;

	if (rp.start.origin == rp.end.origin) {
		if (start <= end && start <= nfr && 1 <= end) {
			step	= 1;
			start	= max (1, start);
			end	= min (nfr, end);
		}
		else if (end <= start && end <= nfr && 1 <= start) {
			step	= -1;
			start	= min (nfr, start);
			end	= max (1, end);
		}
		// else empty interval
	}	
	else if (rp.start.origin == ORG_BEG) {	// We insist +ve steps
		if (start <= end) {
			step	= 1;
			start	= max (1, start);
			end	= min (nfr, end);
		}
		// else empty interval
	}
	else { // start.origin == ORG_END  - We insist -ve steps
		if (start >= end) {
			step	= -1;
			start	= min (nfr, start);
			end	= max (1, end);
		}
		// else empty interval
	}
	if (step != 0) {
		ivp->start	= start;
		ivp->end	= end;
		ivp->step	= step;
		nsteps	= 1 + (end - start)/step;
	}
	return	nsteps;
}

# if	DEBUG
int	main (int argc, char* argv[]) {
	int	nfr	= 4;
	fldnr_t	n	= 0;
	char*	ns	= 0;
	size_t	x	= 0;
	point_t	fld;
	fld_t	rt;
	fldvec_t*	fv	= 0;
	if (argc > 1) {
		int	r	= fldvec_Create (&fv, 10);
		if (argc > 2) {
			nfr	= atoi(argv[2]);
		}
		if (r==ok) {
			r	= fld_parse (fv, argv[1]);
			if (r==ok) {
				int	i = 0;
				interval_t	v;
				fprintf (stderr, "spec: %s\n", argv[1]);
				for (i=0; i < fldvec_last(fv); ++i) {
					fld_t	ft	= fldvec_at (fv, i);
					debug_print_fld ("fv", ft);
					int	nsteps	= fld_interval (&v, ft, nfr);
					fprintf (stderr, "nsteps\t= %d\n", nsteps);
					fprintf (stderr, "start\t= %ld\n", v.start);
					fprintf (stderr, "end\t= %ld\n", v.end);
					fprintf (stderr, "step\t= %ld\n", v.step);
				}
			}
		}
	}
}
# endif
