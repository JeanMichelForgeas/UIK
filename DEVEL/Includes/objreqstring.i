        IFND    UIKDEFS_REQSTRING
UIKDEFS_REQSTRING   SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**


    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC


 STRUCTURE  UIKObjReqString,0
    STRUCT      uors_Obj,UIKOBJ_SIZE
    APTR        uors_Req      ; peut être un UIKObjRequest ou un UIKObjWindow
    APTR        uors_Str
    APTR        uors_KeyHook
    FPTR        uors_OKFunc
    FPTR        uors_CancelFunc
    FPTR        uors_MiddleFunc
    APTR        uors_Str2
    APTR        uors_KeyHook2
    APTR        uors_Text
    ULONG       uors_Flags
    LABEL   uors_SIZE
    LABEL   UIKObjReqString_SIZE

*---------Obj ReqString Flags---------*
ORSF_WINDOW     equ $0001
ORSF_ACTIVATE   equ $0002
ORSF_RETQUIET   equ $0004

*---------Obj ReqString Tags----------*
UIKTAG_ReqString_OKFunc         equ (UIKTAG_USER|$0001)
UIKTAG_ReqString_CancelFunc     equ (UIKTAG_USER|$0002)
UIKTAG_ReqString_StringPtr      equ (UIKTAG_USER|$0005)
UIKTAG_ReqString_TextPtr        equ (UIKTAG_USER|$0006)
UIKTAG_ReqString_StringTagList  equ (UIKTAG_USER|$0007)
UIKTAG_ReqString_OKTitle        equ (UIKTAG_USER|$0008)
UIKTAG_ReqString_CancelTitle    equ (UIKTAG_USER|$0009)
UIKTAG_ReqString_MiddleTitle    equ (UIKTAG_USER|$000a)
UIKTAG_ReqString_MiddleFunc     equ (UIKTAG_USER|$000b)
UIKTAG_ReqString_StringPtr2     equ (UIKTAG_USER|$000c)
UIKTAG_ReqString_StringTagList2 equ (UIKTAG_USER|$000d)
UIKTAG_ReqString_TextListTagList equ (UIKTAG_USER|$000e)
UIKTAG_ReqString_OKTitlePtr     equ (UIKTAG_USER|$000f)
UIKTAG_ReqString_CancelTitlePtr equ (UIKTAG_USER|$0010)
UIKTAG_ReqString_MiddleTitlePtr equ (UIKTAG_USER|$0011)
UIKTAG_ReqString_CloseFunc      equ (UIKTAG_USER|$0012)

*---------Obj ReqString Tag Flags---------*
UIKTAG_ReqStringFl_Window       equ (UIKTAG_USER|$0041)
UIKTAG_ReqStringFl_Activate     equ (UIKTAG_USER|$0042)
UIKTAG_ReqStringFl_StringQuiet  equ (UIKTAG_USER|$0043)


        ENDC
