/*---------------------------------------------------------------------\
|                                                                      |
|                      __   __    ____ _____ ____                      |
|                      \ \ / /_ _/ ___|_   _|___ \                     |
|                       \ V / _` \___ \ | |   __) |                    |
|                        | | (_| |___) || |  / __/                     |
|                        |_|\__,_|____/ |_| |_____|                    |
|                                                                      |
|                               core system                            |
|                                                    (C) SuSE Linux AG |
\----------------------------------------------------------------------/

   File:       SysConfig.cc

   Author:     Cornelius Schumacher <cschum@suse.de>
   Maintainer: Cornelius Schumacher <cschum@suse.de>

/-*/

#include <y2util/SysConfig.h>

#include <y2util/Y2SLog.h>
#include <y2util/stringutil.h>

#include <fstream>

#include <stdlib.h>

using namespace std;

SysConfig::SysConfig( const char *path )
  : _path( path )
{
  load();
}

SysConfig::SysConfig( const string &path )
  : _path( path )
{
  load();
}

SysConfig::SysConfig( const Pathname &path )
  : _path( path )
{
  load();
}

bool SysConfig::load()
{
  if ( _path.relative() ) {
    _path = "/etc/sysconfig" + _path;
  }

  D__ << "Load '" << _path << "'" << endl;

  string line;
  ifstream in( _path.asString().c_str() );
  if ( in.fail() ) {
    ERR << "Unable to load '" << _path << "'" << endl;
    return false;
  }
  while( getline( in, line ) ) {
    if ( *line.begin() != '#' ) {
      string::size_type pos = line.find( '=', 0 );
      if ( pos != string::npos ) {
        string key = stringutil::trim( line.substr( 0, pos ) );
        string value = stringutil::trim( line.substr( pos + 1, line.length() - pos - 1 ) );
        if ( value.length() >= 2 && *(value.begin()) == '"' &&
             *(value.rbegin()) == '"' ) {
          value = value.substr( 1, value.length() - 2 ); 
        }
        if ( value.length() >= 2 && *(value.begin()) == '\'' &&
             *(value.rbegin()) == '\'' ) {
          value = value.substr( 1, value.length() - 2 ); 
        }
        D__ << "KEY: '" << key << "' VALUE: '" << value << "'" << endl;
        
        _entryMap[ key ] = value;
      }
    }
  }

  return true;
}

string SysConfig::readEntry( const string &key, const string &defaultValue )
{
  EntryMap::iterator it = _entryMap.find( key );
  if ( it == _entryMap.end() ) return defaultValue;
  else return it->second;
}

bool SysConfig::readBoolEntry( const string &key, bool defaultValue )
{
  EntryMap::iterator it = _entryMap.find( key );
  if ( it == _entryMap.end() ) return defaultValue;
  
  string value = stringutil::toLower( it->second );

  if ( value == "yes" || value == "1" || value == "true" ) return true;
  
  return false;
}

int SysConfig::readIntEntry( const string &key, int defaultValue )
{
  EntryMap::iterator it = _entryMap.find( key );
  if ( it == _entryMap.end() ) return defaultValue;
  
  return atoi( it->second.c_str() );  
}
