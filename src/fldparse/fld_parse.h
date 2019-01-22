/*
//	@(#) fld_parse.c
*/
# if	!defined(FLD_PARSE_H)
# define	FLD_PARSE_H

# include	"fld.h"
# include	"fldvec.h"

//
// Redefine these to redefine the grammar
//
enum	{
	CHAR_END	= '$',
	CHAR_RANGE	= '-',
	CHAR_LIST	= ',',
	CHAR_SUB	= '_',
};

enum	{
	ORG_END	=	'$',
	ORG_BEG	=	'O',
};
// ---------------------------
struct	interval {
	long	start;
	long	end;
	long	step;
};
typedef	struct	interval	interval_t;

int     fld_parse (fldvec_t* fieldlist, char fieldspec[]);

int	fld_interval (interval_t* ivp, fld_t fld, int nfld_read);

# endif
