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

   File:       LangCode.h

   Author:     Michael Andres <ma@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

/-*/
#ifndef LangCode_h
#define LangCode_h

#include <iosfwd>

#include <y2util/Ustring.h>

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : LangCode
/*
 *
 **/
class LangCode : public Ustring {

  private:

    static UstringHash _nameHash;

  public:

    explicit LangCode( const std::string & n = "" ) : Ustring( _nameHash, n ) {}
};

///////////////////////////////////////////////////////////////////

#endif // LangCode_h
