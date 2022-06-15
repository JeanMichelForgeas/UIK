events: events.c
    LC -s -ccqstu -oRAM: -v $*
    BLink lib:c.o RAM:events.o TO events LIB lib:uik.lib lib:lc.lib lib:amiga.lib SC SD ND
    Delete RAM:events.o
