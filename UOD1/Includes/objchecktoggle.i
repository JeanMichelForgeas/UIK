        IFND    UIKDEFS_OBJCHECK

UIKDEFS_OBJCHECK  SET     1
**
**      Copyright (C) 1993 Jean-Michel Forgeas
**                  Tous Droits Réservés
**

    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC


 STRUCTURE  UIKObjCheckToggle,0
    STRUCT      uochk_Obj,UIKOBJ_SIZE
    STRUCT      uochk_Gad,gg_SIZEOF
    APTR        uochk_IntuitionBase
    APTR        uochk_GfxBase
    APTR        uochk_UIKBase

    ULONG       uochk_Flags

    APTR        uochk_Image1
    APTR        uochk_AltImage1
    APTR        uochk_Image2
    APTR        uochk_AltImage2
    LABEL   UIKObjCheckToggle_SIZE


;-------Fonctions internes publiques------

UIKFUNC_Check_Select    equ 0
UIKFUNC_Check_Unselect  equ 4

UIKFUNC_Check_LastCmd   equ 4


;-----------------------------------------

OBJCHECK_BOX_STDWIDTH   equ 26
OBJCHECK_BOX_STDHEIGHT  equ 11

;----CheckFlags----
UIKF_CHECK_RADIO    equ $0001

;---------Obj Check Tags----------
UIKTAG_OBJCheck_Image1      equ (UIKTAG_USER|$0001)
UIKTAG_OBJCheck_AltImage1   equ (UIKTAG_USER|$0002)
UIKTAG_OBJCheck_Image2      equ (UIKTAG_USER|$0003)
UIKTAG_OBJCheck_AltImage2   equ (UIKTAG_USER|$0004)

;---------Obj Check Flags----------
UIKTAG_OBJCheck_Flags       equ (UIKTAG_USER|$0020)
UIKTAG_OBJCheckFl_Radio     equ (UIKTAG_USER|$0021)

;--

        ENDC
