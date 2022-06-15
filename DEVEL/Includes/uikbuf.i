    IFND    UIKDEFS_BUFFER
UIKDEFS_BUFFER    SET 1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**

    IFND    EXEC_TYPES_I
    INCLUDE "exec/types.i"
    ENDC


 STRUCTURE  UIKBuffer,0
    ULONG       ubu_TotSize
    ULONG       ubu_CurSize
    ULONG       ubu_MemReqs
    UWORD       ubu_BlkSize
    UWORD       ubu_UserUWord
    ULONG       ubu_UserULong
    LABEL   ubu_SIZE
    LABEL   UIKBuffer_SIZE

 STRUCTURE  NegUIKBuffer,0
    ULONG       pad             ; pour ubu_UserULong
    UWORD       nubu_UserULong  ; !!! UWORD pour avoir le bon alignement
    UWORD       nubu_UserUWord
    ULONG       nubu_BlkSize    ; !!! ULONG pour avoir le bon alignement
    ULONG       nubu_MemReqs
    ULONG       nubu_CurSize
    ULONG       nubu_TotSize
    LABEL   nubu_SIZE
    LABEL   NegUIKBuffer_SIZE

 STRUCTURE  UIKPBuffer,0
    APTR        ubup_Pool
    ULONG       ubup_TotSize
    ULONG       ubup_CurSize
    ULONG       ubup_MemReqs
    UWORD       ubup_BlkSize
    UWORD       ubup_UserUWord
    ULONG       ubup_UserULong
    LABEL   ubup_SIZE
    LABEL   UIKPBuffer_SIZE

 STRUCTURE  NegUIKPBuffer,NegUIKBuffer_SIZE
    APTR        nubup_Pool
    LABEL   nubup_SIZE
    LABEL   NegUIKPBuffer_SIZE

UIKBUF      MACRO
\1          EQU     SOFFSET
SOFFSET     SET     SOFFSET+4
            ENDM


    ENDC
