        OPT O+,W-
        OPT d+
*******************************************
*
*       Project:    UIK
*       Function:   place UIK icons
*
*       Created:    24/07/90    Jean-Michel Forgeas
*
****************************************************************

        IDNT    uikicona


******* Includes ************************************************

        INCDIR  "include:"

        INCLUDE "lib/intuition_lib.i"
        INCLUDE "uiki:uikbase.i"
        INCLUDE "uiki:uikobj.i"
        INCLUDE "uiki:uikglobal.i"
        INCLUDE "uiki:objwindow.i"


******* Imported ************************************************

        XREF    _IntuitionBase

        XREF    _UIK_IsSystemVersion2
        XREF    _UIK_MemCopy


******* Exported ************************************************

        XDEF    _UIK_AskIconPlace


****************************************************************
*
        SECTION text,CODE
*
****************************************************************


***********************************************
*   UIK_AskIconPlace( wo, pltop, icwidth )
*                     a0  a1     d0:16
***********************************************
_UIK_AskIconPlace
    move.l  uow_IconLeftTop(a0),d1
    tst.w   d1
    beq.b   .ask
    move.l  d1,(a1)
    rts
.ask
    movem.l  d2-d5/a2-a3/a5-a6,-(sp)
    move.l  a0,a2                   a2 = object window
    move.l  a1,a3                   a3 = pointeur on left/top
    move.w  d0,d2                   d2 = icon box width

    moveq   #0,d0
    move.l  _IntuitionBase,a6
    jsr     _LVOLockIBase(a6)
    move.l  d0,d3                   d3 when UnlockIBase

    sub.w   #1000,sp                a7 = start buffer
    move.l  uo_w(a2),a0
    move.l  wd_WScreen(a0),a0       a0 = screen of this window
    move.l  sc_FirstWindow(a0),a5   a5 = first window of the screen
    moveq   #0,d5                   d5 = total windows

.windowloop     ;---------------------- build a sorted list of UIK icons
    move.l  a5,d0
    beq.b   .placenew

        move.w  wd_TopEdge(a5),d0
        bne.b   .nextwindow
        move.l  wd_UserData(a5),d0      -------------------- d0 = object ?
        beq.b   .nextwindow             si 0: pas une adresse d'objet
        btst    #0,d0
        bne.b   .nextwindow             si 1: adresse impaire
        move.l  d0,a0                   object ok
        move.l  uo_UIK(a0),d0
        beq.b   .nextwindow             si 0: pas l'adresse de uik
        btst    #0,d0
        bne.b   .nextwindow             si 1: adresse impaire
        exg     d0,a0                   ok: uik
        cmp.l   uo_UIK(a0),a0           if (obj->UIK != obj->UIK->Obj.UIK)
        bne.b   .nextwindow             pas un objet uik
        exg     d0,a0                   a0 = obj
        btst    #UIKWB_ICONWINDOW,uow_WinFlags+3(a0) icon window ?
        beq.b   .nextwindow
                                        ;-------------------- ok: window uik
        move.w  wd_Width(a5),d4
        swap    d4
        move.w  wd_LeftEdge(a5),d4      d4 = width:left
        move.l  sp,a1
        move.w  d5,d0                   d0 = counter = totalwindows
        bra.b   .nextsort
.sortloop
            move.l  (a1),d1
            cmp.w   d4,d1
            bcs.b   .endsortloop
            addq.l  #4,a1
.nextsort
        dbra    d0,.sortloop

.endsortloop
        move.l  a1,a0
        addq.l  #4,a1
        move.l  a0,a6                   a6 = where to insert
        move.l  #500,d0
        bsr     _UIK_MemCopy
        move.l  d4,(a6)
        addq.w  #1,d5                   totalwindows++

.nextwindow
    move.l  wd_NextWindow(a5),a5
    bra.b   .windowloop


.placenew   ;---------------------- place the new window
    moveq   #$35,d4
    bsr     _UIK_IsSystemVersion2
    beq.b   .10

    moveq   #0,d4
    move.l  uo_w(a2),a0
    move.l  wd_WScreen(a0),a0       a0 = screen of this window
    lea     sc_FirstGadget(a0),a1
.gadloop
    move.l  (a1),d0
    beq.b   .10
    move.l  d0,a1
    move.w  gg_GadgetType(a1),d0
    and.w   #GTYP_SDEPTH,d0
    beq.b   .gadloop
    move.w  gg_Width(a1),d4
.10
    move.l  uo_w(a2),a0
    move.l  wd_WScreen(a0),a0       a0 = screen of this window
    sub.w   sc_Width(a0),d4
    neg.w   d4                      d4 = right limit + 1

    move.l  sp,a1
    bra.b   .nextfind               d5 = counter = totalwindows
.findloop
        move.l  (a1),d0             d0.w = left
        add.w   (a1),d0             d0.w = left + width = max window x + 1

        move.w  d4,d1
        sub.w   d2,d1               d1 = icon leftedge

        cmp.w   d0,d1
        bcc.b   .endfindloop

        move.w  2(a1),d4
        addq.l  #4,a1               right limit = left edge of this window
.nextfind
    dbra    d5,.findloop

.endfindloop
    add.w   #1000,sp                restore old stack pointer

    sub.w   d2,d4
    bcc.b   .sw
    move.w  #0,d4
.sw swap    d4                      d4 = LeftEdge
    move.w  #0,d4                   TopEdge = 0
    move.l  d4,(a3)
    move.l  d4,uow_IconLeftTop(a2)

    move.l  d3,a0
    move.l  _IntuitionBase,a6
    jsr     _LVOUnlockIBase(a6)
    movem.l (sp)+,d2-d5/a2-a3/a5-a6
    ext.l   d0
    rts


    END
