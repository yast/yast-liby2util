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

  Purpose: Simple progress counting.

/-*/
#ifndef ProgressCounter_h
#define ProgressCounter_h

#include <iosfwd>

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : ProgressData
/**
 * @short Maintain min, max, and counter value for progress counting.
 *
 * This is what the sender of progress data maintains to send them via
 * a callback. All actualy value manipulating methods are virtual. This
 * is to @ref ProgressCounter to keep track of changing values.
 **/
class ProgressData {
  friend std::ostream & operator<<( std::ostream & str, const ProgressData & obj );

  private:

    /**
     * Current min value.
     **/
    int _min;
    /**
     * Current max value.
     **/
    int _max;
    /**
     * Current counter value.
     **/
    int _val;

  public:

    /**
     * Constructor. Sets min/val to 0.
     **/
    ProgressData( int max_r = 0 )
      : _min( 0 )
      , _max( max_r )
      , _val( 0 )
    {}
    /**
     * Constructor. Sets val to min_r.
     **/
    ProgressData( int min_r, int max_r )
      : _min( min_r )
      , _max( max_r )
      , _val( min_r )
    {}
    /**
     * Constructor.
     **/
    ProgressData( int min_r, int max_r, int val_r )
      : _min( min_r )
      , _max( max_r )
      , _val( val_r )
    {}
    /**
     * Copy Constructor.
     **/
    ProgressData( const ProgressData & rhs )
      : _min( rhs._min )
      , _max( rhs._max )
      , _val( rhs._val )
    {}

    /**
     * Assignment via @ref init
     **/
    ProgressData & operator=( const ProgressData & rhs ) {
      if ( &rhs != this ) {
	init( rhs._min, rhs._max, rhs._val );
      }
      return *this;
    }

    /**
     * Destructor.
     **/
    virtual ~ProgressData() {}

  public:

    /**
     * Set new min value.
     **/
    virtual ProgressData & min( int min_r )               { _min = min_r; return *this; }
    /**
     * Set new max value.
     **/
    virtual ProgressData & max( int max_r )               { _max = max_r; return *this; }

    /**
     * Set new min and max value.
     **/
    virtual ProgressData & range( int min_r, int max_r )   { _min = min_r; _max = max_r; return *this; }
    /**
     * Set min value to 0 and max value.
     **/
    ProgressData & range( int max_r )                      { return range( 0, max_r ); }

    /**
     * Set new counter value.
     **/
    virtual ProgressData & set( int val_r )                { _val = val_r; return *this; }
    /**
     * Increment counter value (default by 1).
     **/
    ProgressData & incr( int val_r = 1 )                   { return set( _val +val_r ); }
    /**
     * Decrement counter value (default by 1).
     **/
    ProgressData & decr( int val_r = 1 )                   { return set( _val -val_r ); }
    /**
     * Set counter value to current min value.
     **/
    ProgressData & toMin()                                 { return set( _min ); }
    /**
     * Set counter value to current max value.
     **/
    ProgressData & toMax()                                 { return set( _max ); }

    /**
     * Set new min, max and counter value.
     **/
    virtual ProgressData & init( int min_r, int max_r, int val_r ) { _min = min_r; _max = max_r; _val = val_r; return *this; }
    /**
     * Set new min, max value and counter value to min.
     **/
    ProgressData & init( int min_r, int max_r )            { return init( min_r, max_r, min_r ); }
    /**
     * Set min and counter value to 0 and new max value.
     **/
    ProgressData & init( int max_r )                       { return init( 0, max_r ); }

  public:

    /**
     * @return Current min value.
     **/
    int min() const { return _min; }

    /**
     * @return Current max value.
     **/
    int max() const { return _max; }

    /**
     * @return Current counter value.
     **/
    int val() const { return _val; }
};

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : ProgressCounter
/**
 * @short Maintain ProgressData received e.g. via callback.
 *
 * ProgressCounter overloads all value manipulating methods ot the
 * inherited @ProgressData and remembers changing values in a bitmask.
 *
 * A call to @ref update discards previously remembered changes. @ref onUpdate is
 * what derived classes may want to overload, to perform whatever action on a call
 * to @ref update and set the bits returned to the caller.
 **/
