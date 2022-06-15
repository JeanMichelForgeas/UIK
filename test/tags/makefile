testag: testag.c
    LC -s -ccqstu -d -oRAM: -v $*
    BLink lib:c.o RAM:testag.o TO testag LIB lib:uik.lib lib:lc.lib lib:amiga.lib SC SD ND
    Delete RAM:testag.o
