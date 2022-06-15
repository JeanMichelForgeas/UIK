.c.o:
    lc $(FLAGS) -ccqstu -Hram:headers $*

.asm.o:
    genim2 $*.asm -L -O$*.o -Iincludeg:


uiktest: uiktest.c
    LC -s -cqstu -oRAM: -v $*
    BLink lib:c.o RAM:uiktest.o TO uiktest LIB lib:debug.lib lib:uik.lib lib:lc.lib lib:amiga.lib SC SD ND
    Delete RAM:uiktest.o

smalltest: smalltest.o
    blink lib:c.o smalltest.o to smalltest LIB lib:uik.lib lib:lcnb.lib lib:amiga.lib NODEBUG SMALLCODE SMALLDATA

testinc: testinc.o testinca.o
    blink lib:c.o testinc.o testinca.o to testinc LIB lib:lcnb.lib lib:amiga.lib NODEBUG SMALLCODE SMALLDATA
