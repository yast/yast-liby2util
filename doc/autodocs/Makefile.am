#
# Makefile.am for liby2util/doc/autodocs
#

htmldir = $(docdir)/autodocs

html_DATA = index.html heir.html \
	DerivedRefObject.html \
	PathInfo.html \
	Pathname.html \
	RefObject.html \
	Rep.html \
	T.html \
	Tag.html \
	TagParser.html \
	TagSet.html \
	TimeClass.html \
	UniqStr.html \
	UniqStrInternal.html \
	list_elem.html

EXTRA_DIST = $(html_DATA)

CLEANFILES = $(html_DATA)

$(filter-out index.html, $(html_DATA)): index.html
index.html: $(srcdir)/../../src/include/*.h $(srcdir)/../../src/include/y2util/*.h
	cd $(srcdir)/../../src/include && @ybindir@/ydoc -p @RPMNAME@ *.h y2util/*.h
	mv -f $(srcdir)/../../src/include/*.html .
