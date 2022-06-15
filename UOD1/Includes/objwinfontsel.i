        IFND    UIKDEFS_WINFONTSEL

UIKDEFS_WINFONTSEL  SET     1
**
**      Copyright (C) 1993 Jean-Michel Forgeas
**                  Tous Droits Réservés
**

    IFND UIKDEFS_OBJWINDOW
    INCLUDE "UIKI:objwindow.i"
    ENDC

    IFND UIKDEFS_FONTSELECT
    INCLUDE "UIKI:objfontselector.i"
    ENDC


 STRUCTURE  WinFontSelPrefs,0
    STRUCT      wfosp_Box,UIKBox_SIZE
    STRUCT      wfosp_Reserved,4*4
    LABEL   WinFontSelPrefs_SIZE


 STRUCTURE  UIKObjWinFontSel,0
    STRUCT      uowfos_Obj,UIKOBJ_SIZE
    APTR        uowfos_WO
    APTR        uowfos_FS
    APTR        uowfos_UIKBase
    APTR        uowfos_DOSBase
    STRUCT      uowfos_FIB,fib_SIZEOF
    ULONG       uowfos_Flags
    FPTR        uowfos_okfunc
    FPTR        uowfos_cancelfunc
    APTR        uowfos_FontPath,
    STRUCT      uowfos_HighLightTTA,tta_SIZEOF
    STRUCT      uowfos_Prefs,WinFontSelPrefs_SIZE
    STRUCT      uowfos_OldPrefs,WinFontSelPrefs_SIZE
    LABEL   UIKObjWinFontSel_SIZE


WINFOS_STDWIDTH  equ (6+300+6)
WINFOS_STDHEIGHT equ (3+120+3)


;---------WinFontSel Flags----------
UIKF_WFOS_WPROP  equ $0001
UIKF_WFOS_WFIXED equ $0002

;---------WinFontSel Tags----------
; Ces Tags doivent rester en correspondance avec ceux
; de l'objet FontSelector

UIKTAG_WFOS_OKFUNC          equ (UIKTAG_USER|$0001)
UIKTAG_WFOS_CANCELFUNC      equ (UIKTAG_USER|$0002)
UIKTAG_WFOS_FONTSPATH       equ (UIKTAG_USER|$0003)
UIKTAG_WFOS_HighlightFont   equ (UIKTAG_USER|$0004)


;----UIKObj Flags Tags----
; Ces flags doivent rester en correspondance avec ceux
; de l'objet FontSelector

UIKTAG_WFOS_Flags           equ (UIKTAG_USER|$0020)
 ; sinon un par un...
UIKTAG_WFOSFl_PropWidth     equ (UIKTAG_USER|$0021)
UIKTAG_WFOSFl_FixedWidth    equ (UIKTAG_USER|$0022)


;--

        ENDC
