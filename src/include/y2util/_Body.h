/*---------------------------------------------------------------------\
|                                                                      |
|                      __   __    ____ _____ ____                      |
|                      \ \ / /_ _/ ___|_   _|___ \                     |
|                       \ V / _` \___ \ | |   __) |                    |
|                        | | (_| |___) || |  / __/                     |
|                        |_|\__,_|____/ |_| |_____|                    |
|                                                                      |
|                               core system                            |
|                                                    (C) SuSE Linux AG |
\----------------------------------------------------------------------/

  File:       _Body.h

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose: Handle/Body: Base for refcounted representation classes (Body)

/-*/
#ifndef _Body_h
#define _Body_h

#include <iosfwd>

///////////////////////////////////////////////////////////////////
//
//      CLASS NAME : _Body
/**
 * @short Handle/Body: Base for refcounted representation classes (Body)
 *
 * _Body provides the reference counter for representation classes
 * manipulated through _Handle. For details see class @ref _Handle.
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
 * Stream output operator are provided for _Body and _Body*. Both
 * use @ref dumpOn(), which might be overloaded by derived classes.
 **/
class _Body {

  private:

    /**
     * The reference counter.
     **/
    mutable unsigned _counter;

  protected:

    /**
     * Trigger derived classes after refCount was increased.
     **/
    virtual void ref_to( unsigned /* rep_cnt_r */ ) const {}
    /**
     * Trigger derived classes after refCount was decreased.
     * No trigger is sent, if refCount got zero (i.e. the
     * object is deleted).
     **/
    virtual void unref_to( unsigned /* rep_cnt_r */ ) const {}

  public:

    /**
     * Constructor. Initial reference count is zero.
     **/
    _Body() : _counter( 0 ) {}
    /**
     * CopyConstructor. Initial reference count is zero.
     **/
    _Body( const _Body & rhs ) : _counter( 0 ) {}
    /**
     * Assignment. Reference count remains untouched.
     **/
    _Body & operator=( const _Body & rhs ) { return *this; }
    /**
     * Destructor. Throws exception if reference count is not zero.
     **/
    virtual ~_Body() { if ( _counter ) throw( this ); }

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

  public:

    /**
     * Safe increment reference counter. Ignore NULL object pointer
     * passed as argument.
     **/
    static void ref( const _Body * obj_r ) {
      if ( obj_r )
        obj_r->ref();
    }
    /**
     * Safe decrement reference counter. Ignore NULL object pointer
     * passed as argument..
     **/
    static void unref( const _Body * obj_r ) {
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
     * Derived classes may overload this to realize
     * std::ostream & operator<< for _Body classes.
     **/
    virtual std::ostream & dumpOn( std::ostream & str ) const;
};

/**
 * Stream output operator for _Body classes.
 **/
std::ostream & operator<<( std::ostream & str, const _Body & obj );
/**
 * Stream output operator for _Body classes.
 **/
std::ostream & operator<<( std::ostream & str, const _Body * obj );

///////////////////////////////////////////////////////////////////

#endif // _Body_h
