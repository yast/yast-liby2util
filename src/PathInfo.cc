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

   File:       PathInfo.cc

   Author:     Michael Andres <ma@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

/-*/

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include <iostream>
#include <iomanip>

#include <y2util/Y2SLog.h>
#include <y2util/stringutil.h>
#include <y2util/ExternalProgram.h>

#include <y2util/PathInfo.h>

using namespace std;

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : PathInfo::PathInfo
//	METHOD TYPE : Constructor
//
//	DESCRIPTION :
//
PathInfo::PathInfo( const Pathname & path, Mode initial )
    : path_t( path )
    , mode_e( initial )
    , error_i( -1 )
{
  operator()();
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : PathInfo::PathInfo
//	METHOD TYPE : Constructor
//
//	DESCRIPTION :
//
PathInfo::PathInfo( const string & path, Mode initial )
    : path_t( path )
    , mode_e( initial )
    , error_i( -1 )
{
  operator()();
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : PathInfo::PathInfo
//	METHOD TYPE : Constructor
//
//	DESCRIPTION :
//
PathInfo::PathInfo( const char * path, Mode initial )
    : path_t( path )
    , mode_e( initial )
    , error_i( -1 )
{
  operator()();
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : PathInfo::~PathInfo
//	METHOD TYPE : Destructor
//
//	DESCRIPTION :
//
PathInfo::~PathInfo()
{
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : PathInfo::operator()
//	METHOD TYPE : bool
//
//	DESCRIPTION :
//
bool PathInfo::operator()()
{
  if ( path_t.empty() ) {
    error_i = -1;
  } else {
    switch ( mode_e ) {
    case STAT:
      error_i = ::stat( path_t.asString().c_str(), &statbuf_C );
      break;
    case LSTAT:
      error_i = ::lstat( path_t.asString().c_str(), &statbuf_C );
      break;
    }
    if ( error_i == -1 )
      error_i = errno;
  }
  return !error_i;
}

/******************************************************************
**
**
**	FUNCTION NAME : operator<<
**	FUNCTION TYPE : ostream &
**
**	DESCRIPTION :
*/
ostream & operator<<( ostream & str, const PathInfo & obj )
{
  ios::fmtflags state_ii = str.flags();

  str << obj.asString() << "{";
  if ( !obj.isExist() ) {
    str << "does not exist}";
  } else {
    char t = '?';
    if ( obj.isFile() )
      t = '-';
    else if ( obj.isDir() )
      t = 'd';
    else if ( obj.isLink() )
      t = 'l';
    else if ( obj.isChr() )
      t = 'c';
    else if ( obj.isBlk() )
      t = 'b';
    else if ( obj.isFifo() )
      t = 'p';
    else if ( obj.isSock() )
      t = 's';
    str << t
      << " " << setfill( '0' ) << setw( 4 ) << oct << obj.perm()
      << " " << dec << obj.owner() << "/" << obj.group();

    if ( obj.isFile() )
      str << " size " << obj.size();

    str << "}";
  }
  str.flags( state_ii );
  return str;
}


/******************************************************************
**
**
**	FUNCTION NAME : _Log_Result
**	FUNCTION TYPE : int
**
**	DESCRIPTION : Helper function to log return values.
*/
inline int _Log_Result( const int res, const char * rclass = "errno" )
{
  if ( res )
    DBG << " FAILED: " << rclass << " " << res;
  DBG << endl;
  return res;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : PathInfo::mkdir
//	METHOD TYPE : int
//
//	DESCRIPTION :
//
int PathInfo::mkdir( const Pathname & path, unsigned mode )
{
  DBG << "mkdir " << path << ' ' << stringutil::octstring( mode );
  if ( ::mkdir( path.asString().c_str(), mode ) == -1 ) {
    return _Log_Result( errno );
  }
  return _Log_Result( 0 );
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : PathInfo::assert_dir()
//	METHOD TYPE : int
//
//	DESCRIPTION :
//
int PathInfo::assert_dir( const Pathname & path, unsigned mode )
{
    string::size_type pos, lastpos = 0;
    string spath = path.asString()+"/";
    int ret = 0;

    if(path.empty())
	return ENOENT;

    // skip ./
    if(path.relative())
	lastpos=2;
    // skip /
    else
	lastpos=1;

    DBG << "about to create " << spath << endl;
    while((pos = spath.find('/',lastpos)) != string::npos )
    {
	string dir = spath.substr(0,pos);
	ret = ::mkdir(dir.c_str(), mode);
	if(ret == -1)
	{
	    // ignore errors about already existing directorys
	    if(errno == EEXIST)
		ret=0;
	    else
		ret=errno;
	}
	DBG << "creating directory " << dir << (ret?" failed":" succeeded") << endl;
	lastpos = pos+1;
    }
    return ret;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : PathInfo::rmdir
//	METHOD TYPE : int
//
//	DESCRIPTION :
//
int PathInfo::rmdir( const Pathname & path )
{
  DBG << "rmdir " << path;
  if ( ::rmdir( path.asString().c_str() ) == -1 ) {
    return _Log_Result( errno );
  }
  return _Log_Result( 0 );
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : PathInfo::recursive_rmdir
//	METHOD TYPE : int
//
//	DESCRIPTION :
//
int PathInfo::recursive_rmdir( const Pathname & path )
{
  DBG << "recursive_rmdir " << path << ' ';
  PathInfo p( path );

  if ( !p.isExist() ) {
    return _Log_Result( 0 );
  }

  if ( !p.isDir() ) {
    return _Log_Result( ENOTDIR );
  }

  string cmd( stringutil::form( "rm -r '%s'", path.asString().c_str() ) );
  ExternalProgram prog( cmd, ExternalProgram::Stderr_To_Stdout );
  for ( string output( prog.receiveLine() ); output.length(); output = prog.receiveLine() ) {
    DBG << "  " << output;
  }
  int ret = prog.close();
  return ret;
}

