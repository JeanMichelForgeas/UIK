# Exemples de programmes linkés avec astartup.o et amiga.lib en premier
# astartup.o est fourni par Commodore pour faire du code standard.
# -----------------------------------------------------------------------
filebutton: filebutton.c
    LC -s -ccqstu -oRAM: -v $*
    BLink /astartup.o RAM:filebutton.o TO filebutton LIB lib:debug.lib lib:uik.lib lib:amiga.lib lib:lcnb.lib SC SD ND
    Delete RAM:filebutton.o
