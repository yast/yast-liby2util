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

   File:       BitField.h

   Author:     Michael Andres <ma@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

/-*/
#ifndef BitField_h
#define BitField_h

#include <iosfwd>
#include <string>

#include <qbitarray.h>

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : BitField
/**
 * A BitField provides an array of bits. The BitField is not
 * limited in size. Any reading access to indices outside the
 * current array will return '0' (false). On writing to indices
 * outside the array, the array will be enlarged. New bits will
 * be '0'.
 *
 * Operators provided:
 * <PRE>
 *                   &  &=           and
 *                   |  |=           or
 *                   ^  ^=           xor
 *                   -  -=           diff  (lhs & ~rhs)
 *                   ~  complement   complement
 * (not implemented) << <<=          lshift
 * (not implemented) >> >>=          rshift
 *                   ==              equal
 *                   !=              not equal
 * (not implemented) <               subset
 * (not implemented) <=              subset or equal
 * (not implemented) >               superset
 * (not implemented) >=              superset or equal
 * </PRE>
 *
 * Functions for individual bit manipulation:
 * <PRE>
 * test     - return the bit value (on ranges: whether at least one bit is set)
 * assign   - assign the bit value
 * set      - set bit to '1'(true)
 * clear    - set bit to '0'(false)
 * invert   - toggle value
 * </PRE>
 * Each function may address a signle bit, a range of bits, all bits
 * in the array.
 *
 * Iteration:
 *
 * <PRE>
 * for ( unsigned i = f.first( true ); i != BitField::npos; i = f.next( i, true ) ) {
 *   forward iterate all bits set to '1'
 * }
 * for ( unsigned i = f.last( true ); i != BitField::npos; i = f.prev( i, true ) ) {
 *   reverse iterate all bits set to '1'
 * }
 * </PRE>
 *
 * @short Provides an array of bits.
 **/
class BitField {

  public:

    typedef unsigned size_type;

  private:

    QBitArray _rep;

    bool _valid( size_type pos ) const;

    void _assert_pos  ( size_type pos );
    void _assert_range( size_type pos, size_type len );

    void _doinvert( size_type pos );
    void _doassign( size_type pos, bool val );

  public:

    // constructor

    BitField() {}
    explicit BitField( size_type size, bool val = false );
    explicit BitField( const std::string & from, const std::string & set = "1" );
    // DEFAULT: BitField( const BitField & rhs )

    ~BitField() {}

    // assignment
    // DEFAULT: BitField & operator=( const BitField & rhs );

    // constructive operators

    friend BitField operator &  ( const BitField & lhs, const BitField & rhs );
    friend BitField operator |  ( const BitField & lhs, const BitField & rhs );
    friend BitField operator ^  ( const BitField & lhs, const BitField & rhs );
    friend BitField operator -  ( const BitField & lhs, const BitField & rhs );
    friend BitField operator ~  ( const BitField & lhs );
#if 0
    friend BitField operator << ( const BitField & lhs, size_type cnt );
    friend BitField operator >> ( const BitField & lhs, size_type cnt );
#endif

    // assignment operators

    BitField & operator  &= ( const BitField & rhs );
    BitField & operator  |= ( const BitField & rhs );
    BitField & operator  ^= ( const BitField & rhs );
    BitField & operator  -= ( const BitField & rhs );
    BitField & complement();
#if 0
    BitField & operator <<= ( size_type cnt );
    BitField & operator >>= ( size_type cnt );
#endif

    // equality tests

    friend bool operator == ( const BitField & lhs, const BitField & rhs );
    friend bool operator != ( const BitField & lhs, const BitField & rhs );

    // subset tests

#if 0
    friend bool operator <  ( const BitField & lhs, const BitField & rhs );
    friend bool operator <= ( const BitField & lhs, const BitField & rhs );
    friend bool operator >  ( const BitField & lhs, const BitField & rhs );
    friend bool operator >= ( const BitField & lhs, const BitField & rhs );
#endif

