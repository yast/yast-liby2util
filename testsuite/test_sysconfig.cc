#include <y2util/SysConfig.h>

#include <y2util/Y2SLog.h>
#include <y2util/Pathname.h>

#include <iostream>

using namespace std;

int main( int argc, char **argv )
{
  Y2Logging::setLogfileName("-");

  if ( argc != 2 ) {
    cerr << "Usage: test_sysconfig <filepath>" << endl;
    exit( 1 );
  }

  Pathname file = argv[ 1 ];
  D__ << file << endl;

  D__ << file << endl;
  
  SysConfig cfg( file );

#if 1

  string value;
  
  value = cfg.readEntry( "ONE" );
  cout << value << endl;
  
  value = cfg.readEntry( "TWO", "mydefault" );
  cout << value << endl;
  
  value = cfg.readBoolEntry( "TWO" );
  cout << value << endl;

  value = cfg.readIntEntry( "FOUR" );

  bool b;
  
  b = cfg.readBoolEntry( "BOOLONE" );
  cout << ( b ? "true" : "false" ) << endl;

  b = cfg.readBoolEntry( "BOOLTWO", true );
  cout << ( b ? "true" : "false" ) << endl;

  b = cfg.readBoolEntry( "BOOLTWO" );
  cout << ( b ? "true" : "false" ) << endl;

  int i;
  
  i = cfg.readIntEntry( "INTONE" );
  cout << i << endl;

  i = cfg.readIntEntry( "INTTWO", 13 );
  cout << i << endl;

  i = cfg.readIntEntry( "INTTWO" );
  cout << i << endl;

#endif

  cfg.writeEntry( "ONE", "Eins" );
  cfg.writeEntry( "TWO", true );
  cfg.writeEntry( "THREE", false );
  cfg.writeEntry( "FOUR", 4 );

  cfg.save();

  return 0;
}
