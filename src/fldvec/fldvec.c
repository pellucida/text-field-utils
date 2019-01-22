/*
//	@(#) fldvec.c
*/

# include	<stdlib.h>

# include	"constant.h"
# include	"fld.h"
# include	"fldvec.h"

static	inline	size_t	fldvec_size_policy (size_t size) {
	if (size < DEFAULT_FLDVEC_SIZE) {
		size	= DEFAULT_FLDVEC_SIZE;
	}
	else	{
		size	= (size + (FLDVEC_SIZE_STEP - 1)) & FLDVEC_SIZE_MASK;
	}
	return	size;
}
		
int	fldvec_grow (fldvec_t* sv, size_t size_request) {
	int	result	= err;
	size_t	size	= fldvec_size_policy (size_request);
	fld_t*	newvec	= realloc (sv->vec, /* sv->vec == 0 =>  malloc() */
			size * sizeof(sv->vec[0]));
	if (newvec) {
		int	i	= sv->size;
		for (; i < size; ++i) {
			newvec [i]	= EMPTY_FLD;
		}
		sv->vec 	= newvec;
		sv->size	= size;
		result	= ok;
	}
	return	result;
}
int	fldvec_Create (fldvec_t** svp, size_t size) {
	int	result	= err;
	fldvec_t*	sv	= malloc (sizeof(*sv));
	if (sv) {
		sv->size	= 0;
		sv->last	= 0;
		sv->vec		= 0;
		result		= fldvec_grow (sv, size);
		if (result==ok) {
			*svp		= sv;
		}
		else	{
			free (sv);
		}
	}
	return	result;
}
int	fldvec_Delete (fldvec_t** svp) {
	int	result	= ok;
	if (svp) {
		free (*svp);
		*svp	= 0;
	}
	return	result;
}

fld_t	fldvec_at (fldvec_t* sv, size_t index) {
        return  (index < fldvec_last(sv)) ? sv->vec [index] : EMPTY_FLD ;
}
		
int	fldvec_atput (fldvec_t* sv, size_t index, fld_t s) {
	int	result	= err;
	if (index < sv->size) {
		sv->vec [index]	= s;
		if (index >= sv->last) {
			sv->last	= index+1;
		}
		result	= ok;
	}
	else	{
		result	= fldvec_grow (sv, index+1);
		if (result==ok){
			result	= fldvec_atput (sv, index, s);
		}
	}
	return	result;
}
int	fldvec_remove (fldvec_t* sv, size_t index) {
	int	result	= err;
	if (index < sv->last) {
		fld_t*	vec	= sv->vec;
		size_t	last	= sv->last;
		size_t	i	= index;
		for (i=index; i < last-1; ++i) {
			vec [i]	= vec [i+1];
		}
		sv->last	= last-1;
	}
	return	result;
}
/*
//	inserting between [0..last-1] push elements up 
//	inserting [last..oo] just overwrite
*/
int	fldvec_insert (fldvec_t* sv, size_t index, fld_t s) {
	int	result	= err;
	if (index < sv->size && sv->last+1 < sv->size) {
		if (index < sv->last) {
			size_t	i	= sv->last;
			fld_t*	vec	= sv->vec;
			for(i=sv->last; i > index; --i) {	// free up slot vec [index]
				vec[i+1]	= vec [i];
			}
			vec [index]	= s;
			sv->last	= sv->last+1;
		}
	}							// else needs to grow
	result	= fldvec_atput (sv, index, s);			// at put will grow vec
	return	result;
}
