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

   File:       Rep.h

   Author:     Michael Andres <ma@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

   Purpose: Base class for reference counted objects and counted pointer templates.

/-*/
#ifndef Rep_h
#define Rep_h

#include <iosfwd>

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : Rep
/**
 * @short Base class for reference counted objects.
 *
 * The initial reference count is zero. Calling @ref ref(),
 * the reference counter is increased. Calling @ref unref(),
 * the reference counter is decreased. If the reference count
 * becomes zero, the object is deleted.
 *
 * Any attempt to delete an object with a non zero reference
 * count will throw an exeption.
 *
 * Any attempt to @ref unref() an object with a zero reference
 * count will throw an exeption.
 *
 * Stream output operator are provided for Rep and Rep*. Both use
 * @ref dumpOn(), which might be overloaded by derived classes.
 **/
class Rep {
  private:

    /**
     * The reference counter.
     **/
    mutable unsigned _counter;

  protected:

    /**
     * Trigger derived classes after refCount was increased.
     **/
    virtual void ref_to( unsigned rep_cnt_r ) const {}
    /**
     * Trigger derived classes after refCount was decreased.
     * No trigger is sent, if refCount got zero (i.e. the
     * object is deleted).
     **/
    virtual void unref_to( unsigned rep_cnt_r ) const {}

  public:

    /**
     * Constructor. Initial reference count is zero.
     **/
    Rep() : _counter( 0 ) {}
    /**
     * CopyConstructor. Initial reference count is zero.
     **/
    Rep( const Rep & rhs ) : _counter( 0 ) {}
    /**
     * Assignment. Reference count remains untouched.
     **/
    Rep & operator=( const Rep & rhs ) { return *this; }
    /**
     * Destructor. Throws exception if reference count is not zero.
     **/
    virtual ~Rep() { if ( _counter ) throw( this ); }

  public:

    /**
     * Increment reference counter.
     **/
    void ref() const {
      ref_to( ++_counter ); // trigger derived classes
    }
    /**
     * Decrement reference counter and delete the object if reference
     * count got zero. Throws exception if reference count already is
     * zero.
     **/
    void unref() const {
      if ( ! _counter )
	throw( this );
      if ( --_counter )
	unref_to( _counter ); // trigger derived classes
      else
	delete this;
    }

    /**
     * Safe increment reference counter. Ignore NULL object pointer
     * passed as argument.
     **/
    static void ref( const Rep * obj_r ) {
      if ( obj_r )
	obj_r->ref();
    }
    /**
     * Safe decrement reference counter. Ignore NULL object pointer
     * passed as argument..
     **/
    static void unref( const Rep * obj_r ) {
      if ( obj_r )
	obj_r->unref();
    }

  public:

    /**
     * Current reference counter value.
     **/
    unsigned refCount() const { return _counter; }

  public:

    /**
     * Objects name used in dumpOn().
     **/
    virtual const char * repName() const { return "Rep"; }
    /**
     * Derived classes may overload this to realize
     * std::ostream & operator<< for object and pointer
     * classes.
     **/
    virtual std::ostream & dumpOn( std::ostream & str ) const;
};

/**
 * Stream output operator for reference counted objects.
 **/
std::ostream & operator<<( std::ostream & str, const Rep & obj );
/**
 * Stream output operator for reference counted object pointer.
 **/
std::ostream & operator<<( std::ostream & str, const Rep * obj );

////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : CountedRep
/**
 * @short Base class for reference counted objects. Provides numerical object ids.
 *
 * Derived from @ref Rep, CountedRep provides a numerical object id and
 *
 * See class @ref Rep.
 **/
class CountedRep : public Rep {

  private:

    /**
     * Counts total ammount of CountedRep instances in memeory.
     **/
    static unsigned _objectCount;
    /**
     * Provides numerical ids.
     **/
    static unsigned _objectIds;

    /**
     * This objects numerical id.
     **/
    const unsigned _objectId;

  public:

    /**
     * Constructor.
     **/
    CountedRep() : _objectId( ++_objectIds ) { ++_objectCount; }
    /**
     * CopyConstructor.
     **/
    CountedRep( const CountedRep & rhs ) : _objectId( ++_objectIds ) { ++_objectCount; }
    /**
     * Assignment. objectId remains untouched.
     **/
    CountedRep & operator=( const CountedRep & rhs ) {
      Rep::operator=( rhs );
      return *this;
    }
    /**
     * Destructor.
     **/
    virtual ~CountedRep() { --_objectCount; }

