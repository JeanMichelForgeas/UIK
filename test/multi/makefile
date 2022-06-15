
.c.o:
    LC -s -ccqstu -v $*

fake: datachip.c multi

datachip.c: br0 br1 br2 br3 br4 br5 br6 br7 br8 br9
    iff2uik >datachip.c -c -m br0 br1 br2 br3 br4 br5 br6 br7 br8 br9

multi: multi.o datachip.o
    BLink lib:c.o multi.o datachip.o TO multi LIB lib:uik.lib lib:lc.lib lib:amiga.lib SC SD ND
    delete multi.o datachip.o datachip.c
