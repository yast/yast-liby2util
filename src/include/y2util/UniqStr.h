// from PHI
#ifndef _PkgDb_UniqStr_h
#define _PkgDb_UniqStr_h

#include <cstring>
#include <hash.h>

// UniqStrInternal is used as content type of the UniqStr hash; the comparison
// function is strcmp() so that existing strings can be found by hash
// functions.
class UniqStrInternal {
	friend class UniqStr;
	const char *str;
  public:
	UniqStrInternal( const char *p ) : str(p) {}
	bool operator==( const UniqStrInternal& n2 ) const {
		return strcmp(str,n2.str) == 0; }
	bool operator!=( const UniqStrInternal& n2 ) const {
		return strcmp(str,n2.str) != 0; }
	operator void* () const { return (void *)str; }
	operator const char* () const { return str; }
};

inline size_t hashfun( const UniqStrInternal& str ) {
	return hashfun( (const char *)str );
}

// This hash is used for keeping the UniqStrs unique.
class UniqStr {
  public:
	typedef noval_hash<UniqStrInternal> UniqStrHash_type;
  protected:
	UniqStrHash_type UniqStrHash;
  public:
	const char *add( const char *p = "" ) {
		UniqStrInternal i_str(p);
		UniqStrHash_type::list_type *q;
		q = UniqStrHash.insert( i_str );
		if ((void *)(q->key) == (void *)p) {
			q->key.str = strdup(p);
		}
		return q->key.str;
	}
};

#endif  /* _PkgDb_UniqStr_h */


// Local Variables:
// tab-width: 4
// End:
