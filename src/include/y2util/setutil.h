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

  File:       setutil.h

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose: Common set operations

/-*/
#ifndef setutil_h
#define setutil_h

#include <set>

/******************************************************************
**
**
**	FUNCTION NAME : includes
**	FUNCTION TYPE : bool
**
**      Return true if lhs includes rhs
*/
template<typename _T>
bool includes( const std::set<_T> & lhs, const std::set<_T> & rhs )
{
  typename std::set<_T>::const_iterator li = lhs.begin();
  typename std::set<_T>::const_iterator ri = rhs.begin();
  while ( li != lhs.end() && ri != rhs.end() ) {
    if ( *ri < *li ) {
      return false;
    } else if ( *li < *ri ){
      ++li;
    } else {
      ++li;
      ++ri;
    }
  }
  return( ri == rhs.end() );
}

/******************************************************************
**
**
**	FUNCTION NAME : set_union
**	FUNCTION TYPE : set<_T>
*/
template<typename _T>
std::set<_T> set_union( const std::set<_T> & lhs, const std::set<_T> & rhs )
{
  std::set<_T> res( lhs );
  res.insert( rhs.begin(), rhs.end() );
  return res;
}

/******************************************************************
**
**
**	FUNCTION NAME : set_intersection
**	FUNCTION TYPE : set<_T>
*/
template<typename _T>
std::set<_T> set_intersection( const std::set<_T> & lhs, const std::set<_T> & rhs )
{
  std::set<_T> res;
  typename std::set<_T>::const_iterator li = lhs.begin();
  typename std::set<_T>::const_iterator ri = rhs.begin();
  while ( li != lhs.end() && ri != rhs.end() ) {
    if ( *li < *ri ) {
      ++li;
    } else if ( *ri < *li ) {
      ++ri;
    } else {
      res.insert( res.end(), *li );
      ++li;
      ++ri;
    }
  }
  return res;
}

/******************************************************************
**
**
**	FUNCTION NAME : set_difference
**	FUNCTION TYPE : set<_T>
*/
template<typename _T>
std::set<_T> set_difference( const std::set<_T> & lhs, const std::set<_T> & rhs )
{
  std::set<_T> res;
  typename std::set<_T>::const_iterator li = lhs.begin();
  typename std::set<_T>::const_iterator ri = rhs.begin();
  while ( li != lhs.end() && ri != rhs.end() ) {
    if ( *li < *ri ) {
      res.insert( res.end(), *li );
      ++li;
    } else if ( *ri < *li ) {
      ++ri;
    } else {
      ++li;
      ++ri;
    }
  }
  while ( li != lhs.end() ) {
    res.insert( res.end(), *li );
    ++li;
  }
  return res;
}

/******************************************************************
**
**
**	FUNCTION NAME : set_symmetric_difference
**	FUNCTION TYPE : set<_T>
*/
template<typename _T>
std::set<_T> set_symmetric_difference( const std::set<_T> & lhs, const std::set<_T> & rhs )
{
  std::set<_T> res;
  typename std::set<_T>::const_iterator li = lhs.begin();
  typename std::set<_T>::const_iterator ri = rhs.begin();
  while ( li != lhs.end() && ri != rhs.end() ) {
    if ( *li < *ri ) {
      res.insert( res.end(), *li );
      ++li;
    } else if ( *ri < *li ) {
      res.insert( res.end(), *ri );
      ++ri;
    } else {
      ++li;
      ++ri;
    }
  }
  while ( li != lhs.end() ) {
    res.insert( res.end(), *li );
    ++li;
  }
  while ( ri != rhs.end() ) {
    res.insert( res.end(), *ri );
    ++ri;
  }
  return res;
}

#endif // setutil_h
