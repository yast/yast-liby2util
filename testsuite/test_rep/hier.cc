#include "PtrHier.h"

#undef Y2LOG
#define Y2LOG "TEST"

#ifdef FAILCOMPILE
#  define FAIL_OUT(x) DBG << #x"=F " << (x=F) << endl
#else
#  define FAIL_OUT(x)
#endif

#define OUT(x) \
  DBG << "------------------------" << endl; \
  FAIL_OUT( x ); \
  DBG << #x"=B " << (x=B) << endl; \
  DBG << #x"=O " << (x=O) << endl; \
  DBG << #x"=P " << (x=P) << endl; \
  DBG << #x"=S " << (x=S) << endl; \
  DBG << "------------------------" << endl; \
  x = 0;

///////////////////////////////////////////////////////////////////
int main()
{
  set_log_filename("-");
  DBG << "START=================================" << endl;
  ForPtr F( new For );
  BsePtr B( new Bse );
  ObjPtr O( new Obj );
  PkgPtr P( new Pkg );
  SelPtr S( new Sel );

  ForPtr f;
  BsePtr b;
  ObjPtr o;
  PkgPtr p;
  SelPtr s;

#ifdef FAILCOMPILE
  OUT( f );
#endif
  OUT( b );
  OUT( o );
  OUT( p );
  OUT( s );

  DBG << "END===================================" << endl;
  return 0;
}

