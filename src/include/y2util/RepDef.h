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

   File:       RepDef.h

   Author:     Michael Andres <ma@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

/-*/
#ifndef RepDef_h
#define RepDef_h

#include <y2util/Rep.h>

#define HANDLE_CLASS(CL) CL##rep; class CL : virtual public RepHandle


#define HANDLE_BODY(CL) \
  private: \
 \
    CL##rep *const& rep_p; \
 \
  public: \
 \
    explicit CL( const Rep * p = 0 ); \
    CL( const CL & rhs ); \
    const CL & operator=( const Rep * rhs ) const; \
    const CL & operator=( const CL & rhs ) const; \
    virtual ~CL() {} \
 \
    const CL##rep * operator->() const { return rep_p; } \
    CL##rep *       operator->()       { return rep_p; } \
 \
  private:


#define REP_CLASS(CL) CL##rep : virtual public Rep


#define REP_BODY(CL) \
  virtual const char * rep_name() const { return #CL; } \
\
  protected: \
 \
    friend class CL; \
 \
    virtual ~CL##rep() {} \
 \
  private:


#define HANDLE_IMPL(CL) \
CL::CL( const Rep * p ) \
    : RepHandle( dynamic_cast<const CL##rep *>(p) ) \
    , rep_p( (CL##rep*const&)RepHandle::rep() ) \
{} \
 \
CL::CL( const CL & rhs ) \
    : RepHandle( rhs ) \
    , rep_p( (CL##rep*const&)RepHandle::rep() ) \
{} \
 \
const CL & CL::operator=( const Rep * rhs ) const \
{ RepHandle::operator=( dynamic_cast<const CL##rep *>(rhs) ); return *this; } \
 \
const CL & CL::operator=( const CL & rhs ) const \
{ RepHandle::operator=( rhs ); return *this; }


///////////////////////////////////////////////////////////////////

#endif // RepDef_h
