#
# Makefile.am for liby2util/src
#

SUBDIRS = include qt

INCLUDES = -Iqt -I$(srcdir)/include -I$(includedir)

AM_CPPFLAGS = -DY2LOG=\"liby2util\" $(DEBUG_FLAGS)

lib_LTLIBRARIES = liby2util.la

liby2util_la_SOURCES = \
	stringutil.cc		\
	hash.cc			\
	y2logging.cc		\
	LangCode.cc		\
	Rep.cc			\
	BitField.cc		\
	Pathname.cc		\
	PathInfo.cc		\
	TagParser.cc		\
	TagCacheRetrieval.cc	\
	CommonPkdParser.cc	\
	Y2SLog.cc		\
	ExternalDataSource.cc	\
	ExternalProgram.cc	\
	Url.cc			\
	Date.cc			\
	FSize.cc		\
	qt/qbitarray.cpp	\
	qt/qcstring.cpp	\
	qt/qdatastream.cpp	\
	qt/qgarray.cpp	\
	qt/qgdict.cpp	\
	qt/qglist.cpp	\
	qt/qglobal.cpp	\
	qt/qgvector.cpp	\
	qt/qiodevice.cpp	\
	qt/qptrcollection.cpp	\
	qt/qregexp.cpp	\
	qt/qstring.cpp	\
	qt/qstringlist.cpp	\
	qt/qtextstream.cpp


liby2util_la_LDFLAGS = -version-info 2:0

#liby2util_la_LDFLAGS = -version-info 2:0 \
# 	-L$(QTLIBDIR)

liby2util_la_LIBADD = \
	-lutil

# for debug

noinst_LIBRARIES = liby2util.a

liby2util_a_SOURCES = $(liby2util_la_SOURCES)
