        IFND    UIKDEFS_WFILEBUT

UIKDEFS_WFILEBUT  SET     1
**
**      Copyright (C) 1993 Jean-Michel Forgeas
**                  Tous Droits Réservés
**

    IFND UIKDEFS_WINFILESEL
    INCLUDE "UIKI:objwinfilesel.i"
    ENDC

    IFND UIKDEFS_OBJSTRING
    INCLUDE "UIKI:objstring.i"
    ENDC


 STRUCTURE  UIKObjWFileBut,0
    STRUCT      uowfb_Obj,UIKOBJ_SIZE
    APTR        uowfb_WFS
    APTR        uowfb_OS
    APTR        uowfb_OG
    APTR        uowfb_UIKBase
    APTR        uowfb_DOSBase
    ULONG       uowfb_Flags
    APTR        uowfb_Pattern
    ULONG       uowfb_SelTitle
    ULONG       uowfb_SelAltTitle
    FPTR        uowfb_OKFunc
    LABEL   UIKObjWFileBut_SIZE

WFB_STDWIDTH    equ 100
WFB_STDHEIGHT   equ  16

;---------WFileBut Flags----------
UIK_WFB_ASSIGNLIST      equ $00000002
UIK_WFB_RETURNRETURN    equ $00000004
UIK_WFB_OK_IFNOTEXISTS  equ $00000008
UIK_WFB_DIRSONLY        equ $00000010
UIK_WFB_SHOWICONS       equ $00000020
UIK_WFB_LEAVEPATH       equ $00000040
UIK_WFB_STRINGSTOP      equ $00000080

; Flags spéciaux
UIK_WFB_STRINGRIGHT     equ $01000000
UIK_WFB_AUTOHEIGHT      equ $02000000
UIK_WFB_BUTTONNOPUSH    equ $04000000


;---------WFileBut Tags----------
; Ces Tags doivent rester en correspondance avec ceux
; de l'objet FileSelector

UIKTAG_WFB_OKFunc       equ (UIKTAG_USER|$0001)
UIKTAG_WFB_InitPath     equ (UIKTAG_USER|$0003)
UIKTAG_WFB_AssignLst    equ (UIKTAG_USER|$0005)
UIKTAG_WFB_AssignList   equ (UIKTAG_USER|$0005)
UIKTAG_WFB_Pattern      equ (UIKTAG_USER|$0006)

UIKTAG_WFB_ExtraSpace           equ (UIKTAG_USER|$0200)
UIKTAG_WFB_ButtonWidth          equ (UIKTAG_USER|$0201)
UIKTAG_WFB_ButtonTitle          equ (UIKTAG_USER|$0202)
UIKTAG_WFB_ButtonAltTitle       equ (UIKTAG_USER|$0203)
UIKTAG_WFB_ButtonActInactPens   equ (UIKTAG_USER|$0204)
UIKTAG_WFB_ButtonShortCut       equ (UIKTAG_USER|$0205)
UIKTAG_WFB_SelectorTitle        equ (UIKTAG_USER|$0206)
UIKTAG_WFB_SelectorAltTitle     equ (UIKTAG_USER|$0207)


;----WFileBut Flags Tags----
; Ces flags doivent rester en correspondance avec ceux
; de l'objet FileSelector

UIKTAG_WFB_Flags            equ (UIKTAG_USER|$0020)
 ; sinon un par un...
UIKTAG_WFBFl_ReturnReturn   equ (UIKTAG_USER|$0021)
UIKTAG_WFBFl_OK_IfNotExists equ (UIKTAG_USER|$0022)
UIKTAG_WFBFl_DirsOnly       equ (UIKTAG_USER|$0023)
UIKTAG_WFBFl_ShowIcons      equ (UIKTAG_USER|$0024)
UIKTAG_WFBFl_StringsTop     equ (UIKTAG_USER|$0025)
UIKTAG_WFBFl_LeavePath      equ (UIKTAG_USER|$0026)

; Flags spéciaux
UIKTAG_WFBFl_StringRight    equ (UIKTAG_USER|$0100)   ; FALSE
UIKTAG_WFBFl_AutoHeight     equ (UIKTAG_USER|$0101)   ; TRUE
UIKTAG_WFBFl_ButtonImage    equ (UIKTAG_USER|$0102)
UIKTAG_WFBFl_ButtonNoPush   equ (UIKTAG_USER|$0103)


;--

        ENDC
