#
# Makefile.am for liby2util/testsuite
#

AUTOMAKE_OPTIONS = dejagnu

INCLUDES= -I$(QTDIR)/include -I../src/include -I$(includedir)

CPPFLAGS =

LDFLAGS = -L$(libdir)	\
	-Xlinker --whole-archive	\
	../src/liby2util.la	\
	-Xlinker --no-whole-archive


noinst_PROGRAMS = runy2util test_Y2SLog.exe


runy2util_SOURCES = runy2util.cc


test_Y2SLog_exe_SOURCES = test_Y2SLog.cc
test_Y2SLog_exe_LDFLAGS = -static


clean-local:
	rm -f tmp.err.* tmp.out.* ycp.log ycp.sum site.exp libycp.log \
	libycp.sum site.bak

EXTRA_DIST =
