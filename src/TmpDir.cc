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

  File:       TmpDir.cc

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose:

/-*/

#include <cstdlib>
#include <cstring>
#include <cerrno>

#include <iostream>

#include <y2util/Y2SLog.h>
#include <y2util/PathInfo.h>
#include <y2util/TmpDir.h>

using namespace std;

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : TmpDir::Impl
/**
 * Clean or delete a directory on destruction.
 **/
class TmpDir::Impl :public Rep {

  public:

    Impl()
    {}

    Impl( const Pathname & path_r, bool keepDir_r )
    : _path( path_r ), _keepDir( keepDir_r )
    {}

    ~Impl()
    {
      if ( _path.empty() )
        return;

      int res = 0;
      if ( _keepDir )
         res = PathInfo::clean_dir( _path );
      else
        res = PathInfo::recursive_rmdir( _path );

      if ( res )
        {
          INT << "Failed to clean up TmpDir (" << res << ") " << PathInfo(_path) << endl;
        }
    }

    const Pathname &
    path() const
    { return _path; }

  private:
    Pathname _path;
    bool     _keepDir;
};
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : TmpDir
//
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	METHOD NAME : TmpDir::TmpDir
//	METHOD TYPE : Constructor
//
TmpDir::TmpDir( const Pathname & inParentDir_r,
                const std::string & prefix_r )
{
  // parent dir must exist
  PathInfo p( inParentDir_r );
  if ( ! p.isDir() )
    {
      ERR << "Parent directory does not exist: " << p << endl;
      return;
    }

  // create the temp dir
  Pathname tmpDir = (inParentDir_r + prefix_r).extend( "XXXXXX");
  char * buf = ::strdup( tmpDir.asString().c_str() );
  if ( ! buf )
    {
      ERR << "Out of memory" << endl;
      return;
    }

  char * tmp = ::mkdtemp( buf );
  if ( tmp )
    // success
    _impl = makeVarPtr( new Impl( tmp, /*keepDir*/false ) );
  else
    ERR << "Cant create '" << inParentDir_r << "' "
    << ::strerror( errno ) << endl;

  ::free( buf );
}

///////////////////////////////////////////////////////////////////
//
//	METHOD NAME : TmpDir::TmpDir
//	METHOD TYPE : Constructor
//
TmpDir::TmpDir( int )
{}

///////////////////////////////////////////////////////////////////
//
//	METHOD NAME : TmpDir::operator const void *const
//	METHOD TYPE :
//
TmpDir::operator const void *const() const
{
  return _impl;
}

///////////////////////////////////////////////////////////////////
//
//	METHOD NAME : TmpDir::path
//	METHOD TYPE : Pathname
//
Pathname
TmpDir::path() const
{
  return _impl ? _impl->path() : Pathname();
}

///////////////////////////////////////////////////////////////////
//
//	METHOD NAME : TmpDir::defaultLocation
//	METHOD TYPE : const Pathname &
//
const Pathname &
TmpDir::defaultLocation()
{
  static Pathname p( "/var/tmp" );
  return p;
}

///////////////////////////////////////////////////////////////////
//
//	METHOD NAME : TmpDir::defaultPrefix
//	METHOD TYPE : const std::string &
//
const std::string &
TmpDir::defaultPrefix()
{
  static string p( "TmpDir." );
  return p;
}
