/*---------------------------------------------------------------------\
|                                                                      |
|                      __   __    ____ _____ ____                      |
|                      \ \ / /_ _/ ___|_   _|___ \                     |
|                       \ V / _` \___ \ | |   __) |                    |
|                        | | (_| |___) || |  / __/                     |
|                        |_|\__,_|____/ |_| |_____|                    |
|                                                                      |
|                               core system                            |
|                                                     (C) 2002 SuSE AG |
\----------------------------------------------------------------------/

   File:       timeclass.h
   Purpose:    A timer
   Author:     Ludwig Nussel <lnussel@suse.de>
   Maintainer: Ludwig Nussel <lnussel@suse.de>

/-*/

#ifndef timeclass_h
#define timeclass_h

#include <ostream>

class TimeClass_private;

/** A Timer
 * */
class TimeClass
{
    private:
	TimeClass_private* _time;

    public:
	TimeClass();
	TimeClass(const TimeClass&);
	~TimeClass();

	/** start a timer
	 * */
	void startTimer();

	/** reset timer to zero but don't stop it
	 * */
	void resetTimer();

	/** stop the timer
	 *
	 * @return elapsed time since start
	 * */
	TimeClass& stopTimer();


	/** how many seconds have passed since start */
	long seconds();

	/** how many micro seconds have passed since start */
	long useconds();

	/** print timer as seconds.useconds */
	void dumpOn( std::ostream& os ) const;
};

/** print timer as seconds.useconds */
std::ostream& operator<<(std::ostream& os, const TimeClass& t);

#endif

// vim:sw=4
