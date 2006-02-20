#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <vector>

#include <y2util/PathInfo.h>
#include <y2util/Y2SLog.h>

#include <y2util/GPGCheck.h>
#include <y2util/ExternalProgram.h>

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

static int run_gpg(std::vector<const char*> more_args, const std::string& gnupghome, const std::string& keyring)
{
  std::vector<const char*> args;

  args.push_back("/usr/bin/gpg");
  if ( !gnupghome.empty() )
  {
    args.push_back("--homedir");
    args.push_back(gnupghome.c_str());
  }
  args.push_back("--no-default-keyring");
  args.push_back("--keyring");
  args.push_back(keyring.c_str());
  copy(more_args.begin(), more_args.end(), back_inserter(args));

  const char * argv[args.size() + 1];
  const char ** p = argv;
  p = copy( args.begin(), args.end(), p );
  *p = 0;

  ExternalProgram::ExternalProgram gpg( argv, ExternalProgram::Discard_Stderr );
  return gpg.close();
}


bool
GPGCheck::check_file ( const Pathname& filename, bool strip )
{
  int ret;

  if ( strip ) {
    return check_file( filename, filename + ".strip" );
  } else {
    // only verify
    vector<const char*> args;
    args.push_back( "--verify");
    args.push_back(filename.asString().c_str());

    ret = run_gpg(args, _gnupghome, _keyring);
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

  ret = check_file (sourceFile, false);
  if (!ret)
    return ret;

  vector<const char*> args;
  args.push_back("-o");
  args.push_back(destFile.asString().c_str());
  args.push_back(sourceFile.asString().c_str());

  ret = run_gpg(args, _gnupghome, _keyring);

  return ret == 0;
}
