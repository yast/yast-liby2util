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

   File:       timeclass.cc
   Purpose:    A timer
   Author:     Ludwig Nussel <lnussel@suse.de>
   Maintainer: Ludwig Nussel <lnussel@suse.de>

/-*/

#include <sys/time.h>

#include <iomanip>

#include <y2util/timeclass.h>

class TimeClass_private
{
    public:
	TimeClass_private();
	/** start time if running == true, diff otherwise */
	struct timeval tv;
	bool running;

	/** return diff between now and tv if running, tv otherwise */
	struct timeval difftonow();
};

TimeClass_private::TimeClass_private()
    : running(false)
{
    tv.tv_sec = 0;
    tv.tv_usec = 0;
}

struct timeval TimeClass_private::difftonow()
{
    struct timeval now = {0, 0};
    struct timeval diff = {0, 0};

    if(running)
    {
	::gettimeofday(&now,NULL);
	timersub(&now,&tv,&diff);

	return diff;
    }
    else
    {
	return tv;
    }
}

TimeClass::TimeClass()
{
    _time = new TimeClass_private;
}

TimeClass::~TimeClass()
{
    delete _time;
}

TimeClass::TimeClass(const TimeClass& old)
{
    _time = new TimeClass_private;
    *_time = *(old._time);
}

void TimeClass::startTimer()
{
    ::gettimeofday(&(_time->tv),NULL);
    _time->running = true;
}

void TimeClass::resetTimer()
{
    _time->tv.tv_sec = 0;
    _time->tv.tv_usec = 0;
}

TimeClass& TimeClass::stopTimer()
{
    _time->tv = _time->difftonow();
    _time->running = false;
    return *this;
}

long TimeClass::seconds()
{
    return _time->difftonow().tv_sec;
}

long TimeClass::useconds()
{
    return _time->difftonow().tv_usec;
}

void TimeClass::dumpOn(std::ostream& os) const
{
    struct timeval tv = _time->difftonow();
    os << tv.tv_sec << '.' << std::setfill('0') << std::setw(6) << tv.tv_usec;
}

std::ostream& operator<<(std::ostream& os, const TimeClass& t)
{
    t.dumpOn(os);
    return os;
}
