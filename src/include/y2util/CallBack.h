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

  File:       CallBack.h

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose: Template classes for callback handling.

/-*/
#ifndef CallBack_h
#define CallBack_h

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : ReportCallback
/**
 * @short Basic interface definition for callback reports.
 *
 * For a callback recipient it might be helpfull to know begin
 * and end of a report. I.e. a trigger before the 1st actual
 * callback function is invoked, and after the last one.
 *
 * Functions sending a report are encouraged to invoke callback
 * functions through an instance of @ref Report::Send, which
 * triggers @ref reportbegin at its construction, and @ref reportend
 * at destruction.
 **/
struct ReportCallback {
  /**
   * Virtual destructor
   **/
  virtual ~ReportCallback()  {}
  /**
   * Triggered on creation of @ref Report::Send.
   **/
  virtual void reportbegin() {}
  /**
   * Triggered on destruction of @ref Report::Send
   **/
  virtual void reportend()   {}
};

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : RedirectCallback<class CB>
/**
 * @short Base class for callback interfaces which allow redirection.
 *
 * The common way we define a callback interface:
 * <PRE>
 * struct InstCallback : public RedirectCallback&lt;InstCallback> {
 *   virtual void start()    = 0;
 *   virtual void progress() = 0;
 *   virtual void stop()     = 0;
 * };
 *
 * void InstCallback::start()    { ... }
 * void InstCallback::progress() { ... }
 * void InstCallback::stop()     { ... }
 * </PRE>
 * The callback class defines an <B>abstract interface</B> of all functions
 * the sender of the callback may invoke. A <B>default implementation</B>
 * for every function is <B>required</B>, and must provide reasonable
 * defaults for retrurn values and reference arguments passed back to the
 * sender.
 *
 * In other terms, the default implementaion is invoked if there
 * is currently no recipient for a callback. The same way a recipient may
 * invoke the default implementation, if it's got no idea what else to do.
 *
 * The inherited @ref RedirectCallback defines @ref operator-> which per
 * default refers to this, but may be redirected to some other instance.
 * The @ref redirectTo method used to set or clear any redirection is
 * protected per default. A callback recipient (derived from the inteface class)
 * may adjust its scope as desired, or even overload the @ref operator->.
 *
 * <B>NOTE:</B> Assume there is an instance <CODE>InstCallback cbvar</CODE>.
 * There's a sigificant difference between <CODE>cbvar<B>.</B>start()</CODE>
 * and <CODE>cbvar<B>-></B>start()</CODE>. Invokation via '<B>-></B>' follows
 * any redirection applied, and invokes start() on the final recipient.
 * This is, what a calback sender should do. Invokation via .<B>.</B>'
 * always addresses the current instance ignoring any redirection.
 *
 * See: @ref Report and @ref Report::Send
 **/
