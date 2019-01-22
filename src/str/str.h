//
//	@(#) str.h
//
# if	!defined( STR_H)
# define	STR_H

# include	<stdio.h>
# include	<string.h>
# include	<stdint.h>
# include	<alloca.h>

# include	"str_type.h"
# include	"str_inline.h"


//
// Init()/init() construct a str_t from supplied storage and flags
//
int	str_Init (str_t** spp, strsize_t size, void* storage, size_t storesize,
		int baseflags, void* extstore, size_t extsize, int extflags);

str_t*	str_init (strsize_t size, void* storage, size_t storesize, int baseflags,
		void* extstore, size_t extsize, int extflags);

// str_auto() must be a macro to use alloca().
// Create a 'auto' str_t alloc()-> storage, str_init() -> constructs str_t
//
# define	str_auto()	str_init(0, alloca(sizeof(str_t)), sizeof(str_t),ST_AUTO,0,0,0)

int     str_grow (str_t* sp, strsize_t size);
// --
int	str_Create (str_t** spp, strsize_t size);
int	str_Copy (str_t** spp, str_t* orig);
int	str_CreateCstr (str_t** spp, char* s);
int	str_Delete (str_t** spp);
void	str_trim (str_t* s);

// Usual functions
int	str_appendnchar (str_t* dst, char* s, strsize_t n);
int	str_append (str_t* dst, str_t* src);
int	str_copy (str_t* dst, str_t* src);
int	str_appendchar (str_t* dst, int ch);
int	str_substr (str_t* dst, str_t* src, strsize_t start, strsize_t len);
int	str_findchar (str_t* s, int ch);
int	str_findrchar (str_t* s, int ch);
int	str_str (str_t* ref, str_t* pat);
int	str_compare (str_t* ss, str_t* tt);

# endif

