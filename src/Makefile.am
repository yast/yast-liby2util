#
# Makefile.am for liby2util/src
#

SUBDIRS = include

lib_LTLIBRARIES = liby2util.la

liby2util_la_SOURCES = hash.cc Pathname.cc
liby2util_la_LDFLAGS = -static

INCLUDES = -I$(srcdir)/include -I$(includedir)

noinst_PROGRAMS = refptrtest

refptrtest_SOURCES = refptrtest.cc
refptrtest_LDADD = liby2util.la

# only for debug
refptrtest_LDFLAGS = -static
