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

/-*/
#ifndef RepDef_h
#define RepDef_h

#include <y2util/Rep.h>

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#define REP_CLASS(NAME)        GEN_REP_CLASS( NAME )
#define REP_BODY(NAME)         GEN_REP_BODY( NAME, #NAME )

///////////////////////////////////////////////////////////////////

#define GEN_REP_CLASS(CLNAME)	\
  CLNAME : virtual public Rep

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
// NOTE: Handle classes are derived virtual. This allows implicit
// conversion from 'Ptr' to 'constPtr'. BUT they will not work together
// with representation classes, that contain multimple independent
// instances of the same Baseclass.
//
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#define DEFINE_BASIC_HANDLES(NAME)	\
  class CLASS_CONST_PTR(NAME) {		\
    BODY_CONST_PTR(NAME)		\
  };					\
  class CLASS_PTR(NAME) {		\
    BODY_PTR(NAME)			\
  };

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#define DEFINE_DERIVED_HANDLES(NAME,MODE,BASE)	\
  class CLASS_CONST_PTR(NAME), virtual MODE const##BASE##Ptr {	\
    BODY_CONST_PTR(NAME)					\
  };								\
  class CLASS_PTR(NAME), virtual MODE BASE##Ptr {		\
    BODY_PTR(NAME)						\
  };

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#define DEFINE_DOUBLE_DERIVED_HANDLES(NAME,MODE,BASE,MODE2,BASE2)	\
  class CLASS_CONST_PTR(NAME), virtual MODE const##BASE##Ptr, virtual MODE2 const##BASE2##Ptr {	\
    BODY_CONST_PTR(NAME)									\
  };												\
  class CLASS_PTR(NAME), virtual MODE BASE##Ptr, virtual MODE2 BASE2##Ptr {			\
    BODY_PTR(NAME)										\
  };

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#define IMPL_HANDLES(NAME)	\
  IMPL_CONST_PTR(NAME)	\
  IMPL_PTR(NAME)

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#define CLASS_CONST_PTR(NAME) GEN_CLASS( const##NAME##Ptr, NAME, const )
#define CLASS_PTR(NAME)       GEN_CLASS( NAME##Ptr,        NAME,       ), virtual public const##NAME##Ptr

#define BODY_CONST_PTR(NAME)  GEN_BODY( const##NAME##Ptr, NAME, const )
#define BODY_PTR(NAME)        GEN_BODY( NAME##Ptr,        NAME,       )

#define IMPL_CONST_PTR(NAME)  GEN_IMPL( const##NAME##Ptr, NAME, const )
#define IMPL_PTR(NAME)        GEN_IMPL( NAME##Ptr,        NAME,       )

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#define GEN_CLASS(CLNAME,REPNAME,MAYCONST)	\
  REPNAME; class CLNAME : virtual public MAYCONST##RepPtr

///////////////////////////////////////////////////////////////////

#define GEN_BODY(CLNAME,REPNAME,MAYCONST)	\
  public:								\
    CLNAME( MAYCONST REPNAME * p = 0 );					\
    CLNAME( const CLNAME & rhs );					\
    CLNAME( const MAYCONST##RepPtr & rhs );				\
    CLNAME & operator=( MAYCONST REPNAME * p );				\
    CLNAME & operator=( const CLNAME & rhs );				\
    CLNAME & operator=( const MAYCONST##RepPtr & rhs );			\
    MAYCONST REPNAME * operator->() const;				\
  protected:								\
    MAYCONST REPNAME * rep() const;					\
    MAYCONST REPNAME * rep( const MAYCONST##RepPtr & rhs ) const;	\
  private:


///////////////////////////////////////////////////////////////////

//
// ! Always initialize constRepPtr, It's the bottommost class
//

#define GEN_IMPL(CLNAME,REPNAME,MAYCONST)	\
CLNAME::CLNAME( MAYCONST REPNAME * p ) : constRepPtr( p )			\
  {}										\
CLNAME::CLNAME( const CLNAME & rhs ) : constRepPtr( rhs )			\
  {}										\
CLNAME::CLNAME( const MAYCONST##RepPtr & rhs ) : constRepPtr( rep( rhs ) )	\
  {}										\
CLNAME & CLNAME::operator=( MAYCONST REPNAME * p )				\
  { MAYCONST##RepPtr::operator=( p ); return *this; }				\
CLNAME & CLNAME::operator=( const CLNAME & rhs )				\
  { MAYCONST##RepPtr::operator=( rhs ); return *this; }				\
CLNAME & CLNAME::operator=( const MAYCONST##RepPtr & rhs )			\
  { MAYCONST##RepPtr::operator=( rep( rhs ) ); return *this; }			\
MAYCONST REPNAME * CLNAME::operator->() const 					\
  { return rep(); }								\
MAYCONST REPNAME * CLNAME::rep() const						\
  { return dynamic_cast<MAYCONST REPNAME *>(baseRep()); }			\
MAYCONST REPNAME * CLNAME::rep( const MAYCONST##RepPtr & rhs ) const		\
  { return dynamic_cast<MAYCONST REPNAME *>(baseRep( rhs )); }

///////////////////////////////////////////////////////////////////

#endif // RepDef_h
