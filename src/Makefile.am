#
# Makefile.am for liby2util/src
#

SUBDIRS = include

INCLUDES = -I$(srcdir)/include -I$(includedir)

AM_CPPFLAGS = -DY2LOG=\"liby2util\" $(DEBUG_FLAGS) -D_GNU_SOURCE

lib_LTLIBRARIES = liby2util.la

liby2util_la_SOURCES = \
	stringutil.cc		\
	hash.cc			\
	y2logging.cc		\
	LangCode.cc		\
	Vendor.cc		\
	Rep.cc			\
	Pathname.cc		\
	PathInfo.cc		\
	TagParser.cc		\
	TaggedParser.cc		\
	TagRetrievalPos.cc	\
	TagCacheRetrieval.cc	\
	CommonPkdParser.cc	\
	TaggedFile.cc		\
	Y2SLog.cc		\
	ExternalDataSource.cc	\
	ExternalProgram.cc	\
	Url.cc			\
	Date.cc			\
	FSize.cc		\
	YStringTree.cc		\
	YRpmGroupsTree.cc	\
	diff.cc			\
	DiskSpace.cc		\
        GPGCheck.cc             \
        timeclass.cc             \
        SysConfig.cc


liby2util_la_LDFLAGS = -version-info 3:0:0

#liby2util_la_LDFLAGS = -version-info 2:0 \
# 	-L$(QTLIBDIR)

liby2util_la_LIBADD = \
	-lutil
