EXTRA_DIST = dstrukt-entwurf.tex libPkgDb.tex libPkgDep.tex macros.tex \
		header.fig relationmatch.fig phi-update.txt \
		gen-pathmacros.pl run-tex.pl

all-local: ps-local
dvi-local: dstrukt-entwurf.dvi libPkgDb.dvi libPkgDep.dvi
ps-local: dstrukt-entwurf.ps libPkgDb.ps libPkgDep.ps
clean-local:
	rm -f *.ps *.dvi *.aux *.log *.eps

%.ps: %.dvi
	dvips $< -o

%.dvi: %.tex
	./run-tex.pl latex $<

%.eps: %.fig
	fig2dev -L ps $< $@

#pathmacros.tex: gen-pathmacros.pl ../libPkgDb/paths.h
#	./gen-pathmacros.pl
pathmacros.tex:

libPkgDb.tex: relationmatch.eps pathmacros.tex header.eps
