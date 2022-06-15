selectfile: selectfile.c
    LC -s -ccqstu -oRAM: -v $*
    BLink lib:c.o RAM:selectfile.o TO selectfile LIB lib:uik.lib lib:lc.lib lib:amiga.lib SC SD ND
    Delete RAM:selectfile.o
