/*
//	@(#) str_inline.h - short inline mainly access functions
*/
# if	!defined( STR_INLINE_H)
# define	STR_INLINE_H

# include	<stdio.h>
# include	<string.h>
# include	<stdlib.h>

# include	"str_type.h"

/*
// Construct literals GCC extension - supported by CLANG
// Global+/-static
//	str_t*	literal	= str_literal ("A test")
// Auto
//	str_t*	auto_literal	= str_literal ("Auto literal")
//
// !!Block level Static!!
//	static	str_t	static_literal[]	= str_literal ("Static literal")
*/
# define	str_literal( cstr)			\
	(str_t[]){{ .n.length = sizeof (cstr)-1,	\
	.n.flags = makeflags (0, ST_CONST, T_SHORT),	\
	.n.str = cstr }}

# define	str_literal_long( longcstr)			\
	(str_t[]){{ .x.length = sizeof (longcstr)-1,		\
	.x.flags = makeflags (ST_CONST, ST_CONST, T_LONG),	\
	.x.size = sizeof(longcstr),				\
	.x.str = longcstr }}


// --
static	inline	strflag_t	str_flags (str_t* s) {
	return	s->n.flags;
}
static	inline	void	str_setflags (str_t* s, strflag_t flags) {
	s->n.flags	= flags;
}

// --
static	inline	strsize_t	str_size (str_t* s) {
	return	str_is_short (s) ? SHORTSTRSIZE : s->x.size;
}
static	inline	void	str_setsize (str_t* s, strsize_t size) {
	if (str_is_long (s)) {
		s->x.size	= size;
	}
}

// --
static	inline	strsize_t	str_length (str_t* s) {
	return	s->n.length;
}
static	inline	void	str_setlength (str_t* s, strsize_t len) {
	s->n.length	= len;
}
extern	void	str_trim (str_t* s);
static	inline	void	str_clear (str_t* s) {
	str_setlength (s, 0);
	str_trim (s);
}

// --
// Where the string characters are stored.
static	inline	char*		str_storage (str_t* s) {
	return	str_is_short (s) ? s->n.str : s->x.str;
}
static	inline	void	str_setstorage (str_t* s, strflag_t extflag, char* buf) {
	s->x.str	= buf;
	str_setflags (s, str_makeflags (extflag, str_base_store(s), T_LONG));

}

// --
// We append a \0 at the end of the char array as a precaution
static  inline  void guard (str_t* str) {
        if (GUARD) {
                (str_storage(str))[str_length(str)]       = '\0';
        }
}

// --
// Caller must make sure sizes etc are sane
static  inline  void str_copyin (str_t* dst, strsize_t offset, const char* s, size_t len) {
        char*   t       = str_storage (dst);
        memcpy (&t[offset], s, len);
	str_setlength (dst, offset+len);
        guard (dst);
}
// --
// Place a cstr copy in storage
static	inline	char*	str_cstr_storage (char* storage, size_t storesize, str_t* str) {
	size_t	n	= str_length (str);
	n	= n < storesize ?n : storesize-1;
	memcpy (storage, str_storage (str), n);
	storage [n]	= '\0';
	return	storage;
}	
// Make a C string copy of str_t str in heap storage
static	inline	char*	str_cstr (str_t* str) {
	size_t	len	= str_length (str);
	char*	s	= calloc (sizeof (*s), len+1);
	return	str_cstr_storage (s, len+1, str); 
}
// Make a C string copy of str_t str on stack
# define	str_cstr_auto(str)	\
	str_cstr_storage (alloca (str_length(str)+1), str_length(str)+1, str)

# endif

