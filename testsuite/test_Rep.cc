#include <iomanip>
#include <list>
#include <string>

#include "test_rep/PtrHier.h"

int main()
{
  Y2Logging::setLogfileName("-");
  DBG << "START" << endl;

  PkgPtr p;
  PkgPtr p2( new Pkg );

  p = 0;
  p = new Pkg;

  constPkgPtr c( new Pkg );
  DBG << "p " << p << endl;
  DBG << "c " << c << endl;
  p = PkgPtr::cast_away_const( p );
  DBG << "p " << p << endl;
  p = PkgPtr::cast_away_const( c );
  DBG << "p " << p << endl;

  DBG << "==================================" << endl;
  return 0;
}
