#
# Makefile.am for liby2util/src/include/y2util
#

pkgincludedir = $(includedir)/y2util

# these go to $(pkgincludedir)

pkginclude_HEADERS =		\
	CallBack.h		\
	CommonPkdParser.h	\
	Date.h			\
	DiskSpace.h		\
	ExternalDataSource.h	\
	ExternalProgram.h	\
	FSize.h			\
	GPGCheck.h		\
	LangCode.h		\
	MemUsage.h		\
	PathInfo.h		\
	Pathname.h		\
	ProgressCounter.h	\
	Rep.h			\
	RepDef.h		\
	SysConfig.h		\
	TagCacheRetrieval.h	\
	TagCacheRetrievalPtr.h	\
	TagParser.h		\
	TagRetrievalPos.h	\
	TaggedFile.h		\
	TaggedParser.h		\
	TreeItem.h		\
	Url.h			\
	Ustring.h		\
	Vendor.h		\
	Y2SLog.h		\
	YRpmGroupsTree.h	\
	YStringTree.h		\
	YTransText.h		\
	diff.h			\
	hash.h			\
	miniini.h		\
	stringutil.h		\
	timeclass.h		\
	y2log.h


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
