ilbmtest: ilbmtest.c
    LC -s -ccqstu -oRAM: -v $*
    BLink lib:c.o RAM:ilbmtest.o TO ilbmtest LIB lib:uik.lib lib:lc.lib lib:amiga.lib SC SD ND
    Delete RAM:ilbmtest.o
