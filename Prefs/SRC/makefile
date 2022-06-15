# MACROS
# C Compiler options
# SAS C 5.0 or higher

FLAGS = -Hram:headers -s -ccqst -v

# transformation rules
.c.o:
    lc $(FLAGS) $*

.asm.o:
    genim2 -L -O$*.o -Iincludeg: $*.asm


PGM = UC:
OBJ = uikprefs.o objreqmode.o datachip.o menu.o mode.o
CT1 = ct_deutsch.ct
CT2 = ct_francais.ct
CATH = lci:textprefs.h
DIST = PR:UIK/DistributionLibre/

fake: $(CATH) $(PGM)UIKPrefs $(DIST)uikprefs.cd cat1 cat2

$(PGM)UIKPrefs: $(OBJ)
    blink lib:rstartup.o $(OBJ) to $(PGM)UIKPrefs lib lib:debug.lib lib:uik.lib lib:amiga.lib lib:lc.lib SC ND
    protect $(PGM)UIKPrefs +p
    copy $(PGM)UIKPrefs $(DIST) clone

$(DIST)uikprefs.cd: uikprefs.cd
    copy uikprefs.cd $(DIST)

cat1: uikprefs.cd $(CT1)
    CatComp NOOPTIM uikprefs.cd $(CT1) CATALOG cat1
    copy cat1 LOCALE:catalogs/deutsch/uik/uikprefs.catalog
    copy cat1 $(DIST)catalogs/deutsch/uik/uikprefs.catalog

cat2: uikprefs.cd $(CT2)
    CatComp NOOPTIM uikprefs.cd $(CT2) CATALOG cat2
    copy cat2 LOCALE:catalogs/français/uik/uikprefs.catalog
    copy cat2 $(DIST)catalogs/français/uik/uikprefs.catalog

$(CATH): uikprefs.cd
    CatComp uikprefs.cd CFILE $(CATH)
