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
 * <B>Caveat:</B> Operations on BitFields with different size may lead to
 * unexpected reults, as new bits value is always '0':
 * <PRE>
 *   a = "0100"; b = "0000"; c = "0"
 *
 *   a & ~b  =  "0100" & "1111"  =  "0100"
 *   a & ~c  =  "0100" & "1"     =  "0000"
 * </PRE>
 * <B>This should be improved</B>, by introducing a 'virtual bit' that
 * determines the value of bits not actually stored. And it would prevent
 * the necessity to store bits we actually dont need.
 * <PRE>
 *   a = "01(0*)"; b = "(0*)"; c = "(0*)"

 *   a & ~b  =  a & ~c  =  "01(0*)" & "(1*)"  =  "01(0*)"
 * </PRE>
 *
 * <h4>Operators provided</h4>
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
 * <h4>Functions for individual bit manipulation</h4>
 * Each function may address a single bit, a range of bits, all bits
 * in the array. A range is defined by start posistion and length. A
 * length of BitField::npos means 'up to the end of the BitField',
 * or zero length if the start position is outside the array.
 * <PRE>
 * test     - return the bit value (on ranges: whether at least one bit is set)
 * assign   - assign the bit value
 * set      - set bit to '1'(true)
 * clear    - set bit to '0'(false)
 * invert   - toggle value
 * </PRE>
 *
 * <h4>Iteration</h4>
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

    static const size_type npos = (size_type)-1;

  private:

    QBitArray _rep;

    bool _valid( size_type pos ) const;

    void _assert_pos  ( size_type pos );
    void _assert_range( size_type pos, size_type len );

    void _doinvert( size_type pos );
    void _doassign( size_type pos, bool val );

  public:

    ///////////////////////////////////////////////////////////////////
    // constructor
    // DEFAULT: BitField( const BitField & rhs )
    // DEFAULT: BitField & operator = ( const BitField & rhs );
    ///////////////////////////////////////////////////////////////////

    /**
     * Constuct an empty BitField
     **/
    BitField() {}

    /**
     * Constuct a BitField of given 'size' with all bits set to 'val'
     **/
    explicit BitField( size_type size, bool val = false );

    /**
     * Constuct a BitField out of a string. The BitField will be of the
     * same size as the given string. The corresponding position of all
     * characters in 'from' that occur in 'set' will be set to '1'.
     * <PRE>
     *   BitField( "10011" )       ==> "10011"
     *   BitField( "abcba" )       ==> "00000"
     *   BitField( "abcba", "a" )  ==> "10001"
     *   BitField( "abcba", "ac" ) ==> "10101"
     * </PRE>
     **/
    explicit BitField( const std::string & from, const std::string & set = "1" );

    ~BitField() {}

    ///////////////////////////////////////////////////////////////////
    // constructive operators
    ///////////////////////////////////////////////////////////////////

    /**
     *
     **/
    friend BitField operator &  ( const BitField & lhs, const BitField & rhs );
    friend BitField operator |  ( const BitField & lhs, const BitField & rhs );
    friend BitField operator ^  ( const BitField & lhs, const BitField & rhs );
    friend BitField operator -  ( const BitField & lhs, const BitField & rhs );
    friend BitField operator ~  ( const BitField & lhs );
#if 0
    friend BitField operator << ( const BitField & lhs, size_type cnt );
    friend BitField operator >> ( const BitField & lhs, size_type cnt );
#endif

    ///////////////////////////////////////////////////////////////////
    // assignment operators
    ///////////////////////////////////////////////////////////////////

    /**
     *
     **/
    BitField & operator  &= ( const BitField & rhs );
    BitField & operator  |= ( const BitField & rhs );
    BitField & operator  ^= ( const BitField & rhs );
    BitField & operator  -= ( const BitField & rhs );
    BitField & complement();
#if 0
    BitField & operator <<= ( size_type cnt );
    BitField & operator >>= ( size_type cnt );
#endif

    ///////////////////////////////////////////////////////////////////
    // equality tests
    ///////////////////////////////////////////////////////////////////

    /**
     *
     **/
    friend bool operator == ( const BitField & lhs, const BitField & rhs );
    friend bool operator != ( const BitField & lhs, const BitField & rhs );

    ///////////////////////////////////////////////////////////////////
    // subset tests
    ///////////////////////////////////////////////////////////////////

#if 0
    /**
     *
     **/
    friend bool operator <  ( const BitField & lhs, const BitField & rhs );
    friend bool operator <= ( const BitField & lhs, const BitField & rhs );
    friend bool operator >  ( const BitField & lhs, const BitField & rhs );
    friend bool operator >= ( const BitField & lhs, const BitField & rhs );
