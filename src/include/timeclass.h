#ifndef timeclasse_h
#define timeclasse_h
#include <time.h>

class TimeClass
{
    private:
	time_t _time;
	bool _running;
    public:
	TimeClass() : _time(0), _running(false) {};
	void startTimer()
	{
	    _time = time(NULL);
	    _running = true;
	};
	time_t stopTimer()
	{
	    if(_running)
	    {
		_time = time(NULL) - _time;
		_running = false;
	    }
	    return _time;
	};
	time_t getTimer() const
	{
	    if(_running)
		return (time(NULL) - _time);
	    return _time;
	};
	void resetTimer()
	{
	    _time=0;
	};
};

#endif

// vim:sw=4
