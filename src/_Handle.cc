/*---------------------------------------------------------------------\
|                                                                      |
|                      __   __    ____ _____ ____                      |
|                      \ \ / /_ _/ ___|_   _|___ \                     |
|                       \ V / _` \___ \ | |   __) |                    |
|                        | | (_| |___) || |  / __/                     |
|                        |_|\__,_|____/ |_| |_____|                    |
|                                                                      |
|                               core system                            |
|                                                    (C) SuSE Linux AG |
\----------------------------------------------------------------------/

  File:       _Handle.cc

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose: Handle/Body: Base for Handle classes manipuating a Body

/-*/

#include <iostream>

#include <y2util/_Handle.h>

using namespace std;

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : _Handle::dumpOn
//	METHOD TYPE : std::ostream &
//
std::ostream & _Handle::dumpOn( std::ostream & str ) const
{
  return str << _body;
}

/******************************************************************
**
**
**	FUNCTION NAME : operator<<
**	FUNCTION TYPE : ostream &
*/
ostream & operator<<( ostream & str, const _Handle & obj )
{
  return obj.dumpOn( str );
}

