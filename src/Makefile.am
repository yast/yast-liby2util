#
# Makefile.am for liby2util/src
#

SUBDIRS = include

INCLUDES = -I$(srcdir)/include -I$(includedir)

AM_CPPFLAGS = -DY2LOG=\"liby2util\" $(DEBUG_FLAGS) -D_GNU_SOURCE -DLOCALEDIR=\"${localedir}\"

lib_LTLIBRARIES = liby2util.la

liby2util_la_SOURCES =		\
	CommonPkdParser.cc	\
	Date.cc			\
	DiskSpace.cc		\
	ExternalDataSource.cc	\
	ExternalProgram.cc	\
	FSize.cc		\
	GPGCheck.cc		\
	LangCode.cc		\
	PathInfo.cc		\
	Pathname.cc		\
	ProgressCounter.cc	\
	Rep.cc			\
	SysConfig.cc		\
	TagCacheRetrieval.cc	\
	TagParser.cc		\
	TagRetrievalPos.cc	\
	TaggedFile.cc		\
	TaggedParser.cc		\
	Url.cc			\
	Vendor.cc		\
	Y2SLog.cc		\
	YRpmGroupsTree.cc	\
	YStringTree.cc		\
	diff.cc			\
	hash.cc			\
	miniini.cc		\
	stringutil.cc		\
	timeclass.cc		\
	y2log.cc


liby2util_la_LDFLAGS = -version-info 3:0:0

#liby2util_la_LDFLAGS = -version-info 2:0 \
# 	-L$(QTLIBDIR)

liby2util_la_LIBADD = \
	-lutil
