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

   File:       BitField.cc

   Author:     Michael Andres <ma@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

/-*/

#include <iostream>

#include <y2util/BitField.h>

using namespace std;

///////////////////////////////////////////////////////////////////
//
// NOTE:
// =====
//
// QBITARRAY uses a pointer to an explicitly shared data block
// that contains a reference count and the data.
// In other words, it DOES NOT COPY ON WRITE.
//
// Carefully think about, whether you're changing _rep's data or
// not, as other BitFields may reference it too.
//
// Using QBitArray::operator= is save, because it doesn't change
// the data, but lets the QBitArray reference an other data block.
//
// BE SHURE TO call QBitArray::detach() prior to any modification
// of _rep's data block (as fill, resize, &=, |=, ^= or bit maipulation).
//
// QBitArray::detach() will copy the data block if, and only if, it's
// reference count is greater than 1. Thus it's quite cheap to call
// QBitArray::detach(), even if you already own an individual copy.
// But try to avoid calling it, if you don't actualy modify the data.
// It schouldn't do any harm, but waste some memory.
//
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::BitField
//	METHOD TYPE : Constructor
//
//	DESCRIPTION :
//
BitField::BitField( size_type size, bool val )
    : _rep( size )
{
  _rep.fill( val );
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::BitField
//	METHOD TYPE : Constructor
//
//	DESCRIPTION :
//
BitField::BitField( const string & from, const string & set )
    : _rep( from.size() )
{
  _rep.fill( false );
  for ( string::size_type i = from.find_first_of( set );
	i != string::npos; i = from.find_first_of( set, i+1 ) ) {
    _rep.setBit( i );
  }
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::size
//	METHOD TYPE : BitField::size_type
//
//	DESCRIPTION :
//
BitField::size_type BitField::size() const
{
  return _rep.size();
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::_valid
//	METHOD TYPE : bool
//
//	DESCRIPTION :
//
inline bool BitField::_valid( size_type pos ) const
{
  return ( pos < _rep.size() );
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::_assert_range
//	METHOD TYPE : void
//
//	DESCRIPTION :
//
inline void BitField::_assert_range( size_type pos, size_type len )
{
  size_type nsze( pos + len );
  if ( nsze <= _rep.size() ) {
    return;
  }
  _rep.detach();
  _rep.resize( nsze );
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::_assert_pos
//	METHOD TYPE : void
//
//	DESCRIPTION :
//
inline void BitField::_assert_pos( size_type pos )
{
  if ( !_valid( pos ) ) {
    _assert_range( pos, 1 );
  }
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::_doinvert
//	METHOD TYPE : void
//
//	DESCRIPTION :
//
inline void BitField::_doinvert( size_type pos )
{
  _rep.detach();
  _rep.toggleBit( pos );
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::_doassign
//	METHOD TYPE : void
//
//	DESCRIPTION :
//
inline void BitField::_doassign( size_type pos, bool val )
{
  if ( _rep.testBit( pos ) != val ) {
    _doinvert( pos );
  }
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::operator &=
//	METHOD TYPE : BitField &
//
//	DESCRIPTION :
//
BitField & BitField::operator &=( const BitField & rhs )
{
  _rep = _rep & rhs._rep;
  return *this;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::operator|=
//	METHOD TYPE : BitField &
//
//	DESCRIPTION :
//
BitField & BitField::operator|=( const BitField & rhs )
{
  _rep = _rep | rhs._rep;
  return *this;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::operator^=
//	METHOD TYPE : BitField &
//
//	DESCRIPTION :
//
BitField & BitField::operator^=( const BitField & rhs )
{
  _rep = _rep ^ rhs._rep;
  return *this;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::operator -=
//	METHOD TYPE : BitField &
//
//	DESCRIPTION :
//
BitField & BitField::operator -=( const BitField & rhs )
{
  _rep = _rep & ~rhs._rep;
  return *this;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::complement
//	METHOD TYPE : BitField &
//
//	DESCRIPTION :
//
BitField & BitField::complement()
{
  _rep = ~_rep;
  return *this;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::operator <<=
//	METHOD TYPE : BitField &
//
//	DESCRIPTION :
//
#if 0
BitField & BitField::operator <<=( size_type cnt )
{
  _rep.detach();
  return *this;
}
#endif

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::operator >>=
//	METHOD TYPE : BitField &
//
//	DESCRIPTION :
//
#if 0
BitField & BitField::operator >>=( size_type cnt )
{
  _rep.detach();
  return *this;
}
#endif

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::append
//	METHOD TYPE : BitField &
//
//	DESCRIPTION :
//
BitField & BitField::append( const BitField & rhs )
{
  if ( ! rhs._rep.size() ) {
    return *this; // nothing to append
  }

  if ( !_rep.size() ) {
    _rep = rhs._rep;
  } else {
    _rep.detach();
    size_type osize = _rep.size();
    _rep.resize( osize + rhs._rep.size() );
    for ( size_type i = 0; i < rhs._rep.size(); ++i, ++osize ) {
      _rep.setBit( osize, rhs._rep.testBit( i ) );
    }
  }
  return *this;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::at
//	METHOD TYPE : BitField
//
//	DESCRIPTION :
//
BitField BitField::at( size_type pos, size_type len ) const
{
  if ( ! len )
    return BitField();

  if ( len == npos ) {
    if ( pos < _rep.size() )
      len = _rep.size() - pos;
    else
      return BitField();
  }

  BitField ret( len );
  for ( unsigned i = next( pos - 1 ); i != npos && i - pos < len; i = next( i ) ) {
    ret.set( i - pos );
  }
  return ret;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::delat
//	METHOD TYPE : BitField &
//
//	DESCRIPTION :
//
BitField & BitField::delat( size_type pos, size_type len )
{
  if ( ! ( _valid( pos ) && len ) )
    return *this;

  if ( len == npos )
    len = _rep.size() - pos;

  return operator=( concat( before( pos ), from( pos + len ) ) );
}


///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::resize
//	METHOD TYPE : BitField &
//
//	DESCRIPTION :
//
BitField & BitField::resize( size_type len )
{
  if ( len == npos || len == _rep.size() )
    return *this;

  _assert_range( 0, len );
  return delfrom( len );
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::next
//	METHOD TYPE : BitField::size_type
//
//	DESCRIPTION :
//
BitField::size_type BitField::next( size_type pos, bool val ) const
{
  for ( ++pos; pos < _rep.size(); ++pos ) {
    if ( _rep.testBit( pos ) == val ) {
      return pos;
    }
  }
  return npos;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::prev
//	METHOD TYPE : BitField::size_type
//
//	DESCRIPTION :
//
BitField::size_type BitField::prev( size_type pos, bool val ) const
{
  if ( pos > _rep.size() )
    pos = _rep.size();

  while( pos && --pos < _rep.size() ) {
    if ( _rep.testBit( pos ) == val ) {
      return pos;
    }
  }
  return npos;
}

/******************************************************************
**
**
**	FUNCTION NAME : operator ==
**	FUNCTION TYPE : bool
**
**	DESCRIPTION :
*/
bool operator ==( const BitField & lhs, const BitField & rhs )
{
  return ( lhs._rep == rhs._rep );
}

/******************************************************************
**
**
**	FUNCTION NAME : operator <
**	FUNCTION TYPE : bool
**
**	DESCRIPTION :
*/
#if 0
bool operator <( const BitField & lhs, const BitField & rhs )
{

}
#endif

/******************************************************************
**
**
**	FUNCTION NAME : operator <=
**	FUNCTION TYPE : bool
**
**	DESCRIPTION :
*/
#if 0
bool operator <=( const BitField & lhs, const BitField & rhs )
{

}
#endif

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::test
//	METHOD TYPE : bool
//
//	DESCRIPTION :
//
bool BitField::test( size_type pos ) const
{
  if ( _valid( pos ) ) {
    return _rep.testBit( pos );
  }
  return false;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::test
//	METHOD TYPE : bool
//
//	DESCRIPTION :
//
bool BitField::test( size_type pos, size_type len ) const
{
  if ( !( len && _valid( pos ) ) ) {
    return false;
  }

  if ( pos + len > _rep.size() ) {
    len = _rep.size() - pos;
  }
  for ( ; len; ++pos, --len ) {
    if ( _rep.testBit( pos ) ) {
      return true;
    }
  }
  return false;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::test
//	METHOD TYPE : bool
//
//	DESCRIPTION :
//
bool BitField::test() const
{
  if ( !_rep.size() ) {
    return false;
  }
  return test( 0, _rep.size() );
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::assign
//	METHOD TYPE : void
//
//	DESCRIPTION :
//
void BitField::assign( size_type pos, bool val )
{
  _assert_pos( pos );
  _doassign( pos, val );
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::assign
//	METHOD TYPE : void
//
//	DESCRIPTION :
//
void BitField::assign( size_type pos, size_type len, bool val )
{
  if ( !len ) {
    return;
  }

  if ( len == npos ) {
    if ( pos < _rep.size() )
      len = _rep.size() - pos;
    else
      return;
  }

  _assert_range( pos, len );
  for ( ; len; ++pos, --len ) {
    _doassign( pos, val );
  }
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::assign
//	METHOD TYPE : void
//
//	DESCRIPTION :
//
void BitField::assign( bool val )
{
  for ( size_type pos = 0; pos <_rep.size(); ++pos ) {
    if ( _rep.testBit( pos ) != val ) {
      _rep.detach();
      _rep.fill( val );

      return;
    }
  }
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::invert
//	METHOD TYPE : void
//
//	DESCRIPTION :
//
void BitField::invert( size_type pos )
{
  _assert_pos( pos );
  _doinvert( pos );
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::invert
//	METHOD TYPE : void
//
//	DESCRIPTION :
//
void BitField::invert( size_type pos, size_type len )
{
  if ( !len ) {
    return;
  }

  if ( len == npos ) {
    if ( pos < _rep.size() )
      len = _rep.size() - pos;
    else
      return;
  }

  _assert_range( pos, len );
  for ( ; len; ++pos, --len ) {
    _doinvert( pos );
  }
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::count
//	METHOD TYPE : size_type
//
//	DESCRIPTION :
//
BitField::size_type BitField::count( bool val ) const
{
  size_type ret = 0;
  for ( size_type pos = 0; pos < _rep.size(); ++pos ) {
    if ( _rep.testBit( pos ) == val ) {
      ++ret;
    }
  }
  return ret;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : BitField::asString
//	METHOD TYPE : string
//
//	DESCRIPTION :
//
string BitField::asString( char t, char f ) const
{
  string ret( size(), f );
  for ( size_type pos = 0; pos < _rep.size(); ++pos ) {
    if( _rep.testBit( pos ) )
      ret[pos] = t;
  }
  return ret;
}

