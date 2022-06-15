
buffers: buffers.c
    LC -s -ccqstu -oRAM: -v $*
    BLink lib:c.o RAM:buffers.o TO buffers LIB lib:uik.lib lib:lc.lib lib:amiga.lib SC SD ND
    Delete RAM:buffers.o
