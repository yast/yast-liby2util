#
# Makefile.am for liby2/testsuite
#

AUTOMAKE_OPTIONS = dejagnu

INCLUDES= -I../src/include -I$(includedir)

CPPFLAGS = -DDEBUG

noinst_PROGRAMS = runy2util refptrtest

runy2util_SOURCES = runy2util.cc
runy2util_LDADD = -ly2util
runy2util_LDFLAGS = -L$(libdir)	\
	-Xlinker --whole-archive	\
	../src/liby2util.la	\
	-Xlinker --no-whole-archive

refptrtest_SOURCES = refptrtest.cc
refptrtest_LDADD = ../src/liby2util.a

clean-local:
	rm -f tmp.err.* tmp.out.* ycp.log ycp.sum site.exp libycp.log \
	libycp.sum site.bak

EXTRA_DIST =
