#include <stdlib.h>
#include <unistd.h>

#include <iostream>

#include <y2util/GPGCheck.h>

using namespace std;

GPGCheck::GPGCheck ()
    : keyring ("/usr/lib/rpm/gnupg/pubring.gpg")
{
}


void
GPGCheck::set_keyring (const string& keyring)
{
    GPGCheck::keyring = keyring;
}


bool
GPGCheck::check_file (const string& filename, bool strip)
{
  int ret;

  if ( strip ) {
    // verify and strip of gpg lines

    string filename_out = filename + ".strip";
    unlink (filename_out.c_str ());

    string cmd = "/usr/bin/gpg --no-default-keyring --keyring "
	+ keyring + " -o " + filename_out + " " + filename
        + " 2>/dev/null >/dev/null";
    ret = system (cmd.c_str ());
  } else {
    // only verify

    string cmd = "/usr/bin/gpg --verify --no-default-keyring --keyring "
	+ keyring + " " + filename + " 2>/dev/null >/dev/null";
    ret = system (cmd.c_str ());
  }

  return ret == 0;
}
