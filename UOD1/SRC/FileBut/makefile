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

$(PGM)UIKObj_WFileButton: head.o filebut.o
    blink head.o filebut.o to $(PGM)UIKObj_WFileButton lib lib:uik.lib lib:amiga.lib lib:lc.lib SC SD ND
    Copy $(PGM)UIKObj_WFileButton //Objects/ clone
    FlushUIK
