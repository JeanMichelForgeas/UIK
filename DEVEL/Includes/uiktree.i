        IFND    UIKDEFS_UIKTREE
UIKDEFS_UIKTREE SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**


    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC


NOSTORE     equ -1

 STRUCTURE  UIKTreeItem,0
    WORD        tri_Level
    ULONG       tri_Var             ; no store if equal -1
    APTR        tri_ObjName
    APTR        tri_TagList
    FPTR        tri_Before          ; user function called before AddObject
    FPTR        tri_After           ; idem but called after Addobject
    APTR        tri_Vectors         ; ptr on array of events/vectors
    LABEL   tri_SIZE
    LABEL   UIKTreeItem_SIZE

 STRUCTURE  UIKHookItem,0       ; Entry
    FPTR        hi_Vector
    ULONG       hi_Events
    LABEL   hi_SIZE
    LABEL   UIKHookItem_SIZE

HOOKITEM MACRO  * fonction, évènement
        dc.l    \1
        dc.l    \2
        ENDM

OBJSTART MACRO
TLEVEL  SET 0
        ENDM

OBJECT  MACRO   * var, ObjName, TagList, 1st func, 2nd func, Events/Vector array
        dc.w    TLEVEL
        dc.l    \1
        dc.l    \2
        dc.l    \3
        dc.l    \4
        dc.l    \5
        dc.l    \6
        ENDM

CHILD   MACRO
TLEVEL  SET TLEVEL+1
        ENDM

CHILDEND MACRO
TLEVEL  SET TLEVEL-1
        ENDM

OBJEND  MACRO
        dc.w    -1
        ENDM


        ENDC