  public:

    /**
     * This objects numerical id.
     **/
    unsigned objectId() const { return _objectId; }

    /**
     * The total ammount of CountedRep instances in memeory.
     **/
    static unsigned objectCount() { return _objectCount; }

  public:

    /**
     * Stream output
     **/
    virtual std::ostream & dumpOn( std::ostream & str ) const;
};

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// Counted pointer
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : RepPtrStore<typename _Tp, typename _Bt>
/**
 * @short A '_Tp*' with reference counting on construction and assign.
 *
 * Class _Tp must inherit class @ref Rep. @ref tryAssign() supports assignment from
 * '_Bt *' via dynamic_cast. Thus _Bt must either be _Tp, or a baseclass of _Tp.
 *
 * The counted pointer template classes @ref Ptr and @ref constPtr use RepPtrStore
 * to store the 'object *'. Intentionally all methods which require more than a
 * forward declaration of classes _Tp and _Bt are defined within RepPtrStore and
 * not inlined.
 *
 * To use an ordinary <code>_Tp*</code>, a forward declaration <code>class _Tp;</code>
 * is sufficient, unless you actually access the '_Tp' object. Using a counted pointer,
 * you'll have to pervent the implicit generation of RepPtrStore<_Tp> instances too.
 * Otherwise the definition of class _Tp had to be provided.
 *
 * <PRE>
 *  #include <y2util/Rep.h>                 // template definition
 *  class Foo;                              // forward declare class Foo
 *  extern template class RepPtrStore<Foo>; // pervent implicit instanciation of RepPtrStore<Foo>
 * </PRE>
 *
 * Then within some translation unit you'll have to explicitly instantiate RepPtrStore<Foo>.
 *
 * <PRE>
 *  #include <y2util/Rep.h>                 // template definition
 *  #include "Foo.h"                        // definition of class Foo
 *  template class RepPtrStore<Foo>;        // explicit instantiation of RepPtrStore<Foo>
 * </PRE>
 *
 **/
template<typename _Tp, typename _Bt = _Tp>
class RepPtrStore {

  private:

    /**
     * The pointer.
     **/
    _Tp * _obj;

  private:

    /**
     * Assign a new value to @ref _obj. Adjusts the objects reference counter
     * according to the old and new pointer value.
     **/
    void _assign( _Tp * new_r );

  public:

    /**
     * DefaultConstructor. NULL
     **/
    RepPtrStore() : _obj( 0 ) {}
    /**
     * Constructor. Uses @ref _assign.
     **/
    RepPtrStore( _Tp * ptr ) : _obj( 0 ) { _assign( ptr ); }
    /**
     * CopyConstructor. Uses @ref _assign.
     **/
    RepPtrStore( const RepPtrStore & rhs ) : _obj( 0 ) { _assign( rhs._obj ); }

    /**
     * Assign. Uses @ref _assign.
     **/
    RepPtrStore & operator=( _Tp * ptr ) { _assign( ptr ); return *this; }
    /**
     * Assign. Uses @ref _assign.
     **/
    RepPtrStore & operator=( const RepPtrStore & rhs ) { _assign( rhs._obj ); return *this; }

    /**
     * Destructor. Uses @ref _assign.
     **/
    ~RepPtrStore() { _assign( 0 ); }

    /**
     * Conversion to '_Tp *'
     **/
    operator _Tp *() const { return _obj; }

  public:

    /**
     * Assign the result of dynamic_cast '_Bt *' to '_Tp *'. Uses @ref _assign.
     **/
    void tryAssign( _Bt * ptr );

  public:

    /**
     * Explicit conversion to '_Bt *'
     **/
    _Bt * base() const;
    /**
     * Explicit conversion to 'const @ref Rep *'
     **/
    const Rep * refbase() const;
};

template<typename _Tp,typename _Bt>
void RepPtrStore<_Tp,_Bt>::_assign( _Tp * new_r ) {
  if ( new_r != _obj ) {
    Rep::unref( _obj );
    _obj = new_r;
    Rep::ref( _obj );
  }
}

