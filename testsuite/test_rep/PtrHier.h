/*---------------------------------------------------------------------\
|                                                                      |
|                      __   __    ____ _____ ____                      |
|                      \ \ / /_ _/ ___|_   _|___ \                     |
|                       \ V / _` \___ \ | |   __) |                    |
|                        | | (_| |___) || |  / __/                     |
|                        |_|\__,_|____/ |_| |_____|                    |
|                                                                      |
|                               core system                            |
|                                                        (C) SuSE GmbH |
\----------------------------------------------------------------------/

  File:       PtrHier.h

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose: Define some pointer and data classes for tests.

/-*/
#ifndef PtrHier_h
#define PtrHier_h

#include <iostream>
#include <y2util/Y2SLog.h>
#include <y2util/RepDef.h>

///////////////////////////////////////////////////////////////////
using namespace std;
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
DEFINE_BASE_POINTER(For);
///////////////////////////////////////////////////////////////////
class For : virtual public Rep {
  REP_BODY(For);
  public:
    For() {}
    virtual ~For() {}
};
IMPL_BASE_POINTER(For);
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
DEFINE_BASE_POINTER(Bse);
DEFINE_DERIVED_POINTER(Obj,Bse,Bse);
DEFINE_DERIVED_POINTER(Pkg,Obj,Bse);
DEFINE_DERIVED_POINTER(Sel,Obj,Bse);
///////////////////////////////////////////////////////////////////
class Bse : virtual public Rep {
  REP_BODY(Bse);
  public:
    Bse() {}
    virtual ~Bse() {}

    virtual PkgPtr p() { PkgPtr ret; ret = this; return ret; }
    virtual PkgPtr q() { return this; }
};
IMPL_BASE_POINTER(Bse);
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
class Obj : virtual public Rep, public Bse {
  REP_BODY(Obj);
  public:
    Obj() {}
    virtual ~Obj() {}

    virtual PkgPtr p() { PkgPtr ret; ret = this; return ret; }
    virtual PkgPtr q() { return this; }
};
IMPL_DERIVED_POINTER(Obj,Bse,Bse);
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
class Pkg : virtual public Rep, public Obj {
  REP_BODY(Pkg);
  public:
    Pkg() {}
    virtual ~Pkg() {}

    virtual PkgPtr p() { PkgPtr ret; ret = this; return ret; }
    virtual PkgPtr q() { return this; }
};
IMPL_DERIVED_POINTER(Pkg,Obj,Bse);
///////////////////////////////////////////////////////////////////
class Sel : virtual public Rep, public Obj {
  REP_BODY(Sel);
  public:
    Sel() {}
    virtual ~Sel() {}

    virtual PkgPtr p() { PkgPtr ret; ret = this; return ret; }
    virtual PkgPtr q() { return this; }
};
IMPL_DERIVED_POINTER(Sel,Obj,Bse);
///////////////////////////////////////////////////////////////////

#endif // PtrHier_h
