    IFND    UIKDEFS_LIST
UIKDEFS_LIST    SET 1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**

    IFND    EXEC_TYPES_I
    INCLUDE "exec/types.i"
    ENDC


 STRUCTURE  UIKNode,0
    APTR        uikn_Buf
    UWORD       uikn_Reserved
    UWORD       uikn_Flags
    LABEL   UIKNODE_SIZE
    LABEL   UIKNode_SIZE

 STRUCTURE  UIKList,0
    ULONG       uikl_NodeNum
    ULONG       uikl_MaxNodes
    ULONG       uikl_BlockSize
    APTR        uikl_UNode
    LABEL   UIKLIST_SIZE
    LABEL   UIKList_SIZE

 STRUCTURE  UIKPList,0
    ULONG       uikpl_NodeNum
    ULONG       uikpl_MaxNodes
    ULONG       uikpl_BlockSize
    APTR        uikpl_UNode
    UWORD       uikpl_Flags
    UWORD       uikpl_Reserved1
    APTR        uikpl_Reserved2
    LABEL   UIKPList_SIZE

    BITDEF  UIKPL,POOLOWNER,0

LISTPTR MACRO
    moveq   #-1,d0
.loop\@
    cmp.l   uikl_BlockSize(a0),d0
    bne.b   .end\@
    move.l  uikl_UNode(a0),a0
    bra.b   .loop\@
.end\@
    ENDM

    ENDC
