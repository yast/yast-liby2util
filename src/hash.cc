/*---------------------------------------------------------------------\
|                                                                      |
|                      __   __    ____ _____ ____                      |
|                      \ \ / /_ _/ ___|_   _|___ \                     |
|                       \ V / _` \___ \ | |   __) |                    |
|                        | | (_| |___) || |  / __/                     |
|                        |_|\__,_|____/ |_| |_____|                    |
|                                                                      |
|                               core system                            |
|                                                     (C) 2002 SuSE AG |
\----------------------------------------------------------------------/

   File:       hash.cc
   Purpose:    Implements a hash
   Author:     Roman Hodek <roman.hodek@informatik.uni-erlangen.de>
   Maintainer: Ludwig Nussel <lnussel@suse.de>

   this file is taken from PHI

/-*/

#include <y2util/hash.h>

using namespace std;

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
