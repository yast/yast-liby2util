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
 * reference counting is done by class RepHandle.
 *
 * The initial reference count is zero. This way you're able to create and
 * destroy objects, which were not referenced by any RepHandle. But as soon as
 * a RepHandle references the object, any attempt to delete it will leed to an
 * exeption.
 *
 * Rep must be virtual base class in case of multiple derivation, as it's not
 * advisable to have multiple reference counters for a single object.
 *
 * @short Base class for representations carrying a reference count.
 * @see   RepHandle
 **/
class Rep {

  private:

    /**
     * The reference counter.
     **/
    mutable unsigned rep_cnt_i;

    /**
     * Actual reference counting is done by class RepHandle.
     * @see RepHandle
     **/
    friend class RepHandle;

    /**
     * Increment reference counter.
     * @see RepHandle
     **/
    void rep_ref() const {
      ++rep_cnt_i;
      _dbg( 'r' );
    }

    /**
     * Decrement reference counter and delete the object if reference
     * count gets zero. Throws exception if reference count already is
     * zero.
     * @see RepHandle
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
    Rep() : rep_id( ++rep_IDs ) {
      rep_cnt_i = 0;
      _dbg( 'c' );
    }

    /**
     * CopyConstructor. Initial reference count is zero.
     **/
    Rep( const Rep & rhs ) : rep_id( ++rep_IDs ) {
      rep_cnt_i = 0; // do not copy refcount
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
     * Objects name used in dumpOn()
     **/
    virtual const char * rep_name() const { return "Rep"; }

    /**
     * Writes objects name, address and reference count. If REP_DEBUG, additionally
     * numerical id and total ammount of objects.
     *
     * Derived classes may overload this to realize std::ostream & operator<< for
     * representation and handle classes.
     * @see RepHandle
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
//	CLASS NAME : RepHandle
/**
 * RepHandle realizes a pointer to an allocated object derived from class Rep.
 * On construction, deletion and assignment, RepHandle adjusts the reference
 * count of the Rep object it is pointing to. Once referenced by a RepHandle,
 * the Rep object will delete itself, after the last RepHandle drops its reference
 * to it.
 *
 * Derived classes should provide an appropriate operator->() to access the
 * representation class.
 *
 *
 * @short Base class for reference counted class Rep pointer.
 * @see   Rep
 **/
class RepHandle {

  private:

    /**
     * Pointer to the referenced Rep object, or 0.
     * @see Rep
     **/
    mutable const Rep * rep_p;

    /**
     * Assign a new Rep object. Prevent self assignment and handle
     * reference counting.
     * @see Rep
     **/
    void _assign( const Rep * new_p ) const {
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
    void _init( const Rep * new_p ) const {
      rep_p = 0;
      _assign( new_p );
    }

  protected:

    /**
     * Constructor.
     **/
    RepHandle( const Rep * new_p = 0 ) {
      _init( new_p );
    }

    /**
     * CopyConstructor.
     **/
    RepHandle( const RepHandle & rhs ) {
      _init( rhs.rep_p );
    }

    /**
     * Assignment.
     **/
    const RepHandle & operator=( const Rep * rhs ) const {
      _assign( rhs );
      return *this;
    }

    /**
     * Assignment.
     **/
    const RepHandle & operator=( const RepHandle & rhs ) const {
      _assign( rhs.rep_p );
      return *this;
    }

    /**
     * Destructor.
     **/
    virtual ~RepHandle() {
      if ( rep_p )
	rep_p->rep_unref();
    }

    /**
     * Return rep_p.
     **/
    const Rep * rep() const { return rep_p; }

    /**
     * Return other handles rep_p.
     **/
    const Rep * rep( const RepHandle & rhs ) const { return rhs.rep_p; }

  public:

    /**
     * Allow test for NULL.
     **/
    operator const void *() const { return rep_p; }

    /**
     * Default output operator for handle classes realized via 'virtual Rep::dumpOn()'.
     * @see Rep
     **/
    friend std::ostream & operator<<( std::ostream & str, const RepHandle & obj );
};

///////////////////////////////////////////////////////////////////

#endif // Rep_h
