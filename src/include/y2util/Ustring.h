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

   File:       Ustring.h

   Author:     Michael Andres <ma@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

/-*/
#ifndef Ustring_h
#define Ustring_h

#include <iostream>
#include <string>

#include <y2util/hash.h>

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : UstringHash
/**
 * @short A Hash of unique strings.
 *
 * A Hash of unique strings.
 *
 * @see Ustring
 **/
class UstringHash {

  protected:

    ///////////////////////////////////////////////////////////////////
    //
    //	CLASS NAME : UstringHash_el
    /**
     * @short Content type of the UstringHash hash.
     *
     * Content type of the UstringHash hash. Comparinson is done by
     * comparing strings. That way existing strings can be found
     * by hash functions.
     *
     * @see UstringHash
     **/
    struct UstringHash_el {

	const std::string _str;

	explicit UstringHash_el( const std::string & str_r ) : _str( str_r ) {}

        bool operator == ( const UstringHash_el & rhs ) const { return( _str == rhs._str ); }
        bool operator != ( const UstringHash_el & rhs ) const { return( _str != rhs._str ); }
    };

    /**
     * Hash function for UstringHash_el
     **/
    friend size_t hashfun( const UstringHash_el & ustr_r ){
      return hashfun( ustr_r._str.c_str() );
    }
    //
    ///////////////////////////////////////////////////////////////////

    typedef noval_hash<UstringHash_el> UstringHash_type;

    /**
     * The hash of UstringHash_el
     **/
    UstringHash_type _UstringHash;

  public:

    /**
     * Store the passed std::string in the hash, if it does not already
     * contain a string with the same value. Return the unique string from
     * the hash.
     **/
    const std::string & add( const std::string & nstr_r ) {
      UstringHash_type::list_type * el = _UstringHash.insert( UstringHash_el( nstr_r ) );
      return el->key._str;
    }

    /**
     * Return the number of unique strings stored in the hash.
     **/
    unsigned size() const { return _UstringHash.size(); }
};

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : Ustring
/**
 * @short Unique strings
 *
 * Ustring provides an immutable string and uses a UstringHash
 * to keep the strings representaion class (which contains the
 * actual data) unique.
 *
 * That way Ustrings of the same value and unsing the same
 * UstringHash can be stored at various locations, while the
 * actual string data are stored only once.
 *
 * The UstringHash to use is passed to the constructor, and no
 * more needed after the string has been stored.
 *
 * Conversion to string is possible and cheap, as the created
 * string will, unless he's modified, share it's data with the
 * one inside the Ustring.
 *
 * Comparison between Ustrings and strings are based on string.
 *
 * A <CODE>-></CODE> operator is provided as an easy way to
 * invoke const string methods, like size() or c_str().
 *
 * <PRE>
 *   ustr->size(); // short for ((const std::string &)u).size();
 * </PRE>
 *
 * Most common usage will be deriving somne class from Ustring,
 * that provides a static UstringHash, and some constructor.
 * Everything else is provided by Ustring.
 *
 * <PRE>
 * class PkgName : public Ustring {
 *   private:
 *     static UstringHash _nameHash;
 *   public:
 *     explicit PkgName( const std::string & n = "" ) : Ustring( _nameHash, n ) {}
 * };
 * </PRE>
 *
 * @see UstringHash
 **/
class Ustring {

  private:

    /**
     * !!! It should actualy be const !!!
     * But that way default copy and assingment can be used.
     **/
    std::string _name;

  public:

    /**
     * Constructor calls @ref UstringHash::add on the given string,
     * and stores the string returned from the hash.
     **/
    Ustring( UstringHash & nameHash_r, const std::string & n  )
      :_name( nameHash_r.add( n ) )
    {}

  public:

    /**
     * Conversion to const std::string &
     **/
    operator const std::string & () const { return _name; }

    /**
     * Conversion to const std::string &
     **/
    const std::string & asString() const { return _name; }

    /**
     * ustr->size(); // short for ((const std::string &)ustr).size();
     **/
    const std::string * operator->() const { return & _name; }

    // operator ==

    friend bool operator==( const Ustring & lhs, const Ustring & rhs ) {
      return ( (const std::string &)lhs == (const std::string &)rhs );
    }

    friend bool operator==( const Ustring & lhs, const std::string & rhs ) {
      return ( (const std::string &)lhs == rhs );
    }

    friend bool operator==( const std::string & lhs, const Ustring & rhs ) {
      return ( lhs == (const std::string &)rhs );
    }

    // operator !=

    friend bool operator!=( const Ustring & lhs, const Ustring & rhs ) {
      return ( ! operator==( lhs, rhs ) );
    }

    friend bool operator!=( const Ustring & lhs, const std::string & rhs ) {
      return ( ! operator==( lhs, rhs ) );
    }

    friend bool operator!=( const std::string & lhs, const Ustring & rhs ) {
      return ( ! operator==( lhs, rhs ) );
    }

    // operator <

    friend bool operator<( const Ustring & lhs, const Ustring & rhs ) {
      return ( (const std::string &)lhs < (const std::string &)rhs );
    }

    friend bool operator<( const Ustring & lhs, const std::string & rhs ) {
      return ( (const std::string &)lhs < rhs );
    }

    friend bool operator<( const std::string & lhs, const Ustring & rhs ) {
      return ( lhs < (const std::string &)rhs );
    }

    // operator >

    friend bool operator>( const Ustring & lhs, const Ustring & rhs ) {
      return ( (const std::string &)lhs > (const std::string &)rhs );
    }

    friend bool operator>( const Ustring & lhs, const std::string & rhs ) {
      return ( (const std::string &)lhs > rhs );
    }

    friend bool operator>( const std::string & lhs, const Ustring & rhs ) {
      return ( lhs > (const std::string &)rhs );
    }

    // operator >=

    friend bool operator>=( const Ustring & lhs, const Ustring & rhs ) {
      return ( ! operator<( lhs, rhs ) );
    }

    friend bool operator>=( const Ustring & lhs, const std::string & rhs ) {
      return ( ! operator<( lhs, rhs ) );
    }

    friend bool operator>=( const std::string & lhs, const Ustring & rhs ) {
      return ( ! operator<( lhs, rhs ) );
    }

    // operator <=

    friend bool operator<=( const Ustring & lhs, const Ustring & rhs ) {
      return ( ! operator>( lhs, rhs ) );
    }

    friend bool operator<=( const Ustring & lhs, const std::string & rhs ) {
      return ( ! operator>( lhs, rhs ) );
    }

    friend bool operator<=( const std::string & lhs, const Ustring & rhs ) {
      return ( ! operator>( lhs, rhs ) );
    }

    // IO

    friend std::ostream & operator<<( std::ostream & str, const Ustring & obj ) {
      return str << (const std::string &)obj;
    }
};

///////////////////////////////////////////////////////////////////

inline size_t hashfun( const Ustring & ustr_r ) {
  return hashfun( (const std::string &)ustr_r );
}

///////////////////////////////////////////////////////////////////

#endif // Ustring_h