class ProgressCounter : public ProgressData {
  friend std::ostream & operator<<( std::ostream & str, const ProgressCounter & obj );

  public:

    typedef unsigned Bits;

    /**
     * Bits and bitmasks
     **/
   enum ChangedBit {
      VC_NONE = 0x00,
      //============
      VC_MIN      = 0x01,
      VC_MAX      = 0x02,
      VC_VAL      = 0x04,
      VC_PERCENT  = 0x10,
      //============
      VC_RANGE = (VC_MIN|VC_MAX),
      VC_COUNT = (VC_RANGE|VC_VAL),
      VC_ALL   = (VC_COUNT|VC_PERCENT)
    };

  private:

    /**
     * Changed values in @ref ProgressData since last @ref update.
     * Mutable because some bits like VC_PERCENT may be set when needed,
     * and not on every value change.
     **/
    mutable Bits _changed;

    /**
     * Remember @ref percent on call to @ref update.
     **/
    int _oldPercent;

    /**
     * Test before value assignment. Set ChangedBit if current an new value differ.
     **/
    void check( int lhs, int rhs, ChangedBit which ) { if ( lhs != rhs ) _changed |= which; }

    /**
     * Test for changed @ref percent value.
     * @ref Whether VC_PERCENT bit is now set.
     **/
    bool checkPercent() const {
      if ( _changed & VC_PERCENT )
	return true; // Dirty percent is reset on update only.
      // Dirty min, max or val -> check precent
      if ( (_changed & VC_COUNT) &&  _oldPercent != percent() ) {
	_changed |= VC_PERCENT;
	return true;
      }
      return false;
    }

  protected:

    /**
     * Derived classes may want to overload this to trigger whatever action.
     * @return Bits to be returned from @ref update.
     **/
     virtual Bits onUpdate() { return _changed; }

  public:

    /**
     * Constructor.
     **/
    ProgressCounter( Bits initial_r = VC_NONE )
      : _changed( initial_r )
      , _oldPercent( percent() )
    {}
    /**
     * Constructor.
     **/
    ProgressCounter( const ProgressData & pd_r, Bits initial_r = VC_NONE )
      : ProgressData( pd_r )
      , _changed( initial_r )
      , _oldPercent( percent() )
    {}

    /**
     * Assign new ProgressData.
     **/
    ProgressCounter & operator=( const ProgressData & pd_r ) {
      ProgressData::operator=( pd_r );
      return *this;
    }

    /**
     * Destructor.
     **/
    virtual ~ProgressCounter() {}

  public:

    /**
     * Overloaded @ref ProgressData::min to check for changed value.
     **/
    virtual ProgressData & min( int min_r ) {
      check( min(), min_r, VC_MIN );
      return ProgressData::min( min_r );
    }
    /**
     * Acess to shadowed @ref ProgressData::min
     **/
    ProgressData::min;

    /**
     * Overloaded @ref ProgressData::max to check for changed value.
     **/
    virtual ProgressData & max( int max_r ) {
      check( max(), max_r, VC_MAX );
      return ProgressData::max( max_r );
    }
    /**
     * Acess to shadowed @ref ProgressData::max
     **/
    ProgressData::max;

    /**
     * Overloaded @ref ProgressData::range to check for changed value.
     **/
    virtual ProgressData & range( int min_r, int max_r ) {
      check( min(), min_r, VC_MIN );
      check( max(), max_r, VC_MAX );
      return ProgressData::range( min_r, max_r );
    }
    /**
     * Acess to shadowed @ref ProgressData::range
     **/
    ProgressData::range;

    /**
     * Overloaded @ref ProgressData::set to check for changed value.
     **/
    virtual ProgressData & set( int val_r ) {
      check( val(), val_r, VC_VAL );
      return ProgressData::set( val_r );
    }

