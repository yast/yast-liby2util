/*---------------------------------------------------------------------\
|                                                                      |
|                      __   __    ____ _____ ____                      |
|                      \ \ / /_ _/ ___|_   _|___ \                     |
|                       \ V / _` \___ \ | |   __) |                    |
|                        | | (_| |___) || |  / __/                     |
|                        |_|\__,_|____/ |_| |_____|                    |
|                                                                      |
|                               core system                            |
|                                         (C) SuSE Linux Products GmbH |
\----------------------------------------------------------------------/

  File:       SourceCodeLocation.cc

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose:

/-*/

#include <iostream>

#include "y2util/SourceCodeLocation.h"

using namespace std;

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : SourceCodeLocation
//
///////////////////////////////////////////////////////////////////

const unsigned SourceCodeLocation::_unknown = unsigned(-1);

///////////////////////////////////////////////////////////////////

/******************************************************************
**
**	FUNCTION NAME : operator<<
**	FUNCTION TYPE : std::ostream &
*/
std::ostream &
operator<<( std::ostream & str, const SourceCodeLocation & obj_r )
{
  if ( ! obj_r )
    return str << "unknown_location";
  return str << obj_r.file() << "(" << obj_r.func() << "):" << obj_r.line();
}
