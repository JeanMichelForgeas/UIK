disable: disable.c
    LC -s -ccqstu -oRAM: -v $*
    BLink lib:c.o RAM:disable.o TO disable LIB lib:uik.lib lib:lc.lib lib:amiga.lib SC SD ND
    Delete RAM:disable.o
