#include <iomanip>
#include <list>
#include <string>

#include <y2util/Y2SLog.h>
#include <y2util/RepDef.h>

using namespace std;

//---------------------------------------------------

DEFINE_BASIC_HANDLES(Slv);

class REP_CLASS(Slv) {
  REP_BODY(Slv)
    public:
    Slv(){}
};

IMPL_HANDLES(Slv);


DEFINE_DERIVED_HANDLES(Obj,public,Slv);

class REP_CLASS(Obj), public Slv {
  REP_BODY(Obj)
    public:
    Obj(){}

    ObjPtr getPtr();
};

ObjPtr Obj::getPtr() {
  ObjPtr r;
  r = this;
  DBG << "r=" << r << endl;
  DBG << "r==this" << (r==this) << endl;
  DBG << "r==(void*)this" << (r==(void*)this) << endl;
  DBG << "r==(void*)(Rep*)this" << (r==(void*)(Rep*)this) << endl;
  return this;
}

IMPL_HANDLES(Obj);


DEFINE_DERIVED_HANDLES(Pak,public,Obj);

class REP_CLASS(Pak), public Obj {
  REP_BODY(Pak)
    public:
    Pak(){}
};

IMPL_HANDLES(Pak);

//---------------------------------------------------
DEFINE_BASIC_HANDLES(M);

class REP_CLASS(M) {
  REP_BODY(M)

  public:
    int i;
    M( int ii ) {i=ii;}
    void fnc() {}
    void cfnc() const {}
};

IMPL_HANDLES(M);
//---------------------------------------------------
//---------------------------------------------------
DEFINE_BASIC_HANDLES(N);

static list<NPtr> lst;

class REP_CLASS(N) {
  REP_BODY(N)

  public:

    N( int ii ) { }
};

IMPL_HANDLES(N);
//---------------------------------------------------
//---------------------------------------------------
DEFINE_DOUBLE_DERIVED_HANDLES(MN, public, N, public, M );

class REP_CLASS(MN), public N, public M {
  REP_BODY(MN)

  public:

    MN( int ii ) : N(ii), M(-ii) {}
};

IMPL_HANDLES(MN);
//---------------------------------------------------

//#define M MN
//#define MPtr MNPtr
//#define constMPtr constMNPtr


void fncP   ( MPtr            P    ) {}
void fncc_P ( const MPtr      c_P  ) {}
void fncCP  ( constMPtr       CP   ) {}
void fncc_CP( const constMPtr c_CP ) {}

void rfncP   ( MPtr            & P    ) {}
void rfncc_P ( const MPtr      & c_P  ) {}
void rfncCP  ( constMPtr       & CP   ) {}
void rfncc_CP( const constMPtr & c_CP ) {}

