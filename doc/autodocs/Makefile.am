#
# Makefile.am for liby2util/doc/autodocs
#

htmldir = $(docdir)/autodocs

html_DATA = index.html $(wildcard *.html *.png) doxygen.css @RPMNAME@.tag

CLEANFILES = $(html_DATA) doxygen.log doxygen.conf installdox

doxygen.css @RPMNAME@.tag: index.html

header_dir = $(srcdir)/../../src/include

index.html: $(header_dir)/*.h $(header_dir)/y2util/*.h
	@ybindir@/ydoxygen PROJECT_NAME=@RPMNAME@ TAGFILES=
