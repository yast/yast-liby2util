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
 * @short Debug helper class to ship <CODE>__FILE__,
 * __(PRETTY_)FUNCTION__, __LINE__</CODE>. Use
 * <CODE>SOURCECODELOCATION</CODE> macro to create.
 **/
class SourceCodeLocation
{
  /**
   * Helper class for verbose stream output.
   **/
  struct _Verbose;

  /**
   * Helper wrapping SourceCodeLocation into a
   * SourceCodeLocation::_Verbose for stream output.
   **/
  friend SourceCodeLocation::_Verbose
  verbose( const SourceCodeLocation & obj_r );

  /**
   * Stream output as "__FILE__(__FUNCTION__):__LINE__"
   **/
  friend std::ostream &
  operator<<( std::ostream & str, const SourceCodeLocation & obj_r );

  /**
   * Stream output as "__FILE__(__PRETTY_FUNCTION__):__LINE__"
   * via <CODE>verbose(SOURCECODELOCATION)</CODE>
   **/
  friend std::ostream &
  operator<<( std::ostream & str, const SourceCodeLocation::_Verbose & obj_r );

  public:

    /**
     * Default ctor: unknown location.
     **/
    SourceCodeLocation()
    : _line( _unknown )
    {}

    /**
     * Ctor, use <CODE>SOURCECODELOCATION</CODE> macro for
     * convenient creation.
     **/
    SourceCodeLocation( const std::string & file_r,
                        const std::string & func_r,
                        const std::string & pretty_func_r,
                        unsigned            line_r )
    : _file( file_r ), _func( func_r ), _pretty_func( pretty_func_r ), _line( line_r )
    {}

    /**
     * @return NULL if unknown location
     **/
    operator const void *const() const
    { return (const void *const)(_line != _unknown); }

    /**
     * @return __FILE__
     **/
    const std::string &
    file() const
    { return _file; }

    /**
     * @return __FUNCTION__
     **/
    const std::string &
    func() const
    { return _func; }

    /**
     * @return __PRETTY_FUNCTION__
     **/
    const std::string &
    pretty_func() const
    { return _pretty_func; }

    /**
     * @return __LINE__
     **/
    unsigned
    line() const
    { return _line; }

  private:

    static const unsigned _unknown;

    std::string _file;
    std::string _func;
    std::string _pretty_func;
    unsigned    _line;
};
///////////////////////////////////////////////////////////////////

#define SOURCECODELOCATION SourceCodeLocation( __FILE__, __FUNCTION__, __PRETTY_FUNCTION__, __LINE__ )

///////////////////////////////////////////////////////////////////

/**
 * Helper class for verbose stream output. In fact nothing but a
 * SourceCodeLocation, but wraping allows to define an alternate
 * <CODE>ostream operator&lt;&lt;<CODE>.
 **/
struct SourceCodeLocation::_Verbose
{ SourceCodeLocation _obj; };

/**
 * Helper wrapping SourceCodeLocation into a
 * SourceCodeLocation::_Verbose for stream output.
 **/
inline SourceCodeLocation::_Verbose
verbose( const SourceCodeLocation & obj_r )
{ SourceCodeLocation::_Verbose obj; obj._obj = obj_r; return obj; }

///////////////////////////////////////////////////////////////////
#endif // SourceCodeLocation_h
