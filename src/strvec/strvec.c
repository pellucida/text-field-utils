
# include	<stdlib.h>

# include	"constant.h"
# include	"str.h"

# include	"strvec.h"

static	str_t*	STR_EMPTY	= str_literal ("");

static	inline	size_t	strvec_size_policy (size_t size) {
	if (size < DEFAULT_STRVEC_SIZE) {
		size	= DEFAULT_STRVEC_SIZE;
	}
	else    {
                size    = (size + (STRVEC_SIZE_STEP - 1)) & STRVEC_SIZE_MASK;
        }
	return	size;
}
int	strvec_grow (strvec_t* sv, size_t size_request) {
	int	result	= err;
	size_t	size	= strvec_size_policy (size_request);
	str_t**	newvec	= realloc (sv->vec, /* sv->vec == 0 =>  malloc() */
			size*sizeof(sv->vec[0]));
	if (newvec) {
		int	i	= sv->size;
		for (; i < size; ++i) {
			newvec [i]	= STR_EMPTY;
		}
		sv->vec 	= newvec;
		sv->size	= size;
		result	= ok;
	}
	return	result;
}
int	strvec_Create (strvec_t** svp, size_t size) {
	int	result	= err;
	strvec_t*	sv	= malloc (sizeof(*sv));
	if (sv) {
		sv->size	= 0;
		sv->last	= 0;
		sv->vec		= 0;
		result		= strvec_grow (sv, size);
		if (result==ok) {
			*svp		= sv;
		}
		else	{
			free (sv);
		}
	}
	return	result;
}
int	strvec_Delete (strvec_t** svp) {
	int	result	= ok;
	if (svp) {
		strvec_t*	sv	= *svp;
		int	i	= 0;
		for (i=0; i < sv->size; ++i) {
			str_Delete (&(sv->vec [i]));
		}
		free (sv);
		*svp	= 0;
	}
	return	result;
}

str_t*  strvec_at_nocopy (strvec_t* sv, size_t index) {
        return  (index < strvec_last(sv)) ? sv->vec [index] : STR_EMPTY;
}
int	strvec_atget (strvec_t* sv, size_t index, str_t** sp) {
	str_t*	s	= strvec_at_nocopy (sv, index);
	str_t*	copy	= 0;
	int	result	= str_Copy (&copy, s);
	if (result == ok) {
		*sp	= copy;
	}
	return	result;
}
str_t*	strvec_at (strvec_t* sv, size_t index) {
	str_t*	s	= strvec_at_nocopy (sv, index);
	str_t*	result	= 0;
	str_Copy (&result, s);	// return == err ==> result == NULL
	return	result; 
}
		
/*
// Since we copy in and out - if there is an existing str_t in a vec[]
// slot we reuse it if its heap allocated (ie not constant)
*/

int	strvec_atput (strvec_t* sv, size_t index, str_t* s) {
	int	result	= err;
	if (index < sv->size) {
		str_t*	ss	= sv->vec [index];
		strflag_t	flags	= str_flags (ss);
		if (ss == STR_EMPTY) {	// first time to use this slot
			str_t*	copy	= 0;
			result	= str_Copy (&copy, s);
			if (result == ok) {
				sv->vec [index]	= copy;
			}
		}
		else	{
			result	= str_copy (ss, s);
		}
		if (index >= sv->last) {
			sv->last	= index+1;
		}
	}
	else	{
		result	= strvec_grow (sv, index+1);
		if (result==ok){
			result	= strvec_atput (sv, index, s);
		}
	}
	return	result;
}
int	strvec_remove (strvec_t* sv, size_t index) {
	int	result	= err;
	if (index < sv->last) {
		str_t**	vec	= sv->vec;
		size_t	last	= sv->last;
		size_t	i	= index;
		result	= str_Delete (&(vec [index]));
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
int	strvec_insert (strvec_t* sv, size_t index, str_t* s) {
	int	result	= err;
	if (index < sv->size && sv->last+1 < sv->size) {
		if (index < sv->last) {
			size_t	i	= sv->last;
			str_t**	vec	= sv->vec;
			for(i=sv->last; i > index; --i) {	// free up slot vec [index]
				vec[i+1]	= vec [i];
			}
			vec [index]	= STR_EMPTY;
			sv->last	= sv->last+1;
		}
	}							// else needs to grow
	result	= strvec_atput (sv, index, s);			// at put will grow vec
	return	result;
}
int	strvec_clear (strvec_t* sv) {
	int	result	= err;
	int	i	= 0;
	for (; i < sv->last; ++i) {
		result	= str_Delete (&(sv->vec [i]));
		sv->vec [i]	= STR_EMPTY;	
	}
	sv->last	= 0;
	return	result;
}
int	strvec_find (strvec_t* sv, str_t* s) {
	int	result	= err;
	int	i	= 0;
	int	j	= sv->last;
	while (i!=j) {
		if (str_compare (sv->vec[i], s)==0) {
			j	= i;
			result	= i;
		}
		else	++i;
	}
	return	result;
}
static  int     sort_compare (const void* a, const void* b) {
        str_t*  aa      = *(str_t**)a;
        str_t*  bb      = *(str_t**)b;
        return  str_compare (aa, bb);
}
void	strvec_sort (strvec_t* vec) {
        void*   base    = vec->vec;
        size_t  nmem    = vec->last;
        size_t  size    = sizeof(str_t*);
        qsort (base, nmem, size, sort_compare);
}

