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

  File:       FSize.h

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose: Store and operate on (file/package/partition) sizes.

/-*/
#ifndef _FSize_h_
#define _FSize_h_

#include <iosfwd>
#include <string>

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : FSize
//
/**
 * @short Store and operate on (file/package/partition) sizes.
 **/
class FSize {

  public:

    enum Unit { B = 0, K, M, G, T };

  private:

    long long _size;

  public:

    FSize( long long size_r, const Unit unit_r = B  ) {
      _size = size_r; // preliminarily
    }

    operator long long() const { return _size; }
};

///////////////////////////////////////////////////////////////////

#endif // _FSize_h_
