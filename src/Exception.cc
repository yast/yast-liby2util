/*---------------------------------------------------------------------\
|                                                                      |
|                      __   __    ____ _____ ____                      |
|                      \ \ / /_ _/ ___|_   _|___ \                     |
|                       \ V / _` \___ \ | |   __) |                    |
|                        | | (_| |___) || |  / __/                     |
|                        |_|\__,_|____/ |_| |_____|                    |
|                                                                      |
|                               core system                            |
|                                         (C) SuSE Linux Products GmbH |
\----------------------------------------------------------------------/

  File:       Exception.cc

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose:

/-*/

#include <sstream>

#include "y2util/Exception.h"

using namespace std;

///////////////////////////////////////////////////////////////////
//
//	METHOD NAME : Exception::Exception
//	METHOD TYPE : Constructor
//
Exception::Exception( const std::string & msg_r ) throw()
: _msg( msg_r )
{}

///////////////////////////////////////////////////////////////////
//
//	METHOD NAME : Exception::~Exception
//	METHOD TYPE : Destructor
//
Exception::~Exception() throw()
{}

///////////////////////////////////////////////////////////////////
//
//	METHOD NAME : Exception::what
//	METHOD TYPE : const char *
//
const char*
Exception::what() const throw()
{
  return _msg.c_str();
}

///////////////////////////////////////////////////////////////////
//
//	METHOD NAME : Exception::where
//	METHOD TYPE : const Exception::location_type &
//
const Exception::location_type &
Exception::where() const throw()
{
  return _loc;
}

///////////////////////////////////////////////////////////////////
//
//	METHOD NAME : Exception::dumpOn
//	METHOD TYPE : std::ostream &
//
std::ostream &
Exception::dumpOn( std::ostream & str ) const
{
  return str << "EXCEPTION: " << what() << " AT " << verbose(where());
}

