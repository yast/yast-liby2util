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

  File:       _Handle.h

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose: Handle/Body: Base for Handle classes manipuating a Body

/-*/
#ifndef _Handle_h
#define _Handle_h

#include <iosfwd>

#include <y2util/_Body.h>

///////////////////////////////////////////////////////////////////
//
//      CLASS NAME : _Handle
/**
 * @short Handle/Body: Base for Handle classes manipuating a Body
 *
 * _Handle provides the basic methods to access and manipulate a
 * @ref _Body (reference counted representation class). It maintains
 * the _Body reference counter on construction, assignment and deletion
 * of the _Handle. It provides operator-> and operator* to access the _Body.
 *
 * As real life representation class will be derived from _Body, real life
 * handles will be derived from _Handle. For convenience the template
 * class @ref _DHandle is provided. It realizes the handle to a specific
 * Body class.
 *
 * <pre>
 * struct DataRep : public _Body {
 *   void baa() {}
 * };
 *
 * class VarAccess {
 *   private:
 *     _DHandle<DataTypeRep> _rep;
 *   public:
 *     void baa() { return _rep->baa(); }
 * };
 *
 * class PtrAccess : _DHandle<DataRep> {
 * };
 *
 * VarAccess v;
 * v.baa();
 *
 * PtrAccess p;
 * p->baa();
 * (*p).baa();
 * </pre>
 *
 **/
class _Handle {

  private:

    /**
     * The _Body
     **/
    _Body * _body;

    static void * operator new( size_t );

  public:

    /**
     * Constructor.
     **/
    _Handle() : _body( 0 ) {}
    /**
     * Constructor.
     **/
    _Handle( _Body * body_r ) : _body( body_r ) {
      _Body::ref( _body );
    }
    /**
     * CopyConstructor.
     **/
    _Handle( const _Handle & rhs ) : _body( rhs._body ) {
      _Body::ref( _body );
    }
    /**
     * Assignment.
     **/
    _Handle & operator=( _Body * rhs ) {
      if ( rhs != _body ) {
	_Body::unref( _body );
	_body = rhs;
	_Body::ref( _body );
      }
      return *this;
    }
    /**
     * Assignment.
     **/
    _Handle & operator=( const _Handle & rhs ) {
      return operator=( rhs._body );
    }
    /**
     * Destructor.
     **/
    virtual ~_Handle() {
      _Body::unref( _body );
    }

  public:

    /**
     * Test for _body.
     **/
    operator const void *() const { return _body; }

    /**
     * Forward access to _body.
     **/
    _Body * operator->() { return _body; }

    /**
     * Forward const access to _body.
     **/
    const _Body * operator->() const { return _body; }

    /**
     * Access _body.
     **/
    _Body & operator*() { if ( !_body ) throw( this ); return *_body; }

    /**
     * Const access _body.
     **/
    const _Body & operator*() const { if ( !_body ) throw( this ); return *_body; }

  public:

    /**
     * Derived classes may overload this to realize
     * std::ostream & operator<< for _Handle classes.
     *
     **/
    virtual std::ostream & dumpOn( std::ostream & str ) const;
};

/**
 * Stream output operator for _Handle classes.
 **/
std::ostream & operator<<( std::ostream & str, const _Handle & obj );

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//      CLASS NAME : _DHandle<_Tp>
/**
 * @short Handle/Body: Handle template for specifc body
 *
 * See class @ref _Handle.
 **/
template<typename _Tp> struct _DHandle : public _Handle {

  _DHandle() : _Handle() {}

  _DHandle( _Body * body_r ) : _Handle( body_r ) {}

  _Tp * operator->() {
    return static_cast<_Tp *>(_Handle::operator->());
  }

  const _Tp * operator->() const {
    return static_cast<const _Tp *>(_Handle::operator->());
  }

  _Tp & _D() {
    return *operator->();
  }

  const _Tp & _D() const {
    return *operator->();
  }
};

///////////////////////////////////////////////////////////////////

#endif // _Handle_h
