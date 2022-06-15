        IFND    UIKDEFS_WFONTBUT

UIKDEFS_WFONTBUT  SET     1
**
**      Copyright (C) 1993 Jean-Michel Forgeas
**                  Tous Droits Réservés
**

    IFND UIKDEFS_WINFONTSEL
    INCLUDE "UIKI:objwinfontsel.i"
    ENDC

    IFND UIKDEFS_OBJSTRING
    INCLUDE "UIKI:objstring.i"
    ENDC


 STRUCTURE  UIKObjWFontBut,0
    STRUCT      uowfob_Obj,UIKOBJ_SIZE
    APTR        uowfob_WFS
    APTR        uowfob_OS
    APTR        uowfob_OSH
    APTR        uowfob_OG
    APTR        uowfob_UIKBase
    APTR        uowfob_DOSBase
    APTR        uowfob_GfxBase
    APTR        uowfob_IntuitionBase
    ULONG       uowfob_Flags
    APTR        uowfob_FontPath
    STRUCT      uowfob_FontName,32
    STRUCT      uowfob_TTA,tta_SIZEOF
    ULONG       uowfob_SelTitle
    ULONG       uowfob_SelAltTitle
    FPTR        uowfob_OKFunc
    LABEL   UIKObjWFontBut_SIZE

WFOB_STDWIDTH   equ 100
WFOB_STDHEIGHT  equ  16

;---------WFontBut Flags----------
UIKF_WFOB_WPROP             equ $0001
UIKF_WFOB_WFIXED            equ $0002

; Flags spéciaux
UIK_WFOB_STRINGRIGHT        equ $01000000
UIK_WFOB_AUTOHEIGHT         equ $02000000
UIK_WFOB_BUTTONNOPUSH       equ $04000000


;---------WFontBut Tags----------
; Ces Tags doivent rester en correspondance avec ceux
; de l'objet FontSelector

UIKTAG_WFOB_OKFunc              equ (UIKTAG_USER|$0001)
UIKTAG_WFOB_FONTSPATH           equ (UIKTAG_USER|$0003)
UIKTAG_WFOB_HighlightFont       equ (UIKTAG_USER|$0004)

UIKTAG_WFOB_ExtraSpace          equ (UIKTAG_USER|$0200)
UIKTAG_WFOB_ButtonWidth         equ (UIKTAG_USER|$0201)
UIKTAG_WFOB_ButtonTitle         equ (UIKTAG_USER|$0202)
UIKTAG_WFOB_ButtonAltTitle      equ (UIKTAG_USER|$0203)
UIKTAG_WFOB_ButtonActInactPens  equ (UIKTAG_USER|$0204)
UIKTAG_WFOB_ButtonShortCut      equ (UIKTAG_USER|$0205)
UIKTAG_WFOB_SelectorTitle       equ (UIKTAG_USER|$0206)
UIKTAG_WFOB_SelectorAltTitle    equ (UIKTAG_USER|$0207)


;----WFontBut Flags Tags----
; Ces flags doivent rester en correspondance avec ceux
; de l'objet FontSelector

UIKTAG_WFOB_Flags               equ (UIKTAG_USER|$0020)
 ; sinon un par un...
UIKTAG_WFOBFl_PropWidth         equ (UIKTAG_USER|$0021)
UIKTAG_WFOBFl_FixedWidth        equ (UIKTAG_USER|$0022)

; Flags spéciaux
UIKTAG_WFOBFl_StringRight       equ (UIKTAG_USER|$0100)
UIKTAG_WFOBFl_AutoHeight        equ (UIKTAG_USER|$0101)
UIKTAG_WFOBFl_ButtonImage       equ (UIKTAG_USER|$0102)
UIKTAG_WFOBFl_ButtonNoPush      equ (UIKTAG_USER|$0103)


;--

        ENDC
