#include <iostream>

#include <y2util/GPGCheck.h>

using namespace std;

int main( int argc, const char **argv )
{
  if ( argc != 2 ) {
    cerr << "Usage: " << argv[0] << " <file>" << endl;
    exit( 1 );
  }
  
  string filename = argv[1];
  
  GPGCheck check;
  
  bool ok = check.check_file( filename );
  
  cout << filename << ": ";
  if ( ok ) cout << "OK";
  else cout << "FAILED";
  cout << endl;
}
