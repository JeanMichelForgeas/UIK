        IFND    UIKDEFS_MACRO
UIKDEFS_MACRO   SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**

    IFND EXEC_TYPES_I
    INCLUDE "exec/types.i"
    ENDC


CALLUIK MACRO   * function
        JSR     _LVOUIK_\1(a6)
        ENDM

TAGITEM MACRO   * Tag, Data
        dc.l    \1,\2
        ENDM

SETL    MACRO   * const, 1st word, 2nd word
\1      SET     (\2<<16)|\3
        ENDM

W2L     MACRO   * const, 1st word, 2nd word
\1      SET     (\2<<16)|\3
        ENDM

B2L     MACRO   * const, 1st byte, 2nd byte, 3rd byte, 4th byte
\1      SET     (\2<<24)|(\3<<16)|(\4<<8)|\5
        ENDM

UIKCOLS MACRO   * const, 1st color, 2nd color, 3rd color, 4th color
\1      SET     (\2<<24)|(\3<<16)|(\4<<8)|\5
        ENDM

ALLOC   MACRO   * size
        move.l  #\1,d0
        move.l  #MEMF_PUBLIC|MEMF_CLEAR,d1
        movea.l 4.w,a6
        jsr     _LVOAllocMem(a6)
        ENDM

* Commodore DBUG macro (needs debug.lib)
* Usage: pass name of format string,value,value
*        values may be register name, variablename(PC), or #value
* ex: DBUG fmtRtnD0,#testname,d0  ; format string, addr of rtn name, d0
DBUG    MACRO   * passed name of format string, with args on stack
        movem.l d0-d7/a0-a6,-(sp)
        move.l  \3,-(sp)
        move.l  \2,-(sp)
        lea     .fmt\@(pc),a0
        lea     (sp),a1
        XREF    KPrintF
        jsr     KPrintF
        addq.w  #8,sp
        movem.l (sp)+,d0-d7/a0-a6
        bra.b   .\@
.fmt\@  dc.b    \1,0
        cnop    0,4
.\@
        ENDM


        ENDC
