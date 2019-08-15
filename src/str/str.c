# include	<stdarg.h>
# include	<stdlib.h>
# include	<string.h>

# include	"constant.h"
# include	"str.h"

//
// Storage size allocation policy
//	Smallest (MINALLOC) and round request up to STEP blocks
enum	{
	SHIFT		= 9,
	MINALLOC	= (1u<<SHIFT)-GUARD, 
	STEP		= 1u<<SHIFT,	
	MASK		= (~0u<<SHIFT),
};
static	inline strsize_t size_policy (strsize_t size) {
	strsize_t	result	= size;
	if (size < MINALLOC) {
		result	= MINALLOC;
	}
	else	{
		result	= ((size + (STEP - 1)) & MASK) - GUARD;
	}
	return	result;
} 

// ------------------------------------

//
// Make a 'str_t' from naked storage
//
int	str_Init (str_t** spp, strsize_t strsize, void* basestore, size_t basestoresize, int baseflags,
			 void* extstore, size_t extstoresize, int extflags)
{
	int	result	= err;
	str_t*	sp	= 0;
	if (sizeof(*sp) <= basestoresize) {
		sp	= basestore;
		sp->n.length	= 0;
		sp->n.flags	= str_makeflags (0, baseflags, T_SHORT);
// Now sp is a str_t
		if (strsize <= SHORTSTRSIZE) {
			result	= ok;
		}
		else if (strsize+sizeof(longstr_t)+GUARD <= basestoresize && extstore == 0) {
			str_setflags (sp, str_makeflags(ST_AUTO, baseflags, T_LONG));
			str_setstorage (sp, ST_AUTO, (char*)&(sp->x.str[1])); 
			str_setsize (sp, strsize - sizeof(longstr_t));
			result	= ok;
		}
		else if (strsize+GUARD <= extstoresize) {
			str_setflags (sp, str_makeflags(extflags, baseflags, T_LONG));
			str_setstorage (sp, extflags, extstore); 
			str_setsize (sp, strsize);
			result	= ok;
		}
		else	{ // last resort allocate storage from heap
			result	= str_grow (sp, strsize);
		}
	}
	if (result == ok) {
		guard (sp);
		*spp	= sp;
	}
	return	result;
}	
//	Example:	Make an auto string
//
//	str_t*	autostr	= str_init (0, alloca(sizeof(str_t)), sizeof(str_t),ST_AUTO, 0, 0);
//
str_t*	str_init (strsize_t strsize, void* basestore, size_t basestoresize, int baseflags,
			 void* extstore, size_t extstoresize, int extflags)
{
	str_t*	result	= 0;
	str_Init (&result, strsize, basestore, basestoresize, baseflags, extstore, extstoresize,  extflags);
	return	result;
}
//--------------------------------------

int	str_grow (str_t* sp, strsize_t size) {
	int	result	= err;
	char*	nstr	= 0;
	if (size <= str_size (sp))
		return	ok;

	size	= size_policy (size);
	if (str_is_short (sp) || (str_ext_store (sp) != ST_HEAP)){	// move to external
		nstr	= malloc (size + GUARD);
		if (nstr) {
			memcpy (nstr, str_storage (sp), str_length(sp));
			result		= ok;
		}
	}
	else {	// str is LONG and ext_store is HEAP
		nstr	= realloc (str_storage (sp), size+GUARD);
		if (nstr) {
			result	= ok;
		}
	}
	if (result==ok) {
		str_setstorage (sp, ST_HEAP, nstr);
		str_setsize (sp, size);
		guard (sp);
	}
	return	result;
}

