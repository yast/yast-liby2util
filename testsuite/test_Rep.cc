#include <iostream>
#include <list>

#include <y2util/Y2SLog.h>
#define REP_DEBUG_REF
#define REP_DEBUG_STREAM DBG

#include <y2util/RepDef.h>

using namespace std;

//---------------------------------------------------
class HANDLE_CLASS(M) {
  HANDLE_BODY(M)

  public:

    M( int ii );

};

class REP_CLASS(M) {
  REP_BODY(M)

  protected:

    Mrep( int ii ) {}
};

HANDLE_IMPL(M);

M::M( int ii )
    : RepHandle( new Mrep( ii ) )
    , rep_p( (Mrep*const&)RepHandle::rep() )
{}
//---------------------------------------------------
//---------------------------------------------------
class HANDLE_CLASS(N) {
  HANDLE_BODY(N)

  public:

    N( int ii );

};

static list<N> lst;

class REP_CLASS(N) {
  REP_BODY(N)

  protected:

    Nrep( int ii ) { _init(); }

    void _init() { lst.push_back( N(this) ); }
};

HANDLE_IMPL(N);

N::N( int ii )
    : RepHandle( new Nrep( ii ) )
    , rep_p( (Nrep*const&)RepHandle::rep() )
{}
//---------------------------------------------------
//---------------------------------------------------
class HANDLE_CLASS(MN), public M, public N {
  HANDLE_BODY(MN)

  public:

    MN( int ii );
};

class REP_CLASS(MN), public Mrep, public Nrep {
  REP_BODY(MN)

  protected:
  public:

    MNrep( int ii ) : Mrep(ii), Nrep(-ii) {}
};

HANDLE_IMPL(MN);

MN::MN( int ii )
    : RepHandle( new MNrep( ii ) )
    , rep_p( (MNrep*const&)RepHandle::rep() )
{}
//---------------------------------------------------
void tt( const RepHandle & xx ) {
  if ( xx )
    cerr << "--> " << xx << endl;
  else
    cerr << "--> no xx" << endl;
}

ostream & operator<<( std::ostream & str, const MN & obj ) {
  return str << "operator<<(MN)";
}
ostream & operator<<( std::ostream & str, const M & obj ) {
  return str << "operator<<(M)";
}
int main()
{
  Y2SLog::setLogfileName("-");
  DBG << "START" << endl;
  {
  MN a( new MNrep( 3 ) );
  }
  cerr << "==========================================" << endl;
  for ( list<N>::iterator i = lst.begin(); i != lst.end(); ++i ) {
    cerr << *i << endl;
  }
  return 0;
}
