#include <stdlib.h>
#include <unistd.h>

#include <iostream>

#include <y2util/PathInfo.h>

#include <y2util/GPGCheck.h>

using namespace std;

GPGCheck::GPGCheck ()
    : _keyring ("/usr/lib/rpm/gnupg/pubring.gpg"),
      _gnupghome( "/var/lib/YaST2/.gnupg" )
{
  PathInfo p( _gnupghome + "/options" );
  if ( !p.isExist() ) {
    system( assembleCommand( "dummy" ).c_str() );
  }
}


void
GPGCheck::set_keyring (const string& keyring)
{
    _keyring = keyring;
}


bool
GPGCheck::check_file (const string& filename, bool strip)
{
  int ret;

  if ( strip ) {
    return check_file( filename, filename + ".strip" );
  } else {
    // only verify

    string cmd = assembleCommand( "--verify " + filename );
    ret = system( cmd.c_str () );
  }

  return ret == 0;
}

bool GPGCheck::check_file( const string &sourceFile, const string &destFile )
{
  PathInfo::unlink( destFile );

  string cmd = assembleCommand( "-o " + destFile + " " + sourceFile );

  cerr << cmd << endl;

  int ret = system( cmd.c_str() );

  return ret == 0;
}

string GPGCheck::assembleCommand( const string &args )
{
  string cmd = "/usr/bin/gpg";
  cmd += " 2>/dev/null >/dev/null";
  cmd += " --homedir " + _gnupghome;
  cmd += " --no-default-keyring";
  cmd += " --keyring " + _keyring;
  cmd += " " + args;

  return cmd;
}