template<typename _Tp,typename _Bt>
void RepPtrStore<_Tp,_Bt>::tryAssign( _Bt * ptr ) {
  _assign( dynamic_cast<_Tp*>(ptr) );
  if ( !_obj && ptr && ! ptr->refCount() ) {
    Rep::ref( ptr );
    Rep::unref( ptr );
  }
}

template<typename _Tp,typename _Bt>
_Bt * RepPtrStore<_Tp,_Bt>::base() const { return _obj; }

template<typename _Tp,typename _Bt>
const Rep * RepPtrStore<_Tp,_Bt>::refbase() const { return _obj; }

////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : RepPtrBase
/**
 * @short Common base class for all counted pointer classes
 *
 * Provides stream output operator and conversion to 'const void *'
 * to allow pointer comparison and test for NULL.
 *
 * See @ref PtrBase and @ref constPtrBase
 **/
class RepPtrBase {
  /**
   * Stream output operator for all counted pointer classes.
   **/
  friend std::ostream & operator<<( std::ostream & str, const RepPtrBase & obj );

  protected:

    /**
     * virtual destructor;
     **/
    virtual ~RepPtrBase() {}
    /**
     * Derived class must provide the objects '@ref Rep *'
     **/
    virtual const Rep * refbase() const = 0;

  public:

    /**
     * Conversion to 'const void *' to allow pointer comparison
     * and test for NULL.
     **/
    operator const void *() const  { return refbase(); }
};

///////////////////////////////////////////////////////////////////

template<typename _Bt> class constPtrBase;

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : PtrBase<typename _Bt>
/**
 * @short Common base class for counted '_Bt *' (@ref Ptr)
 *
 * See also @ref constPtrBase and @ref RepPtrStore.
 **/
template<typename _Bt>
class PtrBase : public RepPtrBase {

  protected:

    friend class constPtrBase<_Bt>;

    /**
     * Derived class must provide the objects '_Bt *'. See @ref RepPtrStore.
     **/
    virtual _Bt * base() const = 0;

    /**
     * Retrieve base() from another PtrBase<_Bt> object.
     **/
    _Bt * getBase( const PtrBase & rhs ) const {
      return rhs.base();
    }
};

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : constPtrBase<typename _Bt>
/**
 * @short Common base class for counted 'const _Bt *' (@ref constPtr)
 *
 * See also @ref PtrBase and @ref RepPtrStore.
 **/
template<typename _Bt>
class constPtrBase : public RepPtrBase {

  protected:

    /**
     * Derived class must provide the objects 'const _Bt *'. See @ref RepPtrStore.
     **/
    virtual const _Bt * base() const = 0;

    /**
     * Retrieve base() from another constPtrBase<_Bt> object.
     **/
    const _Bt * getBase( const constPtrBase & rhs ) const {
      return rhs.base();
    }

    /**
     * Retrieve base() from a nonconst PtrBase<_Bt> object. Needed
     * as it's ok to create a constPtr from a Ptr, but not vice versa.
     **/
    const _Bt * getBase( const PtrBase<_Bt> & rhs ) const {
      return rhs.base();
    }
};

////////////////////////////////////////////////////////////////////////////////

template<typename _Tp,typename _Bt> class constPtr;

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : Ptr<typename _Tp, typename _Bt>
/**
 * @short Counted '_Tp *'. Support construction/assignment via dynamic_cast from @ref PtrBase<_Bt>
 **/
template<typename _Tp, typename _Bt = _Tp>
class Ptr : public PtrBase<_Bt> {

  private:

    /**
     * The '_Tp*'.
     **/
    RepPtrStore<_Tp,_Bt> _ptr;

  protected:

    /**
     * Required by inherited @ref RepPtrBase.
     **/
    virtual const Rep * refbase() const { return _ptr.refbase(); }
    /**
     * Required by inherited @ref PtrBase.
     **/
    virtual _Bt * base() const { return _ptr.base(); }

  public:

    /**
     * Constructor. From '_Tp *'. Defaults to NULL.
     **/
    Ptr( _Tp * ptr = 0 ) : _ptr( ptr ) {}
    /**
     * CopyConstructor. From Ptr<_Tp,_Bt>.
     **/
    Ptr( const Ptr & rhs ) : _ptr( rhs._ptr ) {}
    /**
     * Constructor. From Ptr<..,_Bt>, i.e. pointer to an object that inherits _Bt.
     **/
    Ptr( const PtrBase<_Bt> & rhs ) { _ptr.tryAssign( getBase( rhs ) ); }

