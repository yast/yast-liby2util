#
# Makefile.am for liby2util/src
#

SUBDIRS = include

INCLUDES = -I$(srcdir)/include

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
	MemUsage.cc		\
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
	Digest.cc		\
	y2log.cc


liby2util_la_LDFLAGS = -version-info 3:0:0

#liby2util_la_LDFLAGS = -version-info 2:0 \
# 	-L$(QTLIBDIR)

liby2util_la_LIBADD = -lutil $(OPENSSL_LIBS)

noinst_PROGRAMS = digest_bin

digest_bin_SOURCES = Digest.cc
digest_bin_CPPFLAGS = -DDIGEST_TESTSUITE
digest_bin_LDADD = $(liby2util_la_LIBADD)

test_digest: digest_bin
	@echo "output of 2 and 1 is success"
	@for algo in MD5 SHA1; do \
		./digest_bin --openssl $$algo /usr/bin/perl; \
		openssl dgst -$$algo /usr/bin/perl; \
	done | sort -u | wc -l
	@(./digest_bin MD5 /usr/bin/perl; md5sum /usr/bin/perl) | sort -u | wc -l
