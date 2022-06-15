        IFND    UIKDEFS_HELP
UIKDEFS_HELP    SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**


    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC


 STRUCTURE  UIKObjHelp,0
    STRUCT      uoh_Obj,UIKOBJ_SIZE
    APTR        uoh_ObjList
    APTR        uoh_KeyHook
    APTR        uoh_UIKBase
    LABEL   uoh_SIZE
    LABEL   UIKObjHelp_SIZE

HELP_BOX_STDWIDTH   equ 408
HELP_BOX_STDHEIGHT  equ 80

*---------Obj help Tags----------*
UIKTAG_Help_TextPtr         equ (UIKTAG_USER|$0003)

*---------Obj help Flags---------*


        ENDC
