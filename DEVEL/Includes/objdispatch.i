        IFND    UIKDEFS_DISPATCH
UIKDEFS_DISPATCH SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**


    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC


 STRUCTURE  PrEntry,0
    APTR        upre_parm
    ULONG       upre_pri
    LABEL   PrEntry_SIZE


 STRUCTURE  Processing,0
    STRUCT      upr_PNode,LN_SIZE
    FPTR        upr_ProcFunc;
    APTR        upr_Parm;
    LABEL   Processing_SIZE


 STRUCTURE  UIKObjDispatch,0
    STRUCT      udpt_Obj,UIKOBJ_SIZE
    APTR        udpt_DOSBase
    APTR        udpt_UIKBase
    ULONG       udpt_SigBit
    APTR        udpt_EventHook
    FPTR        udpt_EventVect
    STRUCT      udpt_ProcList,LH_SIZE
    LABEL   UIKObjDispatch_SIZE


; fonctions publiques

OBJFUNC_Dispatch_AddProcessing  equ 0
OBJFUNC_Dispatch_RemProcessing  equ 4
OBJFUNC_Dispatch_LastCmd        equ 4


;---------Obj Dispatch Tags----------
UIKTAG_Dispatch_    equ (UIKTAG_USER|$0001)

;---------Obj Dispatch Flags---------

;--

        ENDC
