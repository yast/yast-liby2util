#include <list>
#include <string>

#include <y2util/Y2SLog.h>
#include <y2util/RepDef.h>

using namespace std;
//---------------------------------------------------
class PTR_CLASS(M) {
  PTR_BODY(M)

  public:

    MPtr( int ii );

};

class REP_CLASS(M) {
  REP_BODY(M)

  public:

    M( int ii ) {}
#if DEFOP
    virtual std::ostream & dumpOn( std::ostream & str ) const {
      return str << "PTR_CLASS(M)";
    }
#endif
};

PTR_IMPL(M);

MPtr::MPtr( int ii )
    : RepHandle( new M( ii ) )
{}
//---------------------------------------------------
//---------------------------------------------------
class PTR_CLASS(N) {
  PTR_BODY(N)

  public:

    NPtr( int ii );
};

static list<NPtr> lst;

class REP_CLASS(N) {
  REP_BODY(N)

  public:

    N( int ii ) { _init(); }

    void _init() { /*lst.push_back( N(this) );*/ }
#if DEFOP
    virtual std::ostream & dumpOn( std::ostream & str ) const {
      return str << "PTR_CLASS(N)";
    }
#endif
};

PTR_IMPL(N);

NPtr::NPtr( int ii )
    : RepHandle( new N( ii ) )
{}
//---------------------------------------------------
//---------------------------------------------------
class PTR_CLASS(MN), public MPtr, public NPtr {
  PTR_BODY(MN)

  public:

    MNPtr( int ii );
};

class REP_CLASS(MN), public M, public N {
  REP_BODY(MN)

  public:

    MN( int ii ) : M(ii), N(-ii) {}
#if DEFOP
    virtual std::ostream & dumpOn( std::ostream & str ) const {
      return str << "PTR_CLASS(MN)";
    }
#endif
};

PTR_IMPL(MN);

MNPtr::MNPtr( int ii )
    : RepHandle( new MN( ii ) )
{}
//---------------------------------------------------
#if DEFOP
std::ostream & operator<<( std::ostream & str, const MNPtr & obj ) {
      if ( !obj )
	return str << "MN(nil)";
      return str << "MN(...)";
    }
std::ostream & operator<<( std::ostream & str, const MPtr & obj ) {
      if ( !obj )
	return str << "M(nil)";
      return str << "M(...)";
    }
std::ostream & operator<<( std::ostream & str, const NPtr & obj ) {
      if ( !obj )
	return str << "N(nil)";
      return str << "N(...)";
    }
#endif

int main()
{
  Y2SLog::setLogfileName("-");
  DBG << "START" << endl;

  MPtr   a( new M( 1 ) );
  MPtr   b( NPtr(new N( 1 )) );
  MPtr   c( new MN( 1 ) );
  cerr << "==================================" << endl;
  cerr << a << endl;
  cerr << "==================================" << endl;
  cerr << b << endl;
  cerr << "==================================" << endl;
  cerr << c << endl;
  cerr << "==================================" << endl;

  cerr << "==================================" << endl;
  c = new M( 1 );
  cerr << c << endl;
  cerr << "==================================" << endl;
  c = NPtr(new N( 1 ));
  cerr << c << endl;
  cerr << "==================================" << endl;
  c = new MN( 1 );
  cerr << c << endl;
  cerr << "==================================" << endl;


  return 0;
}
