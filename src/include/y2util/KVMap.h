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

  File:       KVMap.h

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose: Convenience stuff for handling (key,value) pairs

/-*/
#ifndef KVMap_h
#define KVMap_h

#include <iosfwd>
#include <map>

#include <y2util/stringutil.h>

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : _KVMap
/**
 * @short Base class for KVMaps, (key,value) pairs
 **/
struct _KVMap : public std::map<std::string,std::string> {

  /**
   * (key,value) map type
   **/
  typedef std::map<std::string,std::string> map_type;

  _KVMap()
  {}
  _KVMap( const map_type & kvmap_r )
    : std::map<std::string,std::string>( kvmap_r )
  {}

  /**
   * Test whether key is set.
   **/
  bool has( const std::string & key_r ) const {
    return( find( key_r ) != end() );
  }

  ///////////////////////////////////////////////////////////////////
  //
  //	CLASS NAME : _KVMap::Options
  /**
   * @short Options for conversion of KVMaps to/from string.
   *
   * <b>_kvsplit</b>: The string separating key from value
   *
   * <b>_fsplit</b>:  (key,value) pairs are separated by any nonempty
   * sequence of characers occurring in _fsplit
   *
   * <b>_kvjoin</b>: The string used to join key and value.
   *
   * <b>_fjoin</b>: The string used to separate (key,value) pairs.
   *
   * TODO: Maybe options for exact _fsplit handling and timming of values.
   *
   **/
  struct Options {
    std::string _kvsplit;
    std::string _fsplit;
    std::string _kvjoin;
    std::string _fjoin;
    Options( const std::string & kvsplit_r, const std::string & fsplit_r )
      : _kvsplit( kvsplit_r )
      , _fsplit ( fsplit_r )
      , _kvjoin ( _kvsplit )
      , _fjoin  ( _fsplit )
    {}
    Options( const std::string & kvsplit_r, const std::string & fsplit_r,
	     const std::string & kvjoin_r )
      : _kvsplit( kvsplit_r )
      , _fsplit ( fsplit_r )
      , _kvjoin ( kvjoin_r )
      , _fjoin  ( _fsplit )
    {}
    Options( const std::string & kvsplit_r, const std::string & fsplit_r,
	     const std::string & kvjoin_r, const std::string & fjoin_r )
      : _kvsplit( kvsplit_r )
      , _fsplit ( fsplit_r )
      , _kvjoin ( kvjoin_r )
      , _fjoin  ( fjoin_r )
    {}
  };

  /**
   * @short Options for KVMaps using a single char as separator (e.g. mount options).
   **/
  template<char kv, char f>
  struct CharSep : public Options { CharSep() : Options( string(1,kv), string(1,f) ) {} };

  ///////////////////////////////////////////////////////////////////

  /**
   * Split str_r into (key,value) map, using the separators defined
   * by opts_r.
   **/
  static map_type split( const std::string & str_r,
			 const Options & opts_r ) {
    map_type ret;
    std::vector<std::string> fields;
    stringutil::split( str_r, fields, opts_r._fsplit );

    for ( unsigned i = 0; i < fields.size(); ++i ) {
      std::string::size_type pos = fields[i].find( opts_r._kvsplit );
      if ( pos == std::string::npos ) {
	ret[fields[i]] = "";
      } else {
	ret[fields[i].substr( 0, pos )] = fields[i].substr( pos+1 );
      }
    }

    return ret;
  }

  /**
   * Join (key,value) map into string, using the separators defined
   * by opts_r.
   **/
  static std::string join( const map_type & kvmap_r,
			   const Options & opts_r ) {
    std::string ret;

    for ( map_type::const_iterator it = kvmap_r.begin(); it != kvmap_r.end(); ++it ) {
      if ( ! ret.empty() ) {
	ret += opts_r._fjoin;
      }
      ret += it->first;
      if ( !it->second.empty() ) {
	ret += opts_r._kvjoin + it->second;
      }
    }

    return ret;
  }

};

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : KVMap<KVMapOpts>
/**
 * @short A map of (key,value) strings.
 *
 * The template argument defines the @ref _KVMap::Options for
 * split and join.
 *
 * E.g. mount options (a comma separated list of key[=value] pairs)
 * could be handled by KVMap<_KVMap::Comma>.
 **/
template<typename KVMapOpts>
struct KVMap : public _KVMap {

  KVMap()
  {}
  KVMap( const char * str_r )
    : _KVMap( split( (str_r?str_r:""), KVMapOpts() ) )
  {}
  KVMap( const std::string & str_r )
    : _KVMap( split( str_r, KVMapOpts() ) )
  {}
  KVMap( const map_type & map_r )
    : _KVMap( map_r )
  {}

  ~KVMap() {}

  std::string asString() const {
    return join( *this, KVMapOpts() );
  }

};

///////////////////////////////////////////////////////////////////

template<typename KVMapOpts>
std::ostream & operator<<( std::ostream & str, const KVMap<KVMapOpts> & obj ) {
  return str << obj.asString();
}

///////////////////////////////////////////////////////////////////

#endif // KVMap_h
