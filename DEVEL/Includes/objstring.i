        IFND    UIKDEFS_OBJSTRING
UIKDEFS_OBJSTRING SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**


    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC


 STRUCTURE  UIKObjStrGad,0
    STRUCT      uos_Obj,UIKOBJ_SIZE
    STRUCT      uos_Gad,gg_SIZEOF
    UIKBUF      uos_Buffer
    UIKBUF      uos_UndoBuffer
    ULONG       uos_Reserved1
    ULONG       uos_Reserved2
    ULONG       uos_StrFlags
    ULONG       uos_NumChars
    ULONG       uos_CursorPos
    ULONG       uos_CharOffset
    ULONG       uos_YOffset
    UBYTE       uos_ActTextPen
    UBYTE       uos_InactTextPen
    UBYTE       uos_ActBgPen
    UBYTE       uos_InactBgPen
    ULONG       uos_UserHook
    ULONG       uos_LongIntResult
    ULONG       uos_MaxChars
    ULONG       uos_ActiveFunc
    ULONG       uos_InactiveFunc
    LABEL   UIKObjStrGad_SIZE

UIKST_ALLOCBLOCK equ    24


;----- StringGad flags -----
UIKSTB_JUSTIFY   equ    0
UIKSTB_CENTER    equ    1
UIKSTB_RIGHT     equ    2
UIKSTB_AUTOHEIGHT equ   3
UIKSTB_CURSORON   equ   4
UIKSTB_DIGITALINT equ   5
UIKSTB_DIGITALHEX equ   6
UIKSTB_ACTIVATE  equ    7
UIKSTB_NOFILTER  equ    8

UIKST_LEFT       equ    $0000
UIKST_JUSTIFY    equ    $0001
UIKST_CENTER     equ    $0002
UIKST_RIGHT      equ    $0004
UIKST_AUTOHEIGHT equ    $0008
UIKST_CURSORON   equ    $0010
UIKST_DIGITALINT equ    $0020
UIKST_DIGITALHEX equ    $0040
UIKST_ACTIVATE   equ    $0080
UIKST_NOFILTER   equ    $0100


;----UIKObjStr values Tags----
UIKTAG_OBJStr_CursorPos     equ (UIKTAG_USER|$0001)
UIKTAG_OBJStr_TextBuffer    equ (UIKTAG_USER|$0002)
UIKTAG_OBJStr_TextPens      equ (UIKTAG_USER|$0003)
UIKTAG_OBJStr_NumLines      equ (UIKTAG_USER|$0004)
UIKTAG_OBJStr_UserHandler   equ (UIKTAG_USER|$0005)
UIKTAG_OBJStr_ResultPointer equ (UIKTAG_USER|$0006)
UIKTAG_OBJStr_TextPointer   equ (UIKTAG_USER|$0007)
UIKTAG_OBJStr_MaxChars      equ (UIKTAG_USER|$0008)
UIKTAG_OBJStr_ActiveFunc    equ (UIKTAG_USER|$0009)
UIKTAG_OBJStr_InactiveFunc  equ (UIKTAG_USER|$000a)

;----UIKObjStr Flags Tags----
UIKTAG_OBJStr_Flags         equ (UIKTAG_USER|$0020) ; les flags sont déjà prêts
 ; sinon un par un...
UIKTAG_OBJStrFl_CenLeft     equ (UIKTAG_USER|$0021) ; TRUE
UIKTAG_OBJStrFl_CenJustify  equ (UIKTAG_USER|$0022) ; FALSE
UIKTAG_OBJStrFl_CenCenter   equ (UIKTAG_USER|$0023) ; FALSE
UIKTAG_OBJStrFl_CenRight    equ (UIKTAG_USER|$0024) ; FALSE
UIKTAG_OBJStrFl_AutoHeight  equ (UIKTAG_USER|$0025) ; TRUE
UIKTAG_OBJStrFl_DigitalInt  equ (UIKTAG_USER|$0026) ; FALSE
UIKTAG_OBJStrFl_DigitalHex  equ (UIKTAG_USER|$0027) ; FALSE
UIKTAG_OBJStrFl_Activate    equ (UIKTAG_USER|$0028) ; FALSE
UIKTAG_OBJStrFl_NoFilter    equ (UIKTAG_USER|$0029) ; FALSE


        ENDC
