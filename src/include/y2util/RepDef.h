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
// PTR_CLASS header
///////////////////////////////////////////////////////////////////
//
// #include <y2util/RepDef.h>
//
// class PTR_CLASS(Item) {
//   PTR_BODY(Item)
//
//   public:
//
//     ItemPtr( int i );         // forwarded Item constructor
//     ItemPtr( int i, int y );  // forwarded Item constructor
// };
//
///////////////////////////////////////////////////////////////////
#define PTR_CLASS(CL) CL; class CL##Ptr : virtual public RepHandle

#define PTR_BODY(CL)	\
  public:							\
    explicit CL##Ptr( const CL * p = 0 );			\
    CL##Ptr( const CL##Ptr & rhs );				\
    CL##Ptr( const RepHandle & rhs );				\
    const CL##Ptr & operator=( const CL * p ) const;		\
    const CL##Ptr & operator=( const CL##Ptr & rhs ) const;	\
    const CL##Ptr & operator=( const RepHandle & rhs ) const;	\
    const CL *      operator->() const;				\
    CL *            operator->();				\
  protected:							\
    const CL *      rep() const;				\
    const CL *      rep( const RepHandle & rhs ) const;		\
  private:

///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
// PTR_CLASS implementation
///////////////////////////////////////////////////////////////////
//
// #include <Item.h>
//
// PTR_IMPL(Item)
//
// ItemPtr::ItemPtr( int i )
//     : RepHandle( new Item(i) )
// {}
//
// ItemPtr::ItemPtr( int i, int y )
//     : RepHandle( new Item(i,j) )
// {}
//
///////////////////////////////////////////////////////////////////
#define PTR_IMPL(CL)	\
CL##Ptr::CL##Ptr( const CL * p ) : RepHandle( p )				\
  {}										\
CL##Ptr::CL##Ptr( const CL##Ptr & rhs ) : RepHandle( rhs )			\
  {}										\
CL##Ptr::CL##Ptr( const RepHandle & rhs ) : RepHandle( rep( rhs ) )		\
  {}										\
const CL##Ptr & CL##Ptr::operator=( const CL * p ) const			\
  { RepHandle::operator=( p ); return *this; }					\
const CL##Ptr & CL##Ptr::operator=( const CL##Ptr & rhs ) const			\
  { RepHandle::operator=( rhs ); return *this; }				\
const CL##Ptr & CL##Ptr::operator=( const RepHandle & rhs ) const		\
  { RepHandle::operator=( rep( rhs ) ); return *this; }				\
const CL * CL##Ptr::operator->() const 						\
  { return rep(); }								\
CL *       CL##Ptr::operator->()						\
  { return const_cast<CL *>(rep()); }						\
const CL * CL##Ptr::rep() const							\
  { return dynamic_cast<const CL *>(RepHandle::rep()); }			\
const CL * CL##Ptr::rep( const RepHandle & rhs ) const				\
  { return dynamic_cast<const CL *>(RepHandle::rep( rhs )); }

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// REP_CLASS
///////////////////////////////////////////////////////////////////
//
// #include <ItemPtr.h>
//
// class REP_CLASS(Item) {
//   REP_BODY(Item)
//
//   public:
//
//     Item( int i );
//     Item( int i, int y );
//
//     virtual ~Item();
// };
//
///////////////////////////////////////////////////////////////////
#define REP_CLASS(CL) CL : virtual public Rep

#define REP_BODY(CL)	\
  public:		\
    virtual const char * rep_name() const 	\
      { return #CL; }	\
  private:

///////////////////////////////////////////////////////////////////

#endif // RepDef_h
