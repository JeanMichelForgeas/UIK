    OPT O+,W-
    OPT i+                  ; Tells when '#' is probably missing
        OPT d+
****************************************************************
*
*       Project:    UIK
*       Function:   version pile de UIK_AddObject
*
*       Created:    15/01/92    Jean-Michel Forgeas
*
****************************************************************

        IDNT    uikaddtagb


******* Includes ************************************************

        INCLUDE "uiki:uik_lib.i"
        INCLUDE "uiki:uikmacros.i"


******* Imported ************************************************


******* Exported ************************************************

        XDEF    _UIK_AddObjectTagsB
        XDEF    _UIK_AddPrivateObjectTagsB
        XDEF    _UIK_CallObjectFuncTagsB
        XDEF    _BufPrintfB


****************************************************************
*
*       Routines
*
****************************************************************/

        SECTION text,CODE


****************************************************************
* obj = UIK_AddObjectTagsB( uikbase, objname, parent, tag, data, etc...)
*                                       pile
* obj = UIK_AddPrivateObjectTagsB( uikbase, rg, parent, tag, data, etc...)
*                                       pile
* obj = UIK_CallObjectFuncTagsB( uikbase, obj, func, parm, tag, data, etc...)
*                                       pile
****************************************************************

_UIK_AddObjectTagsB
    move.l  a6,-(sp)
    move.l  8(sp),a6
    move.l  12(sp),a0
    move.l  16(sp),d0
    lea     20(sp),a1
    ;DBUG    <'%lx %lx ',0>,a6,a0
    ;DBUG    <'%lx %lx',10,0>,d0,a1
    jsr     _LVOUIK_AddObject(a6)
    move.l  (sp)+,a6
    rts

_UIK_AddPrivateObjectTagsB
    move.l  a6,-(sp)
    move.l  8(sp),a6
    move.l  12(sp),a0
    move.l  16(sp),d0
    lea     20(sp),a1
    jsr     _LVOUIK_AddPrivateObject(a6)
    move.l  (sp)+,a6
    rts

_UIK_CallObjectFuncTagsB
    move.l  a6,-(sp)
    move.l  8(sp),a6
    move.l  12(sp),a1
    move.l  16(sp),d0
    move.l  20(sp),d1
    lea     24(sp),a0
    exg     d1,a0
    jsr     _LVOUIK_CallObjectFunc(a6)
    move.l  (sp)+,a6
    rts

********************************************************************************
*   UIKBUF *BufPrintfB( uikbase, UIKBUF **ubuf, ULONG start, UYTE *fmt, APTR *argv )
*
********************************************************************************

_BufPrintfB
    move.l  a6,-(sp)
    move.l  8(sp),a6
    move.l  12(sp),a0
    move.l  16(sp),d0
    move.l  20(sp),d1
    lea     24(sp),a1
    exg     d1,a1
    jsr     _LVOVBufPrintf(a6)
    move.l  (sp)+,a6
    rts


    END
