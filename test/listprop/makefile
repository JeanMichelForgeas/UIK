listprop: listprop.c
    LC -s -ccqstu -oRAM: -v $*
    BLink lib:c.o RAM:listprop.o TO listprop LIB lib:uik.lib lib:lc.lib lib:amiga.lib SC SD ND
    Delete RAM:listprop.o
