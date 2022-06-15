# Dans cet exemple, objwheelgrad.o contient le code d'un objet privé
# donc il est linké avec le programme de test colorwheel.o
#-------------------------------------------------------------------
colorwheel: colorwheel.c objwheelgrad.c
    LC -s -M -ccqstu -oRAM: -v colorwheel.c
    LC -s -M -ccqstu -oRAM: -v objwheelgrad.c
    BLink lib:c.o RAM:colorwheel.o RAM:objwheelgrad.o TO colorwheel LIB lib:uik.lib lib:lc.lib lib:amiga.lib SC SD ND
    Delete RAM:colorwheel.o RAM:objwheelgrad.o
