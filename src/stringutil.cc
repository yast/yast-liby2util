/*---------------------------------------------------------------------\
|                                                                      |
|                      __   __    ____ _____ ____                      |
|                      \ \ / /_ _/ ___|_   _|___ \                     |
|                       \ V / _` \___ \ | |   __) |                    |
|                        | | (_| |___) || |  / __/                     |
|                        |_|\__,_|____/ |_| |_____|                    |
|                                                                      |
|                               core system                            |
|                                                        (C) SuSE GmbH |
\----------------------------------------------------------------------/

  File:       stringutil.cc

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose:

/-*/

#include <iostream>

#include <y2util/stringutil.h>

using namespace std;
///////////////////////////////////////////////////////////////////
namespace stringutil {
///////////////////////////////////////////////////////////////////


/******************************************************************
**
**
**	FUNCTION NAME : split
**	FUNCTION TYPE : unsigned
**
**	DESCRIPTION :
*/
unsigned split( const std::string          line_tv,
		std::vector<std::string> & words_Vtr,
		const std::string &        sep_tv,
		const bool                 singlesep_bv )
{
  words_Vtr.clear();
  if ( line_tv.empty() )
    return words_Vtr.size();

  struct sepctrl {
    const string & sep_t;
    sepctrl( const string & sep_tv ) : sep_t( sep_tv ) {}
    // Note that '\0' ist neither Sep nor NonSep
    inline bool isSep     ( const char c )    const { return( sep_t.find( c ) != string::npos ); }
    inline bool isNonSep  ( const char c )    const { return( c && !isSep(c) ); }
    inline void skipSep   ( const char *& p ) const { while ( isSep( *p ) ) ++p; }
    inline void skipNonSep( const char *& p ) const { while ( isNonSep( *p ) ) ++p; }
  };

  sepctrl      sep_Ci( sep_tv );
  const char * s_pci = line_tv.c_str();
  const char * c_pci = s_pci;

  // Start with c_pci at the beginning of the 1st field to add.
  // In singlesep the beginning might be equal to the next sep,
  // which makes an empty field before the sep.
  if ( !singlesep_bv && sep_Ci.isSep( *c_pci ) ) {
    sep_Ci.skipSep( c_pci );
  }

  for ( s_pci = c_pci; *s_pci; s_pci = c_pci ) {
    sep_Ci.skipNonSep( c_pci );
    words_Vtr.push_back( string( s_pci, c_pci - s_pci ) );
    if ( *c_pci ) {
      if ( singlesep_bv ) {
        if ( !*(++c_pci) ) {
          // line ends with a sep -> add the empty field behind
          words_Vtr.push_back( "" );
        }
      } else
        sep_Ci.skipSep( c_pci );
    }
  }

  return words_Vtr.size();
}

/******************************************************************
**
**
**	FUNCTION NAME : join
**	FUNCTION TYPE : std::string
**
**	DESCRIPTION :
*/
std::string join( const std::vector<std::string> & words_r,
		  const std::string & sep_r )
{
  if ( words_r.empty() )
    return "";

  string ret( words_r[0] );

  for ( unsigned i = 1; i < words_r.size(); ++i ) {
    ret += sep_r + words_r[i];
  }

  return ret;
}



///////////////////////////////////////////////////////////////////
}  // namespace stringutil
///////////////////////////////////////////////////////////////////