#endif

    ///////////////////////////////////////////////////////////////////
    // status
    ///////////////////////////////////////////////////////////////////

    /**
     * Return the size of the BitField
     **/
    size_type size() const;

    /**
     * Return true if the BitField is empty
     **/
    bool empty() const { return ( size() == 0 ); }

    /**
     * Return the number of 'val' bits in the BitField
     **/
    size_type count( bool val = true ) const;

    ///////////////////////////////////////////////////////////////////
    // bit manipulation
    ///////////////////////////////////////////////////////////////////

    /**
     *
     **/
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

    ///////////////////////////////////////////////////////////////////
    // concat
    ///////////////////////////////////////////////////////////////////

    /**
     * Return the concatenation of two BitFields
     **/
    friend BitField concat( const BitField & lhs, const BitField & rhs );

    /**
     * Append the given BitField to this one
     **/
    BitField & append( const BitField & rhs );

    ///////////////////////////////////////////////////////////////////
    // get ranges
    ///////////////////////////////////////////////////////////////////

    /**
     * Return a copy of this BitField starting at position 'pos' and of
     * length 'len'. A length of BitField::npos means 'up to the end of
     * the BitField', or zero length if the start position is outside the array.
     **/
    BitField at( size_type pos, size_type len = 1 ) const;

    /**
     * Return a copy of this BitField up to ( not including ) position 'pos'.
     * If this BitField is shorter the copy it's filled up with '0'.
     **/
    BitField before( size_type pos ) const { return at( 0, pos ); }

    /**
     * Return a copy of this BitField up to ( including ) position 'pos'.
     * If this BitField is shorter the copy it's filled up with '0'.
     **/
    BitField through( size_type pos ) const { return before( pos + 1 ); }

    /**
     * Return a copy of this BitField starting at position 'pos'.
     * If 'pos' is outside the array an empty BitField is returned.
     **/
    BitField from( size_type pos ) const { return at( pos, npos ); }

    /**
     * Return a copy of this BitField starting immediately after position 'pos'.
     * If this is outside the array an empty BitField is returned.
     **/
    BitField after( size_type pos ) const { return from( pos + 1 ); }

    ///////////////////////////////////////////////////////////////////
    // deletion
    ///////////////////////////////////////////////////////////////////

    /**
     * Delete 'len' bits from this BitField starting at position 'pos'.
     * A length of BitField::npos means 'up to the end of the BitField',
     * or no bits if the start position is outside the array.
     **/
    BitField & delat( size_type pos, size_type len = 1 );

    /**
     * Delete all bits before position 'pos' from this BitField. The bit at
     * 'pos' will become the first bit of the BitField. If 'pos' is outside
     * the array, the array will be empty afterwards.
     **/
    BitField & delbefore( size_type pos ) { return delat( 0, pos ); }

    /**
     * Delete all bits up to ( including ) position 'pos' from this BitField.
     * The bit immediately following 'pos' will become the first bit of the
     * BitField, or the array will be empty, if there is no such bit.
     **/
    BitField & delthrough( size_type pos ) { return delbefore( pos + 1 ); }

    /**
     * Delete all bits starting at position 'pos' up to the end of the BitField.
     * If 'pos' is outside the array, nothing will happen.
     **/
    BitField & delfrom( size_type pos ) { return delat( pos, npos ); }

    /**
     * Delete all bits after position 'pos' up to the end of the BitField.
     * The bit at 'pos' will become the last bit of the array.
     * If 'pos' is outside the array, nothing will happen.
     **/
    BitField & delafter( size_type pos ) { return delfrom( pos + 1 ); }

    ///////////////////////////////////////////////////////////////////
    // resize
    ///////////////////////////////////////////////////////////////////

    /**
     * Resize the BitField to size 'len'. Superfluous bits will be deleted, missing
     * bits will be filled with '0'. If 'len' is BitField::npos nothing will happen.
     **/
    BitField & resize( size_type len );

    /**
     * Delete all bits before 'pos' from the BitField and resize the result
     * to size 'len'. Superfluous bits will be deleted, missing bits will be
     * filled with '0'. A length of BitField::npos will not resize the result,
     * thus it's the same as 'delbefore( pos )'.
     **/
    BitField & clipto( size_type pos, size_type len ) { delbefore( pos ); return resize( len ); }

    ///////////////////////////////////////////////////////////////////
    // iteration
    ///////////////////////////////////////////////////////////////////

    /**
     * Return the position of the next bit of value 'val' after position 'pos',
     * or BitField::npos, if there is none or 'pos' is outside the array.
     * If 'pos' is BitField::npos, next() will return the first bit of value 'val'.
     **/
    size_type next( size_type pos, bool val = true ) const;

    /**
     * Return the position of the previous bit of value 'val' before position 'pos',
     * or BitField::nposif there is none. If 'pos' is BitField::npos or outside the
     * array, prev() will return the last bit of value 'val'.
     **/
    size_type prev( size_type pos, bool val = true ) const;

    /**
     * Return the position of the first bit of value 'val', or BitField::npos
     * if there is none.
     **/
    size_type first( bool val = true ) const { return next( npos, val ); }

    /**
     * Return the position of the last bit of value 'val', or BitField::npos
     * if there is none.
     **/
    size_type last( bool val = true ) const { return prev( npos, val ); }

    ///////////////////////////////////////////////////////////////////
    // conversion & IO
    ///////////////////////////////////////////////////////////////////

    /**
     * Convert the BitField to string using 't' for all set and 'f' for all
     * unset bits.
     * <PRE>
     *    BitField a( "0110" );
     *    a.asString()           ==> "0110"
     *    a.asString( 't', 'f' ) ==> "fttf"
     * </PRE>
     **/
    std::string asString( char t = '1', char f = '0' ) const;

    /**
     * Writes out the BitField as string enclosed in '"'
     **/
    friend std::ostream & operator<<( std::ostream & str, const BitField & obj );
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
// concat
///////////////////////////////////////////////////////////////////

inline BitField concat( const BitField & lhs, const BitField & rhs )
{
  BitField r( lhs ); return r.append( rhs );
}

///////////////////////////////////////////////////////////////////
// IO
///////////////////////////////////////////////////////////////////

inline std::ostream & operator<<( std::ostream & str, const BitField & obj )
{
  return str << '"' << obj.asString() << '"';
}

#endif // BitField_h
