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

   File:       logging.cc

   Author:     Michael Andres <ma@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

   Purpose:	keep common logging functions for y2util/Y2SLog
		and ycp/y2log here.
/-*/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <errno.h>

#include <string>
#include <iostream>

#include <y2util/y2logging.h>

using namespace std;

#define _GNU_SOURCE 1		// Needed for vasprintf below

#define Y2LOG_DATE	"%Y-%m-%d %H:%M:%S"	/* The date format */
#define Y2LOG_FORMAT	"%s <%d> %s(%d)%s %s%s:%d %s%s"
#define Y2LOG_MAXSIZE	1024 * 1024		/* Maximal logfile size */
#define Y2LOG_MAXNUM	10			/* Maximum logfiles number */

// FIXME
#define Y2LOG_ROOT	LOGDIR "/y2log"
#define Y2LOG_USER	"/.y2log"		/* Relative to $HOME */
#define Y2LOG_FALLBACK	"/y2log"

#define LOGDIR		"/var/log/YaST2"


///////////////////////////////////////////////////////////////////
//
namespace Y2Logging {
//
///////////////////////////////////////////////////////////////////

//---------------------------------------------------------
// static variables

static char *loggingname;
static int  logging_initialized;
int loggingInitialized (void) { return logging_initialized; }

static int	y2maxlogsize;
static int	y2maxlognum;

static FILE * Y2LOG_STDERR = stderr;            /* Default output */
FILE *getY2LogStderr () { return Y2LOG_STDERR; }
/* static prototypes */

static void	shiftLogfiles( string filename );

//---------------------------------------------------------
// static functions

// Tue Jan 29 16:51:39 CET 2002 - ma@suse.de
// y2log must use a private copy of stderr, esp. in case we're always logging to
// it (option "-l -"). Some classes like liby2(ExternalProgram) redirect stderr in
// order to redirect an external programs error output. As a side effect y2log output
// done after the redirection would show up in the external programs output file instead
// of yast2's stderr.
static int dupStderr()
{
    int dupstderr = dup( 2 );
    if ( dupstderr != -1 ) {
	FILE * newstderr = fdopen( dupstderr, "a" );

	if ( newstderr == NULL ) {
	    fprintf( Y2LOG_STDERR, "y2log: Can't fdopen new stderr: %s.\n", strerror (errno) );
	}
	else {
	    fcntl (fileno (newstderr), F_SETFD, fcntl (fileno (newstderr), F_GETFD) | FD_CLOEXEC);
	    Y2LOG_STDERR = newstderr;
	}
    }
    else {
	fprintf( Y2LOG_STDERR, "y2log: Can't dup stderr: %s.\n", strerror (errno) );
    }
    return 1;
}
static int x = dupStderr();
// Tue Jan 29 16:51:39 CET 2002 - ma@suse.de

/* Maintain logfiles */

static void
shiftLogfiles( string filename )
{
    struct stat buf;
    char numbuf[8];

    if( stat(filename.c_str(), &buf) )
	return;

    if( buf.st_size <= y2maxlogsize )
	return;

    /* Delete the last logfile, rename existing ones */
    sprintf (numbuf, "%d", y2maxlognum - 1);
    remove ((filename + "-" + numbuf).c_str());

    for( int f = y2maxlognum-2; f > 0; f-- )
    {
	sprintf (numbuf, "%d", f);
	string oldname = filename + "-" + numbuf;
	sprintf (numbuf, "%d", f+1);
	rename (oldname.c_str(), (filename + "-" + numbuf).c_str());
    }
    rename( filename.c_str(), (filename + "-1").c_str() );
}

//---------------------------------------------------------
// global functions

/* The universal logger function */

void
y2_logger(loglevel_t level, const char *component, const char *file,
	  const int line, const char *func, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    y2_vlogger(level, component, file, line, func, format, ap);
    va_end(ap);
}

/* The universal logger function */

void
y2_vlogger(loglevel_t level, const char *component, const char *file,
	   const int line, const char *function, const char *format, va_list ap)
{
    /* Prepare the logfile name */
    if (!logging_initialized)
	setLogfileName (""); /* The default */

    /* Prepare the log text */
    char *logtext = NULL;
    vasprintf(&logtext, format, ap); /* GNU extension needs the define above */

    /* Prepare the PID */
    pid_t pid = getpid();

    /* Prepare the host name */
    char hostname[1024];
    if (gethostname(hostname, 1024))
	strncpy(hostname, "unknown", 1024);

    /* Prepare the logfile */
    shiftLogfiles (string (loggingname));

    FILE *logfile = Y2LOG_STDERR;
    if (*loggingname != '-') {
	logfile = fopen (loggingname, "a");
	if (!logfile) {
	    fprintf (Y2LOG_STDERR, "y2log: Error opening logfile '%s': %s.\n",
		     loggingname, strerror (errno));
	    return;
	}
    }

    /* Prepare the date */
#if 1
    // just 1 second precision
    time_t timestamp = time (NULL);
    struct tm *brokentime = localtime (&timestamp);
    char date[50];		// that's big enough
    strftime (date, sizeof (date), Y2LOG_DATE, brokentime);
#else
    // 1 millisecond precision (use only for testing)
    timeval time;
    gettimeofday (&time, NULL);
    time_t timestamp = time.tv_sec;
    struct tm *brokentime = localtime (&timestamp);
    char tmp1[50], date[50];	// that's big enough
    strftime (tmp1, sizeof (tmp1), Y2LOG_DATE, brokentime);
    snprintf (date, sizeof (date), "%s.%03ld", tmp1, time.tv_usec / 1000);
#endif

#if 0
    // print "bytes from system" and "bytes in use" per thread (use only
    // for testing)
    struct mallinfo mi;
    mi = mallinfo ();
    char tmp2[50];
    snprintf (tmp2, sizeof (tmp2), " [%5dkB %5dkB]", mi.arena >> 10,
	      mi.uordblks >> 10);
    strcat (date, tmp2);
#endif

    /* Prepare the component */
    string comp = component;
    if (!comp.empty ())
	comp = " [" + comp + "]";

    /* Prepare the component */
    string func = function;
    if (!func.empty ())
	func = "(" + func + ")";

    /* Prepare the file, strip rooted path  */

    if (*file == '/')		     // rooted path
    {
	char *slashptr = strrchr (file, '/');
	if (slashptr > file)		    // last slash is second slash
	{
	    char *slashptr2 = slashptr-1;

	    // find last but one slash

	    while ((slashptr2 > file) && (*slashptr2 != '/'))
	    {
		slashptr2--;
	    }
	    // if last but one slash exists, use this as start
	    if (slashptr2 != file)
		file = slashptr2 + 1;
	}
    }

    /* do we need EOL? */
    bool eol = false;
    size_t len = strlen(logtext);
    if ((len==0) || ((len>0) && (logtext[len-1]!='\n')))
	eol = true;

    /* Do the log */
    fprintf (logfile, Y2LOG_FORMAT, date, level, hostname, pid, comp.c_str (),
	    file, func.c_str (), line, logtext, eol?"\n":"");

    /* Clean everything */
    if (logfile && logfile != Y2LOG_STDERR)
	fclose (logfile);
    else
	fflush (logfile);

    if (logtext)
	free (logtext);
}

/* Logfile name initialization */

void
setLogfileName (const char *filename)
{
    struct passwd *pw = getpwuid( geteuid() );

    char *env_maxlogsize = getenv("Y2MAXLOGSIZE");
    y2maxlogsize = env_maxlogsize ? atoi(env_maxlogsize) * 1024 : Y2LOG_MAXSIZE;

    char *env_maxlognum = getenv("Y2MAXLOGNUM");
    y2maxlognum = env_maxlognum ? atoi(env_maxlognum) : Y2LOG_MAXNUM;

    /* Assign logfile name */

    if ((filename == 0)
	|| (*filename == 0))
    {		/* No filename --> use defaults */
	if (geteuid()) {			/* Non root */
	    if (!pw) {
		fprintf( Y2LOG_STDERR,
			 "Cannot read pwd entry of user id %d. Logfile will be '%s'.\n",
			 geteuid(), Y2LOG_FALLBACK );

		loggingname = Y2LOG_FALLBACK;
	    }
	    else {
		loggingname = (char *)malloc (strlen (pw->pw_dir) + strlen (Y2LOG_USER) + 1);
		*loggingname = 0;
		strcat (loggingname, pw->pw_dir);
		strcat (loggingname, Y2LOG_USER);
	    }
	}
	else		    /* Root */
	    loggingname = Y2LOG_ROOT;
    }
    else
	loggingname = strdup (filename);  /* Explicit assignment */

    logging_initialized = 2;
}


/* Logfile name reporting */

string
getLogfileName ()
{
    if (!logging_initialized)
	setLogfileName (""); /* The default */

    return string (loggingname);
}

} // namespace Y2Logging
