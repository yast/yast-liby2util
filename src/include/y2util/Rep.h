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

/-*/
#ifndef Rep_h
#define Rep_h

#include <iosfwd>

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : Rep
/**
 * Base class for representations carrying a reference count. Actual
 * reference counting is done by class constRepPtr.
 *
 * The initial reference count is zero. This way you're able to create and
 * destroy objects, which were not referenced by any constRepPtr. But as soon as
 * a constRepPtr references the object, any attempt to delete it will lead to an
 * exeption.
 *
 * Rep must be virtual base class in case of multiple derivation, as it's not
 * advisable to have multiple reference counters for a single object.
 *
 * @short Base class for representations carrying a reference count.
 * @see   constRepPtr
 **/
class Rep {

  private:

    /**
     * The reference counter.
     **/
    mutable unsigned rep_cnt_i;

    /**
     * Actual reference counting is done by class constRepPtr.
     * @see constRepPtr
     **/
    friend class constRepPtr;

    /**
     * Increment reference counter.
     * @see constRepPtr
     **/
    void rep_ref() const {
      ++rep_cnt_i;
      _dbg( 'r' );
    }

    /**
     * Decrement reference counter and delete the object if reference
     * count gets zero. Throws exception if reference count already is
     * zero.
     * @see constRepPtr
     **/
    void rep_unref() const {
      if ( !rep_cnt_i )
	throw( this );
      --rep_cnt_i;
      _dbg( 'u' );
      if ( !rep_cnt_i )
	delete this;
    }

  protected:

    /**
     * Constructor. Initial reference count is zero.
     **/
    Rep() : rep_cnt_i( 0 ), rep_id( ++rep_IDs ) {
      _dbg( 'c' );
    }

    /**
     * CopyConstructor. Initial reference count is zero.
     **/
    Rep( const Rep & rhs ) : rep_cnt_i( 0 ), rep_id( ++rep_IDs ) {
      // do not copy refcount
      _dbg( 'C' );
    }

    /**
     * Assignment. Reference count remains untouched.
     **/
    void operator=( const Rep & rhs ) const {
      // do not assign refcount
    }

    /**
     * Destructor. Throws exception if reference count is not zero.
     **/
    virtual ~Rep() {
      if ( rep_cnt_i )
	throw( this );
      _dbg( 'd' );
    }

  private:

    /**
     * REP_DEBUG: Provides numerical ids.
     **/
    static unsigned rep_IDs;
    /**
     * REP_DEBUG: Counts total ammount of objects in memeory.
     **/
    static unsigned rep_Total;
    /**
     * REP_DEBUG: This objects numerical id.
     **/
    const unsigned rep_id;
    /**
     * REP_DEBUG: Write debug line
     **/
    void _dbg( const char f ) const;

  public:

    /**
     * The reference counter. Might be interesting for copy on write.
     **/
    unsigned rep_cnt() const { return  rep_cnt_i; }

    /**
     * Objects name used in dumpOn()
     **/
    virtual const char * rep_name() const { return "Rep"; }

    /**
     * Writes objects name, address and reference count. If REP_DEBUG, additionally
     * numerical id and total ammount of objects.
     *
     * Derived classes may overload this to realize std::ostream & operator<< for
     * representation and handle classes.
     * @see constRepPtr
     **/
    virtual std::ostream & dumpOn( std::ostream & str ) const;

    /**
     * Default output operator for representation classes realized via 'virtual Rep::dumpOn()'.
     **/
    friend std::ostream & operator<<( std::ostream & str, const Rep & obj );
};

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : constRepPtr
/**
 * constRepPtr realizes a 'const Rep *' to an allocated object derived from Rep.
 * Thus a 'const constRepPtr' is a 'const Rep *const'.
 *
 * On construction, deletion and assignment, constRepPtr adjusts the reference
 * count of the Rep object it is pointing to. Once referenced by a constRepPtr,
 * the Rep object will delete itself, after the last constRepPtr drops its reference
 * to it.
 *
 * Derived classes should provide an appropriate operator->() to access the
 * representation class.
 *
 * @short Base class for reference counted class Rep pointer.
 * @see   Rep
 **/
