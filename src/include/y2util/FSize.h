/* $Header$
//
// FSize.h
//
// Helper class for file size related data
//
// AUTHOR	Michael Andres <ma@suse.de>
// MAINTAINER	Michael Andres <ma@suse.de>
//
// Based on Revision 10.14.1.2  2001/08/27 14:44:12 from YaST_SuSE_Linux_7_3/PkgLib/RCS/FSize.h
//
*/

#ifndef _FSize_h_
#define _FSize_h_

#include <string>
#include <iostream>


///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : FSize
//
//	DESCRIPTION : Store and operate on (file/package/partition)
//                    sizes. Use Str(...) to create various std::string
//                    representaitons of FSize.
//
class FSize {

  public:

    enum UNIT { B = 0, K, M, G };

    friend istream & operator>>( istream & STREAM, FSize & OBJ );
    friend ostream & operator<<( ostream & STREAM, const FSize & OBJ );
    friend ostream & operator<<( ostream & STREAM, const FSize::UNIT & OBJ );

    static const double & Fac( const UNIT u_bv = B ) {
      static const double fac[4] = { 1, 1024, 1024*1024, 1024*1024*1024 };
      return fac[u_bv];
    }

    static const char * Unit( const UNIT u_bv = B ) {
      static const char * unit[4] = { "B", "K", "M", "G" };
      return unit[u_bv];
    }

  private:

    double Size_f;

  public:

    FSize( const double Size_fv = 0.0, const UNIT u_bv = B ) { Set( Size_fv, u_bv ); }
    ~FSize(){}

  public:

    operator double() const { return Size_f; }

    FSize operator-() { return -Size_f ; }

    FSize operator+( const FSize & Rhs ) { return Size_f + Rhs.Size_f; }
    FSize operator-( const FSize & Rhs ) { return Size_f - Rhs.Size_f; }
    FSize operator*( const FSize & Rhs ) { return Size_f * Rhs.Size_f; }
    FSize operator/( const FSize & Rhs ) { return Size_f / Rhs.Size_f; }

    FSize & operator= ( const FSize & Rhs ) { Size_f  = Rhs.Size_f; return *this; }
    FSize & operator+=( const FSize & Rhs ) { Size_f += Rhs.Size_f; return *this; }
    FSize & operator-=( const FSize & Rhs ) { Size_f -= Rhs.Size_f; return *this; }
    FSize & operator*=( const FSize & Rhs ) { Size_f *= Rhs.Size_f; return *this; }
    FSize & operator/=( const FSize & Rhs ) { Size_f /= Rhs.Size_f; return *this; }


    FSize & Set( const double Size_fv, const UNIT u_bv = B ) { Size_f = Size_fv * Fac( u_bv ); return *this; }
    double  operator()( const UNIT u_bv = B ) const          { return Size_f  / Fac( u_bv ); }

  public:

    UNIT   Best() const;

    static int DefStrLen( const bool ShowUnit_bv = true ) { return ShowUnit_bv ? 7 : 5; }

    std::string Str( const UNIT u_bv, int Fw_iv, const bool ShowUnit_bv = true ) const;
    std::string Str( const UNIT u_bv, const bool ShowUnit_bv = true ) const { return Str(   u_bv, DefStrLen( ShowUnit_bv ), ShowUnit_bv ); }
    std::string Str( int Fw_iv, const bool ShowUnit_bv = true )       const { return Str( Best(),                    Fw_iv, ShowUnit_bv ); }
    std::string Str( const bool ShowUnit_bv = true )                  const { return Str( Best(), DefStrLen( ShowUnit_bv ), ShowUnit_bv ); }
};

///////////////////////////////////////////////////////////////////

#endif _FSize_h_
