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
//	CLASS NAME : CallBackData<typename Func>
/**
 * @short Template class storing callback function and userdata pointer.
 *
 * <CODE>CallBackData&lt;typename Func></CODE> stores a pointer to
 * function of type <CODE>Func</CODE> (passed as template argument),
 * and a <CODE>void*</CODE> which may serve as hook for user data
 * that shall be passed to the callback function.
 *
 * The default constructor asserts both pointer being initialized
 * to <CODE>0</CODE>.
 *
 * Serves as base class for some more convenient CallBack classes.
 * @see CallBack
 **/
template <typename Func> class CallBackData {

  protected:

    /**
     * Pointer to callback function.
     **/
    Func   _func;

    /**
     * Hook for user data.
     **/
    const void * _data;

  public:

    /**
     * Returns the current callback function pointer.
     **/
    Func   func() const { return _func; }

    /**
     * Returns the current user data.
     **/
    const void * data() const { return _data; }

  public:

    /**
     * Set a new callback function and user data pointer.
     **/
    void set( Func func_r, const void * data_r = 0 ) {
      _func = func_r;
      _data = data_r;
    }
    /**
     * Set callback function pointer and user data pointer to 0.
     **/
    void unset() { set( 0 ); }

  public:

    /**
     * Constructor.
     **/
    CallBackData( Func func_r = 0, const void * data_r = 0 ) { set( func_r, data_r ); }
};

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : CallBack
/**
 * @short Template class to store and invoke a calback.
 *
 * The template classes <CODE>CallBack<B>[1-9]</B></CODE> follow the
 * same rules as <CODE>CallBack</CODE>. The only difference is their
 * callback functions may take <B>[1-9]</B> additional arguments
 * (passed as template argument).
 *
 * Common rule for all callback functions is that they return <CODE>void</CODE>
 * and take a <CODE>const void*</CODE> as last argument.
 *
 * Common to all <CODE>CallBack</CODE> classes is that they define
 * <CODE>bool operator() const</CODE> that takes the same arguments as the
 * callback function, except for the trailing <CODE>const void*</CODE>, which is automaticaly
 * provided.<CODE>operator()</CODE> returns <CODE>false</CODE>, if no callback function
 * is set. Otherwise the callback function is executed and it returns <CODE>true</CODE>.
 *
 * <B>NOTE:</B> CallBack constness means you can't change callback function and userdata,
 * but it's ok to invoce it.
 *
 * Assume a callback function that should return a <CODE>bool</CODE> (via reference argument):
 * <PRE>
 * <B>void mycallback( bool & ret, const void * data )</B> {
 *   ret = true;
 * }
 *
 * <B>typedef CallBack1&lt;bool&> TestCB;</B>
 *
 * int somewhere()
 * {
 *   bool cbret = false;
 *
 *   <B>TestCB callback; // Per default no callback is set.</B>
 *
 *   <B>if ( callback( cbret ) )</B> {
 *     ;
 *   } else {
 *     <B>// No callback is set. cbret remained unchanged.</B>
 *   }
 *
 *   <B>callback = mycallback; // Set callback to mycallback.</B>
 *   // same as e.g.: callback.set( mycallback )
 *
 *   <B>if ( callback( cbret ) )</B> {
 *     <B>// This time mycallback was executed, setting cbret true.</B>
 *   } else {
 *     ;
 *   }
 *
 *   return 0;
 * }
 * </PRE>
 * For a callback function that takes two arguments:
 * <PRE>
 * <B>void two_arg_callback( int a1, const std::string & a2, const void * data )</B> {
 *   ....;
 * }
 *
 * <B>typedef CallBack2&lt;int,const std::string&> TwoArgCB;</B>
 *
 * int somewhereelse()
 * {
 *   <B>TwoArgCB callback( two_arg_callback );</B>
 *
 *   <B>if ( callback( 1, "some string" ) )</B> {
 *     ...
 * }
 * </PRE>
 **/