class constRepPtr {

  private:

    /**
     * Let class RepPtr access rep_p
     * @see RepPtr
     **/
    friend class RepPtr;

    /**
     * Pointer to the referenced Rep object, or 0.
     * @see Rep
     **/
    const Rep * rep_p;

    /**
     * Assign a new Rep object. Prevent self assignment and handle
     * reference counting.
     * @see Rep
     **/
    void _assign( const Rep * new_p ) {
      if ( new_p != rep_p ) {
	if ( rep_p )
	  rep_p->rep_unref();
	rep_p = new_p;
	if ( rep_p )
	  rep_p->rep_ref();
      }
    }

    /**
     * Initial assignment. Called from constructor.
     **/
    void _init( const Rep * new_p ) {
      rep_p = 0;
      _assign( new_p );
    }

  protected:

    /**
     * Constructor.
     **/
    constRepPtr( const Rep * new_p ) {
      _init( new_p );
    }

    /**
     * CopyConstructor.
     **/
    constRepPtr( const constRepPtr & rhs ) {
      _init( rhs.rep_p );
    }

    /**
     * Assignment.
     **/
    constRepPtr & operator=( const Rep * rhs ) {
      _assign( rhs );
      return *this;
    }

    /**
     * Assignment.
     **/
    constRepPtr & operator=( const constRepPtr & rhs ) {
      _assign( rhs.rep_p );
      return *this;
    }

    /**
     * Destructor.
     **/
    virtual ~constRepPtr() {
      if ( rep_p )
	rep_p->rep_unref();
    }

    /**
     * Return rep_p.
     **/
    const Rep *const baseRep() const { return rep_p; }

    /**
     * Return other handles rep_p.
     **/
    const Rep *const baseRep( const constRepPtr & rhs ) const { return rhs.rep_p; }

  public:

    /**
     * Allow test for NULL.
     **/
    operator const void *() const { return rep_p; }

    /**
     * Default output operator for handle classes realized via 'virtual Rep::dumpOn()'.
     * @see Rep
     **/
    friend std::ostream & operator<<( std::ostream & str, const constRepPtr & obj );
};

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : RepPtr
/**
 * RepPtr realizes a 'Rep *' to an allocated object derived from Rep.
 * Thus a 'const RepPtr' is a 'Rep *const'.
 *
 * @short Base class for reference counted class Rep pointer.
 * @see   Rep
 * @see   constRepPtr
 **/
class RepPtr : virtual public constRepPtr {

  private:

    Rep *& rep_p;

  protected:

    /**
     * Constructor.
     **/
    RepPtr( Rep * new_p = 0 ) : constRepPtr( new_p ), rep_p( const_cast<Rep *&>(constRepPtr::rep_p) ) {}

    /**
     * CopyConstructor.
     **/
    RepPtr( const RepPtr & rhs ) : constRepPtr( rhs ), rep_p( const_cast<Rep *&>(constRepPtr::rep_p) ) {}

    /**
     * Assignment.
     **/
    RepPtr & operator=( Rep * rhs ) {
      constRepPtr::operator=( rhs );
      return *this;
    }

    /**
     * Assignment.
     **/
    RepPtr & operator=( const RepPtr & rhs ) {
      constRepPtr::operator=( rhs );
      return *this;
    }

    /**
     * Destructor.
     **/
    virtual ~RepPtr() {}

    /**
     * Return rep_p.
     **/
    Rep *const baseRep() const { return rep_p; }

    /**
     * Return other handles rep_p.
     **/
    Rep *const baseRep( const RepPtr & rhs ) const { return rhs.rep_p; }

};

///////////////////////////////////////////////////////////////////

#endif // Rep_h
