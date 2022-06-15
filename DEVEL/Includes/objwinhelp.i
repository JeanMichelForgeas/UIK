        IFND    UIKDEFS_WINHELP
UIKDEFS_WINHELP SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**


    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC


 STRUCTURE  UIKObjWinHelp,0
    STRUCT      uowh_Obj,UIKOBJ_SIZE
    APTR        uowh_WO
    APTR        uowh_HO
    APTR        uowh_UIKBase
    LABEL   uowh_SIZE
    LABEL   UIKObjWinHelp_SIZE

WINHELP_BOX_STDWIDTH    equ 408
WINHELP_BOX_STDHEIGHT   equ 80

*---------Obj help Tags----------*
UIKTAG_WinHelp_WindowTitle  equ (UIKTAG_USER|$0001)      * titre de la fenêtre (UBYTE *)
UIKTAG_WinHelp_CloseWindowAction equ (UIKTAG_USER|$0004) * UIK_Stop() par defaut
UIKTAG_WinHelp_TextPtr      equ (UIKTAG_USER|$0005)

*---------Obj help Flags----------*


        ENDC