int main()
{
  Y2SLog::setLogfileName("-");
  DBG << "START" << endl;

  ObjPtr x( new Obj );
  DBG << x << endl;
  ObjPtr y( x->getPtr() );
  DBG << y << endl;
  return 0;


#if 0
#define TT(X) INT << setw(5) << #X << " " << sizeof( X ) << endl
  TT( SlvPtr );
  TT( ObjPtr );
  TT( PakPtr );
  TT( constSlvPtr );
  TT( constObjPtr );
  TT( constPakPtr );

  return 0;
#endif

  MPtr            P   ( new M( 1 ) );
  const MPtr      c_P ( new M( 1 ) );
  constMPtr       CP  ( new M( 1 ) );
  const constMPtr c_CP( new M( 1 ) );

#define DOTEST(T,V) T v##V( V );
  //	// must fail
  {
    DOTEST( MPtr, P    );
    DOTEST( MPtr, c_P  );
#if 0
    DOTEST( MPtr, CP   );	// must fail
    DOTEST( MPtr, c_CP );	// must fail
#endif
  }
  {
    DOTEST( const MPtr, P    );
    DOTEST( const MPtr, c_P  );
#if 0
    DOTEST( const MPtr, CP   );	// must fail
    DOTEST( const MPtr, c_CP );	// must fail
#endif
  }
  {
    DOTEST( constMPtr, P    );
    DOTEST( constMPtr, c_P  );
    DOTEST( constMPtr, CP   );
    DOTEST( constMPtr, c_CP );
  }
  {
    DOTEST( const constMPtr, P    );
    DOTEST( const constMPtr, c_P  );
    DOTEST( const constMPtr, CP   );
    DOTEST( const constMPtr, c_CP );
  }

#undef DOTEST
#define DOTEST(T,V) T v##V; v##V = V;

  {
    DOTEST( MPtr, P    );
    DOTEST( MPtr, c_P  );
#if 0
    DOTEST( MPtr, CP   );	// must fail
    DOTEST( MPtr, c_CP );	// must fail
#endif
  }
  {
#if 0
    DOTEST( const MPtr, P    );	// must fail
    DOTEST( const MPtr, c_P  );	// must fail
    DOTEST( const MPtr, CP   );	// must fail
    DOTEST( const MPtr, c_CP );	// must fail
#endif
  }
  {
    DOTEST( constMPtr, P    );
    DOTEST( constMPtr, c_P  );
    DOTEST( constMPtr, CP   );
    DOTEST( constMPtr, c_CP );
  }
  {
#if 0
    DOTEST( const constMPtr, P    );	// must fail
    DOTEST( const constMPtr, c_P  );	// must fail
    DOTEST( const constMPtr, CP   );	// must fail
    DOTEST( const constMPtr, c_CP );	// must fail
#endif
  }

#undef DOTEST
#define DOTEST(T,V) T( V );

  {
    DOTEST( fncP, P    );
    DOTEST( fncP, c_P  );
#if 0
    DOTEST( fncP, CP   );	// must fail
    DOTEST( fncP, c_CP );	// must fail
#endif
  }
  {
    DOTEST( fncc_P, P    );
    DOTEST( fncc_P, c_P  );
#if 0
    DOTEST( fncc_P, CP   );	// must fail
    DOTEST( fncc_P, c_CP );	// must fail
#endif
  }
  {
    DOTEST( fncCP, P    );
    DOTEST( fncCP, c_P  );
    DOTEST( fncCP, CP   );
    DOTEST( fncCP, c_CP );
  }
  {
    DOTEST( fncc_CP, P    );
    DOTEST( fncc_CP, c_P  );
    DOTEST( fncc_CP, CP   );
    DOTEST( fncc_CP, c_CP );
  }

#undef DOTEST
#define DOTEST(T,V) r##T( V );

  {
    DOTEST( fncP, P    );
#if 0
    DOTEST( fncP, c_P  );	// must fail
    DOTEST( fncP, CP   );	// must fail
    DOTEST( fncP, c_CP );	// must fail
#endif
  }
  {
    DOTEST( fncc_P, P    );
    DOTEST( fncc_P, c_P  );
#if 0
    DOTEST( fncc_P, CP   );	// must fail
    DOTEST( fncc_P, c_CP );	// must fail
#endif
  }
  {
    DOTEST( fncCP, P    );
#if 0
    DOTEST( fncCP, c_P  );	// must fail
#endif
    DOTEST( fncCP, CP   );
#if 0
    DOTEST( fncCP, c_CP );	// must fail
#endif
  }
  {
    DOTEST( fncc_CP, P    );
    DOTEST( fncc_CP, c_P  );
    DOTEST( fncc_CP, CP   );
    DOTEST( fncc_CP, c_CP );
  }

  cerr << "==================================" << endl;

  MNPtr mn( new MN( 1 ) );
  constMPtr  m( mn );
  constNPtr  n( mn );
  cerr << mn << endl;
  cerr << m << endl;
  cerr << n << endl;

  cerr << "==================================" << endl;
  return 0;
}