class CallBack : public CallBackData<void(*)(const void*)> {
  public:
    typedef void(*Func)(const void*);
    CallBack( Func func_r = 0, const void * data_r = 0 )
      : CallBackData<void(*)(const void*)>( func_r, data_r )
    {}
  public:
    bool operator()() const {
      if ( !_func )
	return false;
      (*_func)( _data );
      return true;
    }
};

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : CallBackN<..>
//
///////////////////////////////////////////////////////////////////
#define DEF_CBCLASS(N,TAL,FAL,CAL) \
template < TAL > class CallBack##N : public CallBackData<void(*)(FAL,const void*)> { \
  public: \
    typedef void(*Func)(FAL,const void*); \
    CallBack##N( Func func_r = 0, const void * data_r = 0 ) \
      : CallBackData<void(*)(FAL,const void*)>( func_r, data_r ) \
    {} \
  public: \
    bool operator()( FAL ) const { if ( !_func ) return false; (*_func)( CAL, _data ); return true; } \
}

#define DEF_CBTAL(N) , typename _arg##N
#define DEF_CBFAL(N) , _arg##N a##N
#define DEF_CBCAL(N) , a##N

DEF_CBCLASS( 1
	     , typename _arg1
	     , _arg1 a1
	     , a1
	     );
DEF_CBCLASS( 2
	     , typename _arg1 DEF_CBTAL(2)
	     , _arg1 a1       DEF_CBFAL(2)
	     , a1             DEF_CBCAL(2)
	     );
DEF_CBCLASS( 3
	     , typename _arg1 DEF_CBTAL(2) DEF_CBTAL(3)
	     , _arg1 a1       DEF_CBFAL(2) DEF_CBFAL(3)
	     , a1             DEF_CBCAL(2) DEF_CBCAL(3)
	     );
DEF_CBCLASS( 4
	     , typename _arg1 DEF_CBTAL(2) DEF_CBTAL(3) DEF_CBTAL(4)
	     , _arg1 a1       DEF_CBFAL(2) DEF_CBFAL(3) DEF_CBFAL(4)
	     , a1             DEF_CBCAL(2) DEF_CBCAL(3) DEF_CBCAL(4)
	     );
DEF_CBCLASS( 5
	     , typename _arg1 DEF_CBTAL(2) DEF_CBTAL(3) DEF_CBTAL(4) DEF_CBTAL(5)
	     , _arg1 a1       DEF_CBFAL(2) DEF_CBFAL(3) DEF_CBFAL(4) DEF_CBFAL(5)
	     , a1             DEF_CBCAL(2) DEF_CBCAL(3) DEF_CBCAL(4) DEF_CBCAL(5)
	     );
DEF_CBCLASS( 6
	     , typename _arg1 DEF_CBTAL(2) DEF_CBTAL(3) DEF_CBTAL(4) DEF_CBTAL(5) DEF_CBTAL(6)
	     , _arg1 a1       DEF_CBFAL(2) DEF_CBFAL(3) DEF_CBFAL(4) DEF_CBFAL(5) DEF_CBFAL(6)
	     , a1             DEF_CBCAL(2) DEF_CBCAL(3) DEF_CBCAL(4) DEF_CBCAL(5) DEF_CBCAL(6)
	     );
DEF_CBCLASS( 7
	     , typename _arg1 DEF_CBTAL(2) DEF_CBTAL(3) DEF_CBTAL(4) DEF_CBTAL(5) DEF_CBTAL(6) DEF_CBTAL(7)
	     , _arg1 a1       DEF_CBFAL(2) DEF_CBFAL(3) DEF_CBFAL(4) DEF_CBFAL(5) DEF_CBFAL(6) DEF_CBFAL(7)
	     , a1             DEF_CBCAL(2) DEF_CBCAL(3) DEF_CBCAL(4) DEF_CBCAL(5) DEF_CBCAL(6) DEF_CBCAL(7)
	     );
