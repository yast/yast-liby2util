#include <iomanip>
#include <iostream>

#include <y2util/Y2SLog.h>
#include <y2util/BitField.h>

using namespace std;

void iter( const string & fs ) {
  BitField f( fs );
  DBG << f << endl;

  DBG << "  1fw:";
  for ( unsigned i = f.first( true ); i != BitField::npos; i = f.next( i, true ) ) {
    DBG << " " << i;
  }
  DBG << endl;

  DBG << "  1re:";
  for ( unsigned i = f.last( true ); i != BitField::npos; i = f.prev( i, true ) ) {
    DBG << " " << i;
  }
  DBG << endl;

  DBG << "  0fw:";
  for ( unsigned i = f.first( false ); i != BitField::npos; i = f.next( i, false ) ) {
    DBG << " " << i;
  }
  DBG << endl;

  DBG << "  0re:";
  for ( unsigned i = f.last( false ); i != BitField::npos; i = f.prev( i, false ) ) {
    DBG << " " << i;
  }
  DBG << endl;
  DBG << "================================" << endl;
}

#define XOUT( z ) t=a; \
DBG << " a = " << setw( 20 ) << a << " | b = " << setw( 20 ) << b << " | " \
  << setw( 20 ) << #z << " = " << setw( 20 ) << (z) \
  << " (" << setw( 20 ) << t << ")" << endl

int main ()
{
  Y2SLog::setLogfileName("-");
  DBG << "START" << endl;

  int fw = 6;
  int ow = 15;

#define TR(OP,R) \
  { \
    BitField a( "1100" ); \
    BitField b( "1010" ); \
    BitField t( a ); \
    DBG  \
      << " a = "   << setw( fw ) << a  \
      << " | b = " << setw( fw ) << b  \
      << " | "     << setw( ow ) << #OP << " = "; \
    OP; \
    DBG << (R);  \
    DBG << " ("      << setw( fw ) << t << ")" << endl; \
  }

#define TO(OP) \
  { \
    BitField a( "1100" ); \
    BitField b( "1010" ); \
    BitField t( a ); \
    DBG  \
      << " a = "   << setw( fw ) << a  \
      << " | b = " << setw( fw ) << b  \
      << " | "     << setw( ow ) << #OP << " = "; \
    DBG  << (OP);  \
    DBG  << " ("      << setw( fw ) << t << ")" << endl; \
  }

  TR( a.clear (), a );
  TR( a.set   (), a );
  TR( a.invert(), a );

  TR( a.clear (3), a );
  TR( a.set   (3), a );
  TR( a.invert(3), a );

  TR( a.clear (5), a );
  TR( a.set   (5), a );
  TR( a.invert(5), a );

  TR( a.clear (1,1), a );
  TR( a.set   (1,1), a );
  TR( a.invert(1,1), a );

  TR( a.clear (1,3), a );
  TR( a.set   (1,3), a );
  TR( a.invert(1,3), a );

  TR( a.clear (1,5), a );
  TR( a.set   (1,5), a );
  TR( a.invert(1,5), a );

  TR( a.clear (6,7), a );
  TR( a.set   (6,7), a );
  TR( a.invert(6,7), a );

  TR( a.clear (6,0), a );
  TR( a.set   (6,0), a );
  TR( a.invert(6,0), a );
  DBG << "===================================" << endl;
  TO( a.size() );
  TO( a.empty() );
  TO( a.count( true ));
  TO( a.count( false ));
  DBG << "===================================" << endl;
  TO( a & b );
  TO( a &= b );
  TO( a | b );
  TO( a |= b );
  TO( a ^ b );
  TO( a ^= b );
  TO( a - b );
  TO( a -= b );
  TO( ~a );
  TO( a.complement() );
  DBG << "===================================" << endl;
  TO( concat(a,b) );
  TO( a.append(b) );
  DBG << "===================================" << endl;
  TO( a.at(0,0) );
  TO( a.at(0,1) );
  TO( a.at(0,5) );
  TO( a.at(1,4) );
  TO( a.at(4,4) );
  TO( a.at(0,BitField::npos) );
  TO( a.at(3,BitField::npos) );
  TO( a.at(4,BitField::npos) );
  DBG << "===================================" << endl;
  TO( a.delat(0,0) );
  TO( a.delat(0,1) );
  TO( a.delat(0,5) );
  TO( a.delat(1,4) );
  TO( a.delat(4,4) );
  TO( a.delat(0,BitField::npos) );
  TO( a.delat(3,BitField::npos) );
  TO( a.delat(4,BitField::npos) );
  DBG << "===================================" << endl;
  TO( a.clipto(0,0) );
  TO( a.clipto(0,1) );
  TO( a.clipto(0,5) );
  TO( a.clipto(1,4) );
  TO( a.clipto(4,4) );
  TO( a.clipto(0,BitField::npos) );
  TO( a.clipto(3,BitField::npos) );
  TO( a.clipto(4,BitField::npos) );
  DBG << "===================================" << endl;
  DBG << "===================================" << endl;

  iter( "" );
  iter( "0" );
  iter( "1" );
  iter( "0000" );
  iter( "101010" );
  iter( "110011" );
#if 0
#endif

  return 0;
}

