#------------- Macros

FLAGS = -HRAM:headers -s -ms -ccstqu -v -qRAM: -O -d
OBJET = UIK:Objects/
LFLAGS = lib lib:uik.lib lib:lc.lib lib:amiga.lib BUFSIZE 1024 SC SD ND
LFLAGS2 = lib lib:uik.lib lib:amiga.lib lib:lc.lib lib:debug.lib BUFSIZE 1024 SC SD ND
LANGUAGE = FRENCH
DEBUGF = -dDEBUG

# transformation rules
.c.o:
   lc $(FLAGS) -d$(LANGUAGE) $*

.asm.o:
   genam -L -O$*.o $*.asm

fake : $(OBJET)UIKObj_DisplayMode test

test: test.o
   BLink lib:c.o test.o TO test $(LFLAGS)

$(OBJET)UIKObj_DisplayMode: head.o DisplayMode.o DisplayModeStrings.o #Beta.o
   BLink head.o DisplayMode.o DisplayModeStrings.o to $(OBJET)UIKObj_DisplayMode $(LFLAGS2)
   FlushUIK
