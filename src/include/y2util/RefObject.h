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

   File:       RefObject.h

   Author:     Michael Andres <ma@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

/-*/
#ifndef RefObject_h
#define RefObject_h

#include <iostream>

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : RefObject
//
//	DESCRIPTION :
//
template <class ObjT> class RefObject {

  protected:

    ///////////////////////////////////////////////////////////////////
    //
    //	CLASS NAME : RefObject::Ref
    //
    class Ref {

      Ref( const Ref & );             // no copy
      Ref & operator=( const Ref & ); // no assign
      Ref * operator&();              // no address

      public:

	ObjT &   obj_p;
	unsigned cnt_i;

	void doDelete();

	Ref( ObjT & obj_pr )
	  : obj_p( obj_pr )
	  , cnt_i( 0 )
	{}

	~Ref() {
	  if ( cnt_i )
	    throw( this );
	  doDelete();
	}
    };
    ///////////////////////////////////////////////////////////////////

  protected:

    mutable Ref * body_p;

    void body_assign( Ref * nbody_p ) const {
      if ( body_p != nbody_p ) {
	if ( body_p && --(body_p->cnt_i) == 0 )
	  delete body_p;
	body_p = nbody_p;
	if ( body_p )
	  ++(body_p->cnt_i);
      }
    }

    ObjT * assert_body() const {
      if ( !body_p )
	throw( this );
      return &body_p->obj_p;
    }

  public:

    RefObject( ObjT * obj_pr = 0 ) {
      body_p = 0;
      body_assign( obj_pr ? new Ref( *obj_pr ) : 0 );
    }

    RefObject( const RefObject & handle_v )
    {
      body_p = 0;
      body_assign( handle_v.body_p );
    }

    const RefObject & operator=( const RefObject & handle_v ) const {
      body_assign( handle_v.body_p );
      return *this;
    }

    virtual ~RefObject() { body_assign( 0 ); }

  public:

    operator void *() const { return ( body_p ? (void*)&body_p->obj_p : (void*)0 ); }

    const ObjT * operator->() const { return assert_body(); }
    ObjT *       operator->()       { return assert_body(); }

    friend std::ostream & operator<<( std::ostream & str, const RefObject & obj ) {
      if ( obj )
	str << *obj.operator->();
      else
	str << "(nil)";
      return str;
    }
};

template <class ObjT> void RefObject<ObjT>::Ref::doDelete()
{
  delete &obj_p;
}

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//      CLASS NAME : DerivedRefObject
//
//      DESCRIPTION :
//
template <class ObjT, class BaseT> class DerivedRefObject : public BaseT {

  public:

    DerivedRefObject( ObjT * obj_pr = 0 )                 : BaseT( obj_pr ) {}
    DerivedRefObject( const DerivedRefObject & handle_v ) : BaseT( handle_v ) {}

    const DerivedRefObject & operator=( const DerivedRefObject & handle_v ) const {
      BaseT::operator=( handle_v );
      return *this;
    }

    virtual ~DerivedRefObject() {}

  public:

    const ObjT * operator->() const { return static_cast<ObjT*>(assert_body()); }
    ObjT *       operator->()       { return static_cast<ObjT*>(assert_body()); }

    friend std::ostream & operator<<( std::ostream & str, const DerivedRefObject & obj ) {
      if ( obj )
	str << *obj.operator->();
      else
	str << "(nil)";
      return str;
    }
};

///////////////////////////////////////////////////////////////////

#endif // RefObject_h