    // status

    size_type size()   const;
    bool      empty()  const { return ( size() == 0 ); }

    size_type count( bool val = true ) const;

    // bit manipulation

    bool test( size_type pos )                const;
    bool test( size_type pos, size_type len ) const;
    bool test()                               const;

    void assign( size_type pos, bool val );
    void assign( size_type pos, size_type len, bool val );
    void assign( bool val );

    void set( size_type pos )                  { assign( pos, true ); }
    void set( size_type pos, size_type len )   { assign( pos, len, true ); }
    void set()                                 { assign( true ); }

    void clear( size_type pos )                { assign( pos, false ); }
    void clear( size_type pos, size_type len ) { assign( pos, len, false ); }
    void clear()                               { assign( false ); }

    void invert( size_type pos );
    void invert( size_type pos, size_type len );
    void invert()                              { complement(); }

    // concat and split

    friend BitField concat( const BitField & lhs, const BitField & rhs );
    BitField &      append( const BitField & rhs );

    friend BitField range ( const BitField & lhs, size_type pos, size_type len );
    BitField &      clipto( size_type pos, size_type len );
    BitField &      resize( size_type len )   { return clipto( 0, len ); }

    // iteration

    static const size_type npos = (size_type)-1;

    size_type first( bool val = true ) const;
    size_type last ( bool val = true ) const;

    size_type next ( size_type pos, bool val = true ) const;
    size_type prev ( size_type pos, bool val = true ) const;

    // conversion & IO

    std::string asString( char t = '1', char f = '0' ) const;

    std::ostream & dumpOn( std::ostream & str ) const;

    friend std::ostream & operator<<( std::ostream & str, const BitField & obj );

    // iterator
};

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// constructive operators
///////////////////////////////////////////////////////////////////

inline BitField operator &  ( const BitField & lhs, const BitField & rhs )
{
  BitField r( lhs ); return r &= rhs;
}

inline BitField operator |  ( const BitField & lhs, const BitField & rhs )
{
  BitField r( lhs ); return r |= rhs;
}

inline BitField operator ^  ( const BitField & lhs, const BitField & rhs )
{
  BitField r( lhs ); return r ^= rhs;
}

inline BitField operator -  ( const BitField & lhs, const BitField & rhs )
{
  BitField r( lhs ); return r -= rhs;
}

inline BitField operator ~  ( const BitField & lhs )
{
  BitField r( lhs ); return r.complement();
}

#if 0
inline BitField operator << ( const BitField & lhs, size_type cnt )
{
  BitField r( lhs ); return r <<= cnt;
}

inline BitField operator >> ( const BitField & lhs, size_type cnt )
{
  BitField r( lhs ); return r >>= cnt;
}
#endif

///////////////////////////////////////////////////////////////////
// equality tests
///////////////////////////////////////////////////////////////////

inline bool operator != ( const BitField & lhs, const BitField & rhs )
{
  return !( lhs == rhs );
}

///////////////////////////////////////////////////////////////////
// subset  tests
///////////////////////////////////////////////////////////////////

#if 0
inline bool operator >  ( const BitField & lhs, const BitField & rhs )
{
  return ( rhs <= lhs );
}

inline bool operator >= ( const BitField & lhs, const BitField & rhs )
{
  return ( rhs < lhs );
}
#endif

///////////////////////////////////////////////////////////////////
// concat and split
///////////////////////////////////////////////////////////////////

inline BitField concat( const BitField & lhs, const BitField & rhs )
{
  BitField r( lhs ); return r.append( rhs );
}

inline BitField range ( const BitField & lhs, BitField::size_type pos, BitField::size_type len ) {
  BitField r( lhs ); return r.clipto( pos, len );
}

///////////////////////////////////////////////////////////////////
// IO
///////////////////////////////////////////////////////////////////

inline std::ostream & operator<<( std::ostream & str, const BitField & obj )
{
  return obj.dumpOn( str );
}

#endif // BitField_h
