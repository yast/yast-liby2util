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

   File:       Vendor.h

   Author:     Michael Andres <ma@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

/-*/
#ifndef Vendor_h
#define Vendor_h

#include <iosfwd>

#include <y2util/Ustring.h>

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : Vendor
/*
 *
 **/
class Vendor : public Ustring {

  private:

    static UstringHash _nameHash;

  public:

    explicit Vendor( const std::string & n = "" ) : Ustring( _nameHash, n ) {}
};

///////////////////////////////////////////////////////////////////

#endif // Vendor_h
