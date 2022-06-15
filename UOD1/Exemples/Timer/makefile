timer: timer.c
    LC -s -ccqstu -oRAM: -v $*
    BLink lib:c.o RAM:timer.o TO timer LIB lib:debug.lib lib:uik.lib lib:lc.lib lib:amiga.lib SC SD ND
    Delete RAM:timer.o
