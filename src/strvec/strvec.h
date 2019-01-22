/*
// @(#) strvec.h
*/

# if	!defined(STRVEC_H)
# define	STRVEC_H

# include	<stdlib.h>

# include	"constant.h"
# include	"str.h"

enum	{ // 32
	STRVEC_SIZE_SHIFT	= 5,
	DEFAULT_STRVEC_SIZE	= (1u<<STRVEC_SIZE_SHIFT),
	STRVEC_SIZE_STEP	= (1u<<STRVEC_SIZE_SHIFT),
	STRVEC_SIZE_MASK	= (~0u<<STRVEC_SIZE_SHIFT),
};
typedef	struct	str_vec	strvec_t;
struct	str_vec	{
	size_t	size;
	size_t	last;
	str_t**	vec;
};

static	inline	size_t	strvec_size (strvec_t* sv) {
	return	sv->size;
}
static	inline	size_t	strvec_last (strvec_t* sv) {
	return	sv->last;
}

int	strvec_grow (strvec_t* sv, size_t size);
int	strvec_Create (strvec_t** svp, size_t size);
int	strvec_Delete (strvec_t** svp);


int	strvec_atget (strvec_t* sv, size_t index, str_t** sp);
str_t*	strvec_at_nocopy (strvec_t* sv, size_t index);
str_t*	strvec_at (strvec_t* sv, size_t index);

int	strvec_atput (strvec_t* sv, size_t index, str_t* s);
int	strvec_insert (strvec_t* sv, size_t index, str_t* s);
int	strvec_remove (strvec_t* sv, size_t index);
int	strvec_clear (strvec_t* sv);
int	strvec_find (strvec_t* sv, str_t* s);
void	strvec_sort (strvec_t* sv);

/*
// Model vec[0..+oo] initially == ""; vec.append(x): vec[]=="" -> vec[0]=x, vec[1..oo]=""
// So fetching from any unused slot will return an empty string.
*/
static	inline	int     strvec_atget_nocopy (strvec_t* sv, size_t index, str_t** sp) {
        *sp     = strvec_at_nocopy (sv, index);
        return  ok;
}
static	inline	int     strvec_append (strvec_t* sv, str_t* s) {
        return	strvec_atput (sv, strvec_last(sv), s);
}

# endif
