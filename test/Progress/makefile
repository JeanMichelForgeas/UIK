#------------- Macros

FLAGS = -Hram:headers -s -ccstqu -v -O
LFLAGS = lib lib:lc.lib lib:amiga.lib BUFSIZE 1024 SC ND VERBOSE

# transformation rules
.c.o:
   lc $(FLAGS) $*

.asm.o:
   genim2 -L -O$*.o $*.asm

progress: progress.o
   BLink lib:c.o progress.o TO progress $(LFLAGS) lib lib:uik.lib lib:lc.lib lib:amiga.lib
   delete progress.o
