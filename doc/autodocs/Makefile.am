#
# Makefile.am for liby2util/doc/autodocs
#

htmldir = $(docdir)/autodocs

html_DATA = index.html $(wildcard *.html)

CLEANFILES = $(html_DATA)

header_dir = $(srcdir)/../../src/include
$(filter-out index.html, $(html_DATA)): index.html
index.html: $(header_dir)/*.h $(header_dir)/y2util/*.h
	@ybindir@/ydoc @RPMNAME@ --private --url $(htmldir) $^
