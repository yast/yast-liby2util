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

   File:       Y2UTIL.h

   Author:     Michael Andres <ma@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

/-*/
#ifndef Y2UTIL_h
#define Y2UTIL_h

#include <iosfwd>

#ifdef DEBUG
#  include<iostream>
#  define debug(expr)	do { std::cout << __PRETTY_FUNCTION__ << ": " << expr << std::endl; } while(0)
#else
#  define debug(expr)
#endif

#endif // Y2UTIL_h
