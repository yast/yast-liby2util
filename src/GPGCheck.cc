#include <stdlib.h>
#include <unistd.h>

#include <iostream>

#include <y2util/PathInfo.h>
#include <y2util/Y2SLog.h>

#include <y2util/GPGCheck.h>

using namespace std;

GPGCheck::GPGCheck ()
    : _keyring ("/usr/lib/rpm/gnupg/pubring.gpg")
{
  if ( getuid() == 0 ) {
    _gnupghome = "/var/lib/YaST2/gnupg";
  } else {
    string home = getenv( "HOME" );
    _gnupghome = home + "/.yast2/gnupg";
  }

  int error = PathInfo::assert_dir( _gnupghome );
  if ( error ) {
    ERR << "Error creating directory '" << _gnupghome << "': " << error
        << endl;
  } else {
    DBG << "Directory '" << _gnupghome << "' exists." << endl;
  }
}


void
GPGCheck::set_keyring ( const Pathname& keyring )
{
    _keyring = keyring.asString();
}


bool
GPGCheck::check_file ( const Pathname& filename, bool strip )
{
  int ret;

  if ( strip ) {
    return check_file( filename, filename + ".strip" );
  } else {
    // only verify

    string cmd = assembleCommand( "--verify " + filename.asString() );
    ret = system( cmd.c_str () );
  }

  return ret == 0;
}

bool GPGCheck::check_file( const Pathname &sourceFile,
                           const Pathname &destFile )
{
  int ret = PathInfo::unlink( destFile );

  if ( ret != 0 && ret != ENOENT ) {
    return false;
  }

  string cmd = assembleCommand( "-o " + destFile.asString() + " " +
                                sourceFile.asString() );

  ret = system( cmd.c_str() );

  return ret == 0;
}

string GPGCheck::assembleCommand( const string &args )
{
  string cmd = "/usr/bin/gpg";
  cmd += " 2>/dev/null >/dev/null";
  if ( !_gnupghome.empty() ) cmd += " --homedir " + _gnupghome;
  cmd += " --no-default-keyring";
  cmd += " --keyring " + _keyring;
  cmd += " " + args;

  D__ << cmd << endl;

  return cmd;
}
