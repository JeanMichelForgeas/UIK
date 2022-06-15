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

$(PGM)UIKObj_WFontButton: head.o fontbut.o
    blink head.o fontbut.o to $(PGM)UIKObj_WFontButton lib lib:uik.lib lib:amiga.lib lib:lc.lib SC SD ND
    Copy $(PGM)UIKObj_WFontButton //Objects/ clone
    FlushUIK
