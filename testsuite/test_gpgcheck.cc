#include <iostream>

#include <y2util/GPGCheck.h>

using namespace std;

int main( int argc, const char **argv )
{
  if ( argc < 2 ) {
    cerr << "Usage: " << argv[0] << " <file> [destination]" << endl;
    exit( 1 );
  }
  
  string filename = argv[1];
  
  GPGCheck check;

  bool ok;
  if ( argc == 2 ) {
    ok = check.check_file( filename );
  } else {
    cout << "tick" << endl;
    string dest = argv[ 2 ];
    cout << "DEST: " << dest << endl;
    ok = check.check_file( filename, dest );
  }

  cout << filename << ": ";
  if ( ok ) cout << "OK";
  else cout << "FAILED";
  cout << endl;
}
