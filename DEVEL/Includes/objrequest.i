        IFND    UIKDEFS_OBJREQUEST
UIKDEFS_OBJREQUEST SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**


    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC


 STRUCTURE  UIKObjReq,0
    STRUCT      uor_Obj,UIKOBJ_SIZE
    STRUCT      uor_Req,rq_SIZEOF
    ULONG       uor_ReqFlags
    APTR        uor_CustomBitMap
    APTR        uor_CustomAltBitMap
    WORD        uor_OldLeft
    WORD        uor_OldTop
    STRUCT      uor_ShortCutList,UIKLIST_SIZE
    LABEL   UIKObjReq_SIZE
    LABEL   UIKOBJREQ_SIZE


;------ req flags -------
UIKREQ_BITMAPED     equ $0001
UIKREQ_CENTERED     equ $0002

;------ req bits -------
UIKREQB_BITMAPED    equ 0
UIKREQB_CENTERED    equ 1


;----UIKObjReq values Tags----
UIKTAG_OBJReq_CustomBitMap      equ (UIKTAG_USER|$0001)
UIKTAG_OBJReq_CustomAltBitMap   equ (UIKTAG_USER|$0002)

;----UIKObjReq ReqFlags Tags----
UIKTAG_OBJReq_Flags             equ (UIKTAG_USER|$0020) ; les flags sont déjà prêts


        ENDC
