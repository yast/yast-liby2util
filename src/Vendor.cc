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

   File:       Vendor.cc

   Author:     Michael Andres <ma@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

/-*/

#include <iostream>

#include <y2util/Vendor.h>

using namespace std;

///////////////////////////////////////////////////////////////////

UstringHash Vendor::_nameHash;

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : Vendor::Vendor
//	METHOD TYPE : Constructor
//
Vendor::Vendor( const std::string & n )
    : Ustring( _nameHash, n )
{
  _isSuSE = (    asString().substr( 0, 4 )  == "SuSE"
	      || asString().substr( 0, 4 )  == "SUSE" // hooray!
	      || asString().substr( 0, 11 ) == "UnitedLinux"
	      || asString().substr( 0, 6 )  == "Novell" );
}
