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

  File:       stringutil.h

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose: Contains 'std::string form(const char * format, ...)' for
  printf style creation of strings and some more string utility
  functions.

/-*/
#ifndef stringutil_h
#define stringutil_h

#include <cstdio>
#include <cstdarg>

#include <vector>
#include <string>

/**
 * Utility functions for std::strings. Most of them based on stringutil::form.
 **/
///////////////////////////////////////////////////////////////////
namespace stringutil {
///////////////////////////////////////////////////////////////////

inline std::string form( const char * format, ... )
    __attribute__ ((format (printf, 1, 2)));

/**
 * Printf style building of strings via format string.
 * <PRE>
 * std::string ex( stringutil::form( "Example number %d", 1 ) );
 * std::cout << ex << stringutil::form( " and number %d.", 2 ) << endl;
 *
 * Will print: Example number 1 and number 2.
 * </PRE>
 **/
inline std::string form( const char * format, ... ) {
  char * buf = 0;
  std::string val;

  va_list ap;
  va_start( ap, format );

#if 0
  vasprintf( &buf, format, ap );
  if ( buf ) {
    val = buf;
    free( buf );
  }
#else
  // Don't know wheter we actually nedd two va_lists, one to
  // evaluate the buffer size needed, and one to actually fill
  // the buffer. Maybe there's a save way to reuse a va_lists.
  va_list ap1;
  va_start( ap1, format );
  buf = new char[vsnprintf( NULL, 0, format, ap ) + 1];
  vsprintf( buf, format, ap1 );
  val = buf;
  delete [] buf;
  va_end( ap1 );
#endif

  va_end( ap );
  return val;
}

/**
 * Print number. Optional second argument sets the minimal string width (' ' padded).
 * Negative values will cause the number to be left adjusted within the string. Default
 * width is 0.
 * <PRE>
 * numstring(42)           -> "42"
 * numstring(42, 4)        -> "  42"
 * numstring(42,-4)        -> "42  "
 * </PRE>
 **/
inline std::string numstring( char n,          int w = 0 ) { return form( "%*hhd", w, n ); }
inline std::string numstring( unsigned char n, int w = 0 ) { return form( "%*hhu", w, n ); }
inline std::string numstring( int n,           int w = 0 ) { return form( "%*d",   w, n ); }
inline std::string numstring( unsigned n,      int w = 0 ) { return form( "%*u",   w, n ); }
inline std::string numstring( long n,          int w = 0 ) { return form( "%*ld",  w, n ); }
inline std::string numstring( unsigned long n, int w = 0 ) { return form( "%*lu",  w, n ); }

/**
 * Print number as hex value with leading '0x'. Optional second argument sets the minimal
 * string width (0 padded). Negative values will cause the number to be left adjusted
 * within the string. Default width is 10 (4 for char).
 * <PRE>
 * hexstring(42)           -> "0x0000002a"
 * hexstring(42, 4)        -> "0x2a"
 * hexstring(42,-4)        -> "0x2a"
 * </PRE>
 **/
inline std::string hexstring( char n,          int w = 4 ) { return form( "%#0*hhx", w, n ); }
inline std::string hexstring( unsigned char n, int w = 4 ) { return form( "%#0*hhx", w, n ); }
inline std::string hexstring( int n,           int w = 10 ){ return form( "%#0*x",   w, n ); }
inline std::string hexstring( unsigned n,      int w = 10 ){ return form( "%#0*x",   w, n ); }
inline std::string hexstring( long n,          int w = 10 ){ return form( "%#0*lx",  w, n ); }
inline std::string hexstring( unsigned long n, int w = 10 ){ return form( "%#0*lx",  w, n ); }

/**
 * Print number as octal value with leading '0'. Optional second argument sets the minimal
 * string width (0 padded). Negative values will cause the number to be left adjusted
 * within the string. Default width is 5 (4 for char).
 * <PRE>
 * octstring(42)           -> "00052"
 * octstring(42, 4)        -> "0052"
 * octstring(42,-4)        -> "052 "
 * </PRE>
 **/
inline std::string octstring( char n,          int w = 4 ) { return form( "%#0*hho", w, n ); }
inline std::string octstring( unsigned char n, int w = 4 ) { return form( "%#0*hho", w, n ); }
inline std::string octstring( int n,           int w = 5 ) { return form( "%#0*o",    w, n ); }
inline std::string octstring( unsigned n,      int w = 5 ) { return form( "%#0*o",    w, n ); }
inline std::string octstring( long n,          int w = 5 ) { return form( "%#0*lo",   w, n ); }
inline std::string octstring( unsigned long n, int w = 5 ) { return form( "%#0*lo",   w, n ); }

/**
 * Split line into words
 *
 * <b>singlesep_r = false</b>: Separator is any nonenmpty sequence of characters listed in sep_t.
 * Leading trailing separators are ignored.
 *
 * <b>Example:</b> singlesep_r = false, sep_t = ":"
 * <PRE>
 * ""        -> words 0
 * ":"       -> words 0
 * "a"       -> words 1  |a|
 * "::a"     -> words 1  |a|
 * "::a::"   -> words 1  |a|
 * ":a::b:c:"-> words 3  |a|b|c|
 * </PRE>
 *
 * <b>singlesep_r = true</b>: Separator is any single character occuring in sep_t.
 * Leading trailing separators are not ignored (i.e will cause an empty word).
 *
 * <b>Example:</b> singlesep_r = true, sep_t = ":"
 * <PRE>
 * ""        -> words 0
 * ":"       -> words 2  |||
 * "a"       -> words 1  |a|
 * ":a"      -> words 2  ||a|
 * "a:"      -> words 2  |a||
 * ":a:"     -> words 3  ||a||
 * </PRE>
 *
 **/
extern unsigned split( const std::string          line_r,
                       std::vector<std::string> & words_r,
                       const std::string &        sep_t       = " \t",
                       const bool                 singlesep_r = false );

extern std::string join( const std::vector<std::string> & words_r,
			 const std::string & sep_r = " " );

/**
 * Return string with leading/trailing/surrounding whitespace removed
 **/
extern std::string ltrim( const std::string & s );
extern std::string rtrim( const std::string & s );
inline std::string  trim( const std::string & s ) { return ltrim( rtrim( s ) ); }

/**
 * Return string converted to lower/upper case
 **/
extern std::string toLower( const std::string & s );
extern std::string toUpper( const std::string & s );

///////////////////////////////////////////////////////////////////
}  // namespace stringutil
///////////////////////////////////////////////////////////////////

#endif // stringutil_h
