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
   Purpose:    declare class for handling time and timers
   Author:     Ludwig Nussel <lnussel@suse.de>
   Maintainer: Ludwig Nussel <lnussel@suse.de>

/-*/

#ifndef timeclasse_h
#define timeclasse_h
#include <time.h>

/** common class for handling of time and timers
 * */
class TimeClass
{
    private:
	time_t _time;
	bool _running;
    public:
	TimeClass() : _time(0), _running(false) {};
	/** start a timer
	 * */
	void startTimer()
	{
	    _time = time(NULL);
	    _running = true;
	};

	/** reset timer to zero
	 * */
	void resetTimer()
	{
	    _time=0;
	};

	/** stop the timer
	 *
	 * @return elapsed time since start
	 * */
	time_t stopTimer()
	{
	    if(_running)
	    {
		_time = time(NULL) - _time;
		_running = false;
	    }
	    return _time;
	};

	/** get the time since timer start without stopping the timer
	 *
	 * @return elapsed time since start
	 * */
	time_t getTimer() const
	{
	    if(_running)
		return (time(NULL) - _time);
	    return _time;
	};
};

#endif

// vim:sw=4
