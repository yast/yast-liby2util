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

  File:       Exception.h

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose:

/-*/
#ifndef Exception_h
#define Exception_h

#include <iosfwd>
#include <stdexcept>

#include <y2util/SourceCodeLocation.h>

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : Exception
/**
 * @short Base class for exceptions, stores message and source code location.
 *
 * Fore convenience use macro <CODE>THROW( Exception("message") );</CODE>.
 * It automatically stores SOURCECODELOCATION. See class @ref SourceCodeLocation.
 **/
class Exception : public std::exception
{
  /**
   * Stream output as ""EXCEPTION: 'what()' AT 'where()'"
   **/
  friend std::ostream &
  operator<<( std::ostream & str, const Exception & obj_r )
  { return obj_r.dumpOn( str ); }

  public:

    typedef SourceCodeLocation location_type;

    /**
     * Ctor: stores message
     **/
    explicit
    Exception( const std::string & msg_r ) throw();

    virtual
    ~Exception() throw();

    /**
     * @return Message strored in ctor. Overloads
     * std::exception::what.
     **/
    virtual const char *
    what() const throw();

    /**
     * @return Source code location if provided.
     **/
    const location_type &
    where() const throw();

    /**
     * Set source code location.
     **/
    void setLocation( const location_type & loc_r ) const throw()
    { _loc = loc_r; }

    /**
     * Stream output, used by the default std::ostream::operator<<.
     **/
    virtual std::ostream &
    dumpOn( std::ostream & str ) const;

  private:

    std::string           _msg;
    mutable location_type _loc;
};
///////////////////////////////////////////////////////////////////

#define THROW(_Ex) Throw( _Ex, SOURCECODELOCATION )

template<typename _Ex>
  inline void Throw( const _Ex & exception_r, const Exception::location_type & loc_r )
  { exception_r.setLocation( loc_r ); throw exception_r; }

///////////////////////////////////////////////////////////////////

#endif // Exception_h
