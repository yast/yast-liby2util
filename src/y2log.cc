/* y2log.cc
 *
 * YaST2: Core system
 *
 * YaST2 logging implementation
 *
 * Authors: Mathias Kettner <kettner@suse.de>
 *          Thomas Roelz <tom@suse.de>
 *          Michal Svec <msvec@suse.cz>
 *          Arvin Schnell <arvin@suse.de>
 *
 * $Id$
 */

#include <errno.h>
#include <fcntl.h>
#include <malloc.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "y2util/y2log.h"
#include "y2util/miniini.h"
#include <syslog.h>

/* Defines */

#define _GNU_SOURCE 1				/* Needed for vasprintf below */

#define Y2LOG_DATE	"%Y-%m-%d %H:%M:%S"	/* The date format */
#define Y2LOG_FORMAT	"%s <%d> %s(%d)%s %s%s:%d %s%s"
#define Y2LOG_SYSLOG	"<%d>%s %s%s:%d %s%s"
#define Y2LOG_MAXSIZE	1024 * 1024		/* Maximal logfile size */
#define Y2LOG_MAXNUM	10			/* Maximum logfiles number */

// FIXME
#define LOGDIR		"/var/log/YaST2"

#define Y2LOG_ROOT	LOGDIR "/y2log"
#define Y2LOG_USER	"/.y2log"		/* Relative to $HOME */
#define Y2LOG_FALLBACK	"/y2log"

#define Y2LOG_CONF	"log.conf"	/* Relative to $HOME or /etc/YaST2 */

#define Y2LOG_VAR_DEBUG	"Y2DEBUG"
#define Y2LOG_VAR_ALL	"Y2DEBUGALL"
#define Y2LOG_VAR_SIZE	"Y2MAXLOGSIZE"
#define Y2LOG_VAR_NUM	"Y2MAXLOGNUM"

#define Y2LOG_FACILITY	"yast2"

inisection logconf;

static bool did_set_logname = false;
static bool did_read_logconf = false;

static char *logname;

static int maxlogsize;
static int maxlognum;

static bool log_debug = false;
static bool log_to_file = true;
static bool log_to_syslog = false;

static bool log_all_variable = false;
static bool log_debug_variable = false;

static FILE *Y2LOG_STDERR = stderr;		/* Default output */

/* static prototypes */
static void shift_log_files(string filename);
static void read_logconf();

/**
 * y2log must use a private copy of stderr, esp. in case we're always logging
 * to it (option "-l -"). Some classes like liby2(ExternalProgram) redirect
 * stderr in order to redirect an external programs error output. As a side
 * effect y2log output done after the redirection would show up in the external
 * programs output file instead of yast2's stderr.
 */
static int dup_stderr()
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
static int variable_not_used = dup_stderr();


/**
 * The universal logger function
 */
void y2_logger_function(loglevel_t level, const char *component, const char *file,
	  const int line, const char *func, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    y2_vlogger(level, component, file, line, func, format, ap);
    va_end(ap);
}

/**
 * The universal logger function
 */