    /**
     * Overloaded @ref ProgressData::init to check for changed value.
     **/
    virtual ProgressData & init( int min_r, int max_r, int val_r ) {
      check( min(), min_r, VC_MIN );
      check( max(), max_r, VC_MAX );
      check( val(), val_r, VC_VAL );
      return ProgressData::init( min_r, max_r, val_r );
    }
    /**
     * Acess to shadowed @ref ProgressData::init
     **/
    ProgressData::init;

  public:

    /**
     * Whether min value changed since last @ref update.
     **/
    bool newMin() const { return( _changed & VC_MIN ); }
    /**
     * Whether max value changed since last @ref update.
     **/
    bool newMax() const { return( _changed & VC_MAX ); }
    /**
     * Whether counter value changed since last @ref update.
     **/
    bool newVal() const { return( _changed & VC_VAL ); }
    /**
     * Whether min or max value changed since last @ref update.
     **/
    bool newRange() const { return( _changed & VC_RANGE ); }
    /**
     * Whether @ref percent changed since last @ref update.
     **/
    bool newPercent() const { return checkPercent(); }
    /**
     * Whether any value changed since last @ref update.
     **/
    bool needsUpdate() const {
      checkPercent(); // Check, as VC_PERCENT isn't updated on the fly.
      return _changed;
    }

  public:

    /**
     * Calculate percentage (as double). If <CODE>(max == min)</CODE>,
     * division by zero is avoided and the provided
     * ifepmty_r (defaults to 0.0) is returned.
     **/
    double fPercent( double ifepmty_r = 0.0 ) const {
      if ( max() == min() ) {
	return ifepmty_r;
      }
      return double( val() - min() ) * 100.0 / double( max() - min() );
    }
    /**
     * Calculate percentage (as int). If <CODE>(max == min)</CODE>,
     * division by zero is avoided and the provided ifepmty_r (defaults to 0)
     * ifepmty_r (defaults to 0) is returned.
     **/
    int percent( int ifepmty_r = 0 ) const {
      if ( max() == min() ) {
	return ifepmty_r;
      }
      return (int)fPercent();
    }

  public:

    /**
     * Add bits to remembered changes.
     **/
    void tagDirty( Bits bits_r = VC_ALL ) { _changed |= bits_r; }
    /**
     * Set remembered changes to bits.
     **/
    void setDirty( Bits bits_r = VC_ALL ) { _changed = bits_r; }

    /**
     * Discard all previously remembered changes. Derived classes
     * want to overload @ref onUpdate. The caller may interpret
     * the bits returned as 'unprocessed changes'.
     * @return The changes discarded.
     **/
    Bits update() {
      needsUpdate();         // update bits not set ont the fly (percent)
      Bits ret = onUpdate(); // get the discarded bits to return
      _changed = VC_NONE;
      _oldPercent = percent();
      return ret;
    }

    /**
     * Call @ref update, if @ref newPercent. If a nonzero limit_r is provided,
     * a call to @ref update is omitted, if old and new percentage differ less
     * than limit_r. Thus <CODE>updateIfNewPercent( 5 )</CODE> will perform at
     * most every 5%. However, if the old percentage is at min or max, or if
     * the new percentage hits min or max, @ref update is called regardless
     * to the limit_r (if the percentage actually changed).
     * @return Whether update was called.
     **/
    bool updateIfNewPercent( unsigned limit_r = 0 ) {
      if ( checkPercent() ) {
	if ( limit_r ) {
	  int newpc = percent();
	  if ( newpc == _oldPercent )
	    return false;
	  if ( ! ( newpc == 100 || _oldPercent == 0 || newpc == 0 || _oldPercent == 100 ) ) {
	    unsigned dist = newpc > _oldPercent ? newpc - _oldPercent
						: _oldPercent - newpc;
	    if ( dist < limit_r )
	      return false;
	  }
	}
	update();
	return true;
      }
      return false;
    }

    /**
     * Reset all values to 0 and tag all values dirty.
     **/
    void reset() { init( 0 ); _oldPercent = 0; setDirty(); }

};

///////////////////////////////////////////////////////////////////

#endif // ProgressCounter_h
