#
# Makefile.am for liby2util/testsuite
#
AUTOMAKE_OPTIONS = dejagnu

INCLUDES= -I$(top_srcdir)/src/include -I$(includedir) -I$(QTDIR)/include

LDFLAGS = -L$(libdir)			\
	-Xlinker --whole-archive	\
	$(top_srcdir)/src/liby2util.la	\
	-Xlinker --no-whole-archive

clean-local:
	rm -f tmp.* ycp.log ycp.sum site.exp libycp.log \
	libycp.sum site.bak

EXTRA_DIST =

#==================================================================
#
# - test programms are suffixed '_test'
# - for each 'prog_test' there's an EXTRA_PROGRAM 'nc_prog_test'
#   compiled with -DFAILCOMPILE to test ecpected compiler errors.
#
# 'make test'             tests all progs
# 'make test_prog_test'   tests prog_test
#
noinst_PROGRAMS = 	hier_test const_test

hier_test_SOURCES = 	hier.cc
const_test_SOURCES = 	const.cc

# nc_ targets compiled with -DFAILCOMPILE
#
EXTRA_PROGRAMS = nc_hier_test nc_const_test

nc_hier_test_SOURCES = $(hier_test_SOURCES)
nc_hier_test_CXXFLAGS = -DFAILCOMPILE

nc_const_test_SOURCES = $(const_test_SOURCES)
nc_const_test_CXXFLAGS = -DFAILCOMPILE

test:	$(addprefix test_, $(noinst_PROGRAMS))
	@echo
	@echo "tested $(noinst_PROGRAMS)"

# test expected log messages
test_%:	% fail_%
	./$< 2>&1 >/dev/null | grep "\[TEST]" | cut -d ' ' -f 3,5- > tmp.$<.out
	diff -u $<.out tmp.$<.out

# test expected compiler errors
fail_%:	%
	-$(MAKE) nc_$< 2> tmp.$<.fail >/dev/null
	diff -u $<.fail tmp.$<.fail
