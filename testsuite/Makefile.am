#
# Makefile.am for liby2util/testsuite
#
SUBDIRS = lib
AUTOMAKE_OPTIONS = dejagnu

INCLUDES= -I$(QTDIR)/include -I../src/include -I$(includedir)

CPPFLAGS = -DY2LOG=__FILE__

LDFLAGS = -L$(libdir)			\
	-Xlinker --whole-archive	\
	../src/liby2util.la		\
	-Xlinker --no-whole-archive


noinst_PROGRAMS = parser.prg	\
	test_Y2SLog.prg		\
	test_Rep.prg		\
	test_BitField.prg	\
	test_mkdir.prg


parser_prg_SOURCES = parser.cc


test_Y2SLog_prg_SOURCES = test_Y2SLog.cc
#test_Y2SLog_prg_LDFLAGS = -static

test_Rep_prg_SOURCES = test_Rep.cc
#test_Rep_prg_LDFLAGS = -static

test_BitField_prg_SOURCES = test_BitField.cc
#test_BitField_prg_LDFLAGS = -static

test_mkdir_prg_SOURCES = test_mkdir.cc

clean-local:
	rm -f tmp.err.* tmp.out.* ycp.log ycp.sum site.exp libycp.log \
	libycp.sum site.bak

EXTRA_DIST =
