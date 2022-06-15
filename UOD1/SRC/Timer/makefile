# MACROS
# C Compiler options
# SAS C 5.0 or higher

FLAGS = -Hram:headers -s -ccstqu -v -O

# transformation rules
.c.o:
    lc $(FLAGS) $*

.asm.o:
    genim2 -L -O$*.o -Iincludeg: $*.asm


PGM = UIK:Objects/

$(PGM)UIKObj_Timer: head.o objtimer.o
    blink head.o objtimer.o to $(PGM)UIKObj_Timer lib lib:uik.lib lib:amiga.lib lib:lc.lib SC SD ND
    Copy $(PGM)UIKObj_Timer //Objects/ clone
    FlushUIK
