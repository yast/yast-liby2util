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

#include <iosfwd>

#ifdef RefObjectDEBUG
#include <iostream>
extern std::ostream & RefObject_debug;
extern unsigned RefObject_total_i;
extern unsigned RefObject_ids_i;
#endif // RefObjectDEBUG

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

#ifdef RefObjectDEBUG
	unsigned id_i;
#endif //RefObjectDEBUG

	ObjT &   obj_p;
	unsigned cnt_i;

	Ref( ObjT & obj_pr )
	  : obj_p( obj_pr )
	  , cnt_i( 0 )
	{
#ifdef RefObjectDEBUG
	  ++RefObject_total_i;
	  id_i = ++RefObject_ids_i;
	  RefObject_debug << "+++Ref(" << id_i << "/" << RefObject_total_i << ") "
		    << cnt_i << ":" << (void*)this << " -> " << (void*)&obj_p << std::endl;
#endif // RefObjectDEBUG
	}

	~Ref() {
#ifdef RefObjectDEBUG
	  --RefObject_total_i;
	  RefObject_debug << "---Ref(" << id_i << "/" << RefObject_total_i << ") "
		    << cnt_i << ":" << (void*)this << " -> " << (void*)&obj_p << std::endl;
#endif // RefObjectDEBUG
	  if ( cnt_i )
	    throw( this );
	  delete &obj_p;
#ifdef RefObjectDEBUG
	  RefObject_debug << "---Ref" << std::endl;
#endif // RefObjectDEBUG
	}
    };
    ///////////////////////////////////////////////////////////////////

  protected:

    Ref * body_p;

    void body_assign( Ref * nbody_p ) {
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

    RefObject( const RefObject & handle_v ) {
      body_p = 0;
      body_assign( handle_v.body_p );
    }

    RefObject & operator=( const RefObject & handle_v ) {
      body_assign( handle_v.body_p );
      return *this;
    }

    virtual ~RefObject() { body_assign( 0 ); }

  public:

    operator void *()  const { return ( body_p ? (void*)&body_p->obj_p : (void*)0 ); }

    ObjT * operator->() const { return assert_body(); }
    ObjT & operator*() const { return *assert_body(); }

    friend std::ostream & operator<<( std::ostream & str, RefObject & obj ) {
#ifdef RefObjectDEBUG
      if ( obj )
	str << "   Ref(" << obj.body_p->id_i << "/" << RefObject_total_i << ") "
	    << obj.body_p->cnt_i << ":" << (void*)obj.body_p << " -> " << (void*)&obj.body_p->obj_p << std::endl;
      else
	str << "   Ref(NULL)";
#endif // RefObjectDEBUG
      return str;
    }
};
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : DerRefObject
//
//	DESCRIPTION :
//
template <class ObjT, class BaseT> class DerRefObject : public RefObject<BaseT> {

  public:

    DerRefObject( ObjT * obj_pr = 0 )             : RefObject<BaseT>( obj_pr ) {}
    DerRefObject( const DerRefObject & handle_v ) : RefObject<BaseT>( handle_v ) {}

    DerRefObject & operator=( const DerRefObject & handle_v ) {
      RefObject<BaseT>::operator=( handle_v );
      return *this;
    }

    virtual ~DerRefObject() {}

  public:

    ObjT * operator->() const { return static_cast<ObjT*>(assert_body()); }
    ObjT & operator*() const { return *static_cast<ObjT*>(assert_body()); }
};

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : RefHandle
//
//	DESCRIPTION :
//
template <class ObjT> class RefHandle : protected RefObject<ObjT> {

  public:

    RefHandle( ObjT * obj_pr = 0 )                : RefObject<ObjT>( obj_pr )   {std::cout << __PRETTY_FUNCTION__ << std::endl;}
    RefHandle( const RefObject<ObjT> & handle_v ) : RefObject<ObjT>( handle_v ) {std::cout << __PRETTY_FUNCTION__ << std::endl;}

    virtual ~RefHandle() {}

  public:

    RefObject<ObjT>::operator void *;
    const typename RefObject<ObjT>::Ref & operator->() const { return RefObject<ObjT>::operator->(); }

    const ObjT & operator*() const { return RefObject<ObjT>::operator*(); }

};
///////////////////////////////////////////////////////////////////

#endif // RefObject_h

// vim:sw=2
