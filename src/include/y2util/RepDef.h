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

  File:       RepDef.h

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose: Provides a set of macros to define data and pointer classes.
  For details see the liby2util documentation (CountedPtr.html).

/-*/
#ifndef RepDef_h
#define RepDef_h

#include <y2util/Rep.h>

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//
// NOTE: Data classes should virtual inherit class Rep:
//
// class CLNAME : virtual public Rep {
//   REP_BODY(CLNAME);
//   ...
// };
//
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#define REP_BODY(NAME)         GEN_REP_BODY( NAME, #NAME )

///////////////////////////////////////////////////////////////////

#define GEN_REP_BODY(CLNAME,STRNAME)	\
  CLNAME( const CLNAME & );            /* no copy */	\
  CLNAME & operator=(const CLNAME & ); /* no assign */	\
  public:						\
    virtual const char * rep_name() const 		\
      { return STRNAME; }				\
  private:

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//
// NOTE: Pointerclasses are derived virtual. This allows implicit
// conversion from 'Ptr' to 'constPtr'. BUT they will not work together
// with data classes, that inherit multimple independent instances of
// the same base class.
//
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Define base pointer classes directly derived from constRepPtr/RepPtr:
//
// class const##NAME##Ptr : virtual public constRepPtr
// class NAME##Ptr        : virtual public RepPtr
///////////////////////////////////////////////////////////////////
#define DEFINE_BASE_POINTER(NAME)				\
  class NAME;							\
  class const##NAME##Ptr : virtual public constRepPtr  {	\
    GEN_BODY( const##NAME##Ptr, const NAME *, constRepPtr )	\
  };								\
  class NAME##Ptr : virtual public const##NAME##Ptr, virtual public RepPtr {			\
    GEN_BODY( NAME##Ptr, NAME *, RepPtr )			\
    GEN_CONSTCAST( NAME  )					\
  };


///////////////////////////////////////////////////////////////////
// Define derived pointer classes:
//
// class const##NAME##Ptr : virtual public const##FROM##Ptr
// class NAME##Ptr        : virtual public FROM##Ptr
//
// Third arg BASE is the name of the bottommost pointer class of this
// hierarchy. We need to know it, to allow construction and assignment
// from const##BASE##Ptr or BASE##Ptr via dynamic_casts.
//
// A typical hierarchy would look like this:
//
// DEFINE_BASE_POINTER(Bse);
// DEFINE_DERIVED_POINTER(Obj,Bse,Bse);
// DEFINE_DERIVED_POINTER(Pkg,Obj,Bse);
// DEFINE_DERIVED_POINTER(Sel,Obj,Bse);
//
///////////////////////////////////////////////////////////////////
#define DEFINE_DERIVED_POINTER(NAME,FROM,BASE)						\
  class NAME;										\
  class const##NAME##Ptr : virtual public const##FROM##Ptr  {				\
    GEN_BODY( const##NAME##Ptr, const NAME *, constRepPtr )				\
    GEN_BASECONSTRUCT( const##NAME##Ptr, const##BASE##Ptr, const BASE *, constRepPtr )	\
  };											\
  class NAME##Ptr : virtual public const##NAME##Ptr, virtual public FROM##Ptr {		\
    GEN_BODY( NAME##Ptr, NAME *, RepPtr )						\
    GEN_BASECONSTRUCT( NAME##Ptr, BASE##Ptr, BASE *, RepPtr )				\
    GEN_CONSTCAST( NAME )								\
  };


///////////////////////////////////////////////////////////////////
// Implementation for base pointer classes
///////////////////////////////////////////////////////////////////
#define IMPL_BASE_POINTER(NAME)					\
  GEN_IMPL( const##NAME##Ptr, const NAME *, constRepPtr )	\
  GEN_IMPL( NAME##Ptr, NAME *, RepPtr )

///////////////////////////////////////////////////////////////////
// Implementation for derived pointer classes
///////////////////////////////////////////////////////////////////
#define IMPL_DERIVED_POINTER(NAME,FROM,BASE)							\
  IMPL_BASE_POINTER(NAME)									\
  GEN_BASECONSTRUCT_IMPL( const##NAME##Ptr, const##BASE##Ptr, const BASE *, constRepPtr )	\
  GEN_BASECONSTRUCT_IMPL( NAME##Ptr, BASE##Ptr, BASE *, RepPtr )

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#define GEN_BODY(PTRCLASS,DATAPTR,REPPTRCLASS)		\
  public:						\
    PTRCLASS( DATAPTR p = 0 );				\
    PTRCLASS( const PTRCLASS & rhs );			\
    PTRCLASS & operator=( DATAPTR p );			\
    PTRCLASS & operator=( const PTRCLASS & rhs );	\
  public:						\
    DATAPTR operator->() const;				\
  protected:						\
    DATAPTR rep() const;				\
    DATAPTR rep( const REPPTRCLASS & rhs ) const;	\
  private:


#define GEN_BASECONSTRUCT(PTRCLASS,BASEPTRCLASS,BASEDATAPTR,REPPTRCLASS)	\
  public:									\
    PTRCLASS( BASEDATAPTR p );							\
    PTRCLASS( const BASEPTRCLASS & rhs );					\
  private:


#define GEN_CONSTCAST( NAME )							\
  public:									\
    static NAME##Ptr cast_away_const( const NAME##Ptr & rhs ) {			\
      return rhs;								\
    }										\
    static NAME##Ptr cast_away_const( const const##NAME##Ptr & rhs ) {		\
      return const_cast<NAME *>( rhs.operator->() );				\
    }										\
  private:

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#define GEN_IMPL(PTRCLASS,DATAPTR,REPPTRCLASS)			\
PTRCLASS::PTRCLASS( DATAPTR p ) : constRepPtr( p )		\
  {}								\
PTRCLASS::PTRCLASS( const PTRCLASS & rhs ) : constRepPtr( rhs )	\
  {}								\
PTRCLASS & PTRCLASS::operator=( DATAPTR p )			\
  { REPPTRCLASS::operator=( p ); return *this; }		\
PTRCLASS & PTRCLASS::operator=( const PTRCLASS & rhs )		\
  { REPPTRCLASS::operator=( rhs ); return *this; }		\
DATAPTR PTRCLASS::operator->() const 				\
  { return rep(); }						\
DATAPTR PTRCLASS::rep() const					\
  { return dynamic_cast<DATAPTR>(baseRep()); }			\
DATAPTR PTRCLASS::rep( const REPPTRCLASS & rhs ) const		\
  { return dynamic_cast<DATAPTR>(baseRep( rhs )); }


#define GEN_BASECONSTRUCT_IMPL(PTRCLASS,BASEPTRCLASS,BASEDATAPTR,REPPTRCLASS)		\
PTRCLASS::PTRCLASS( BASEDATAPTR p ) : constRepPtr( 0 )					\
  { operator=( BASEPTRCLASS( p ) ); }							\
PTRCLASS::PTRCLASS( const BASEPTRCLASS & rhs ) : constRepPtr( rep( rhs ) )		\
  {}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#endif // RepDef_h
