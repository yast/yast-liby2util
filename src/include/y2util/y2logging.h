/* y2logging.h
 *
 * YaST2: Core system
 *
 * YaST2 logging implementation
 *
 * Authors: Mathias Kettner <kettner@suse.de>
 *          Michal Svec <msvec@suse.cz>
 *          Arvin Schnell <arvin@suse.de>
 *
 * $Id$
 */

#ifndef _y2logging_h
#define _y2logging_h

#include <string>
#include <stdio.h>

enum loglevel_t
{
    LOG_DEBUG = 0,	// debug message
    LOG_MILESTONE = 1,	// log great events, big steps
    LOG_WARNING = 2,	// warning in application level
    LOG_ERROR = 3,	// error in application level
    LOG_SECURITY = 4,	// security relevant problem or incident
    LOG_INTERNAL = 5	// internal bug. Please report to...
};

///////////////////////////////////////////////////////////////////
//
namespace Y2Logging {
//
///////////////////////////////////////////////////////////////////

/* Logging levels */

/* Logging functions */

void y2_logger (loglevel_t level, const char *component, const char *file,
		const int line, const char *func, const char *format, ...)
    __attribute__ ((format (printf, 6, 7)));

void y2_vlogger (loglevel_t level, const char *component, const char *file,
		 const int line, const char *func, const char *format, va_list ap);

/**
 * Set an alternate logfile name for @ref y2log. If this is not done by the
 * application the first call to y2log sets the logfile name as follows:
 * users: $HOME/.y2log
 * root: /var/log/YaST2/y2log
 * The logname "-" is special: The log messages are written to stderr.
 */
void setLogfileName( const char * logto_tr );
std::string getLogfileName (void);

FILE *getY2LogStderr (void);

int loggingInitialized (void);

}	// namespace Y2Logging

#endif /* _y2logging_h */