void y2_vlogger_function(loglevel_t level, const char *component, const char *file,
	   const int line, const char *function, const char *format, va_list ap)
{
    /* Prepare the logfile name */
    if(!did_set_logname) set_log_filename("");

    /* Prepare the log text */
    char *logtext = NULL;
    vasprintf(&logtext, format, ap); /* GNU extension needs the define above */

    /* Prepare the component */
    string comp = component;
    if (!comp.empty ())
	comp = " [" + comp + "]";

    /* Prepare the file, strip rooted path  */
    if(*file == '/')		     // rooted path
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

    /* Prepare the component */
    string func = function;
    if (!func.empty ())
	func = "(" + func + ")";

    /* do we need EOL? */
    bool eol = false;
    size_t len = strlen(logtext);
    if ((len==0) || ((len>0) && (logtext[len-1]!='\n')))
	eol = true;

    if(log_to_syslog) {
	syslog (LOG_NOTICE, Y2LOG_SYSLOG, level, comp.c_str (),
	    file, func.c_str (), line, logtext, eol?"\n":"");
    }

    if(!log_to_file) {
	if(logtext) free(logtext);
	return;
    }

    /* Prepare the PID */
    pid_t pid = getpid();

    /* Prepare the host name */
    char hostname[1024];
    if (gethostname(hostname, 1024))
	strncpy(hostname, "unknown", 1024);

    /* Prepare the logfile */
    shift_log_files (string (logname));

    FILE *logfile = Y2LOG_STDERR;
    if (*logname != '-') {
	logfile = fopen (logname, "a");
	if (!logfile) {
	    fprintf (Y2LOG_STDERR, "y2log: Error opening logfile '%s': %s (%s:%d).\n",
		     logname, strerror (errno), file, line);
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

/**
 * Logfile name initialization
 */
void set_log_filename (string fname)
{
    did_set_logname = true;

    if(!did_read_logconf) read_logconf();

    if(log_to_syslog) openlog("yast2", LOG_PID, LOG_DAEMON);
    if(!log_to_file) return;

    struct passwd *pw = getpwuid( geteuid() );
    const char *filename = fname.c_str();

    char *env_maxlogsize = getenv("Y2MAXLOGSIZE");
    maxlogsize = env_maxlogsize ? atoi(env_maxlogsize) * 1024 : Y2LOG_MAXSIZE;

    char *env_maxlognum = getenv("Y2MAXLOGNUM");
    maxlognum = env_maxlognum ? atoi(env_maxlognum) : Y2LOG_MAXNUM;

    /* Assign logfile name */

    if ((filename == 0) || (*filename == 0))
    {		/* No filename --> use defaults */
	if (geteuid()) {			/* Non root */
	    if (!pw)
	    {
		fprintf( Y2LOG_STDERR,
			 "Cannot read pwd entry of user id %d. Logfile will be '%s'.\n",
			 geteuid(), Y2LOG_FALLBACK );

		logname = Y2LOG_FALLBACK;
	    }
	    else
	    {
		logname = (char *)malloc (strlen (pw->pw_dir) + strlen (Y2LOG_USER) + 1);
		strcpy (logname, pw->pw_dir);
		strcat (logname, Y2LOG_USER);
	    }
	}
	else		    /* Root */
	    logname = Y2LOG_ROOT;
    }
    else
	logname = strdup (filename);  /* Explicit assignment */
}


/**
 * Logfile name reporting
 */
string get_log_filename()
{
    if(!did_set_logname) set_log_filename("");
    return string (logname);
}


/**
 * Maintain logfiles
 */
static void shift_log_files(string filename)
{
    struct stat buf;
    char numbuf[8];

    if( stat(filename.c_str(), &buf) )
	return;

    if( buf.st_size <= maxlogsize )
	return;

    /* Delete the last logfile, rename existing ones */
    sprintf (numbuf, "%d", maxlognum - 1);
    remove ((filename + "-" + numbuf).c_str());

    for( int f = maxlognum-2; f > 0; f-- )
    {
	sprintf (numbuf, "%d", f);
	string oldname = filename + "-" + numbuf;
	sprintf (numbuf, "%d", f+1);
	rename (oldname.c_str(), (filename + "-" + numbuf).c_str());
    }
    rename( filename.c_str(), (filename + "-1").c_str() );
}


/**
 * Parse the log.conf
 */
static void read_logconf() {

    did_read_logconf = true;

    if(getenv(Y2LOG_VAR_DEBUG)) log_debug_variable = true;
    if(getenv(Y2LOG_VAR_ALL)) log_all_variable = true;

    /* Read the logconf.ycp */
    struct passwd *pw = getpwuid(geteuid());
    if(pw) {

	string logconfname;
	if(getuid())
	    logconfname = string (pw->pw_dir) + "/.yast2/" + Y2LOG_CONF;
	else
	    logconfname = "/etc/YaST2/" Y2LOG_CONF;

	/* We have to remember the errno. Otherwise a call of
	 * y2error ("error: %m") can display a wrong message. */
	int save_errno = errno;
	inifile i = miniini(logconfname.c_str());
	logconf = i["Debug"];

	log_to_file = i["Log"]["file"] != "false";
	log_to_syslog = i["Log"]["syslog"] == "true";
	log_debug = i["Log"]["debug"] == "true";

	errno = save_errno;
    }
    else
    {
	fprintf (stderr, "Cannot read pwd entry for user "
		 "id %d. No logconf, using defaults.\n", geteuid ());
    }
}


/**
 * Test if we should create a log entry
 */
bool should_be_logged (int loglevel, string componentname) {

    /* Only debug level is controllable */
    if(loglevel > 0) return true;

    /* Prepare the logfile name. */
    if(!did_set_logname)
	set_log_filename("");

    /* Read log configuration. */
    if(!did_read_logconf)
	read_logconf();

    /* Everything should be logged */
    if(log_all_variable) return true;

    /* Specific component */
    if(logconf.find(componentname) != logconf.end())
	return logconf[componentname] == "true";

    /* Environment variable */
    if(log_debug_variable) return true;

    /* Config setting */
    return log_debug;
}

/* EOF */
