#
# Makefile.am for liby2util/src
#

SUBDIRS = include

INCLUDES = -I$(QTDIR)/include -I$(srcdir)/include -I$(includedir)

#AM_CPPFLAGS = -DREP_DEBUG
AM_CPPFLAGS = -DY2LOG=\"liby2util\"

lib_LTLIBRARIES = liby2util.la

liby2util_la_SOURCES = \
	hash.cc \
	Rep.cc \
	BitField.cc \
	Pathname.cc \
	PathInfo.cc \
	TagParser.cc \
	CommonPkdParser.cc \
	Y2SLog.cc

liby2util_la_LDFLAGS = -version-info 2:0

liby2util_la_LIBADD = \
	-L$(QTLIBDIR) -lqt-mt \
	-lycp

# for debug

noinst_LIBRARIES = liby2util.a

liby2util_a_SOURCES = $(liby2util_la_SOURCES)
