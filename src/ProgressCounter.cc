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

  File:       ProgressCounter.cc

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose:

/-*/

#include <iostream>

#include <y2util/ProgressCounter.h>

using namespace std;

/******************************************************************
**
**
**	FUNCTION NAME : operator<<
**	FUNCTION TYPE : ostream &
*/
ostream & operator<<( ostream & str, const ProgressData & obj )
{
  return str << "PD{" << obj.val() << "[" << obj.min() << ":" << obj.max() << "]}";
}

/******************************************************************
**
**
**	FUNCTION NAME : operator<<
**	FUNCTION TYPE : ostream &
*/
ostream & operator<<( ostream & str, const ProgressCounter & obj )
{
  return str << "PC{" << (obj.newPercent() ? "*" : "") << obj.percent() << '%'
	       << " " << (obj.newVal() ? "*" : "") << obj.val()
	       << "[" << (obj.newMin() ? "*" : "") << obj.min()
	       << ":" << (obj.newMax() ? "*" : "") << obj.max()
	       << "]}";
}

