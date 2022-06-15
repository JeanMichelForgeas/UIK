        IFND    UIKDEFS_FONTSELECT
UIKDEFS_FONTSELECT    SET     1
**
**      Copyright (C) 1991,1992,1993 Jean-Michel Forgeas
**                  Tous Droits Réservés
**


    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC

 STRUCTURE  FOSVars,0
        APTR        lo1
        APTR        po1
        APTR        bxo1
        APTR        tbo1
        APTR        bbo1
        APTR        upo1
        APTR        downo1
        APTR        to0
        APTR        lo2
        APTR        po2
        APTR        bxo2
        APTR        tbo2
        APTR        bbo2
        APTR        upo2
        APTR        downo2
        APTR        stro1
        APTR        stro2
        APTR        to1
        APTR        to2
        APTR        to3
        APTR        meo1
        APTR        meo2
        APTR        meo3
        APTR        bo
        APTR        oko
        APTR        co
    LABEL   FOSVars_SIZE
    LABEL   FOSVARS_SIZE

 STRUCTURE  UIKObjFontSelect,0
    STRUCT      ufos_Obj,UIKOBJ_SIZE

    APTR        ufos_DOSBase
    APTR        ufos_UIKBase
    APTR        ufos_GfxBase
    APTR        ufos_IntuitionBase
    APTR        ufos_LayersBase
    APTR        ufos_DiskfontBase

    STRUCT      ufos_NodeHead,UIKLIST_SIZE
    STRUCT      ufos_NameHead,UIKLIST_SIZE
    STRUCT      ufos_HeightHead,UIKLIST_SIZE
    STRUCT      ufos_DisplayHead,UIKLIST_SIZE
    ULONG       ufos_Scanned

    APTR        ufos_InsStr
    APTR        ufos_InsStr2
    ULONG       ufos_Index
    ULONG       ufos_Result
    APTR        ufos_FontsPath
    FPTR        ufos_OKFunc
    FPTR        ufos_CancelFunc

    STRUCT      ufos_ObjVars,FOSVars_SIZE
    UWORD       ufos_CurrentSize
    UBYTE       ufos_Style
    UBYTE       ufos_DispStatus
    APTR        ufos_Nil
    APTR        ufos_Com

    ULONG       ufos_FFlags
    UWORD       ufos_MinWidth
    UWORD       ufos_MinHeight
    APTR        ufos_ResizeHook
    APTR        ufos_ResizeWindow
    LABEL   UIKObjFontSelect_SIZE
    LABEL   UIKOBJFONTSELECT_SIZE


;--------DispStatus--------------------
NODISPLAY       equ 0
MONODISPLAY     equ 1
MULTIDISPLAY    equ 2

;---------Font Selector Flags----------
UIKF_FOS_WPROP  equ $0001
UIKF_FOS_WFIXED equ $0002

;---------Font Selector Bits----------
UIKB_FOS_WPROP  equ 0
UIKB_FOS_WFIXED equ 1

;---------Font Selector Tags----------
UIKTAG_FOS_OKFUNC        equ (UIKTAG_USER|$0001)
UIKTAG_FOS_CANCELFUNC    equ (UIKTAG_USER|$0002)
UIKTAG_FOS_FONTSPATH     equ (UIKTAG_USER|$0003)
UIKTAG_FOS_HighlightFont equ (UIKTAG_USER|$0004)

;----UIKObj Flags Tags----
UIKTAG_FOS_Flags         equ (UIKTAG_USER|$0020)
 ; sinon un par un...
UIKTAG_FOSFl_PropWidth   equ (UIKTAG_USER|$0021)
UIKTAG_FOSFl_FixedWidth  equ (UIKTAG_USER|$0022)

;--

        ENDC
