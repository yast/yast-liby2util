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

  File:       Date.h

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose: Store and operate on date (time_t).

/-*/
#ifndef _Date_h_
#define _Date_h_

#include <ctime>
#include <iosfwd>
#include <string>

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : Date
//
/**
 * @short Store and operate on date (time_t).
 **/
class Date {

  private:

    time_t _date;

  public:

    Date( time_t date_r = 0 ) {
      _date = date_r;
    }

    operator time_t() const { return _date; }
};

///////////////////////////////////////////////////////////////////

#endif // _Date_h_
