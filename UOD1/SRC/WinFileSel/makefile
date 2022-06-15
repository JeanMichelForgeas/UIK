# MACROS
# C Compiler options
# SAS C 5.0 or higher

FLAGS = -Hram:headers -s -ccstqu -v -O -d

# transformation rules
.c.o:
    lc $(FLAGS) $*

.asm.o:
    genim2 -L -O$*.o -Iincludeg: $*.asm


PGM = UIK:Objects/

$(PGM)UIKObj_WinFileSel: head.o winfilesel.o
    blink head.o winfilesel.o to $(PGM)UIKObj_WinFileSel lib lib:debug.lib lib:uik.lib lib:amiga.lib lib:lc.lib SC SD ND
    Copy $(PGM)UIKObj_WinFileSel //Objects/ clone
    FlushUIK
