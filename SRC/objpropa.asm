    OPT O+,W-
        OPT d+
****************************************************************
*
*       Project:    UIK
*       Function:   prop gadget
*
*       Created:    17/09/90    Jean-Michel Forgeas
*
****************************************************************

        IDNT    objpropa


******* Includes ************************************************

        INCDIR  "include:"

        INCLUDE "intuition/intuition.i"
        INCLUDE "uiki:uikobj.i"
        INCLUDE "uiki:objproplift.i"
        INCLUDE "uiki:uikbox.i"
        INCLUDE "uiki:uikmacros.i"


******* Imported ************************************************

        XREF    LockObjVars
        XREF    UnlockObjVars
        XREF    _UIK_LockObjVars
        XREF    _UIK_UnlockObjVars
        XREF    _IntUIK_SetPropVars
        XREF    _IntUIK_SetPropPos
        XREF    _IntUIK_DrawKnob


******* Exported ************************************************

        ;XDEF    _UIK_OPr_ChangePropGadget
        ;XDEF    _UIK_OPr_ChangePropNoDraw

        XDEF    _HdlUIK_InactivePropGad
        XDEF    _HdlUIK_ActivePropGad
        XDEF    _HdlUIK_MovePropGad


        SECTION text,CODE


*************************************
*   HdlUIK_InactivePropGad()
*   HdlUIK_ActivePropGad()
*   HdlUIK_MovePropGad()
*   installées par AddPropGadget()
*   appelée par do_gadget
*   donc a3=message a1=objet
*************************************

_HdlUIK_InactivePropGad
    bsr     LockObjVars
    pea     UnlockObjVars(pc)
    move.l  a1,-(sp)
    bsr     _IntUIK_SetPropVars
    move.l  (sp),a1
    cmp.w   #-1,uop_MouseStart(a1)
    beq.b   .end
    move.l  (sp),a1
    bsr     _IntUIK_DrawKnob
    clr.l   uop_SaveKnob(a1)
    clr.l   uop_SaveKnob+4(a1)
    move.l  uop_KnobNorAutomate(a1),uop_KnobAutomate(a1)
    bsr     _IntUIK_DrawKnob
.end
    move.l  (sp)+,a1
    move.b  #0,uop_Moving(a1)
    rts


_HdlUIK_ActivePropGad
    ;cmp.w   #-1,uop_VisPC(a1)
    ;beq     PropCancel
    bsr     LockObjVars
    pea     UnlockObjVars(pc)

    moveq   #0,d1
    move.l  uo_rq(a1),d0                d0 = requester
    beq.b   .win                        si pas de requester
    exg.l   d0,a1                       indicateurs non affectes
    move.l  rq_LeftEdge(a1),d1          offset du requester
    exg.l   d0,a1                       indicateurs non affectes

.win
    tst.b   uop_Ver(a1)
    bne.b   .vert
.horiz
    move.w  im_MouseX(a3),a0
    move.w  uo_Box+box_Left(a1),d0
    swap    d1
    bra.b   .both
.vert
    move.w  im_MouseY(a3),a0
    move.w  uo_Box+box_Top(a1),d0
.both
    add.w   d1,d0                   ajoute abscisse ou ordonnée du requester
    move.w  a0,d1
    sub.w   d0,d1
    subq.w  #1,d1                   offset dans le container
    moveq   #0,d0
    sub.w   uop_KnobOffs(a1),d1     offset dans knob
    bcs.b   .inf
    sub.w   uop_KnobWidth(a1),d1    apres le knob ?
    bcc.s   .sup
.active
    move.b  #1,uop_Moving(a1)       dans le knob...

    ;------ calcul de l'offset de départ
    move.l  uo_ObjExt(a1),a0
    tst.b   uop_Ver(a1)
    bne.b   .vert2
.horiz2
    move.w  im_MouseX(a3),d1        calcul de l'offset entre le pointeur
    sub.w   uop_KnobOffs(a1),d1                 et le début du knob
    sub.w   uoe_InnerRect+ra_MinX(a0),d1
    bra.s   .both2
.vert2
    move.w  im_MouseY(a3),d1        calcul de l'offset entre le pointeur
    sub.w   uop_KnobOffs(a1),d1                 et le début du knob
    sub.w   uoe_InnerRect+ra_MinY(a0),d1
.both2
    move.w  d1,uop_MouseStart(a1)   MouseStart == offset

    ;------ redessine le knob
    move.l  a1,-(sp)
    clr.l   uop_SaveKnob(a1)
    clr.l   uop_SaveKnob+4(a1)
    move.l  uop_KnobAltAutomate(a1),uop_KnobAutomate(a1)
    bsr     _IntUIK_DrawKnob
    move.l  (sp)+,a1
    rts

