#include "PtrHier.h"

#undef Y2LOG
#define Y2LOG "TEST"

void fncP   ( ForPtr            P    ) { WAR << P << endl; }
void fncc_P ( const ForPtr      c_P  ) { WAR << c_P << endl; }
void fncCP  ( constForPtr       CP   ) { WAR << CP << endl; }
void fncc_CP( const constForPtr c_CP ) { WAR << c_CP << endl; }

void rfncP   ( ForPtr            & P    ) { ERR << P << endl; }
void rfncc_P ( const ForPtr      & c_P  ) { ERR << c_P << endl; }
void rfncCP  ( constForPtr       & CP   ) { ERR << CP << endl; }
void rfncc_CP( const constForPtr & c_CP ) { ERR << c_CP << endl; }

int main()
{
  Y2SLog::setLogfileName("-");
  DBG << "START=================================" << endl;

  ForPtr            P   ( new For );
  const ForPtr      c_P ( new For );
  constForPtr       CP  ( new For );
  const constForPtr c_CP( new For );

//
// construction
//
#define DOTEST(T,V) T v##V( V ); DBG << v##V << endl;

  {
    DOTEST( ForPtr, P    );
    DOTEST( ForPtr, c_P  );
#if FAILCOMPILE
    DOTEST( ForPtr, CP   );	// must fail
    DOTEST( ForPtr, c_CP );	// must fail
#endif
  }
  {
    DOTEST( const ForPtr, P    );
    DOTEST( const ForPtr, c_P  );
#if FAILCOMPILE
    DOTEST( const ForPtr, CP   );	// must fail
    DOTEST( const ForPtr, c_CP );	// must fail
#endif
  }
  {
    DOTEST( constForPtr, P    );
    DOTEST( constForPtr, c_P  );
    DOTEST( constForPtr, CP   );
    DOTEST( constForPtr, c_CP );
  }
  {
    DOTEST( const constForPtr, P    );
    DOTEST( const constForPtr, c_P  );
    DOTEST( const constForPtr, CP   );
    DOTEST( const constForPtr, c_CP );
  }

//
// assignment
//
#undef DOTEST
#define DOTEST(T,V) T v##V; MIL << (v##V = V) << endl;

  {
    DOTEST( ForPtr, P    );
    DOTEST( ForPtr, c_P  );
#if FAILCOMPILE
    DOTEST( ForPtr, CP   );	// must fail
    DOTEST( ForPtr, c_CP );	// must fail
#endif
  }
  {
#if FAILCOMPILE
    DOTEST( const ForPtr, P    );	// must fail
    DOTEST( const ForPtr, c_P  );	// must fail
    DOTEST( const ForPtr, CP   );	// must fail
    DOTEST( const ForPtr, c_CP );	// must fail
#endif
  }
  {
    DOTEST( constForPtr, P    );
    DOTEST( constForPtr, c_P  );
    DOTEST( constForPtr, CP   );
    DOTEST( constForPtr, c_CP );
  }
  {
#if FAILCOMPILE
    DOTEST( const constForPtr, P    );	// must fail
    DOTEST( const constForPtr, c_P  );	// must fail
    DOTEST( const constForPtr, CP   );	// must fail
    DOTEST( const constForPtr, c_CP );	// must fail
#endif
  }

//
// pass by value
//
#undef DOTEST
#define DOTEST(T,V) T( V );

  {
    DOTEST( fncP, P    );
    DOTEST( fncP, c_P  );
#if FAILCOMPILE
    DOTEST( fncP, CP   );	// must fail
    DOTEST( fncP, c_CP );	// must fail
#endif
  }
  {
    DOTEST( fncc_P, P    );
    DOTEST( fncc_P, c_P  );
#if FAILCOMPILE
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

//
// pass by reference
//
  {
    DOTEST( rfncP, P    );
#if FAILCOMPILE
    DOTEST( rfncP, c_P  );	// must fail
    DOTEST( rfncP, CP   );	// must fail
    DOTEST( rfncP, c_CP );	// must fail
#endif
  }
  {
    DOTEST( rfncc_P, P    );
    DOTEST( rfncc_P, c_P  );
#if FAILCOMPILE
    DOTEST( rfncc_P, CP   );	// must fail
    DOTEST( rfncc_P, c_CP );	// must fail
#endif
  }
  {
    DOTEST( rfncCP, P    );
#if FAILCOMPILE
    DOTEST( rfncCP, c_P  );	// must fail
#endif
    DOTEST( rfncCP, CP   );
#if FAILCOMPILE
    DOTEST( rfncCP, c_CP );	// must fail
#endif
  }
  {
    DOTEST( rfncc_CP, P    );
    DOTEST( rfncc_CP, c_P  );
    DOTEST( rfncc_CP, CP   );
    DOTEST( rfncc_CP, c_CP );
  }

  DBG << "END===================================" << endl;
  return 0;
}

