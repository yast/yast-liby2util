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

  File:       SourceCodeLocation.h

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose:

/-*/
#ifndef SourceCodeLocation_h
#define SourceCodeLocation_h

#include <iosfwd>
#include <string>

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : SourceCodeLocation
/**
 * @short Debug helper class to ship __FILE__, __FUNCTION__, __LINE__
 * Usually created using the SOURCECODELOCATION define.
 **/
class SourceCodeLocation
{
  public:

    SourceCodeLocation()
    : _line( _unknown )
    {}

    SourceCodeLocation( const std::string & file_r,
                        const std::string & func_r,
                        unsigned            line_r )
    : _file( file_r ), _func( func_r ), _line( line_r )
    {}

    operator const void *const() const
    { return (const void *const)(_line != _unknown); }

    const std::string &
    file() const
    { return _file; }

    const std::string &
    func() const
    { return _func; }

    unsigned
    line() const
    { return _line; }

  private:

    static const unsigned _unknown;

    std::string _file;
    std::string _func;
    unsigned    _line;
};
///////////////////////////////////////////////////////////////////

#define SOURCECODELOCATION SourceCodeLocation( __FILE__, __FUNCTION__, __LINE__ )

///////////////////////////////////////////////////////////////////

/**
 * Stream output as "__FILE__(__FUNCTION__):__LINE__"
 **/
extern std::ostream &
operator<<( std::ostream & str, const SourceCodeLocation & obj_r );

///////////////////////////////////////////////////////////////////

#endif // SourceCodeLocation_h
