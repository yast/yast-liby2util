#
# Makefile.am for liby2util/testsuite
#
SUBDIRS = lib test_rep
PACKAGE = liby2util
AUTOMAKE_OPTIONS = dejagnu

INCLUDES= -I$(top_srcdir)/src/qt -I$(top_srcdir)/src/include -I$(includedir)

LDFLAGS = -L$(libdir)			\
	-Xlinker --whole-archive	\
	$(top_builddir)/src/liby2util.la		\
	-Xlinker --no-whole-archive


noinst_PROGRAMS = parser.prg	\
	test_Y2SLog.prg		\
	test_Rep.prg		\
	test_strutil.prg	\
	test_BitField.prg	\
	test_mkdir.prg		\
	test_chroot.prg		\
	test_url.prg            \
        test_gpgcheck.prg


parser_prg_SOURCES = parser.cc


test_Y2SLog_prg_SOURCES = test_Y2SLog.cc
#test_Y2SLog_prg_LDFLAGS = -static

test_Rep_prg_SOURCES = test_Rep.cc
#test_Rep_prg_LDFLAGS = -static

test_strutil_prg_SOURCES = test_strutil.cc
#test_strutil_prg_LDFLAGS = -static

test_BitField_prg_SOURCES = test_BitField.cc
#test_BitField_prg_LDFLAGS = -static

test_mkdir_prg_SOURCES = test_mkdir.cc

test_url_prg_SOURCES = test_url.cc

test_chroot_prg_SOURCES = test_chroot.cc
test_chroot_prg_LDFLAGS = -static

test_gpgcheck_prg_SOURCES = test_gpgcheck.cc

clean-local:
	rm -f tmp.err.* tmp.out.* y2util.log y2util.sum site.exp site.bak

EXTRA_DIST =
