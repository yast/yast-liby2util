/* $Header$
//
// $Log$
// Revision 1.4  2002/05/27 13:30:06  lnussel
// made void* operator work
//
// Revision 1.3  2002/05/27 10:56:41  lnussel
// - testfunction for refobjects stored in a list
// - removed private operator new and implemented operator& in RefObject.h
// - added debug() macro
//
// Revision 1.2  2002/05/27 08:30:56  lnussel
// - build static library
// - fix gcc3 warnings
// - add skeleton class for time handling
//
// Revision 1.1  2002/05/21 14:07:29  lnussel
// added Pathname, RefObject, UniqueStr and hash classes
//
*/
#ifndef RefObject_h
#define RefObject_h

#include <unistd.h> // size_t
#include <iosfwd>
#include <Y2UTIL.h>

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : RefObject
//
//	DESCRIPTION :
//
template <class ObjT> class RefObject {

//  void * operator new( size_t ); // no new
  protected:

    ///////////////////////////////////////////////////////////////////
    //
    //	CLASS NAME : RefObject::Ref
    //
    class Ref {

      Ref( const Ref & );             // no copy
      Ref & operator=( const Ref & ); // no assign
      Ref * operator&();              // no address

      private:

	ObjT &   obj_p;
	unsigned cnt_i;

	Ref( ObjT & obj_pr )
	  : obj_p( obj_pr )
	  , cnt_i( 0 )
	{}

	~Ref() {
	  if ( cnt_i )
	    throw( this );
	  delete &obj_p;
	}

      private:

	friend class RefObject;

      public:

	operator void *() const { return &obj_p; }

	const ObjT * operator->() const { return &obj_p; }
	ObjT *       operator->()       { return &obj_p; }

	const ObjT & operator*() const { return obj_p; }
	ObjT &       operator*()       { return obj_p; }
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

    Ref & assert_body() const {
      if ( !body_p )
	throw( this );
      return *body_p;
    }

  public:

    RefObject * operator&()
    {
#warning "RefObject::operator&()";
      return this;
    }

    RefObject( ObjT * obj_pr = 0 ) {
      debug( "default constructor" );
      body_p = 0;
      body_assign( obj_pr ? new Ref( *obj_pr ) : 0 );
    }

    RefObject( const RefObject & handle_v ) {
      debug ( "copy constructor" );
      body_p = 0;
      body_assign( handle_v.body_p );
    }

    RefObject & operator=( const RefObject & handle_v ) {
      debug ( "assignment" );
      body_assign( handle_v.body_p );
      return *this;
    }

    bool null()
    {
	    return body_p?false:true;
    }

    virtual ~RefObject() { body_assign( 0 ); }

  public:

    operator void *()  const { return ( body_p != (void*)NULL ? (void*)*body_p : (void*)NULL ); }
    Ref & operator->() const { return assert_body(); }

    ObjT & operator*() const { return *assert_body(); }

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