template <class CB> class RedirectCallback : public ReportCallback {

  private:

    /**
     * Target of redirection. NULL if not redirected.
     **/
    CB * _redirectTo;

    /**
     * Expecting to be inherited by CB: dynamic_cast<CB*>(this)
     **/
    CB * self() { return dynamic_cast<CB*>(this); }

  protected:

    /**
     * Set redirection to another CB instance. Clear redirection if NULL.
     * @return The previois redirection, or NULL if not redirected
     **/
    CB * redirectTo( CB * to_r ) {
      CB * ret = _redirectTo;
      _redirectTo = ( to_r == self() ? 0 : to_r );
      return ret;
    }
    /**
     * Set redirection to another CB instance.
     * @return The previois redirection, or NULL if not redirected
     **/
    CB * redirectTo( CB & to_r ) {
      return redirectTo( &to_r );
    }

  public:

    /**
     * Forward access if redirected, otherwise refer to this.
     **/
    virtual CB * operator->() {
      return( _redirectTo ? _redirectTo->operator->() : self() );
    }

  public:

    /**
     * Constructor.
     **/
    RedirectCallback()
      : _redirectTo( 0 )
    {}
    /**
     * Destructor.
     **/
    virtual ~RedirectCallback() {}
};

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : Report<class CB>
/**
 * @short Hook managing callback sending and recieving.
 *
 * See @ref RedirectCallback for how to define a callback interface class.
 *
 * A Report is the instance of a callback interface, the sender addresses.
 * Usg. a Report will define all interface methods to use the provided default
 * implementation. The inherited callback interface itself is inaccessible from
 * outside Report (and deived classes are encourgaged tokeep it that way). But it
 * may be redirected to any real recipient using @ref redirectTo.
 *
 * A sender may invoke the callbacks by creating an instance of @ref Send.
 * @ref Send provides the callback inteface via operator->. Additionally it
 * triggers reportbegin and reportend (see @ref ReportCallback).
 *
 * What's to be done is, define the abstract callback interface and provide
 * a reasonable default implementation:
 * <PRE>
 * struct InstCallback : public RedirectCallback&lt;InstCallback> {
 *   virtual void start()    = 0;
 *   virtual void progress() = 0;
 *   virtual void stop()     = 0;
 * };
 *
 * void InstCallback::start()    { ... }
 * void InstCallback::progress() { ... }
 * void InstCallback::stop()     { ... }
 * </PRE>
 *
 * Create a Report class:
 * <PRE>
 * class InstReport : public Report&lt;InstCallback> {
 *   virtual void reportbegin() { InstCallback::reportbegin(); }
 *   virtual void reportend()   { InstCallback::reportend(); }
 *   virtual void start()       { InstCallback::start(); }
 *   virtual void progress()    { InstCallback::progress(); }
 *   virtual void stop()        { InstCallback::stop(); }
 * };
 *
 * InstReport instReport;
 * </PRE>
 *
 * Send reports to instReport:
 * <PRE>
 * void inst() {
 *   InstReport::Send report( instReport ); <B>1</B> // construction of report
 *   report->start();                       <B>2</B>
 *   ...
 *   report->progress();                    <B>3</B>
 *   ...
 *   report->stop();                        <B>4</B>
 * }                                        <B>5</B> // destruction of report
 * </PRE>
 *
 * Create a recipient:
 * <PRE>
 * class InstRecieve : public InstCallback {
 *   virtual void reportbegin() { ... }
 *   virtual void reportend()   { ... }
 *   virtual void start()       { ... }
 *   virtual void progress()    { ... }
 *   virtual void stop()        { ... }
 * };
 *
 * InstRecieve instRecieve;
 * </PRE>
 *
 * Let it recieve reports to instReport:
 * <PRE>
 * instReport.redirectTo( instRecieve );
 * </PRE>
 *
 * Invocation of inst() now triggers:
 * <PRE>
 * <B>1</B> instRecieve->reportbegin();
 * <B>2</B> instRecieve->start();
 * <B>3</B> instRecieve->progress();
 * <B>4</B> instRecieve->stop();
 * <B>5</B> instRecieve->reportend();
 * </PRE>
 *
 **/
template <class CB> class Report : protected CB {

  public:

    /**
     * Constructor.
     **/
    Report() {}
    /**
     * Destructor.
     **/
    virtual ~Report() {}

  public:

    /**
     * Adjust to public scope to allow redirection.
     **/
    CB::redirectTo;

  public:

    ///////////////////////////////////////////////////////////////////
    //
    //	CLASS NAME : Report<CB>::Send
    /**
     * @short Interface to send reports.
     *
     * Send provides the Reports callback inteface via operator->. Additionally it
     * triggers @ref ReportCallback::reportbegin and @ref ReportCallback::reportend.
     * Thus any callback recipient listening, may count on receiving a trigger before
     * the 1st actual callback function is invoked, and after the last one.
     *
     * See @ref Report
     **/
    class Send {

      private:

	/**
	 * The @ref Report in use.
	 **/
	Report<CB> & _report;

      public:

	/**
	 * Constructor. Triggers Report->reportbegin().
	 **/
	Send( Report<CB> & report_r )
	  : _report( report_r )
	{ operator->()->reportbegin(); }
	/**
	 * Destructor. Triggers Report->reportend().
	 **/
	~Send() {
	  operator->()->reportend();
	}

	/**
	 * Access to callback interface.
	 **/
	CB * operator->() { return _report.operator->(); }
    };

    ///////////////////////////////////////////////////////////////////
};

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME :
/**
 * @short Temporarily redirect @ref Report to some recipient
 *
 * Restores original redirection in destructor.
 **/
template <class CB> class ReportRedirect {
  ReportRedirect( const ReportRedirect & rhs );             // FORBIDDEN
  ReportRedirect & operator=( const ReportRedirect & rhs ); // FORBIDDEN
  private:
    Report<CB> & _report;
    CB *         _redirect;
    CB *         _oredirect;
  public:
    ReportRedirect( Report<CB> & report_r, CB & redirect_r )
      : _report( report_r )
      , _redirect( &redirect_r )
    {
      _oredirect = _report.redirectTo( _redirect );
    }
    ReportRedirect( Report<CB> & report_r, CB * redirect_r )
      : _report( report_r )
      , _redirect( redirect_r )
    {
      _oredirect = _report.redirectTo( _redirect );
    }
    virtual ~ReportRedirect() {
      _report.redirectTo( _oredirect );
    };
};

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME :
/**
 * @short Temporarily redirect @ref Report to this recipient.
 *
 * Restores original redirection in destructor.
 **/
template <class CB> struct ReportReceive : public CB, public ReportRedirect<CB> {
  ReportReceive( Report<CB> & report_r )
    : ReportRedirect<CB>( report_r, static_cast<CB*>(this) )
  {}
};

///////////////////////////////////////////////////////////////////

#endif // CallBack_h
