//
//	@(#) str_type.h - define data structures for str_t
//
# if	!defined( STR_TYPE_H)
# define	STR_TYPE_H

# include	<stdint.h>

typedef	uint32_t	strsize_t;
typedef	uint8_t		strflag_t;

// Parameterize the basic string size by define storage size less overhead
// eg 128 bytes storage less 4 (strsize_t) 1 (strflag_t) 1 GUARD

# define	GUARD	(1)
enum	{
	SHORTSTRSTORAGE	= 128,	// ** 

	SHORTSTRSIZE	= (SHORTSTRSTORAGE - (sizeof(strsize_t)+sizeof(strflag_t)+GUARD)),
};

// Strings are either short  or long
enum	{
	T_SHORT	= 0,
	T_LONG	= 1,
};

// Where the strings and their parts are stored 
enum	{
	ST_HEAP		= 0,	// malloc()
	ST_AUTO		= 1,	// alloca()
	ST_STATIC	= 2,	// global 
	ST_CONST	= 3,	// like 1 & 2 doesn't get free()d
};

// Bit fiddling to encode the flags without using C bit fields.
enum	{
	SHIFT_TYPE	= 0,
	WIDTH_TYPE	= 1,

	SHIFT_BASE	= SHIFT_TYPE + WIDTH_TYPE,
	WIDTH_BASE	= 2,

	SHIFT_EXT	= SHIFT_BASE + WIDTH_BASE,
	WIDTH_EXT	= 2,

	BM_TYPE		= ~((~0u)<<WIDTH_TYPE),
	MASK_TYPE	= BM_TYPE<<SHIFT_TYPE,
	BM_BASE		= ~((~0u)<<WIDTH_BASE),
	MASK_BASE	= BM_BASE<<SHIFT_BASE,
	BM_EXT		= ~((~0u)<<WIDTH_EXT),
	MASK_EXT	= BM_EXT<<SHIFT_EXT,
};

// Flags	EXTSTORAGE(HEAP,AUTO,STATIC,CONST) // LONG
//		BASESTORAGE(HEAP,AUTO,STATIC,CONST)// LONG and SHORT
//		STRTYPE(SHORT,LONG)

// Macro for encoding flags
# define	makeflags( ext,base,type) 			\
			((((ext)  & BM_EXT ) <<SHIFT_EXT )	\
			|(((base) & BM_BASE) <<SHIFT_BASE)	\
			|(((type) & BM_TYPE) <<SHIFT_TYPE))

// Same macro wrapped in a function
static	inline	strflag_t	str_makeflags (int ext, int base, int type) {
	return	makeflags( ext, base, type); 
}

// Macros for unwrapping flags
# define	str_is_short(s)		(((s)->n.flags & MASK_TYPE) == T_SHORT)
# define	str_is_long(s)		(((s)->n.flags & MASK_TYPE) == T_LONG)
# define	str_base_store(s)	(((s)->n.flags & MASK_BASE) >> SHIFT_BASE)
# define	str_ext_store(s)	(((s)->n.flags & MASK_EXT) >> SHIFT_EXT)

typedef	union	str	str_t; // Forward

struct	short_str {
	strsize_t	length;
	strflag_t	flags;
	char		str [SHORTSTRSIZE+GUARD];
}; 
typedef	struct	short_str	shortstr_t;

struct	long_str {
	strsize_t	length;
	strflag_t	flags;
	strsize_t	size;
	char*		str;
};
typedef	struct	long_str	longstr_t;

// This layout means the first two fields are common (length, flags)
union	str {
	shortstr_t	n;	// iNternal
	longstr_t	x;	// eXternal
};

# endif
