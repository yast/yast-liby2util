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

   File:       LangCode.cc

   Author:     Michael Andres <ma@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

/-*/

#include <iostream>

#include <y2util/LangCode.h>

using namespace std;

///////////////////////////////////////////////////////////////////

UstringHash LangCode::_nameHash;

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : LangCode::hasCountry
//	METHOD TYPE : bool
//
//	DESCRIPTION :
//
bool LangCode::hasCountry() const
{
  return( asString().size() > 2 );
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : LangCode::languageOnly
//	METHOD TYPE : LangCode
//
//	DESCRIPTION :
//
LangCode LangCode::languageOnly() const
{
  if ( asString().size() > 2 ) {
    return LangCode( asString().substr( 0, 2 ) );
  }
  return *this;
}

