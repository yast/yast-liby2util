/* $Header$
//
// Date.h
//
// Helper class for date related data
//
// AUTHOR	Michael Andres <ma@suse.de>
// MAINTAINER	Michael Andres <ma@suse.de>
//
// Based on Revision 10.14.1.3  2001/08/27 14:44:12 from YaST_SuSE_Linux_7_3/WidLib/../PkgLib/RCS/Date.h
//
*/

#ifndef _Date_h_
#define _Date_h_

extern "C"
{
#include <time.h>
}
#include <string>

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : Date
//
//	DESCRIPTION :
//
class Date {

  friend ostream & operator<<( ostream & outs, const Date & obj ) {
    return outs << obj.Asstd::string();
  }

  private:

    time_t Date_i;

  public:

    /**
     * "YYYYMMDDhhmm" for construct and coding
     */
    Date( time_t i = 0 )          { Date_i = i; }
    Date( const std::string & date_t );

    static Date   Decode( const std::string & date_tv );
    static std::string Encode( const Date &   date_iv );

    operator time_t() const { return Date_i; }

    static const time_t Second_i;
    static const time_t Minute_i;
    static const time_t Hour_i;
    static const time_t Day_i;
    static const time_t HalfYear_i;
    static const time_t Year_i;

    static time_t Now() { return time( 0 ); }

    /**
     * "Wed Jan 13 17:45:41 1999"
     */
    std::string   AsString() const {
      std::string Ret( ctime( &Date_i ) );
      Ret.at( "\n" ) = "";
      return Ret;
    }

    /**
     *  "Feb  3 00:18" for last half year
     *  "Feb  3  1999" else
     */
    std::string ShortDate() const;
};

///////////////////////////////////////////////////////////////////

#endif
