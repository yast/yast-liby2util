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

  File:       ProgressCounter.h

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose: Simple progress counter with callback support

/-*/
#ifndef ProgressCounter_h
#define ProgressCounter_h

#include <iosfwd>

#include <y2util/CallBack.h>

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : ProgressCounter / ProgressCallBack
/**
 * @short Simple progress counter with callback support
 *
 * Any function willing to report progress may provide an (optional)
 * <CODE>ProgressCounter</CODE> argument:
 * <PRE>
 * #include &lt;y2util/ProgressCounter.h>
 *
 * void action( ..., ProgressCounter pcnt = ProgressCounter() )
 * {
 *    pcnt.start();
 *    for ( int i = 0; i < 100; ++i ) {
 *      ...
 *        if ( error )
 *          return; // it's ok to do this.
 *      ...
 *      pcnt.incr();
 *    }
 *    pcnt.done();
 * }
 * </PRE>
 * A counter cycle always consits of:
 * <OL>
 * <LI>Calling <CODE>start</CODE> to specify the counters min/max and initial value.
 * <LI>Changing the counter value as needed.
 * <LI>Calling <CODE>stop</CODE> or <CODE>done</CODE> to end a cycle.
 * </OL>
 * It's asserted that a cycle always completes, by calling <CODE>stop</CODE> before a new
 * cycle is started, or from the destructor. While <CODE>stop</CODE> reports the current values,
 * <CODE>done</CODE> sets the counter to the specified max value (reporting 100%).
 *
 * No action will be performed (and reported) if <CODE>start</CODE> was not called. After
 * a cycle was stopped, no action will be performed (and reported) untill the next call to
 * <CODE>start</CODE>.
 *
 * Besides this there are no checks or optimizations. <CODE>pcnt.set( 5 )</CODE> will set the
 * counter value to 5 and report it, no matter if the value was 5 or it's outside the specified
 * min/max value. You may as well set a min value greater than the max value.
 *
 * A <CODE>ProgressCounter::Callback</CODE> function has to be:
 * <PRE>
 * #include &lt;y2util/ProgressCounter.h>
 *
 * void mycallback( const ProgressCounter & pcnt, void * userdata );
 * </PRE>
 *
 * Code connecting action and callback might look like:
 * <PRE>
 * {
 *   // run action without any callback.
 *   action();
 * }
 * </PRE>
 * <PRE>
 * {
 *   // directly pass callback function (userdata will be NULL)
 *   action( mycallback );
 * }
 * </PRE>
 * <PRE>
 * {
 *   // use a @ref CallBack class
 *   ProgressCounter::Callback cbvar; // no callback, no userdata per default
 *
 *   if ( ... ) {
 *     cbvar.setFunc( mycallback );   // maybe assign callback
 *     if ( ... ) {
 *       cbvar.setData( &mydata );    // maybe assign userdata too
 *     }
 *   }
 *
 *   action( cbvar );
 * }
 * </PRE>
 **/
class ProgressCounter {

  public:

    typedef CallBack1<const ProgressCounter &> Callback;

    enum State { st_stop = 0, st_start, st_value };

  private:

    const Callback _cb;

    State    _state;
    unsigned _cycle;

    int _min;
    int _max;
    int _val;

    /**
     * Invoce callback
     **/
    void invoke() { _cb( *this ); }

  public:

    /**
     * Constructor. Create Callback from function pointer.
     * Defaults to no Callback.
     **/
    ProgressCounter( Callback::Func cb_r = 0 )
      : _cb( cb_r )
      , _state( st_stop )
      , _cycle( 0 )
      , _min( 0 )
      , _max( 0 )
      , _val( 0 )
    {}

    /**
     * Constructor. Copy provided Callback.
     **/
    ProgressCounter( const Callback & cb_r )
      : _cb( cb_r )
      , _state( st_stop )
      , _cycle( 0 )
      , _min( 0 )
      , _max( 0 )
      , _val( 0 )
    {}

    /**
     * Destructor. Stops cycle if necessary.
     **/
    ~ProgressCounter() {
	stop();
    }

  public:

    ///////////////////////////////////////////////////////////////////
    //
    // Counter action
    //
    ///////////////////////////////////////////////////////////////////

    /**
     * Start a new cycle, setting min/max and counter value.
     **/
    void start( int min_r, int max_r, int val_r ) {
      stop();
      ++_cycle;
      _state = st_start;
      _min = min_r;
      _max = max_r;
      _val = val_r;
      invoke();
      _state = st_value;
    }
    /**
     * Start a new cycle, setting min/max value. Counter value defaults to min.
     **/
    void start( int min_r, int max_r ) { return start( min_r, max_r, min_r ); }
    /**
     * Start a new cycle, setting max value. Counter and min value default to 0.
     **/
    void start( int max_r )            { return start( 0, max_r ); }
    /**
     * Start a new cycle, Max value defaults to 100. Counter and min value default to 0.
     **/
    void start()                       { return start( 0, 100 ); }


    /**
     * Set new counter value.
     **/
    void set ( int val_r )     { if ( _state ) { _val  = val_r; invoke(); } }
    /**
     * Increment counter value (default by 1)
     **/
    void incr( int val_r = 1 ) { if ( _state ) { _val += val_r; invoke(); } }
    /**
     * Decrement counter value (default by 1)
     **/
    void decr( int val_r = 1 ) { if ( _state ) { _val -= val_r; invoke(); } }

    /**
     * Stop cycle, setting counter value to max.
     **/
    void done() { if ( _state ) { _val = _max; stop(); } }
    /**
     * Stop cycle, leaving counter value as it is.
     **/
    void stop() {
      if ( _state ) {
	_state = st_stop;
	invoke();
      }
    }

  public:

    ///////////////////////////////////////////////////////////////////
    //
    // Callback data
    //
    ///////////////////////////////////////////////////////////////////

    /**
     * Current cycle state:
     * <TABLE border=3>
     * <TR><TH>enum<TH>bool<TH>Event</TR>
     * <TR><TD>st_start<TD>true<TD>start of a new cycle (prob. new min/max values)</TR>
     * <TR><TD>st_value<TD>true<TD>intermediate report (prob. new counter value)</TR>
     * <TR><TD>st_stop<TD>false<TD>end of cycle (prob. new counter value)</TR>
     * </TABLE>
     **/
    State state() const { return _state; }

    /**
     * Cycle counter. Incremented at each st_start.
     **/
    unsigned cycle() const { return _cycle; }

    /**
     * Min value provided.
     **/
    int min() const { return _min; }

    /**
     * Max value provided.
     **/
    int max() const { return _max; }

    /**
     * Counter value provided.
     **/
    int val() const { return _val; }

    /**
     * Calculate percentage (returns 0 if max == min).
     **/
    double precent() const {
      if ( _max == _min )
	return 0;
      return double( _val - _min ) * 100 / double( _max - _min );
    }
};

///////////////////////////////////////////////////////////////////

#endif // ProgressCounter_h
