#------------- Macros

FLAGS = -Hram:headers -s -ccstqu -v -d -O
OBJET = UIK:Objects/
LFLAGS = lib lib:uik.lib lib:amiga.lib lib:lc.lib BUFSIZE 1024 SC ND
PGM = UIK:Objects/

# transformation rules
.c.o:
   lc $(FLAGS) $*

.asm.o:
   genim2 -L -O$*.o $*.asm

fake : datachip.c $(OBJET)UIKObj_CheckToggle $(OBJET)UIKObj_RadioExclude

datachip.c: PicCheck PicImage1 PicAltImage1 PicImage2 PicAltImage2
    iff2uik >datachip.c PicCheck PicImage1 PicAltImage1 PicImage2 PicAltImage2

$(OBJET)UIKObj_CheckToggle: headtoggle.o toggle.o datachip.o
    BLink headtoggle.o toggle.o datachip.o to $(OBJET)UIKObj_CheckToggle $(LFLAGS)
    Copy $(PGM)UIKObj_CheckToggle //Objects/ clone
    FlushUIK

$(OBJET)UIKObj_RadioExclude: headexclude.o exclude.o datachip.o
    BLink headexclude.o exclude.o datachip.o to $(OBJET)UIKObj_RadioExclude $(LFLAGS)
    Copy $(PGM)UIKObj_RadioExclude //Objects/ clone
    FlushUIK
