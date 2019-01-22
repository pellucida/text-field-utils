/*
// @(#) fldvec.h
*/

# if	!defined(FLDVEC_H)
# define	FLDVEC_H

# include	<stdlib.h>

# include	"constant.h"
# include	"fld.h"

enum	{	// 32
	FLDVEC_SIZE_SHIFT	= 5,
	DEFAULT_FLDVEC_SIZE	= (1u<<FLDVEC_SIZE_SHIFT),
	FLDVEC_SIZE_STEP	= (1u<<FLDVEC_SIZE_SHIFT),
	FLDVEC_SIZE_MASK	= (~0u<<FLDVEC_SIZE_SHIFT),
};
struct	fldvec	{
	size_t	size;
	size_t	last;
	fld_t*	vec;
};
typedef	struct	fldvec	fldvec_t;

static	inline	size_t	fldvec_size (fldvec_t* fv) {
	return	fv->size;
}
static	inline	size_t	fldvec_last (fldvec_t* fv) {
	return	fv->last;
}

int	fldvec_grow (fldvec_t* fv, size_t size);
int	fldvec_Create (fldvec_t** fvp, size_t size);
int	fldvec_Delete (fldvec_t** fvp);


int	fldvec_atget (fldvec_t* fv, size_t index, fld_t* sp);
fld_t	fldvec_at (fldvec_t* fv, size_t index);

int	fldvec_atput (fldvec_t* fv, size_t index, fld_t s);
int	fldvec_insert (fldvec_t* fv, size_t index, fld_t s);
int	fldvec_remove (fldvec_t* fv, size_t index);

/*
// Model vec[0..+oo] initially == 0; vec.append(x): vec[]==0 -> vec[0]=x, vec[1..oo]=0
*/
static	inline	int     fldvec_append (fldvec_t* fv, fld_t s) {
        return	fldvec_atput (fv, fldvec_last(fv), s);
}
# endif
