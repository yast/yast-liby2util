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

  File:       _Body.cc

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose: Handle/Body: Base for refcounted representation classes (Body)

/-*/

#include <iostream>

#include <y2util/_Body.h>

using namespace std;

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : _Body::dumpOn
//	METHOD TYPE : std::ostream &
//
std::ostream & _Body::dumpOn( std::ostream & str ) const
{
  return str << "_(" << (const void *)this << "<=" << _counter << ')';
}

/******************************************************************
**
**
**	FUNCTION NAME : operator<<
**	FUNCTION TYPE : ostream &
*/
ostream & operator<<( ostream & str, const _Body & obj )
{
  return obj.dumpOn( str );
}

/******************************************************************
**
**
**	FUNCTION NAME : operator<<
**	FUNCTION TYPE : ostream &
*/
ostream & operator<<( ostream & str, const _Body * obj )
{
  if ( obj )
    return obj->dumpOn( str );
  return str << "(NULL)";
}

