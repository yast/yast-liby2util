#
# Makefile.am for liby2/testsuite
#

AUTOMAKE_OPTIONS = dejagnu

INCLUDES= -I../src/include -I$(includedir)

noinst_PROGRAMS = runy2util

runy2util_SOURCES = runy2util.cc
runy2util_LDADD = -ly2util
runy2util_LDFLAGS = -L$(libdir)	\
	-Xlinker --whole-archive	\
	../src/liby2util.la	\
	-Xlinker --no-whole-archive


clean-local:
	rm -f tmp.err.* tmp.out.* ycp.log ycp.sum site.exp libycp.log \
	libycp.sum site.bak

EXTRA_DIST =
