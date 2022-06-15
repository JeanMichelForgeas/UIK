    IFND    UIKDEFS_KEYSUPP
UIKDEFS_KEYSUPP  SET 1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**

    IFND    EXEC_TYPES_I
    INCLUDE "exec/types.i"
    ENDC

    IFND    EXEC_IO_I
    INCLUDE "exec/io.i"
    ENDC


UIK_KEYBUFLEN   equ     256

 STRUCTURE  UIKKeySupp,0
    STRUCT      uks_ConsoleMsg,IOSTD_SIZE
    ULONG       uks_Length
    STRUCT      uks_KeyBuffer,UIK_KEYBUFLEN
    LABEL   UIKKEYSUPP_SIZE
    LABEL   UIKKeySupp_SIZE


        ENDC
