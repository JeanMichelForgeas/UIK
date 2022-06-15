        IFND    UIKDEFS_OBJLIST
UIKDEFS_OBJLIST SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**


    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC


 STRUCTURE  TextEntry,0
    UWORD       ute_Flags
    APTR        ute_String
    LABEL   TextEntry_SIZE


 STRUCTURE  UIKObjList,0
    STRUCT      uol_Obj,UIKOBJ_SIZE
    STRUCT      uol_Gad,gg_SIZEOF
    UWORD       uol_Vis
    UWORD       uol_Pos
    UWORD       uol_Max
    UWORD       uol_Sel
    UBYTE       uol_ListPen
    UBYTE       uol_SelectPen
    UBYTE       uol_SelectTextPen
    UBYTE       uol_SelectLen
    APTR        uol_FeedBack
    ULONG       uol_ListFlags
    STRUCT      uol_ListText,UIKLIST_SIZE
    FPTR        uol_SortFunc
    LABEL   UIKObjList_SIZE
    LABEL   UIKOBJLIST_SIZE


;----- Text List flags -----
UIKLB_JUSTIFY   equ     0
UIKLB_CENTER    equ     1
UIKLB_RIGHT     equ     2
UIKLB_CLICKSEL  equ     3
UIKLB_SELDISPL  equ     4
UIKLB_SORTED    equ     5
UIKLB_FIXED     equ     6

UIKL_LEFT       equ     $0000
UIKL_JUSTIFY    equ     $0001
UIKL_CENTER     equ     $0002
UIKL_RIGHT      equ     $0004
UIKL_CLICKSEL   equ     $0008
UIKL_SELDISPL   equ     $0010
UIKL_SORTED     equ     $0020
UIKL_FIXED      equ     $0040

;-----List Line flags----
UIKNB_IMAGEFIRST equ    0
UIKNB_IMAGEAFTER equ    1

UIKN_IMAGEFIRST equ     $0001
UIKN_IMAGEAFTER equ     $0002

UIKN_PRIVATEFLAGS   equ $00ff
UIKN_USERFLAGS      equ $ff00

;---------------------------------------------------UIKObjList
;----UIKObjList values Tags----
UIKTAG_OBJList_TextEntry    equ (UIKTAG_USER|$0001)
UIKTAG_OBJList_ListPen      equ (UIKTAG_USER|$0002)
UIKTAG_OBJList_SelectPen    equ (UIKTAG_USER|$0003)
UIKTAG_OBJList_SelectTextPen equ (UIKTAG_USER|$0004)
UIKTAG_OBJList_MaxLines     equ (UIKTAG_USER|$0005)
UIKTAG_OBJList_TopPosition  equ (UIKTAG_USER|$0006)
UIKTAG_OBJList_SelectedLine equ (UIKTAG_USER|$0007)
UIKTAG_OBJList_SortFunc     equ (UIKTAG_USER|$0008)
UIKTAG_OBJList_TextPtr      equ (UIKTAG_USER|$0009)

;----UIKObjList Flags Tags----
UIKTAG_OBJList_Flags        equ (UIKTAG_USER|$0020) ; les flags sont déjà prêts
 ; sinon un par un...
UIKTAG_OBJListFl_CenLeft    equ (UIKTAG_USER|$0021) ; TRUE
UIKTAG_OBJListFl_CenJustify equ (UIKTAG_USER|$0022) ; FALSE
UIKTAG_OBJListFl_CenCenter  equ (UIKTAG_USER|$0023) ; FALSE
UIKTAG_OBJListFl_CenRight   equ (UIKTAG_USER|$0024) ; FALSE
UIKTAG_OBJListFl_ClickSel   equ (UIKTAG_USER|$0025) ; FALSE
UIKTAG_OBJListFl_SelDispl   equ (UIKTAG_USER|$0026) ; FALSE
UIKTAG_OBJListFl_Sorted     equ (UIKTAG_USER|$0027) ; FALSE


        ENDC