int	str_Create (str_t** spp, strsize_t size) {
	int	result	= ok;
	str_t*	sp	= calloc (sizeof(*sp), 1);
	if (sp) {
		result	= str_Init (&sp, 0, sp, sizeof(*sp), ST_HEAP, 0, 0, 0);
	}
	if (result==ok) { 
		if (size > str_size (sp)) {
			result	= str_grow (sp, size);
		}
		if (result==ok) {
			*spp	= sp;
		}
	}
	return	result;
}
int	str_Copy (str_t** spp, str_t* orig) {
	int	result	= str_Create (spp, str_size (orig));
	if (result == ok) {
		result	= str_copy (*spp, orig);
	}
	return	result;
}
/*
// Only free HEAP allocated storage
*/
int	str_Delete (str_t** spp) {
	if (spp) {
		str_t*	sp	= *spp;
		if (str_is_long (sp)) {
			if (str_ext_store (sp) == ST_HEAP) {
				free (str_storage(sp));
			} 
		}
		if (str_base_store(sp) == ST_HEAP) {
			free (sp);
		}
	}
	return	ok;
}
int	str_CreateCstr (str_t** spp, char* s) {
	size_t	slen	= strlen (s);
	int	result	= str_Create (spp, slen);
	if (result==ok) {
		str_copyin ( *spp, 0, s, slen);
	}
	return	result;
}
void	str_trim (str_t* s) {
	if (str_is_long (s)) {
		strsize_t	len	= str_length (s);
		strflag_t	ext_store	= str_ext_store (s);
		if (len < SHORTSTRSIZE && ext_store == ST_HEAP) {
			strflag_t	base_store	= str_base_store (s);
			char*	data			= str_storage (s);
		
			/* Flip 's' to short representation */
			str_setflags (s, str_makeflags (0, base_store, T_SHORT));
			str_copyin (s, 0, data, len);
			free (data);
		}
	}
}
int	str_appendnchar (str_t* dst, char* s, strsize_t n) {
	int		result	= ok;
	strsize_t	dlen	= str_length (dst);
	if ((dlen+n) <= str_size (dst)) {
		str_copyin (dst, dlen, s, n);
	}
	else	{
		result	= str_grow (dst, dlen+n);
		if (result==ok) {
			result	= str_appendnchar (dst, s, n);
		}
	}
	return	result;
}
// Have to be careful of src == dst when representation flips from short->long
// as part of the "src" string data is overwriten before copying
// Subtle bug where src == dst and _appendnchar() call _grow() and relocates ".str"

int	str_append (str_t* dst, str_t* src) {
	int	result	= err;
	char*		cstr	= str_storage (src);
	strsize_t	slen	= str_length (src);

	if (dst == src) {
		char*	srccpy	= alloca (slen); // We can ignore the terminal \0
		memcpy (srccpy, cstr, slen);
		result	= str_appendnchar (dst, srccpy, slen);
	}
	else	{
		result	= str_appendnchar (dst, cstr, slen);
	}
	return	result;
}

int	str_copy (str_t* dst, str_t* src) {
	int		result	= ok;
	strsize_t	slen	= str_length (src);
	if (slen <= str_size (dst)) {
		str_copyin (dst, 0, str_storage (src), slen);
	}
	else	{
		result	= str_grow (dst, slen);
		if (result==ok) {
			result	= str_copy (dst, src);
		}
	}
	return	result;
}

int	str_appendchar (str_t* dst, int ch) {
	int	result	= ok;
	strsize_t	len	= str_length (dst);
	char*		t	= str_storage (dst);

	if (len <= str_size(dst)) {
        	t [len]	= ch;
		str_setlength (dst, len+1);
		guard (dst);
	}
	else	{
		result	= str_grow (dst, len+1);
		if (result==ok) {
			result	= str_appendchar (dst, ch);
		}
	}
	return	result;
}
int	str_substr (str_t* dst, str_t* src, strsize_t start, strsize_t len) {
	int		result	= ok;
	strsize_t	slen	= str_length (src);
	strsize_t	ssize	= str_size (src);

	str_setlength (dst, 0);
	if (start <= slen && (start + len) <= slen) {
		if (len <= str_size (dst)) {
			str_copyin (dst, 0, &(str_storage(src))[start], len);
		}
		else	{
			result	= str_grow (dst, len+1);
			if (result==ok) {
				result	= str_substr (dst, src, start, len);
			}
		}
	}
	return	result;
} 
int	str_findchar (str_t* s, int ch) {
	int	result	= -1;
	char*	p	= str_storage (s);
	char*	t	= memchr (p, ch, str_length (s));
	if (t) {
		result	= t - p;
	}
	return	result;
}
/*
// Can use memrchr() but isn't on MacOSX
*/
int	str_findrchar (str_t* s, int ch) {
	int	len	= str_length (s);
	char*	p	= str_storage (s) + len;
	int	i	= len;
	int	j	= 0;
	while (i!=j) {
		if (p[i-1] == ch) {
			j	= i;
		}
		else	{
			--i;
		}
	}
	return	i-1;	// returns -1 if not found
}
int	str_str (str_t* ref, str_t* pat) {
	int	result	= -1;
	char*	r	= str_storage (ref);
	char*	p	= str_storage (pat);
	strsize_t	rlen	= str_length (ref);
	strsize_t	plen	= str_length (pat);
	
	strsize_t	start	= 0;
	while ( (start + plen) <= rlen && result < 0) {
		if (memcmp (&r[start], p, plen)==0) {
			result	= start;
		}
		else	{
			++start;
		}
	}
	return	result;
}
int	str_compare (str_t* ss, str_t* tt) {
	int	result	= 0;
	char*	s	= str_storage (ss);
	char*	t	= str_storage (tt);
	strsize_t	slen	= str_length (ss);
	strsize_t	tlen	= str_length (tt);
	strsize_t	minlen	= slen <= tlen ? slen: tlen;
	result	= memcmp (s, t, minlen);
	if (result==0) {
		result	= slen - tlen;
	}
	return	result;
}
