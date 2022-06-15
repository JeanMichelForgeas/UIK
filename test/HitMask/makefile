hitmask: hitmask.c datachip.c
    LC -s -ccqstu -oRAM: -v hitmask
    LC -s -ccqstu -oRAM: -v datachip
    BLink lib:c.o RAM:hitmask.o RAM:datachip.o TO hitmask LIB lib:uik.lib lib:lc.lib lib:amiga.lib SC SD ND
    Delete RAM:hitmask.o RAM:datachip.o
