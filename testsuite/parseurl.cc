#include <iostream>
#include <string>

#include <y2util/Url.h>

using namespace std;

int main( int argc, char *argv[] )
{
  if ( argc < 2 ) {
    cerr << "Usage: parseurl <url>" << endl;
    return 1;
  }

  string u = argv[ 1 ];

  cout << "Input: " << u << endl;

  Url url( u );

  cout << "asString: " << url.asString() << endl;
  cout << "isValid: " << ( url.isValid() ? "true" : "false" ) << endl;
  cout << "protocol: " << url.getProtocol() << endl;
  cout << "host: " << url.getHost() << endl;
  cout << "port: " << url.getPort() << endl;
  cout << "path: " << url.getPath() << endl;

  return 0;
}
