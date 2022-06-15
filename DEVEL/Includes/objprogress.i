        IFND    UIKDEFS_OBJPROGRESS
UIKDEFS_OBJPROGRESS SET     1
*****************************************************************************
*                                                                           *
* ObjProgress.i                                                             *
*                                                                           *
* Created 19/9/91 Copyright © 1992 Frédéric DENIS, The Software Laboratory  *
*                                                                           *
* All rights reserved.                                                      *
*                                                                           *
*****************************************************************************


    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC


 STRUCTURE  UIKObjProgress,0
    STRUCT      uopg_Obj,UIKOBJ_SIZE
    STRUCT      uopg_Gadget,gg_SIZEOF

    STRUCT      uopg_Image,ig_SIZEOF
    STRUCT      uopg_RastPort,rp_SIZEOF
    STRUCT      uopg_BitMap,bm_SIZEOF

    ULONG       uopg_Total
    ULONG       uopg_Pos
    ULONG       uopg_RenderMode

    UBYTE       uopg_DonePen
    UBYTE       uopg_ToDoPen

    APTR        uopg_IntuitionBase
    APTR        uopg_GfxBase
    APTR        uopg_UIKBase
    LABEL   UIKObjProgress_SIZE


OBJPROGRESS_BOX_STDWIDTH  equ 10
OBJPROGRESS_BOX_STDHEIGHT equ 10


;--------- Progress Object Render Modes -----

PROG_RenderFromLeft            equ 1
PROG_RenderFromTop             equ 2
PROG_RenderFromRight           equ 3
PROG_RenderFromBottom          equ 4
PROG_RenderFromUpperLeft       equ 5
PROG_RenderFromUpperRight      equ 6
PROG_RenderFromLowerLeft       equ 7
PROG_RenderFromLowerRight      equ 8


;-------- fonctions publiques -------

UIKFUNC_Progress_ChangeProgress equ 0
UIKFUNC_Progress_LastCmd        equ 0


;---------Obj Tags----------
UIKTAG_OBJProgress_Total        equ (UIKTAG_USER|$0001)
UIKTAG_OBJProgress_Pos          equ (UIKTAG_USER|$0002)
UIKTAG_OBJProgress_ToDoPen      equ (UIKTAG_USER|$0003)
UIKTAG_OBJProgress_DonePen      equ (UIKTAG_USER|$0004)
UIKTAG_OBJProgress_RenderMode   equ (UIKTAG_USER|$0005)

;---------Obj Flags---------

;--

        ENDC
