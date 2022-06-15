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

        IDNT    uikaddtaga


******* Includes ************************************************

        INCLUDE "uiki:uik_lib.i"


******* Imported ************************************************

        XREF    _UIKBase


******* Exported ************************************************

        XDEF    _UIK_AddObjectTags
        XDEF    _UIK_AddPrivateObjectTags
        XDEF    _UIK_CallObjectFuncTags
        XDEF    _BufPrintf


****************************************************************
*
*       Routines
*
****************************************************************/

        SECTION text,CODE


****************************************************************
* obj = UIK_AddObjectTags( objname, parent, tag, data, etc...)
*                                       pile
* obj = UIK_AddPrivateObjectTags( rg, parent, tag, data, etc...)
*                                       pile
* obj = UIK_CallObjectFuncTags( obj, func, parm, tag, data, etc...)
*                                       pile
****************************************************************

_UIK_AddObjectTags
    move.l  4(sp),a0
    move.l  8(sp),d0
    lea     12(sp),a1
    move.l  a6,-(sp)
    move.l  _UIKBase,a6
    jsr     _LVOUIK_AddObject(a6)
    move.l  (sp)+,a6
    rts

_UIK_AddPrivateObjectTags
    move.l  4(sp),a0
    move.l  8(sp),d0
    lea     12(sp),a1
    move.l  a6,-(sp)
    move.l  _UIKBase,a6
    jsr     _LVOUIK_AddPrivateObject(a6)
    move.l  (sp)+,a6
    rts

_UIK_CallObjectFuncTags
    move.l  4(sp),a1
    move.l  8(sp),d0
    move.l  12(sp),d1
    lea     16(sp),a0
    exg     d1,a0
    move.l  a6,-(sp)
    move.l  _UIKBase,a6
    jsr     _LVOUIK_CallObjectFunc(a6)
    move.l  (sp)+,a6
    rts

*******************************************************************************
*   UIKBUF *BufPrintf( UIKBUF **ubuf, ULONG start, UYTE *fmt, APTR *argv )
*                               a0          d0           a1         d1
*******************************************************************************

_BufPrintf
    move.l  4(sp),a0
    move.l  8(sp),d0
    move.l  12(sp),d1
    lea     16(sp),a1
    exg     d1,a1
    move.l  a6,-(sp)
    move.l  _UIKBase,a6
    jsr     _LVOVBufPrintf(a6)
    move.l  (sp)+,a6
    rts



    END
