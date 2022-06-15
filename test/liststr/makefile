liststr: liststr.c
    LC -s -ccqstu -oRAM: -v $*
    BLink lib:c.o RAM:liststr.o TO liststr LIB lib:debug.lib lib:uik.lib lib:lc.lib lib:amiga.lib SC SD ND
    Delete RAM:liststr.o
