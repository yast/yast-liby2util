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
 * See <A HREF="../CountedPtr.html">Counted Pointers</A>.
 *
 * <CODE>Rep</CODE> provides the reference counter that tracks the number
 * of outstanding refrences to an object.
 *
 * <B>Data classes should be virtual derived from <CODE>Rep</CODE></B>, because in
 * case of multiple inheritance it's not advisable to have multiple reference
 * counters within a single object.
 *
 * The initial reference count is zero. In this state it's ok to manualy delete
 * the object again. But as soon as there are outstanding references, any attempt
 * to manualy delete it will lead to an exeption. The object will be automaticaly
 * deleted after the last reference left.
 *
 * Derived Classes may want to overload <CODE>rep_name()</CODE> and <CODE>dumpOn()</CODE>.
 * Besides this, they don't need to care about <CODE>Rep</CODE>.
 *
 * @short <A HREF="../CountedPtr.html">Counted Pointers</A>: Base for <CODE>data</CODE> classes. Provides the reference counter.
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
     * Class basicPtr provides a minimal interface to handle reference counting.
     * @see basicPtr
     **/
    friend class basicPtr;

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
    Rep() : rep_cnt_i( 0 ), rep_id_i( ++rep_IDs ) {
      ++rep_Total;
      _dbg( 'c' );
    }

    /**
     * CopyConstructor. Initial reference count is zero.
     **/
    Rep( const Rep & rhs ) : rep_cnt_i( 0 ), rep_id_i( ++rep_IDs ) {
      // do not copy refcount
      ++rep_Total;
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
      --rep_Total;
      _dbg( 'd' );
    }

  private:

    /**
     * Provides numerical ids.
     **/
    static unsigned rep_IDs;
    /**
     * Counts total ammount of objects in memeory.
     **/
    static unsigned rep_Total;
    /**
     * This objects numerical id.
     **/
    const unsigned rep_id_i;
    /**
     * Write debug lines if liby2util was compiled with REP_DEBUG
     * or REP_DEBUG_REF. See Rep.cc for details.
     **/
    void _dbg( const char f ) const;

  public:

    /**
     * This objects numerical id. Might be interesting for debug.
     **/
    unsigned rep_id() const { return  rep_id_i; }

    /**
     * The reference counter. Might be interesting for copy on write.
     **/
    unsigned rep_cnt() const { return  rep_cnt_i; }

    /**
     * Objects name used in dumpOn().
     **/
    virtual const char * rep_name() const { return "Rep"; }

    /**
     * Writes objects name, address and reference count. If REP_DEBUG, additionally
     * numerical id and total ammount of objects.
     *
     * Derived classes may overload this to realize std::ostream & operator<< for
     * data and pointer classes.
     * @see constRepPtr
     **/
    virtual std::ostream & dumpOn( std::ostream & str ) const;

    /**
     * Default output operator for data classes realized via 'virtual Rep::dumpOn()'.
     **/
    friend std::ostream & operator<<( std::ostream & str, const Rep & obj );

  public:

    /**
     * Merely a hook for debug purpose. Content and format are alternating,
     **/
    static std::ostream & dumpRepStats( std::ostream & str );
};

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : constRepPtr
/**
 * See <A HREF="../CountedPtr.html">Counted Pointers</A>.
 *
 * <CODE>constRepPtr</CODE> realizes a '<CODE>const Rep *</CODE>' to an allocated object derived from <CODE>Rep</CODE>.
 * Thus a '<CODE>const constRepPtr</CODE>' is a '<CODE>const Rep *const</CODE>'.
 *
 * On construction, deletion and assignment, <CODE>constRepPtr</CODE> adjusts the reference
 * count of the <CODE>Rep</CODE> object it is pointing to. Once referenced by a <CODE>constRepPtr</CODE>,
 * the <CODE>Rep</CODE> object will delete itself, after the reference is dropped.
 *
 * <B>Derived classes should provide an appropriate operator->()</B> to access the
 * data class.
 *
 * @short <A HREF="../CountedPtr.html">Counted Pointers</A>: Base for <CODE>const pointer</CODE> classes. Manages reference counting.
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
     * Return other pointers rep_p.
     **/
    const Rep *const baseRep( const constRepPtr & rhs ) const { return rhs.rep_p; }

  public:

    /**
     * Allow test '== NULL'
     **/
    operator const void *() const { return rep_p; }

    /**
     * Default output operator for pointer classes realized via 'virtual Rep::dumpOn()'.
     * @see Rep
     **/
    friend std::ostream & operator<<( std::ostream & str, const constRepPtr & obj );
};

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : RepPtr
/**
 * See <A HREF="../CountedPtr.html">Counted Pointers</A>.
 *
 * <CODE>RepPtr</CODE> realizes a '<CODE>Rep *</CODE>' to an allocated object derived from <CODE>Rep</CODE>.
 * Thus a '<CODE>const RepPtr</CODE>' is a '<CODE>Rep *const</CODE>'.
 *
 * <B>Derived classes should provide an appropriate operator->()</B> to access the
 * data class.
 *
 * @short <A HREF="../CountedPtr.html">Counted Pointers</A>: Base for <CODE>pointer</CODE> classes. Manages reference counting.
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
     * Return other pointers rep_p.
     **/
    Rep *const baseRep( const RepPtr & rhs ) const { return rhs.rep_p; }

};

///////////////////////////////////////////////////////////////////

/**
 * Comparison of constRepPtr and Rep*. Provided because all pointer
 * classes provide an <CODE>operator const void *()</CODE> that returns
 * the internal Rep*'s value. But a data classes this pointer needs
 * not to have the same value as the inherited class Rep's one.
 **/
inline bool operator==( const constRepPtr & lhs, const Rep * rhs )
{
  return ( (const void *)lhs == (const void *)rhs );
}

/**
 * Comparison of constRepPtr and Rep*. Provided because all pointer
 * classes provide an <CODE>operator const void *()</CODE> that returns
 * the internal Rep*'s value. But a data classes this pointer needs
 * not to have the same value as the inherited class Rep's one.
 **/
inline bool operator==( const Rep * lhs, const constRepPtr & rhs )
{
  return ( (const void *)lhs == (const void *)rhs );
}

/**
 * Comparison of constRepPtr and Rep*. Provided because all pointer
 * classes provide an <CODE>operator const void *()</CODE> that returns
 * the internal Rep*'s value. But a data classes this pointer needs
 * not to have the same value as the inherited class Rep's one.
 **/
inline bool operator!=( const constRepPtr & lhs, const Rep * rhs )
{
  return ( ! operator==( lhs, rhs ) );
}

/**
 * Comparison of constRepPtr and Rep*. Provided because all pointer
 * classes provide an <CODE>operator const void *()</CODE> that returns
 * the internal Rep*'s value. But a data classes this pointer needs
 * not to have the same value as the inherited class Rep's one.
 **/
inline bool operator!=( const Rep * lhs, const constRepPtr & rhs )
{
  return ( ! operator==( lhs, rhs ) );
}

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : basicPtr
/**
 * @short Minimal interface to handle reference counter Rep
 * @see rep
 **/
class basicPtr {
  protected:
    void _rep_change( const Rep * from_r, const Rep * to_r ) {
      if ( from_r != to_r ) {
	if ( from_r )
	  from_r->rep_unref();
	if ( to_r )
	  to_r->rep_ref();
      }
    }
};

///////////////////////////////////////////////////////////////////

#endif // Rep_h