.inf
    move.l  uop_LPos(a1),d0
    sub.l   uop_LVis(a1),d0
    bcc.b   .calc
    moveq   #0,d0
    bra.b   .calc
.sup
    move.l  uop_LPos(a1),d0
    add.l   uop_LVis(a1),d0                 déplacement positif
.calc
    move.l  d0,uop_LPos(a1)
    move.w  #-1,uop_MouseStart(a1)
    move.l  a1,-(sp)
    bsr     _IntUIK_SetPropVars
    bsr     _IntUIK_DrawKnob
    move.l  (sp)+,a1
    rts


_HdlUIK_MovePropGad
    bsr     LockObjVars
    pea     UnlockObjVars(pc)
    move.w  uop_MouseStart(a1),d1           si -1 pas de move
    bmi.s   PropCancel

    ;------ contrôle de l'intervalle
    move.l  uo_ObjExt(a1),a0
    tst.b   uop_Ver(a1)
    bne.s   .vert
.horiz
    move.w  im_MouseX(a3),d0
    sub.w   d1,d0                           d0 = x ramené au début du knob
    move.w  uoe_InnerRect+ra_MinX(a0),d2
    cmp.w   d2,d0
    bge.s   .h1
    move.w  d2,d0
    bra.s   .both
.h1 move.w  uoe_InnerRect+ra_MaxX(a0),d1
    sub.w   uop_KnobWidth(a1),d1            d1 = max
    addq.w  #1,d1
    cmp.w   d1,d0
    ble.s   .both
    move.w  d1,d0
    bra.s   .both
.vert
    move.w  im_MouseY(a3),d0
    sub.w   d1,d0                           d0 = y ramené au début du knob
    move.w  uoe_InnerRect+ra_MinY(a0),d2
    cmp.w   d2,d0
    bge.s   .v1
    move.w  d2,d0
    bra.s   .both
.v1 move.w  uoe_InnerRect+ra_MaxY(a0),d1
    sub.w   uop_KnobWidth(a1),d1            d1 = max
    addq.w  #1,d1
    cmp.w   d1,d0
    ble.s   .both
    move.w  d1,d0
.both
    sub.w   d2,d0

    ;------ redessine le knob
    cmp.w   uop_KnobOffs(a1),d0             meme position ?
    beq.s   PropCancel
    move.w  d0,uop_KnobOffs(a1)
    move.l  a1,-(sp)
    bsr     _IntUIK_SetPropPos
    bsr     _IntUIK_DrawKnob
    move.l  (sp)+,a1
    moveq   #1,d2                           message continue
    rts

PropCancel
    moveq   #0,d2                           cancel message
    rts

;PropKnobOut
;    move.w  d1,d0                           collé à gauche
;    bra.b   PropDrawKnob


*************************************
        END
*************************************

************************************************************************
*   UIK_OPr_ChangePropGadget(
*      struct UIKObjPropGad *obj, int Total, int Visible, int Position )
*                            a0       a1         d0           d1
************************************************************************

_UIK_OPr_ChangePropGadget
    bsr     _UIK_LockObjVars

    cmp.l   #-1,d0
    beq.b   .no_visible
    move.l  d0,uop_LVis(a0)
.no_visible
    moveq   #-1,d0
    cmp.l   d0,a1
    beq.b   .no_total
    move.l  a1,uop_LTot(a0)
.no_total

    tst.b   uop_Moving(a0)          ;-----------------
    bne.b   .end

    cmp.l   d0,d1
    beq.b   .no_position
    move.l  d1,uop_LPos(a0)
.no_position
    move.l  a0,-(sp)
    bsr     _IntUIK_SetPropVars
    bsr     _IntUIK_DrawKnob
    move.l  (sp)+,a0                ;------------------
.end
    bsr     _UIK_UnlockObjVars
    rts


************************************************************************
*   UIK_OPr_ChangePropNoDraw(
*      struct UIKObjPropGad *obj, int Total, int Visible, int Position )
*                            a0       a1         d0           d1
************************************************************************

_UIK_OPr_ChangePropNoDraw
    bsr     _UIK_LockObjVars

    cmp.l   #-1,d0
    beq.b   .no_visible
    move.l  d0,uop_LVis(a0)
.no_visible
    moveq   #-1,d0
    cmp.l   d0,a1
    beq.b   .no_total
    move.l  a1,uop_LTot(a0)
.no_total

    tst.b   uop_Moving(a0)          ;-----------------
    bne.b   .end

    cmp.l   d0,d1
    beq.b   .no_position
    move.l  d1,uop_LPos(a0)
.no_position
    move.l  a0,-(sp)
    bsr     _IntUIK_SetPropVars
    move.l  (sp)+,a0                ;------------------
.end
    bsr     _UIK_UnlockObjVars
    rts


