# Exemples de programmes linkés avec astartup.o et amiga.lib en premier
# astartup.o est fourni par Commodore pour faire du code standard.
# -----------------------------------------------------------------------
8svx: 8svx.c
    LC -s -ccqstu -oRAM: -v $*
    BLink /astartup.o RAM:8svx.o TO 8svx LIB lib:uik.lib lib:amiga.lib lib:lcnb.lib SC SD ND
    Delete RAM:8svx.o