DEF_CBCLASS( 8
	     , typename _arg1 DEF_CBTAL(2) DEF_CBTAL(3) DEF_CBTAL(4) DEF_CBTAL(5) DEF_CBTAL(6) DEF_CBTAL(7) DEF_CBTAL(8)
	     , _arg1 a1       DEF_CBFAL(2) DEF_CBFAL(3) DEF_CBFAL(4) DEF_CBFAL(5) DEF_CBFAL(6) DEF_CBFAL(7) DEF_CBFAL(8)
	     , a1             DEF_CBCAL(2) DEF_CBCAL(3) DEF_CBCAL(4) DEF_CBCAL(5) DEF_CBCAL(6) DEF_CBCAL(7) DEF_CBCAL(8)
	     );
DEF_CBCLASS( 9
	     , typename _arg1 DEF_CBTAL(2) DEF_CBTAL(3) DEF_CBTAL(4) DEF_CBTAL(5) DEF_CBTAL(6) DEF_CBTAL(7) DEF_CBTAL(8) DEF_CBTAL(9)
	     , _arg1 a1       DEF_CBFAL(2) DEF_CBFAL(3) DEF_CBFAL(4) DEF_CBFAL(5) DEF_CBFAL(6) DEF_CBFAL(7) DEF_CBFAL(8) DEF_CBFAL(9)
	     , a1             DEF_CBCAL(2) DEF_CBCAL(3) DEF_CBCAL(4) DEF_CBCAL(5) DEF_CBCAL(6) DEF_CBCAL(7) DEF_CBCAL(8) DEF_CBCAL(9)
	     );

#undef DEF_CBCLASS
#undef DEF_CBTAL
#undef DEF_CBFAL
#undef DEF_CBCAL

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : BasicSettings<..>
/**
 * @short Template base class managing e.g. callback settings
 *
 * This class maintains static default settings and a pointer to
 * redirect the settings to use to a different location. The
 * concrete type of settings is passed as template argument.
 * <PRE>
 * class RpmDb {
 *   public:
 *     class Callbacks : protected BasicSettings&lt;Callbacks> {
 *       friend class RpmDb; // may set _defaults
 *       public:
 *         BasicSettings&lt;Callbacks>::inUse;     // adjusted to public scope
 *         BasicSettings&lt;Callbacks>::use;       // adjusted to public scope
 *         //BasicSettings&lt;Callbacks>::provide; // stays protected
 *       public:
 *         ProgressCounter::Callback _convertDb;
 *         ProgressCounter::Callback _rebuildDb;
 *         ProgressCounter::Callback _installPkg;
 *     };
 *     ...
 * };
 *
 * {
 *   RpmDb rpmdb;
 *   RpmDb::Callbacks my_cb;
 *
 *   my_cb._convertDb.set( my_convertDb );
 *   my_cb._rebuildDb.set( my_rebuildDb );
 *   my_cb._installPkg.set( my_installPkg );
 *
 *   RpmDb::Callbacks::use( &my_cb );
 *   // using my callbacks
 *   ...
 *
 *   RpmDb::Callbacks::use( 0 );
 *   // using defaults
 *   ...
 * }
 * </PRE>
 **/
template <typename T> class BasicSettings {

  protected:

    /**
     * Default settings.
     **/
    static T _default;

    /**
     * Settings in use.
     **/
    static const T * _inuse;

  public:

    /**
     * @return Settings in use, NULL if defaults are used.
     **/
    static const T * inUse() { return _inuse; }

    /**
     * Assign settings to use, NULL for using defaults.
     * @return Previously used settigns.
     **/
    static const T * use( const T * ncb_r ) {
      const T * ret = _inuse;
      _inuse = ncb_r;
      return ret;
    }

    /**
     * @return Current settings (either settings in use, or defaults).
     **/
    static const T & provide() { return( _inuse ? *_inuse : _default ); }
};

///////////////////////////////////////////////////////////////////

#endif // CallBack_h
