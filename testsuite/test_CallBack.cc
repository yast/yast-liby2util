#include <iostream>
#include <y2util/Y2SLog.h>
#include <y2util/CallBack.h>

using namespace std;

///////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////

struct DnlCallback : public RedirectCallback<DnlCallback> {
  virtual void reportbegin() { ERR << __PRETTY_FUNCTION__ << endl; }
  virtual void reportend()   { ERR << __PRETTY_FUNCTION__ << endl; }
  virtual void start()    = 0;
  virtual void progress() = 0;
  virtual void stop()     = 0;
};

void DnlCallback::start()       { WAR << __PRETTY_FUNCTION__ << endl; }
void DnlCallback::progress()    { WAR << __PRETTY_FUNCTION__ << endl; }
void DnlCallback::stop()        { WAR << __PRETTY_FUNCTION__ << endl; }

struct InstCallback : public RedirectCallback<InstCallback> {
  virtual void reportbegin() { ERR << __PRETTY_FUNCTION__ << endl; }
  virtual void reportend()   { ERR << __PRETTY_FUNCTION__ << endl; }
  virtual void start()    = 0;
  virtual void progress() = 0;
  virtual void stop()     = 0;
};

void InstCallback::start()       { WAR << __PRETTY_FUNCTION__ << endl; }
void InstCallback::progress()    { WAR << __PRETTY_FUNCTION__ << endl; }
void InstCallback::stop()        { WAR << __PRETTY_FUNCTION__ << endl; }

///////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////

class DnlReport : public Report<DnlCallback> {
  virtual void reportbegin() { DnlCallback::reportbegin(); }
  virtual void reportend()   { DnlCallback::reportend(); }
  virtual void start()       { DnlCallback::start(); }
  virtual void progress()    { DnlCallback::progress(); }
  virtual void stop()        { DnlCallback::stop(); }
};

class InstReport : public Report<InstCallback> {
  virtual void reportbegin() { InstCallback::reportbegin(); }
  virtual void reportend()   { InstCallback::reportend(); }
  virtual void start()       { InstCallback::start(); }
  virtual void progress()    { InstCallback::progress(); }
  virtual void stop()        { InstCallback::stop(); }
};

///////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////

struct Callbacks {
  DnlReport  dnl;
  InstReport inst;
};

static Callbacks cbl;

struct GLOBALDNL : public DnlCallback {
  virtual void reportbegin() { SEC << __PRETTY_FUNCTION__ << endl; }
  virtual void reportend()   { SEC << __PRETTY_FUNCTION__ << endl; }
  virtual void start()       { MIL << __PRETTY_FUNCTION__ << endl; }
  virtual void progress()    { MIL << __PRETTY_FUNCTION__ << endl; }
  virtual void stop()        { MIL << __PRETTY_FUNCTION__ << endl; }
};

static GLOBALDNL globaldnl;

struct Receive {
  struct RecDnlCallback : public DnlCallback {
    virtual void reportbegin() { SEC << __PRETTY_FUNCTION__ << endl; }
    virtual void reportend()   { SEC << __PRETTY_FUNCTION__ << endl; }
    virtual void start()       { MIL << __PRETTY_FUNCTION__ << endl; }
    virtual void progress()    { MIL << __PRETTY_FUNCTION__ << endl; }
    virtual void stop()        { MIL << __PRETTY_FUNCTION__ << endl; }
    RecDnlCallback() {
      redirectTo( globaldnl );
    }
  };
  struct RecInstCallback : public InstCallback {
    virtual void reportbegin() { SEC << __PRETTY_FUNCTION__ << endl; }
    virtual void reportend()   { SEC << __PRETTY_FUNCTION__ << endl; }
    virtual void start()       { MIL << __PRETTY_FUNCTION__ << endl; }
    virtual void progress()    { MIL << __PRETTY_FUNCTION__ << endl; }
    virtual void stop()        { MIL << __PRETTY_FUNCTION__ << endl; }
  };
  RecDnlCallback  recDnl;
  RecInstCallback recInst;
};

static Receive receive;

/******************************************************************
**
**
**	FUNCTION NAME : dnl
**	FUNCTION TYPE : void
*/
void dnl() {
  Report<DnlCallback>::Send report( cbl.dnl );
  report->start();
  report->progress();
  report->stop();
}

/******************************************************************
**
**
**	FUNCTION NAME : inst
**	FUNCTION TYPE : void
*/
void inst() {
  Report<InstCallback>::Send report( cbl.inst );
  report->start();
  dnl();
  report->progress();
  report->stop();
}

/******************************************************************
**
**
**	FUNCTION NAME : main
**	FUNCTION TYPE : int
*/
int main( int argc, char ** argv )
{
  set_log_filename("-");
  INT << "START" << endl;

  inst();
  INT << "==================================" << endl;
  cbl.dnl.redirectTo( receive.recDnl );
  cbl.inst.redirectTo( receive.recInst );

  inst();

  receive.recDnl.start();
  receive.recDnl->start();

  INT << "DONE" << endl;
  return 0;
}
