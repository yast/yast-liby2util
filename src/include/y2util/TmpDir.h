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

  File:       TmpDir.h

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose: Provide a newly created temporary directory, automaticaly
           deleted when it's no longer needed.

/-*/
#ifndef TmpDir_h
#define TmpDir_h

#include <iosfwd>

#include <y2util/Rep.h>
#include <y2util/Pathname.h>

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : TmpDir
/**
 * @short Provide a newly created temporary directory, automaticaly
 * deleted when it's no longer needed.
 *
 * The temporary directory is per default created in @ref defaultLocation
 * (/var/tmp), is named 'TmpDir.XXXXXX' with premission 0700
 * (see manual page mkdtemp(3)). Different location and prefix may be
 * passed to the ctor.
 *
 * The directory in which TmpDir is created must exist. If the temporary
 * directory can not be created, TmpDir resolves into an empty Pathname.
 *
 * Multiple TmpDir instances created by copy and assign, share the same
 * reference counted internal repesentation. The temporary directory is
 * recursively deleted, when the last reference drops.
 **/
class TmpDir {

  public:
    /**
     * Ctor. Create a temporary directory in inParentDir.
     * Let the directory name start with prefix.
     * See @ref defaultLocation and @ref defaultPrefix.
     **/
    explicit
    TmpDir( const Pathname & inParentDir_r = defaultLocation(),
            const std::string & prefix_r = defaultPrefix() );

    /**
     * Test whether the directory is usable.
     **/
    operator const void *const() const;

    /**
     * @return The directories path.
     **/
    Pathname
    path() const;

    /**
     * Type conversion to Pathname. See @ref path.
     **/
    operator Pathname() const
    { return path(); }

  public:

    /**
     * @return The default location where temporary directories
     * are created (/var/tmp)
     **/
    static const Pathname &
    defaultLocation();

    /**
     * @return The default prefix for temporary directories (TmpDir.)
     **/
    static const std::string &
    defaultPrefix();

  protected:

    /**
     * Ctor for derived classes. Does nothing.
     **/
    explicit
    TmpDir( int );

    class Impl;
    VarPtr<Impl> _impl;
};
///////////////////////////////////////////////////////////////////

/**
 * Stream output as pathname.
 **/
inline std::ostream &
operator<<( std::ostream & str, const TmpDir & obj )
{ return str << static_cast<Pathname>(obj); }

///////////////////////////////////////////////////////////////////

#endif // TmpDir_h
