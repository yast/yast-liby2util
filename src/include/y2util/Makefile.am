#
# Makefile.am for liby2util/src/include/y2util
#

pkgincludedir = $(includedir)/y2util

# these go to $(pkgincludedir)

pkginclude_HEADERS = \
	stringutil.h		\
	hash.h			\
	y2logging.h		\
	Ustring.h		\
	LangCode.h		\
	Pathname.h		\
	PathInfo.h		\
	Rep.h			\
	RepDef.h		\
	BitField.h		\
	RefObject.h		\
	Y2SLog.h		\
	TagParser.h		\
	TagCacheRetrieval.h	\
	TagCacheRetrievalPtr.h	\
	CommonPkdParser.h	\
	timeclass.h		\
	ExternalDataSource.h	\
	ExternalProgram.h	\
	Url.h			\
	Date.h			\
	FSize.h			\
	TreeItem.h		\
	YRpmGroupsTree.h	\
	diff.h			\
	YStringTree.h		\
	YTransText.h


#<INSTALL-HEADER-TARGET>

# Automatically inserted by insert_install_target
# DO NOT REMOVE THE TAGS!

# Install header files only if different to version already installed -
# prevent unnecessary installations to keep make dependency changes
# and thus rebuilds for dependent packages as low as possible.

install-pkgincludeHEADERS: $(pkginclude_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(pkgincludedir)
	@list='$(pkginclude_HEADERS)';							\
	for p in $$list; do								\
		if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi;			\
		if ! cmp -s $$d$$p $(DESTDIR)$(pkgincludedir)/$$p; then			\
			echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(pkgincludedir)/$$p";	\
			$(INSTALL_DATA) $$d$$p $(DESTDIR)$(pkgincludedir)/$$p;		\
		fi									\
	done

#</INSTALL-HEADER-TARGET>
