#
# Makefile.am for liby2util/testsuite
#
SUBDIRS = lib test_rep
PACKAGE = liby2util
AUTOMAKE_OPTIONS = dejagnu

INCLUDES= -I$(top_srcdir)/src/include

LDFLAGS = -L$(libdir)			\
	-Xlinker --whole-archive	\
	$(top_builddir)/src/liby2util.la	\
	-Xlinker --no-whole-archive

bin_PROGRAMS = validate_tagged	\
	validate_packages	\
	validate_packages_lang	\
	validate_selection

noinst_PROGRAMS = parser.prg	\
	test_taggedparser.prg	\
	test_Y2SLog.prg		\
	test_Rep.prg		\
	test_strutil.prg	\
	test_mkdir.prg		\
	test_chroot.prg		\
	test_url.prg            \
        test_gpgcheck.prg	\
	test_oldsel

test_oldsel_SOURCES = test_oldsel.cc

validate_tagged_SOURCES = validate_tagged.cc
validate_packages_SOURCES = validate_packages.cc
validate_packages_lang_SOURCES = validate_packages_lang.cc
validate_selection_SOURCES = validate_selection.cc

parser_prg_SOURCES = parser.cc
test_taggedparser_prg_SOURCES = test_taggedparser.cc

test_Y2SLog_prg_SOURCES = test_Y2SLog.cc
#test_Y2SLog_prg_LDFLAGS = -static

test_Rep_prg_SOURCES = test_Rep.cc
#test_Rep_prg_LDFLAGS = -static

test_strutil_prg_SOURCES = test_strutil.cc
#test_strutil_prg_LDFLAGS = -static

test_mkdir_prg_SOURCES = test_mkdir.cc

test_url_prg_SOURCES = test_url.cc

test_chroot_prg_SOURCES = test_chroot.cc
test_chroot_prg_LDFLAGS = -static

test_gpgcheck_prg_SOURCES = test_gpgcheck.cc

clean-local:
	rm -f tmp.err.* tmp.out.* y2util.log y2util.sum site.exp site.bak

EXTRA_DIST =
