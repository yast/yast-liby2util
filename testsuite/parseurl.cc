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
  cout << "isLocal: " << ( url.isLocal() ? "true" : "false" ) << endl;
  cout << "protocol: " << url.protocolString() << endl;
  cout << "host: " << url.host() << endl;
  cout << "port: " << url.port() << endl;
  cout << "path: " << url.path() << endl;

  cout << "options: ";
  Url::OptionMapType options = url.options();
  Url::OptionMapType::const_iterator it;
  for( it = options.begin(); it != options.end(); ++it ) {
    cout << it->first << "=" << it->second << ";";
  }
  cout << endl;

  return 0;
}
