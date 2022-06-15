
buffer: buffer.c
    LC -s -ccqstu -oRAM: -v $*
    BLink lib:c.o RAM:buffer.o TO buffer LIB lib:uik.lib lib:lc.lib lib:amiga.lib SC SD ND
    Delete RAM:buffer.o
