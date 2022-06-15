
ip: ip.c lib:uikip.lib
    LC -s -ccqstu -oRAM: -v $*
    BLink lib:c.o RAM:ip.o TO ip LIB lib:uikip.lib lib:uik.lib lib:debug.lib lib:lc.lib lib:amiga.lib SC SD ND
    Delete RAM:ip.o
