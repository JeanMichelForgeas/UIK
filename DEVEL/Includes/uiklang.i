    IFND    UIKDEFS_LANGAGE
UIKDEFS_LANGAGE  SET 1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**

    IFND    EXEC_TYPES_I
    INCLUDE "exec/types.i"
    ENDC


 STRUCTURE  UIKLang,0
    APTR        ula_English
    APTR        ula_Other
    APTR        ula_Langage
    ULONG       ula_Flags
    APTR        ula_Master
    APTR        ula_CatName
    APTR        ula_Catalog
    LABEL   ula_SIZEOF
    LABEL   UIKLang_SIZE


UIKLANG_OWNER   equ $0001


        ENDC
