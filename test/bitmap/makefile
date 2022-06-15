bitmap: bitmap.c
    LC -s -ccqstu -oRAM: -v $*
    BLink lib:c.o RAM:bitmap.o TO bitmap LIB lib:uik.lib lib:lc.lib lib:amiga.lib SC SD ND
    Delete RAM:bitmap.o
