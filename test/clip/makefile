# Exemple de programme réexécutable et réentrant: on peut mettre le bit p (pure)
# rstartup.o est fourni par Commodore pour faire du code 'pure'.
# ------------------------------------------------------------------------------
#attention ne pas mettre SMALLDATA !!! (a cause datas définies dans rstartup.o)

clipndraw: clipndraw.c
    LC -s -ccqstu -oRAM: -v $*
    BLink rstartup.o RAM:clipndraw.o TO clipndraw LIB lib:amiga.lib SC ND
    delete RAM:clipndraw.o
    protect clipndraw +p
