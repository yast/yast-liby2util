#
# Makefile.am for liby2util/doc/autodocs
#

htmldir = $(docdir)/autodocs

html_DATA = index.html $(wildcard *.html)

CLEANFILES = $(html_DATA)

$(filter-out index.html, $(html_DATA)): index.html
index.html: $(srcdir)/../../src/include/*.h $(srcdir)/../../src/include/y2util/*.h
	cd $(srcdir)/../../src/include && @ybindir@/ydoc -p @RPMNAME@ *.h y2util/*.h
	mv -f $(srcdir)/../../src/include/*.html .
