// from PHI

#include "hash.h"

size_t hashfun( string& s ) {
	unsigned h = 0;
	const unsigned char *p = (const unsigned char *)s.c_str();
	while( *p )
		h = (h<<1) /* | ((h>>31)&1) */ ^ *p++;
	return h;
}

size_t hashfun( const string& s ) {
	unsigned h = 0;
	const unsigned char *p = (const unsigned char *)s.c_str();
	while( *p )
		h = (h<<1) /* | ((h>>31)&1) */ ^ *p++;
	return h;
}

size_t hashfun( const char * p ) {
	unsigned h = 0;
	while( *p )
		h = (h<<1) /* | ((h>>31)&1) */ ^ *p++;
	return h;
}


// Local Variables:
// tab-width: 4
// End:
