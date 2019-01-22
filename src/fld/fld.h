/*
//	@(#) fld.h
*/

# if	!defined( FLD_H)
# define	FLD_H
# include	<stdint.h>

typedef	int32_t	fldnr_t;
struct	point	{
	int	origin;
	fldnr_t	offset;
};
typedef	struct	point	point_t;

struct	fld	{
	point_t	start;
	point_t	end;
};
typedef	struct	fld	fld_t;
static	fld_t	EMPTY_FLD	= {{ 0, 0}, {0,0}};

# endif
