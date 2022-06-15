        IFND    UIKDEFS_OBJMEMORY
UIKDEFS_OBJMEMORY SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**


    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC


 STRUCTURE  UIKObjMem,0
    STRUCT      uom_Obj,UIKOBJ_SIZE
    APTR        uom_MemPtr
    FPTR        uom_CleanupFunc
    LABEL   UIKObjMem_SIZE


;----UIKObjMem values Tags----
UIKTAG_OBJMem_MemSize       equ (UIKTAG_USER|$0001)
UIKTAG_OBJMem_MemType       equ (UIKTAG_USER|$0002)
UIKTAG_OBJMem_UserCleanup   equ (UIKTAG_USER|$0003)


        ENDC
