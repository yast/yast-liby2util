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

   File:       Y2SLog.cc

   Author:     Michael Andres <ma@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

/-*/

#include <cstdlib>
#include <string>

#include <qasciidict.h>

#include <y2util/Y2SLog.h>

using namespace std;

///////////////////////////////////////////////////////////////////
//
namespace Y2SLog {
//
///////////////////////////////////////////////////////////////////

class Y2Loglinestreamset;

static ostream                        no_stream_Fm( 0 );
static QAsciiDict<Y2Loglinestreamset> streamset_VpCm( 17, true, false );

static bool init();

bool dbg_enabled_bm( init() );

/******************************************************************
**
**
**	FUNCTION NAME : init
**	FUNCTION TYPE : QAsciiDict<Y2Loglinestreamset>
**
**	DESCRIPTION :
*/
static bool init() {
  streamset_VpCm.setAutoDelete( true );
  Y2Logging::setLogfileName (getenv( "Y2SLOG_FILE" ));
  return (getenv( "Y2SLOG_DEBUG" ) != NULL);
}

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : Y2Loglinebuf
//
//	DESCRIPTION :
//
class Y2Loglinebuf : public streambuf {

  private:

    friend class Y2Loglinestream;

    const char *const name;
    const loglevel_t  level;

    const char *      file;
    const char *      func;
    int               line;

    string       buffer;

    virtual streamsize xsputn( const char * s, streamsize n ) {
      return writeout( s, n );
    }

    virtual int overflow( int ch = EOF ) {
      if ( ch != EOF ) {
        writeout( (const char*)&ch, 1 );
      }
      return 0;
    }

    virtual int writeout( const char* s, streamsize n ) {
      if ( s && n ) {
	const char * c = s;
	for ( int i = 0; i < n; ++i, ++c ) {
	  if ( *c == '\n' ) {
	    buffer += string( s, c-s );
	    Y2Logging::y2_logger( level, name, file, line, func, "%s", buffer.c_str() );
	    buffer = "";
	    s = c+1;
	  }
	}
	if ( s < c ) {
	  buffer += string( s, c-s );
	}
      }
      return n;
    }

    Y2Loglinebuf(  const char * myname, const unsigned mylevel )
      : name( myname )
      , level( (loglevel_t)mylevel )
    {
      file = func = "";
      line = -1;
    }

    ~Y2Loglinebuf() {
      if ( buffer.length() )
	writeout( "\n", 1 );
    }
};
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : Y2Loglinestream
//
//	DESCRIPTION :
//
class Y2Loglinestream {

  private:

    Y2Loglinebuf mybuf;
    ostream      mystream;

  public:

    Y2Loglinestream( const char *const name, const unsigned level )
      : mybuf( name, level )
      , mystream( &mybuf )
    {}
    ~Y2Loglinestream() { mystream.flush(); }

  public:

    ostream & getStream( const char * fil, const char * fnc, int lne ) {
      mybuf.file = fil;
      mybuf.func = fnc;
      mybuf.line = lne;
      return mystream;
    }
};
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : Y2Loglinestreamset
//
//	DESCRIPTION :
//
class Y2Loglinestreamset {

  private:

    static const unsigned maxSet_i = 6;

    const string      class_t;
    Y2Loglinestream * set_VpC[maxSet_i];

  public:

    Y2Loglinestreamset( const char * which )
      : class_t( which )
    {
      for ( unsigned i = 0; i < maxSet_i; ++i ) {
	set_VpC[i] = 0;
      }
    }

    ~Y2Loglinestreamset() {
      for ( unsigned i = 0; i < maxSet_i; ++i ) {
	delete set_VpC[i];
      }
    }

    const char * key() const { return class_t.c_str(); }

    ostream & getStream( const unsigned level, const char * fil, const char * fnc, int lne ) {
      if ( level >= maxSet_i ) {
	return no_stream_Fm;
      }
      if ( !set_VpC[level] ) {
	set_VpC[level] = new Y2Loglinestream( class_t.c_str(), level );
      }
      return set_VpC[level]->getStream( fil, fnc, lne );
    }
};

///////////////////////////////////////////////////////////////////

/******************************************************************
**
**
**	FUNCTION NAME : get
**	FUNCTION TYPE : ostream &
**
**	DESCRIPTION :
*/
ostream & get( const char * which, const unsigned level,
	       const char * fil, const char * fnc, const int lne )
{
  Y2Loglinestreamset * set_pCi = streamset_VpCm.find( which );
  if ( !set_pCi ) {
    set_pCi = new Y2Loglinestreamset( which );
    streamset_VpCm.insert( set_pCi->key(), set_pCi );
  }
  return set_pCi->getStream( level, fil, fnc, lne );
}

/******************************************************************
**
**
**	FUNCTION NAME : getdbg
**	FUNCTION TYPE : ostream &
**
**	DESCRIPTION :
*/
ostream & getdbg( const char * which, const unsigned level,
		  const char * fil, const char * fnc, const int lne )
{
  if ( dbg_enabled_bm ) {
    return get( which, level, fil, fnc, lne );
  }
  return no_stream_Fm;
}

} // namespace Y2SLog
///////////////////////////////////////////////////////////////////
