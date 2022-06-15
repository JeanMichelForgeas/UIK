# Exemples de programmes linkés avec astartup.o et amiga.lib en premier
# astartup.o est fourni par Commodore pour faire du code standard.
# -----------------------------------------------------------------------
filebutton: check.c
    LC -s -ccqstu -oRAM: -v $*
    BLink /astartup.o RAM:check.o TO check LIB lib:debug.lib lib:uik.lib lib:amiga.lib lib:lcnb.lib SC SD ND
    Delete RAM:check.o
