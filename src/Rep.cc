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

   File:       Rep.cc

   Author:     Michael Andres <ma@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

/-*/

#include <iostream>

#include <y2util/Rep.h>

///////////////////////////////////////////////////////////////////

unsigned Rep::rep_IDs   = 0;
unsigned Rep::rep_Total = 0;

///////////////////////////////////////////////////////////////////
/**
 * #define REP_DEBUG to log constructor/destructor calls.
 *
 * #define REP_DEBUG_REF to additionally log calls to rep_ref()/rep_unref().
 *
 * #define REP_DEBUG_STREAM to the std::ostream that should be used for logging.
 * Default is std::cerr. It might be helpfull, if derived classes overload rep_name().
 *
 **/
#ifdef REP_DEBUG_REF
#  ifndef REP_DEBUG
#  define REP_DEBUG
#  endif
#endif

#ifdef REP_DEBUG
#  ifndef REP_DEBUG_STREAM
#    undef  Y2LOG
#    define Y2LOG "Rep"
#    include <y2util/Y2SLog.h>
#    define REP_DEBUG_STREAM DBG
#  endif
#endif

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : Rep::_dbg
//	METHOD TYPE : void
//
//	DESCRIPTION :
//
void Rep::_dbg( const char f ) const
{
#ifdef REP_DEBUG
  switch ( f ) {
  case 'c':
    ++rep_Total;
    Rep::dumpOn( REP_DEBUG_STREAM << "+++ " ) << std::endl;
    break;
  case 'C':
    ++rep_Total;
    Rep::dumpOn( REP_DEBUG_STREAM << "+c+ " ) << std::endl;
    break;
  case 'd':
    --rep_Total;
    Rep::dumpOn( REP_DEBUG_STREAM << "--- " ) << std::endl;
    break;

#ifdef REP_DEBUG_REF
  case 'r':
    Rep::dumpOn( REP_DEBUG_STREAM << "  + " ) << std::endl;
    break;
  case 'u':
    Rep::dumpOn( REP_DEBUG_STREAM << "  - " ) << std::endl;
    break;
#endif

  }
#endif
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : Rep::dumpOn
//	METHOD TYPE : std::ostream &
//
//	DESCRIPTION :
//
std::ostream & Rep::dumpOn( std::ostream & str ) const
{
  str << rep_name()
#ifdef REP_DEBUG
    << "_" << rep_id
#endif
    << "(" << (const void*)this << " * " << rep_cnt_i
#ifdef REP_DEBUG
    << " [" << rep_Total << "]"
#endif
    << ")";
  return str;
}

/******************************************************************
**
**
**	FUNCTION NAME : operator<<
**	FUNCTION TYPE : std::ostream &
**
**	DESCRIPTION :
*/
std::ostream & operator<<( std::ostream & str, const Rep & obj )
{
  return obj.dumpOn( str );
}

/******************************************************************
**
**
**	FUNCTION NAME : operator<<
**	FUNCTION TYPE : std::ostream &
**
**	DESCRIPTION :
*/
std::ostream & operator<<( std::ostream & str, const constRepPtr & obj )
{
  if ( !obj.baseRep() )
    return str << "Rep(nil)";
  return obj.baseRep()->dumpOn( str );
}
