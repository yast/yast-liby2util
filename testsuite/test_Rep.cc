#include <iomanip>
#include <list>
#include <string>

#include "test_rep/PtrHier.h"

int main()
{
  Y2SLog::setLogfileName("-");
  DBG << "START" << endl;

  PkgPtr p;
  PkgPtr p2( new Pkg );

  p = 0;
  p = new Pkg;

  DBG << "==================================" << endl;
  return 0;
}
