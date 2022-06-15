        IFND    UIKDEFS_OBJSCREEN
UIKDEFS_OBJSCREEN   SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**


    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC

    IFND UIKDEFS_COLORS
    INCLUDE "UIKI:uikcolors.i"
    ENDC


 STRUCTURE  UIKObjScreen,0
    STRUCT      uosc_Obj,UIKOBJ_SIZE
    STRUCT      uosc_NS,ns_SIZEOF
    ULONG       uosc_SFont
    STRUCT      uosc_ColorTable,MAX_UIKCOLORS*2
    ULONG       uosc_ModeId
    UWORD       uosc_TableColorNum
    STRUCT      uosc_Colors,MAX_UIKCOLORS

    APTR        uosc_IntuitionBase
    APTR        uosc_DOSBase
    APTR        uosc_GfxBase
    APTR        uosc_UIKBase

    APTR        uosc_Pens
    ULONG       uosc_Flags
    APTR        uosc_IntuiTags
    UBYTE       uosc_Mask
    UBYTE       uosc_padb
    UWORD       uosc_padw
    LABEL   uosc_SIZE
    LABEL   UIKObjScreen_SIZE


UIKSCRF_PUBLIC      equ $0001
UIKSCRF_MAXDEPTH    equ $0002


*---------Obj Screen Tags----------*
UIKTAG_ObjScreen_Depth          equ (UIKTAG_USER|$0001)
UIKTAG_ObjScreen_CustomBitMap   equ (UIKTAG_USER|$0002)
UIKTAG_ObjScreen_ViewModes      equ (UIKTAG_USER|$0003)
UIKTAG_ObjScreen_ColorTable     equ (UIKTAG_USER|$0004)
UIKTAG_OBJScreen_ShowTitle      equ (UIKTAG_USER|$0005)
UIKTAG_OBJScreen_TableColorNum  equ (UIKTAG_USER|$0006)
UIKTAG_OBJScreen_RedirectColors equ (UIKTAG_USER|$0007)
UIKTAG_OBJScreen_ScreenQuiet    equ (UIKTAG_USER|$0008)
UIKTAG_OBJScreen_IntuiPens      equ (UIKTAG_USER|$0009)
UIKTAG_OBJScreen_IntuiTags      equ (UIKTAG_USER|$000a)

*---------Obj Screen Flags----------*
UIKTAG_OBJScreenFl_Public       equ (UIKTAG_USER|$0021)
UIKTAG_OBJScreenFl_MaxDepth     equ (UIKTAG_USER|$0022)
UIKTAG_OBJScreenFl_AutoCenter   equ (UIKTAG_USER|$0023)


        ENDC