  public:

    /**
     * Assign from '_Tp *'.
     **/
    Ptr & operator=( _Tp * ptr ) { _ptr = ptr; return *this; }
    /**
     *  Assign from Ptr<_Tp,_Bt>.
     **/
    Ptr & operator=( const Ptr & rhs ) { _ptr = rhs._ptr; return *this; }
    /**
     * Assign from Ptr<..,_Bt>, i.e. pointer to an object that inherits _Bt.
     **/
    Ptr & operator=( const PtrBase<_Bt> & rhs ) { _ptr.tryAssign( getBase( rhs ) ); return *this; }

  public:

    /**
     * Access forwarded to the _Tp object (or SEGV if _ptr is NULL)
     **/
    _Tp * operator->() const { return _ptr; }

  public:

    /**
     * ConstCast. Create a Ptr from constPtr ('_Tp*' from 'const _Tp*').
     **/
    static Ptr cast_away_const( constPtr<_Tp,_Bt> rhs ) {
      return const_cast<_Tp*>(rhs.operator->());
    }
};

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : constPtr<typename _Tp, typename _Bt>
/**
 * @short Counted 'const _Tp *'. Support construction/assignment via dynamic_cast from @ref  constPtrBase<_Bt>
 **/
template<typename _Tp, typename _Bt = _Tp>
class constPtr : public constPtrBase<_Bt> {

  private:

    /**
     * The 'const _Tp*'.
     **/
    RepPtrStore<const _Tp,const _Bt> _ptr;

  protected:

    /**
     * Required by inherited @ref RepPtrBase.
     **/
    virtual const Rep * refbase() const { return _ptr.refbase(); }
    /**
     * Required by inherited @ref PtrBase.
     **/
    virtual const _Bt * base() const { return _ptr.base(); }

  public:

    /**
     * Constructor. From 'const _Tp *'. Defaults to NULL.
     **/
    constPtr( const _Tp * ptr = 0 ) : _ptr( ptr ) {}
    /**
     * CopyConstructor. From constPtr<_Tp,_Bt>.
     **/
    constPtr( const constPtr & rhs ) : _ptr( rhs._ptr ) {}
    /**
     * Constructor. From constPtr<..,_Bt>, i.e. pointer to an object that inherits _Bt.
     **/
    constPtr( const constPtrBase<_Bt> & rhs ) { _ptr.tryAssign( getBase( rhs ) ); }

  public:

    /**
     * Assign from 'const _Tp *'.
     **/
    constPtr & operator=( const _Tp * ptr ) { _ptr = ptr; return *this; }
    /**
     *  Assign from constPtr<_Tp,_Bt>.
     **/
    constPtr & operator=( const constPtr & rhs ) { _ptr = rhs._ptr; return *this; }
    /**
     * Assign from constPtr<..,_Bt>, i.e. pointer to an object that inherits _Bt.
     **/
    constPtr & operator=( const constPtrBase<_Bt> & rhs ) { _ptr.tryAssign( getBase( rhs ) ); return *this; }

  public:

    /**
     * Constructor. From nonconst Ptr<_Tp,_Bt>.
     **/
    constPtr( const Ptr<_Tp,_Bt> & rhs ) : _ptr( rhs.operator->() ) {}
    /**
     * Constructor. From nonconst Ptr<..,_Bt>, i.e. pointer to an object that inherits _Bt.
     **/
    constPtr( const PtrBase<_Bt> & rhs ) { _ptr.tryAssign( getBase( rhs ) ); }

  public:

    /**
     *  Assign from nonconst Ptr<_Tp,_Bt>.
     **/
    constPtr & operator=( const Ptr<_Tp,_Bt> & rhs ) { _ptr = rhs.operator->(); return *this; }
    /**
     * Assign from nonconst Ptr<..,_Bt>, i.e. pointer to an object that inherits _Bt.
     **/
    constPtr & operator=( const PtrBase<_Bt> & rhs ) { _ptr.tryAssign( getBase( rhs ) ); return *this; }

  public:

    /**
     * Access forwarded to the _Tp object (or SEGV if _ptr is NULL)
     **/
    const _Tp * operator->() const { return _ptr; }
};

///////////////////////////////////////////////////////////////////

#endif // Rep_h
