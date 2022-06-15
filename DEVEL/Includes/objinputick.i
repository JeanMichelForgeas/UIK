        IFND    UIKDEFS_INPUTICK
UIKDEFS_INPUTICK    SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**

    IFND UIKDEFS_OBJ
    INCLUDE "uiki:uikobj.i"
    ENDC

 STRUCTURE  UIKObjInputick,0
    STRUCT      uoit_Obj,UIKOBJ_SIZE
    ULONG       uoit_DOSBase
    ULONG       uoit_UIKBase
    FPTR        uoit_ClientFunc
    ULONG       uoit_EventHook
    FPTR        uoit_EventVect
    LABEL   uoipt_SIZE
    LABEL   UIKObjInputick_SIZE


* fonctions publiques

OBJFUNC_Inputick_       equ 0


*---------Obj Inputick Tags---------- *
UIKTAG_Joystick_EventFunc    equ (UIKTAG_USER|$0001)

*---------Obj Inputick Flags---------- *

*-- *

        ENDC
