    OPT O+,W-
        OPT O1+                 ; Tells when a branch could be optimised to short
        OPT i+                  ; Tells when '#' is probably missing
        OPT d+
****************************************************************
*
*       Project:    UIK
*       Function:   draw boxes
*
*       Created:    17/05/90    Jean-Michel Forgeas
*
****************************************************************

        IDNT    drawa

******* Includes ************************************************

        INCDIR  "include:"

        INCLUDE "lib/exec_lib.i"
        INCLUDE "lib/Graphics_lib.i"
        INCLUDE "lib/intuition_lib.i"
        INCLUDE "lib/layers_lib.i"
        INCLUDE "exec/types.i"
        INCLUDE "exec/execbase.i"
        INCLUDE "libraries/dos.i"

        INCLUDE "uiki:uikbase.i"
        INCLUDE "uiki:uikglobal.i"
        INCLUDE "uiki:uikbox.i"
        INCLUDE "uiki:uikgfx.i"
        INCLUDE "uiki:uikmacros.i"
        INCLUDE "uiki:uikcolors.i"
        INCLUDE "uiki:uikdrawauto.i"

        INCLUDE "uiki:objscreen.i"
        INCLUDE "uiki:objstring.i"
        INCLUDE "uiki:objproplift.i"
        INCLUDE "uiki:objlist.i"
        INCLUDE "uiki:objrequest.i"
        INCLUDE "uiki:objwindow.i"


REGOFFS     equ     4*15

PUSHREGS    MACRO
            movem.l d0-d7/a0-a6,-(sp)
            ENDM

POPREGS     MACRO
            movem.l (sp)+,d0-d7/a0-a6
            ENDM


******* Imported ************************************************

        XREF    _UIKBase
        XREF    _LayersBase
        XREF    _GfxBase
        XREF    _IntuitionBase

        XREF    _IntUIK_CalcObjStr
        XREF    _UIK_StrWidth
        XREF    _UIK_CharWidth
        XREF    _UIK_ToUpper

        XREF    LockObjVars
        XREF    UnlockObjVars
        XREF    _UIK_LockObjVars
        XREF    _UIK_UnlockObjVars
        XREF    _UIK_LangString
        XREF    _UIK_nl_ListPtr
        XREF    _UIK_ClipObject
        XREF    _UIK_UnclipObject

        XREF    _StrLen
        XREF    _ZoneUpdateStart
        XREF    _ZoneUpdateEnd

        XREF    _BufLength
        XREF    _UIK_AccessPrefs

        XREF    _UBM_Direc      image pour liste
        ;XREF    _UBM_CranP      image pour prop
        ;XREF    _UBM_Prop1      image pour prop
        ;XREF    _UBM_Prop1Sp    image pour prop
        ;XREF    _UBM_Prop2      image pour prop
        ;XREF    _UBM_Prop2Sp    image pour prop

        XREF    _AutoFillBox


******* Exported ************************************************

        XDEF    _IntUIK_DrawKnob
        XDEF    _IntUIK_DrawKnobX
        XDEF    _IntUIK_DrawKnobY
        XDEF    _IntUIK_GetRastPort
        XDEF    _UIK_GetRastPort
        XDEF    _IntUIK_SetActiveColors
        XDEF    _IntUIK_DrawObjStrText

        XDEF    _IntUIK_DrawAuto
        XDEF    _IntUIK_PushBox
        XDEF    _IntUIK_PopBox
        XDEF    _IntUIK_DrawBox
        XDEF    _UIK_OLi_DrawList
        XDEF    _UIK_OLi_DrawListFrom
        XDEF    _UIK_OLi_DrawLine
        XDEF    _Auto2InnerBox
        XDEF    _IntUIK_DoMask
        XDEF    _UIK_GetColor
        XDEF    _UIK_SetColor
        XDEF    _IntUIK_DrawDisPat


****************************************************************
*
*       Routines
*
****************************************************************/

        SECTION text,CODE


*****************************************
* IntUIK_DrawKnob( obj )
*               pile
*****************************************
_IntUIK_DrawKnob
    move.l  4(sp),a0
    tst.b   uop_Ver(a0)
    bne     _IntUIK_DrawKnobY

X1 EQUR d4
Y1 EQUR d5
X2 EQUR d6
Y2 EQUR d7

_IntUIK_DrawKnobX
    PUSHREGS
    bsr     InitRegs                a2=obj, a3=rport, d4=x, d5=y, d6=X, d7=Y
    tst.b   uo_Status(a2)           Obj->Status == STARTED ?
    bne     EndAllNoRP                  si non: rien a faire avec

    bsr     aut_BoxPen
    addq.w  #1,Y1
    subq.w  #1,Y2
    addq.w  #2,X1
    add.w   uop_KnobOffs(a2),X1
    move.w  X1,X2
    add.w   uop_KnobWidth(a2),X2
    subq.w  #1,X2                       d4-d7 = new knob

    movem.w uop_SaveKnob(a2),d0-d3      d0-d3 = old knob
    movem.w d4-d7,uop_SaveKnob(a2)      save new
    cmp.w   d1,d3
    beq.b   DrawKnob                    première fois

    cmp.w   d0,X2           recouvrement ?
    bmi.b   ClearOld        Xn < xo
    cmp.w   X1,d2
    bmi.b   ClearOld        Xo < xn

    ;--------------------   oui
    movem.w d2/X2,-(sp)
    sub.w   d0,d2   old width
    sub.w   X1,X2   new width
    cmp.w   d2,X2
    movem.w (sp)+,d2/X2
    beq.b   .equals

    pea     DrawKnob(pc)
    bra.b   .elague

.equals
    bsr     do_clipblit
    pea     PropEndAll(pc)

.elague
    movem.l d0-d1,-(sp)
    bsr     aut_fillpaton
    movem.l (sp)+,d0-d1

    move.w  X1,-(sp)
    exg     d2,X1
    subq.w  #1,d2
    cmp.w   d0,d2           clear left ?
    bmi.b   .clearight
    bsr     aut_rec

.clearight
    move.w  X2,d0
    addq.w  #1,d0
    cmp.w   d0,X1
    bmi.b   .endX           clear right ?
    move.w  Y1,d1
    move.w  X1,d2
    bsr     aut_rec
.endX
    move.w  (sp)+,X1
    rts

ClearOld
    movem.l d0-d1,-(sp)
    bsr     aut_fillpaton
    movem.l (sp)+,d0-d1
    bsr     aut_rec         non, efface tout l'ancien
DrawKnob
    bsr     aut_fillpatoff
    move.l  uop_KnobAutomate(a2),a5     a5 = automate
    bsr     ExeAuto
    bsr     aut_Rec

    move.l  uop_KnobImage2(a2),a0
    move.l  uop_KnobSpecialImage2(a2),a1
    cmp.b   #2,uo_AutoRatio(a2)
    beq.b   .ratiook
    move.l  uop_KnobImage1(a2),a0
    move.l  uop_KnobSpecialImage1(a2),a1
.ratiook
    move.l  _UIKBase,a5
    move.l  lb_UIKPrefs(a5),a5
    btst    #UIKB_SPLOOK,uikp_PrefsFlags+1(a5)
    beq.b   .spok
    exg     a0,a1
.spok
    move.l  a0,d0
    beq     PropEndAll                  Pas d'image

    moveq   #1,d2

    move.w  X2,d0
    sub.w   X1,d0
    addq.w  #2,d0                   knob int width
    sub.w   ubm_Width(a0),d0
    asr.w   d2,d0
    add.w   X1,d0                   left offset

    move.w  Y2,d1
    sub.w   Y1,d1
    addq.w  #2,d1                   knob int height
    sub.w   ubm_Rows(a0),d1
    asr.w   d2,d1
    add.w   Y1,d1                   top offset

    move.w  d0,d2                   destX
    move.w  d1,d3                   destY
    moveq   #0,d0                   srcx
    moveq   #0,d1                   srcy

    move.l  a3,a1                   destrp
    move.w  ubm_Width(a0),d4        sizeX
    move.w  ubm_Rows(a0),d5         sizeY
    move.l  ubm_Mask(a0),d6         bltmask
    addq.w  #UIKTOBMAP_OFFSET,a0    srcbm
    beq.b   .bltsimple
    move.l  a2,-(sp)                pour PropEndAll -> draw_pat
    move.l  d6,a2
    moveq   #$e0,d6                 minterm
    jsr     _LVOBltMaskBitMapRastPort(a6)
    move.l  (sp)+,a2
    bra     PropEndAll
.bltsimple
    moveq   #$c0,d6                 minterm
    jsr     _LVOBltBitMapRastPort(a6)
    bra     PropEndAll


_IntUIK_DrawKnobY
    PUSHREGS
    bsr     InitRegs                a2=obj, a3=rport, d4=x, d5=y, d6=X, d7=Y
    tst.b   uo_Status(a2)           Obj->Status == STARTED ?
    bne     EndAllNoRP                  si non: rien a faire avec

    bsr     aut_BoxPen
    addq.w  #2,X1
    subq.w  #2,X2
    addq.w  #1,Y1
    add.w   uop_KnobOffs(a2),Y1
    move.w  Y1,Y2
    add.w   uop_KnobWidth(a2),Y2
    subq.w  #1,Y2

    movem.w uop_SaveKnob(a2),d0-d3      d0-d3 = old knob
    movem.w d4-d7,uop_SaveKnob(a2)      save new
    cmp.w   d0,d2
    beq     DrawKnob                    première fois

    cmp.w   d1,Y2           recouvrement ?
    bmi     ClearOld        Yn < yo
    cmp.w   Y1,d3
    bmi     ClearOld        Yo < yn

    ;--------------------   oui
    movem.w d3/Y2,-(sp)
    sub.w   d1,d3   old width
    sub.w   Y1,Y2   new width
    cmp.w   d3,Y2
    movem.w (sp)+,d3/Y2
    beq.b   .equals

    pea     DrawKnob(pc)
    bra.b   .elague

.equals
    bsr.b   do_clipblit
    pea     PropEndAll(pc)

.elague
    movem.l d0-d1,-(sp)
    bsr     aut_fillpaton
    movem.l (sp)+,d0-d1

    move.w  Y1,-(sp)
    exg     d3,Y1
    subq.w  #1,d3
    cmp.w   d1,d3           clear top ?
    bmi.b   .clearbot
    bsr     aut_rec

.clearbot
    move.w  Y2,d1
    addq.w  #1,d1
    cmp.w   d1,Y1           clear bot ?
    bmi.b   .endY
    move.w  X1,d0
    move.w  Y1,d3
    bsr     aut_rec
.endY
    move.w  (sp)+,Y1
    rts

do_clipblit
    cmp.w   d0,d4   SrcX == DestX ?
    bne.b   .ok
    cmp.w   d1,d5   SrcY == DestY ?
    beq.b   .end
.ok
    movem.w d0-d6,-(sp)
    move.l  a3,a0   ClipBlit(Src, SrcX, SrcY, Dest, DestX, DestY, XSize, YSize, Minterm );
    move.l  a3,a1            a0   d0    d1    a1    d2     d3     d4     d5     d6
    exg     d2,d4
    exg     d3,d5
    sub.w   d0,d4
    addq.w  #1,d4
    sub.w   d1,d5
    addq.w  #1,d5
    moveq   #$c0,d6
    jsr     _LVOClipBlit(a6)
    movem.w (sp)+,d0-d6
.end
    rts

PropEndAll
    btst    #(BOXB_DISABLED-16),uo_Flags+1(a2)
    beq     EndAll
    bsr     draw_pat
    bra     EndAll


*****************************************
*  rp = UIK_GetRastPort( obj )
*  d0                    a0
*****************************************
_UIK_GetRastPort
_IntUIK_GetRastPort
        movem.l d1/a1,-(sp)
        move.l  uo_rp(a0),d0                d0 = rastport task
        move.l  4.w,a1
        move.l  ThisTask(a1),d1
        move.l  uo_UIK(a0),a1
        cmp.l   uike_Task(a1),d1
        beq.b   .end
        move.l  uo_irp(a0),d0               d0 = rastport input.device
.end    movem.l (sp)+,d1/a1
        tst.l   d0
        rts

*****************************************
*  VerifFont()
*
*****************************************
_VerifFont
    move.l  a6,-(sp)
    move.l  _GfxBase,a6

    move.l  uo_ObjExt(a2),a0        a0 = extension
    move.l  uoe_Font(a0),d0
    cmp.l   rp_Font(a3),d0
    beq.b   .style

    move.l  a0,-(sp)
    move.l  d0,a0
    move.l  a3,a1
    jsr     _LVOSetFont(a6)
    move.l  (sp)+,a0
.style
    move.b  uoe_TTA+tta_Style(a0),d0
    moveq   #-1,d1
    move.l  a3,a1
    jsr     _LVOSetSoftStyle(a6)

    move.l  (sp)+,a6
    rts

*****************************************
*
*
*****************************************
InitRegs
    move.l  REGOFFS+4+4(sp),a2          a2 = obj  (+4 pour l'appel de fonction)
    move.l  a2,a0
    bsr.b   _IntUIK_GetRastPort
    move.l  d0,a3                       a3 = rastport
    movem.w uo_Box(a2),d4-d7            d4=left, d5=top, d6=width, d7=height
    add.w   d4,d6
    subq.w  #1,d6                       d6 = X
    add.w   d5,d7
    subq.w  #1,d7                       d7 = Y
    move.l  _GfxBase,a6
    rts


*****************************************
*
*
*****************************************
_SetMask
    move.l  uo_sc(a2),d0
    beq.b   .prefs
    move.l  d0,a0
    move.l  sc_UserData(a0),a0
    move.b  uosc_Mask(a0),rp_Mask(a3)
    bra.b   .go
.prefs
    move.l  _UIKBase,a0
    move.l  lb_UIKPrefs(a0),a0
    move.b  uikp_Mask(a0),rp_Mask(a3)
.go
    rts


*****************************************
* IntUIK_DrawAuto( obj, auto, xoffs, yoffs )
*                  a0   a1    d0     d1
*****************************************
_IntUIK_DrawAuto
    PUSHREGS
    bsr.b   InitRegs               a2=obj, a3=rport, d4=x, d5=y, d6=X, d7=Y
    tst.b   uo_Status(a2)           Obj->Status == STARTED ?
    bne     EndAllNoRP                  si non: rien a faire avec

    move.l  REGOFFS+8(sp),a5       a5 = automate
    movem.l REGOFFS+12(sp),d0/d1   d0/d1 = offs
    add.w   d0,d4
    add.w   d0,d6
    add.w   d1,d5
    add.w   d1,d7
    pea     EndAll(pc)

ExeAuto
    ;move.b  rp_Mask(a3),d0
    ;move.l  d0,-(sp)
    ;bsr.b   _SetMask
    ;pea     endauto(pc)
    bsr     setdrmd2
    lea     Functions,a0
    move.l  a0,-(sp)

auto
    move.l  (sp),a0
    move.w  (a5)+,d0
    pea     auto(pc)
    jmp     (a0,d0.w)

endauto
    ;move.l  (sp)+,d0
    ;move.b  d0,rp_Mask(a3)
    ;rts


*****************************************
* Auto2InnerBox( obj )
*                a0
*****************************************
_Auto2InnerBox
    movem.l d4-d7/a2/a5,-(sp)
    move.l  a0,a2
    move.l  uo_Automate(a2),a5
    btst    #BOXB_OVER,uo_Flags+3(a2)
    bne.b   .ok
    move.l  uo_AltAutomate(a2),a5
.ok
    movem.w uo_Box(a2),d4-d7            d4=left, d5=top, d6=width, d7=height
    move.w  d6,d0
    or.w    d7,d0
    beq.b   .end
    move.l  a5,d0
    beq.b   .end

    add.w   d4,d6
    subq.w  #1,d6                       d6 = X
    add.w   d5,d7
    subq.w  #1,d7                       d7 = Y
    pea     .end(pc)

    lea     SimFunctions,a0
    move.l  a0,-(sp)
    bra.b   auto
.end
    move.l  uo_ObjExt(a2),a2
    movem.w d4-d7,uoe_InnerRect(a2)
    movem.l (sp)+,d4-d7/a2/a5
    rts

*************************************
* IntUIK_PushBox( obj )
* IntUIK_PopBox( obj )
*************************************
_IntUIK_PushBox
    move.l  4(sp),a0
    bsr     _UIK_LockObjVars
    move.l  a0,-(sp)
    bclr    #BOXB_OVER,uo_Flags+3(a0)
    beq.b   .allready
    bsr.b   _IntUIK_DrawBox
.allready
    move.l  (sp)+,a0
    bsr     _UIK_UnlockObjVars
.end
    rts

_IntUIK_PopBox
    move.l  4(sp),a0
    bsr     _UIK_LockObjVars
    move.l  a0,-(sp)
    bset    #BOXB_OVER,uo_Flags+3(a0)
    bne.b   .allready
    bsr.b   _IntUIK_DrawBox
.allready
    move.l  (sp)+,a0
    bsr     _UIK_UnlockObjVars
.end
    rts


*************************************
* IntUIK_SetActiveColors( obj )
*                         sp
*************************************
_IntUIK_SetActiveColors
    move.l  4(sp),a0
IntUIK_SetActiveColors
    btst    #BOXB_OVER,uo_Flags+3(a0)
    bne.b   .inact
    move.b  uo_ActBoxPen(a0),uo_BoxPen(a0)
    move.b  uo_ActTitlePen(a0),uo_TitlePen(a0)
    rts
.inact
    move.b  uo_InactBoxPen(a0),uo_BoxPen(a0)
    move.b  uo_InactTitlePen(a0),uo_TitlePen(a0)
    rts


*************************************
* IntUIK_DrawBox( obj )
*
*************************************
_IntUIK_DrawBox
    PUSHREGS
    bsr     InitRegs
    tst.b   uo_Status(a2)
    bne     EndAllNoRP        ;---- if Z set -> object not started

    move.l  a2,a0
    bsr     _UIK_LockObjVars

    bsr.b   IntUIK_SetActiveColors

    move.l   a2,a0
    bsr     _ZoneUpdateStart

    move.l  uo_Flags(a2),d0

    btst    #BOXB_CUSTOMDRAW,d0
    beq.b   .drawn
    move.l  uo_Flags(a2),d0
    move.l  uo_ObjExt(a2),a0
    move.l  uoe_CustomDrawData(a0),a1
    move.l  uoe_CustomDraw(a0),a0
    movem.l d0/d4/d5/d6/d7/a1/a2/a3/a6,-(sp)
    jsr     (a0)
    add.w   #9*4,sp
    bra     .BoxEndAll2

.drawn
    btst    #BOXB_NDRAWN,d0
    bne     .Title

    btst    #BOXB_USEBITMAP,d0
    beq.b   .auto

    move.l  uo_Flags(a2),d0

    movem.w d2-d6,-(sp)
    move.l  uo_rq(a2),d1            ; requester ?
    beq.b   ..win
    move.l  d1,a0
    move.l  rq_ImageBMap(a0),a0     ; UIKObjReq pointer
    movem.l uor_CustomBitMap(a0),a0/a1
    bra.b   ..ok
..win
    move.l  uo_w(a2),a0            ; window
    move.l  wd_UserData(a0),a0     ; UIKObjWindow pointer
    movem.l uow_CustomBitMap(a0),a0/a1
..ok
    btst    #BOXB_OVER,d0           ; a0 = src bitmap
    bne.b   ..over
    exg     a0,a1
..over                              ; a0 = src bitmap
    move.w  d4,d2                   ; dest x
    move.w  d5,d3                   ; dest y

    btst    #BOXB_TRANSLATED,d0
    beq.b   ..trans
    move.l  uo_w(a2),a1
    move.l  wd_UserData(a1),a1
    sub.w   uow_LeftBorderWidth(a1),d4
    sub.w   uow_TopBorderHeight(a1),d5
..trans
    move.w  d4,d0                   ; src x
    move.w  d5,d1                   ; src y
    move.l  a3,a1                   ; dest rastport
    movem.w uo_Box+box_Width(a2),d4/d5  ; dest width/height
    moveq   #$c0,d6
    jsr     _LVOBltBitMapRastPort(a6)
    movem.w (sp)+,d2-d6
    bra.b   .Title

.auto
    move.l  uo_Automate(a2),a5
    btst    #BOXB_OVER,d0
    bne.b   .exe
    move.l  uo_AltAutomate(a2),a5
.exe
    move.l  a5,d1
    beq.b   .fill
    bsr     ExeAuto

.fill
    move.l  uo_Flags(a2),d0
    btst    #BOXB_FILLED,d0
    beq.b   .Title
    lea     _AutoFillBox,a5
.fillok
    bsr     ExeAuto

.Title
    move.l  uo_ObjExt(a2),a0
    movem.w d4-d7,uoe_InnerRect(a0)

    btst.b  #TITLEB_CLIPPED,uo_TitleFlags+3(a2)
    beq.b   .noclip

    move.l  _LayersBase,a6
    move.l  (a3),a1
    jsr     _LVOLockLayer(a6)

    move.l  uo_Region(a2),a0
    movem.w d4-d7,rg_bounds(a0)
    move.l  rg_RegionRectangle(a0),a0
    movem.w d6-d7,rr_bounds+ra_MaxX(a0)
    sub.w   d4,rr_bounds+ra_MaxX(a0)
    sub.w   d5,rr_bounds+ra_MaxY(a0)
.noreg
    move.l  a2,a1
    bsr     _UIK_ClipObject

    move.l  _GfxBase,a6
.noclip


    move.l  uo_ObjExt(a2),a1
    btst    #BOXB_OVER,uo_Flags+3(a2)
    bne.b   .norm
.alt                                ; titre
    ;moveq   #-4,d1
    lea     uo_AltTitle(a2),a0
    move.l  (a0),d0
    bne.b   .ok
.norm
    ;moveq   #4,d1
    lea     uo_Title(a2),a0
    move.l  (a0),d0
    beq     .BoxEndAll
.ok
    ;move.w  d1,uoe_MultiCount(a1)

            ;---------------------------- largeur / hauteur
    btst    #TITLEB_IMAGE,uo_TitleFlags+3(a2)
    beq.b   .istext
    move.l  d0,a5                   a5 = image ou multimage
    tst.w   uoe_MultiNum(a1)
    beq.b   .image                  ;----------c'est une multimage
    move.l  a5,a1                   a1 = début
    move.l  (a5),a5                 a5 = image
.image                              ;----------c'est une image
    move.w  ubm_Width(a5),d3        a5 = image
    move.w  ubm_Rows(a5),d1
    ;addq.w  #1,d1
    bra.b   .position
                                    ;----------c'est du texte
.istext
    btst.b  #TITLEB_ADDR,uo_TitleFlags+3(a2)
    bne.b   .12
    move.l  a2,a0               ;move.l  uo_UIK(a2),a0          titre de l'objet
    bsr     _UIK_LangString
    tst.l   d0
    beq     .BoxEndAll
.12 move.l  d0,a5

    move.l  d0,a0
    moveq   #-1,d2
.le addq.l  #1,d2
    tst.b   (a0)+
    bne.b   .le                     d2 = strlen()

.setfont
    bsr     _VerifFont

.length
    move.l  a5,a1
    move.l  d2,d0
    move.l  rp_Font(a3),a0          a0 = font
    bsr     _UIK_StrWidth           d0 = string width
    move.l  uo_ObjExt(a2),a0
    move.b  uoe_SCWidth(a0),d1
    ext.w   d1
    sub.w   d1,d0                   d0 = string width - underscore width
    move.w  d0,d3                   d3 = string width
    move.w  rp_TxHeight(a3),d1      d1 = string height

.position   ;---------------------------- positionnement
    movem.w uo_Box(a2),d4-d7        d4=left, d5=top, d6=width, d7=height
    sub.w   d3,d6
    asr.w   #1,d6
    add.w   d4,d6                   d6 = x centré

    move.l  uo_TitleFlags(a2),d0
.above
    btst    #TITLEB_ABOVE+8,d0
    beq.b   .top
    sub.w   rp_TxHeight(a3),d5
    subq.w  #1,d5
    moveq   #0,d7
    bra.b   .draw
.top
    asr.w   #1,d1                   d1 = height / 2
    btst    #TITLEB_TOP,d0
    beq.b   .left
    sub.w   d1,d5
    moveq   #0,d7
    bra.b   .draw
.left
    asr.w   #1,d7
    sub.w   d1,d7                   d7 = y dans box
    btst    #TITLEB_LEFT,d0
    beq.b   .right
    sub.w   d3,d4
    subq.w  #5,d4
    move.w  d4,d6
    bra.b   .draw
.right
    btst    #TITLEB_RIGHT,d0
    beq.b   .insright
    move.w  uo_Box+box_Width(a2),d6
    add.w   d4,d6
    addq.w  #4,d6
    bra.b   .draw

.insright
    move.l  uo_ObjExt(a2),a0
    movem.w uoe_InnerRect(a0),d4-d7 d4=minx, d5=miny, d6=maxx, d7=maxy
    sub.w   d5,d7
    addq.w  #1,d7                   d7=height
    asr.w   #1,d7
    sub.w   d1,d7                   d7 = y dans box

    btst    #TITLEB_INSRIGHT+8,d0
    beq.b   .insleft
    subq.w  #2,d6
    sub.w   d3,d6
    bra.b   .draw
.insleft
    btst    #TITLEB_INSLEFT+8,d0
    beq.b   .center
    addq.w  #2,d4
    move.w  d4,d6
    bra.b   .draw
.center
    sub.w   d4,d6                   d6=width
    sub.w   d3,d6
    addq.w  #2,d6
    asr.w   #1,d6
    add.w   d4,d6                   d6 = x centré

.draw       ;---------------------------- dessin
    add.w   d7,d5                        d5 = y dans box + box offset
    btst    #TITLEB_IMAGE,uo_TitleFlags+3(a2)
    beq     .drawtext       d0 intact

.drawimage
    move.l  uo_ObjExt(a2),a0
    clr.w   uoe_MultiCount(a0)      marqueur 1ère image
    move.w  uoe_MultiNum(a0),d7     d7 = nombre d'images
    movem.l a1-a2,-(sp)             a0 = ext, a1 = images base, a2 = object
    move.w  d6,d2                   destX
    move.w  d5,d3                   destY
    move.w  ubm_Width(a5),d4        sizeX
    move.w  ubm_Rows(a5),d5         sizeY
.loop
    tst.w   d7
    beq.b   .nomulti
    move.l  4(sp),a1                ;---- sync next image
    cmp.l   uo_irp(a1),a3
    beq.b   .rp
    moveq   #0,d7
    bra.b   .nomulti
.rp
    move.l  uo_w(a1),a0
    move.l  wd_WScreen(a0),a0
    lea     sc_ViewPort(a0),a0
    move.l  a0,a2
    move.l  uo_ObjExt(a1),a1
    move.w  uoe_MultiDelay(a1),d6
    ;ext.l d6
    ;move.w  uoe_MultiCount(a1),d0
    ;ext.l   d0
    ;DBUG  <'delay=%ld',10>,d6,d0
    tst.w   uoe_MultiCount(a1)      immédiat la 1ère fois
    bne.b   .dd
    moveq   #0,d6
.dd addq.w  #1,uoe_MultiCount(a1)
.dl     move.l  a2,a0
        jsr     _LVOWaitBOVP(a6)
        dbra    d6,.dl
    move.l  (sp),a1                 ;---- find next image
    move.l  (a1)+,a5
    move.l  a1,(sp)

.nomulti                            ;---- blit
    move.l  a5,a0                   srcbm
    moveq   #0,d0                   srcx
    moveq   #0,d1                   srcy
    move.l  a3,a1                   destrp
    addq.w  #UIKTOBMAP_OFFSET,a0    srcbm
    move.l  ubm_Mask(a5),d6         bltmask
    beq.b   .bltsimple
    move.l  d6,a2
    moveq   #$e0,d6                 minterm
    jsr     _LVOBltMaskBitMapRastPort(a6)
    bra.b   .bltdone
.bltsimple
    moveq   #$c0,d6                 minterm
    jsr     _LVOBltBitMapRastPort(a6)
.bltdone
    subq.w  #1,d7                   ;---- continue?
    bhi     .loop

.imagend
    movem.l (sp)+,a1-a2
    bra     .BoxEndAll

.drawtext            ;d0 intact
    and.w   #TITLE_LEFT!TITLE_RIGHT!TITLE_ABOVE,d0
    beq.b     .jam2
    bsr     setdrmd1
    bra.b   .jamok
.jam2
    bsr     setdrmd2
    move.b  uo_BoxPen(a2),d0
    bsr     setbpen
.jamok
    move.b  uo_TitlePen(a2),d3
    bsr     setapen

    add.w   rp_TxBaseline(a3),d5
    bsr     aut_MXy

    move.l  uo_ObjExt(a2),a0
    tst.b   uoe_SCWidth(a0)
    bne.b   .underscore
    move.l  a3,a1
    move.l  a5,a0
    move.l  d2,d0
    jsr     _LVOText(a6)            Text()

    move.l  uo_ObjExt(a2),a0        ObjExt
    move.b  uoe_SCKey(a0),d3        doit-on chercher une lettre à souligner?
    beq     .BoxEndAll
.keyloop
    move.b  (a5)+,d4                char suivant
    beq     .BoxEndAll
    move.b  d4,d0
    bsr     _UIK_ToUpper            converti en majuscule (SCKey toujours majuscule)
    cmp.b   d0,d3
    beq.b     .keyfound
    move.l  rp_Font(a3),a0          a0=font d0=char
    move.l  d4,d0
    bsr     _UIK_CharWidth
    add.w   d0,d6                   ajoute la largeur du char
    bra.b   .keyloop                puis suivant

.underscore
    moveq   #0,d1
    move.b  uoe_SCPos(a0),d1
    beq.b   .sec
    move.l  a3,a1
    move.l  a5,a0
    move.l  d1,d0
    jsr     _LVOText(a6)            Text()
    move.l  uo_ObjExt(a2),a0
    move.b  uoe_SCPos(a0),d1
.sec
    ext.w   d1
    move.w  d2,d0                   strlen
    sub.w   d1,d0                   - position
    subq.w  #1,d0                   - 1
    move.l  a3,a1
    move.l  a5,a0
    add.w   d1,a0
    addq.w  #1,a0
    move.w  d1,d2                   d2 = pos.w
    jsr     _LVOText(a6)            Text()
    bra.b   .underscore_next

.underscore_loop
 ;DBUG <'%ls     ',10>,a5,d6
    move.l  rp_Font(a3),a0
    move.b  (a5)+,d0                char suivant
    bsr     _UIK_CharWidth          a0=font d0=char
    add.w   d0,d6                   ajoute la largeur du char
 ;DBUG <'%ld %ld',10>,d0,d6
.underscore_next
    dbra    d2,.underscore_loop

    move.l  rp_Font(a3),a0
    move.b  1(a5),d0                char suivant
    bsr     _UIK_CharWidth          a0=font d0=char --> d0 = largeur du char souligné
    move.l  uo_ObjExt(a2),a0
    moveq   #0,d1
    move.b  uoe_SCWidth(a0),d1      d1 = largeur du char de soulignement
    lsr.w   #1,d0
    lsr.w   #1,d1
    add.w   d0,d6
    sub.w   d1,d6

.keyfound
    move.w  rp_TxBaseline(a3),d0
    addq.w  #3,d0
    cmp.w   rp_TxHeight(a3),d0
    bcs.b   .yok
    addq.w  #1,d5
.yok
    bsr     aut_MXy
    bsr.b   setdrmd1                jam1
    lea     SoulignChar(pc),a0
    move.l  a3,a1
    moveq   #1,d0
    jsr     _LVOText(a6)            Text()

.BoxEndAll
    btst    #(BOXB_DISABLED-16),uo_Flags+1(a2)
    beq.b   .nopat
    bsr     draw_pat
.nopat
    btst.b  #TITLEB_CLIPPED,uo_TitleFlags+3(a2)
    beq.b   .BoxEndAll2
    move.l  _LayersBase,a6
    move.l  a2,a1
    bsr     _UIK_UnclipObject
    move.l  (a3),a0
    jsr     _LVOUnlockLayer(a6)

.BoxEndAll2
    moveq   #0,d0
    move.b  d0,rp_AreaPtSz(a3)
    move.l  d0,rp_AreaPtrn(a3)
    move.l  a2,a0
    bsr     _ZoneUpdateEnd
    move.l  a2,a0
    bsr     _UIK_UnlockObjVars

SoulignChar dc.b    '_',0

*************************************
*************************************
EndAll
    moveq   #0,d0
    move.l  d0,rp_AreaPtrn(a3)
EndAllNoRP
    POPREGS
    rts

*************************************
* Gfx calls
*
*************************************

setdrmd1
    move.b  #RP_JAM1,d0
    beq.b   setthisdrmd
setdrmd2
    move.b  #RP_JAM2,d0
setthisdrmd
    move.l  a3,a1
    jmp     _LVOSetDrMd(a6)
setapen
    move.l  a2,a0
    move.b  d3,d0
    bsr.b   _UIK_GetColor
    move.l  a3,a1
    jmp     _LVOSetAPen(a6)
setPenbpen
    move.b  uo_BoxPen(a2),d0
setbpen
    move.l  a2,a0
    bsr.b   _UIK_GetColor
    move.l  a3,a1
    jmp     _LVOSetBPen(a6)
setpaton
    bsr.b   setdrmd2
    move.w  #$5555,d0
    bra.b   setpat
setpatoff
    moveq   #-1,d0
setpat
    move.w  d0,rp_LinePtrn(a3)
    bset    #RPB_FRST_DOT,rp_Flags+1(a3)
    move.b  #15,rp_linpatcnt(a3)
    rts
setfillpat
    move.l  a0,rp_AreaPtrn(a3)
    move.b  uo_PatternPower(a2),rp_AreaPtSz(a3)
    moveq   #UIKCOL_BLACK,d0
    bsr.b   setbpen
    bsr.b   setdrmd2
rts rts
move
    move.l  a3,a1
    jmp     _LVOMove(a6)
draw
    move.l  a3,a1
    jmp     _LVODraw(a6)
rectfill
    move.l  a3,a1
    jmp     _LVORectFill(a6)


*************************************
*   UIK_GetColor()
*   UIK_SetColor()
*
*************************************

_UIK_GetColor       ; PaletteColor = UIK_GetColor( obj, UIKColor )(a0,d0)
    bsr   SearchColors
    ext.w   d0
    move.b  (a0,d0.w),d0
    rts
_UIK_SetColor       ; UIK_SetColor( obj, UIKColor, PaletteColor )(a0,d0/d1)
    bsr   SearchColors
    ext.w   d0
    move.b  d1,(a0,d0.w)
    ; fall into domask

_IntUIK_DoMask      ; IntUIK_DoMask( UBYTE *Colors, UBYTE *PMask )(a0/a1)
    moveq   #0,d1
    or.b    (a0)+,d1    UIKCOL_GREY
    or.b    (a0)+,d1    UIKCOL_BLACK
    or.b    (a0)+,d1    UIKCOL_WHITE
    or.b    (a0)+,d1    UIKCOL_LIGHT
    addq.w  #1,a0       pas UIKCOL_ZONE
    or.b    (a0)+,d1    UIKCOL_LOOK
    or.b    (a0)+,d1    UIKCOL_IMP

    cmp.b   #$01,d1
    bhi.b   .03
    move.b  #$01,(a1)     set mask
    rts
.03 cmp.b   #$03,d1
    bhi.b   .07
    move.b  #$03,(a1)     set mask
    rts
.07 cmp.b   #$07,d1
    bhi.b   .0f
    move.b  #$07,(a1)     set mask
    rts
.0f cmp.b   #$0f,d1
    bhi.b   .1f
    move.b  #$0f,(a1)     set mask
    rts
.1f cmp.b   #$1f,d1
    bhi.b   .3f
    move.b  #$1f,(a1)     set mask
    rts
.3f cmp.b   #$3f,d1
    bhi.b   .7f
    move.b  #$3f,(a1)     set mask
    rts
.7f cmp.b   #$7f,d1
    bhi.b   .ff
    move.b  #$7f,(a1)     set mask
    rts
.ff move.b  #$ff,(a1)     set mask
    rts

SearchColors        ; d0=a0=&color=SearchColors( obj )(a0)
.loop   cmp.l   #0,a0
        beq.b   .uik
        cmp.w   #UIKID_SCREEN,uo_Id(a0)
        beq.b   .screen
        move.l  uo_Parent(a0),a0
        bra.b   .loop
.screen
    lea     uosc_Mask(a0),a1
    lea     uosc_Colors(a0),a0
    rts
.uik
    move.l  _UIKBase,a0
    move.l  lb_UIKPrefs(a0),a0
    lea     uikp_Mask(a0),a1
    lea     uikp_Colors(a0),a0
    rts


*************************************
*   DrawDisPat( obj ) (A0)
*
*************************************

_IntUIK_DrawDisPat
    bsr     _UIK_GetRastPort            (preserve registres et teste d0)
    beq.b   .end
    movem.l d2-d7/a2-a3/a5-a6,-(sp)
    move.l  d0,a3
    move.l  a0,a2

    move.l  _LayersBase,a6
    move.l  (a3),a1
    jsr     _LVOLockLayer(a6)

    move.l  _GfxBase,a6
    bsr     draw_pat

    moveq   #0,d0
    move.b  d0,rp_AreaPtSz(a3)
    move.l  d0,rp_AreaPtrn(a3)

    move.l  _LayersBase,a6
    move.l  (a3),a0
    jsr     _LVOUnlockLayer(a6)

    movem.l (sp)+,d2-d7/a2-a3/a5-a6
.end
    rts

draw_pat
    move.l  #$88882222,-(sp)
    move.b  #1,rp_AreaPtSz(a3)
    move.l  sp,rp_AreaPtrn(a3)
    bsr     setdrmd1
    move.l  a2,a0
    moveq   #UIKCOL_BLACK,d0
    bsr     _UIK_GetColor
    move.l  d0,d3
    bsr     setapen
    move.l  uo_ObjExt(a2),a0
    movem.w uoe_InnerRect(a0),d0-d3     d0=minx, d1=miny, d2=maxx, d3=maxy
    bsr     aut_rec
    addq.w  #4,sp
    rts


*************************************
* DrawList routines
*
*************************************

DrawInit
    move.l  a0,a2                       a2 = obj
    move.l  a2,a0
    bsr     _IntUIK_GetRastPort
    move.l  d0,a3                       a3 = rastport
    bsr     _VerifFont                  SetFont() if not the same
    move.l  a2,a0
    bsr     _UIK_LockObjVars

    move.l  _LayersBase,a6
    move.l  (a3),a1
    jsr     _LVOLockLayer(a6)
    move.l  a2,a1
    bsr     _UIK_ClipObject
    rts

DrawSet
    move.l  _GfxBase,a6
    move.b  uol_ListPen(a2),d3
    bsr     setapen
    bsr     setPenbpen
    bsr     setdrmd2

    move.l  uo_Region(a2),a1            a1 = Region
    movem.w rg_bounds+ra_MinX(a1),d4-d7 d4=left, d5=top, d6=maxx, d7=maxy

    lea     uol_ListText(a2),a0         a0 = UIKList*
    bsr     _UIK_nl_ListPtr                liste attachée?
    move.l  d0,a0
    move.l  uikl_NodeNum(a0),d3         d3 = nombre de lignes
    beq.b   .end
    move.w  d5,d2                       d2 = char top
    move.w  rp_TxBaseline(a3),d0
    add.w   d0,d5
    add.w   d0,d7
    move.l  uikl_UNode(a0),a5           a5 = table des nodes
    moveq   #0,d1
    move.w  uol_Pos(a2),d1              d1 = numéro du 1er node affiché
    cmp.w   #-1,d1
    beq.b   .error
    move.l  d1,d0
    sub.w   d0,d3                       d3 = nbr lignes depuis Pos
    ble.b   .error
    lsl.l   #3,d0                       offest (UIKNODE_SIZE equ 8)
    add.l   d0,a5                       a5 = adresse du 1er node affiché
    moveq   #1,d0                       pour code retour
.end
    rts
.error
    moveq   #0,d0
    bra.b   .end

DrawLine
    movem.w d0/d2-d3/d7,-(sp)
    clr.w   d7                          flags = 0
    move.b  uo_BoxPen(a2),d3            d3 = couleur de fond

    btst.b  #UIKLB_SELDISPL,uol_ListFlags+3(a2)    dessin de la selection ?
    beq.b   .normale

    moveq   #-1,d1
    move.w  uol_Sel(a2),d0              node sélectionné ?
    ext.l   d0
    cmp.w   d1,d0
    beq.b   .normale
    lsl.l   #3,d0                       offset (UIKNODE_SIZE equ 8)
    move.l  d0,-(sp)
    lea     uol_ListText(a2),a0         a0 = UIKList*
    bsr     _UIK_nl_ListPtr                liste attachée?
    move.l  d0,a0
    move.l  (sp)+,d0
    add.l   uikl_UNode(a0),d0           d0 = adresse du 1er node affiché
    cmp.l   a5,d0
    bne.b   .normale
.selected
    ;move.l  a3,a1
    bset    #1,d7                       flag 1 = selected
    move.b  uol_SelectTextPen(a2),d3    d3 = couleur du texte de sélection
    bsr     setapen
    move.b  uol_SelectPen(a2),d3        d3 = couleur du fond de sélection
    move.b  d3,d0
    bsr     setbpen

.normale
    ;moveq   #0,d0                       pas collé à gauche
    ;move.b  uo_AutoRatio(a2),d0         1 ou 2
    ;add.w   d4,d0
    ;subq.w  #1,d0
    move.w  d4,d0
    move.w  d5,d1
    btst    #UIKNB_IMAGEFIRST,uikn_Flags+1(a5)
    beq.b   .text
    add.w   #16,d0                      largeur image
.text
    bsr     move                        Move( x, y )
    move.l  a3,a1
    move.l  uikn_Buf(a5),a0
    bsr     _BufLength
    tst.l   d0
    beq.s   .efface

    btst    #1,d7                       flag 1 = selected
    beq.b   .alltext
    move.b  uol_SelectLen(a2),d1
    beq.s   .alltext
    ext.w   d1
    ext.l   d1
    btst    #UIKNB_IMAGEFIRST,uikn_Flags+1(a5)
    beq.s   .notdir
    addq.l  #2,d1                       +2 espaces à tracer
.notdir
    movem.l d0-d1/a0-a1,-(sp)
    move.l  d1,d0                       trace le début du texte
    jsr     _LVOText(a6)                Text() début en sélectionné
    move.b  uol_ListPen(a2),d3
    bsr     setapen
    move.b  uo_BoxPen(a2),d3            d3 = couleur de fond
    move.b  d3,d0
    bsr     setbpen
    movem.l (sp)+,d0-d1/a0-a1

    add.l   d1,a0       string + déjà tracé
    sub.l   d1,d0       reste à tracer
    bls.s   .efface

.alltext
    jsr     _LVOText(a6)                Text()

.efface
    bsr     setapen
    move.w  rp_cp_x(a3),d0
    move.w  d2,d1
    move.w  d2,d3
    add.w   8+4(sp),d3         8+4(sp) == font height 8:d0/d2-d3/d7 4:bsr
    subq.w  #1,d3
    move.w  d6,d2
    move.w  d1,-(sp)
    bsr     aut_rec
    move.w  (sp)+,d1

    btst    #UIKNB_IMAGEFIRST,uikn_Flags+1(a5)
    beq.s   .end

    ;moveq   #0,d0                       pas collé à gauche
    ;move.b  uo_AutoRatio(a2),d0         1 ou 2
    ;add.w   d4,d0                  LeftOffset
    ;subq.w  #1,d0
    btst    #1,d7                       flag 1 = selected
    beq.s   .notsel
    movem.w d1/d3,-(sp)
    move.b  uol_SelectPen(a2),d3        d3 = couleur du fond de sélection
    bsr     setapen
    movem.w (sp)+,d1/d3
.notsel
    move.w  d4,d0
    move.w  d4,d2                  RightOffset
    add.w   #16,d2
    bsr     rectfill

    movem.l d4-d6/a2,-(sp)
    XREF    _UBM_Direc
    lea     _UBM_Direc,a0
    moveq   #0,d0                   srcx
    moveq   #0,d1                   srcy
    move.l  a3,a1                   destrp
    move.w  d4,d2                   destX
    addq.w  #1,d2
    move.w  d5,d3                   destY
    sub.w   ubm_Rows(a0),d3
    move.w  ubm_Width(a0),d4        sizeX
    move.w  ubm_Rows(a0),d5         sizeY
    move.l  ubm_Mask(a0),d6         bltmask
    addq.w  #UIKTOBMAP_OFFSET,a0    srcbm
    beq.b   .bltsimple
    move.l  d6,a2
    moveq   #$e0,d6                 minterm
    jsr     _LVOBltMaskBitMapRastPort(a6)
    bra.b   .bltdone
.bltsimple
    moveq   #$c0,d6                 minterm
    jsr     _LVOBltBitMapRastPort(a6)
.bltdone
    movem.l (sp)+,d4-d6/a2

.end
    move.b  uol_ListPen(a2),d3
    bsr     setapen
    bsr     setPenbpen
    movem.w (sp)+,d0/d2-d3/d7
    rts

DrawEnd
    move.w  (sp)+,d0
    move.b  d0,rp_Mask(a3)

    btst    #(BOXB_DISABLED-16),uo_Flags+1(a2)
    beq.b   .nopat
    bsr     draw_pat
.nopat
    move.l  _LayersBase,a6
    move.l  a2,a1
    bsr     _UIK_UnclipObject
    move.l  (a3),a0
    jsr     _LVOUnlockLayer(a6)
    move.l  a2,a0
    bsr     _UIK_UnlockObjVars
    bra     EndAll


*************************************
* UIK_OLi_DrawList( obj )
*               a0
*************************************
_UIK_OLi_DrawList
    move.l  #$00007fff,d1
    move.w  uol_Pos(a0),d0
    ext.l   d0
    ;------ fall into _UIK_OLi_DrawListFrom


****************************************
* UIK_OLi_DrawListFrom( obj, line, num )
*                       a0   d0    d1
****************************************
_UIK_OLi_DrawListFrom
    PUSHREGS
    tst.b   uo_Status(a0)           Obj->Status == STARTED ?
    bne     EndAllNoRP                  si non: rien a faire avec

    bsr     DrawInit
    move.b  rp_Mask(a3),d0
    move.w  d0,-(sp)
    bsr     _SetMask
    move.w  rp_TxHeight(a3),-(sp)       (sp) = font height
    bsr     DrawSet
    bne.b   .start
    bsr     aut_BoxPen
    bsr     aut_Rec
    bra.b   .alldone
;    beq     .rect
.start
    move.l  4(sp),d0                    line (2 pour rp_Mask et 2 pour rp_TxHeight)
    cmp.w   d1,d0                       d1 = première ligne affichée = Pos
    bpl.b   .ok1
    move.w  d1,d0                       on affiche au moins à partir de Pos
.ok1
    sub.w   d1,d0                       d0 = ligne depuis début d'affichage
    cmp.w   d3,d0                       d3 = nombre de lignes depuis Pos
    bmi.b   .ok2
    move.w  d3,d0                       si on dessine trop loin
    subq.w  #1,d0                       on dessine à partir de la dernière ligne
.ok2
    move.w  (sp),d1                     d1 = font height
    mulu    d0,d1                       d1 = height * line
    add.w   d1,d2
    add.w   d1,d5
    lsl.l   #3,d0                       UIKNODE_SIZE equ 8
    add.l   d0,a5
    move.l  4+4(sp),d0                  nbr de lignes à tracer
.loop
    bsr     DrawLine                    preserve d0
    subq.w  #1,d3
    beq.b   .rect
    subq.w  #1,d0
    beq.b   .alldone
    addq.l  #UIKNODE_SIZE,a5
    add.w   (sp),d2                     top += font height
    add.w   (sp),d5                     y += font height
    cmp.w   d5,d7                       if (y < maxy) continue
    bpl.b   .loop
.alldone
    addq.l  #2,sp
    bra     DrawEnd
.rect
    bsr     aut_BoxPen
    move.l  uo_Region(a2),a1            a1 = Region
    movem.w rg_bounds+ra_MinX(a1),d0-d3 d0=left, d1=top, d2=maxx, d3=maxy
    move.w  d5,d1
    sub.w   rp_TxBaseline(a3),d1
    add.w   (sp),d1
    bsr     aut_rec
    bra.b   .alldone


*************************************
* UIK_OLi_DrawLine( obj, line )
*                   a0   d0
*************************************
_UIK_OLi_DrawLine
    PUSHREGS
    tst.b   uo_Status(a0)           Obj->Status == STARTED ?
    bne     EndAllNoRP                  si non: rien a faire avec

    bsr     DrawInit
    move.b  rp_Mask(a3),d0
    move.w  d0,-(sp)
    bsr     _SetMask
    move.w  rp_TxHeight(a3),-(sp)       (sp) = font height
    bsr     DrawSet                     d1 = Pos
    beq.b   .end

    move.l  4(sp),d0                    restore line
    cmp.w   #-1,d0
    beq.b   .end
    sub.w   d1,d0                       d0 = line depuis début d'affichage
    bcs.b   .end
    cmp.w   d3,d0
    bpl.b   .end
    move.w  (sp),d1                     d1 = font height
    mulu    d0,d1                       d1 = height * line
    add.w   d1,d2
    add.w   d1,d5
    lsl.l   #3,d0                       UIKNODE_SIZE equ 8
    add.l   d0,a5
    bsr     DrawLine
.end
    addq.l  #2,sp
    bra     DrawEnd


*************************************
* IntUIK_DrawObjStrText( obj )
*
*************************************
_IntUIK_DrawObjStrText
    PUSHREGS
    move.l  REGOFFS+4(sp),a2            a2 = obj
    tst.b   uo_Status(a2)
    bne     EndAllNoRP

    move.l  a2,a0
    bsr     _UIK_LockObjVars

    ;move.l  a2,a0
    bsr     _IntUIK_CalcObjStr

    move.l  a2,a0
    bsr     _IntUIK_GetRastPort

    move.l  d0,a3                       a3 = rastport
    bsr     _VerifFont                  SetFont() if not the same

    move.l  _LayersBase,a6
    move.l  uo_Region(a2),a1            a1 = Region
    movem.w rg_bounds+ra_MinX(a1),d4-d7 d4=left, d5=top, d6=width, d7=height
    ;--------------------
    move.l   a2,a0
    bsr     _ZoneUpdateStart

    move.l  (a3),a1
    jsr     _LVOLockLayer(a6)
    move.l  a2,a1
    bsr     _UIK_ClipObject

    move.b  rp_Mask(a3),d0
    move.w  d0,-(sp)
    bsr     _SetMask

    move.l  _GfxBase,a6                 -- couleurs de fond et de texte
    btst    #BOXB_OVER,uo_Flags+3(a2)
    bne.b   .inact
    move.b  uos_ActTextPen(a2),d3
    bsr     setapen
    move.b  uos_ActBgPen(a2),d0
    bra.s   .colsok
.inact
    move.b  uos_InactTextPen(a2),d3
    bsr     setapen
    move.b  uos_InactBgPen(a2),d0
.colsok
    bsr     setbpen
    bsr     setdrmd2

    ;move.l  d5,d2                       sauve top
    add.l   uos_YOffset(a2),d5
    bsr     aut_Mxy                     -- Move( x, y )

    move.l  uos_Buffer(a2),a0
    move.l  uos_NumChars(a2),d0
    move.l  uos_CharOffset(a2),d1
    add.l   d1,a0                       a0 = string
    movem.l d4-d5/a0,-(sp)
    sub.l   d1,d0                       d0 = len
    bls.b   .nochar
    move.l  a3,a1                       a1 = rastport
    jsr     _LVOText(a6)                -- Text( rp, string, len )
.nochar
    move.b  uos_ActBgPen(a2),d3         -- efface la fin de la ligne
    btst    #BOXB_OVER,uo_Flags+3(a2)
    beq.b   .act
    move.b  uos_InactBgPen(a2),d3
.act
    bsr     setapen
    move.w  rp_cp_x(a3),d0              left
    ;move.w  d2,d1                       top
    ;move.w  d2,d3
    ;add.w   d7,d3                       + font height = bottom
        move.l  uo_ObjExt(a2),a0        a0 = extension
        move.l  uoe_Font(a0),a0
        move.w  d5,d1
        sub.w   tf_Baseline(a0),d1          top
        move.w  d1,d3
        add.w   tf_YSize(a0),d3             + font height = bottom
        subq.w  #1,d3
    move.l  d0,d2
    add.w   d6,d2                       + width = right
    bsr     aut_rec
    movem.l (sp)+,d4-d5/a5

    btst.b  #UIKSTB_CURSORON,uos_StrFlags+3(a2)
    beq.b   .end

    move.b  uos_ActBgPen(a2),d3
    bsr     setapen
    move.b  uos_ActTextPen(a2),d0
    bsr     setbpen

    move.l  a5,a1                       a1 = string
    move.l  uos_CursorPos(a2),d0
    sub.l   uos_CharOffset(a2),d0
    beq.b   .zero
    add.l   d0,a5                       a5 pointe le caractère du curseur
    move.l  rp_Font(a3),a0              a0 = font
    bsr     _UIK_StrWidth               jsr     _LVOTextLength(a6)          -- offset du curseur
    add.w   d0,d4
.zero
    bsr     aut_Mxy                     -- move curseur

    move.l  a5,a0                       a0 = string
    tst.b   (a0)
    bne.b   .te
    lea     SpaceString,a0
.te moveq   #1,d0                       d0 = len
    move.l  a3,a1                       a1 = rastport
    jsr     _LVOText(a6)                -- Text( rp, string, len )

.end
    move.w  (sp)+,d0
    move.b  d0,rp_Mask(a3)

    btst    #(BOXB_DISABLED-16),uo_Flags+1(a2)
    beq.b   .nopat
    bsr     draw_pat
.nopat
    move.l  _LayersBase,a6
    move.l  a2,a1
    bsr     _UIK_UnclipObject
    move.l  (a3),a0
    jsr     _LVOUnlockLayer(a6)

    move.l   a2,a0
    bsr     _ZoneUpdateEnd
    ;--------------------

    move.l  a2,a0
    bsr     _UIK_UnlockObjVars
    bra     EndAll


*************************************
* fonctions elementaires
*
*************************************

;    XDEF   _printdebug
;_printdebug
;        move.l  #EndSimFunctions-SimFunctions,d0
;        move.l  #EndFunctions-Functions,d1
;        DBUG    <'Equals: %ld - %ld',10>,d0,d1
;        rts

testlook
        move.l  _UIKBase,a0
        move.l  lb_UIKPrefs(a0),a0
        btst    #UIKB_SPLOOK,uikp_PrefsFlags+1(a0)
        rts

Raut_LookSkip
        move.w  (a5)+,d0
        bsr.b   testlook
        bne.b   Raut_Sk
        rts
Raut_NotLookSkip
        move.w  (a5)+,d0
        bsr.b   testlook
        beq.b   Raut_Sk
        rts
Raut_Skip
        move.w  (a5)+,d0
Raut_Sk add.w   d0,a5
        rts
Raut_NotLookPatOn
        bsr.b   testlook
        beq     setpaton
        rts
Raut_LookPatOn
        bsr.b   testlook
        bne     setpaton
        rts
Raut_NotWhiteSkip
        move.w  (a5)+,d0
        cmpi.b  #UIKCOL_WHITE,uo_BoxPen(a2)
        bne.b   Raut_Sk
        rts
Raut_Ratio1Skip
        moveq   #1,d1
        bra.b   Raut_RatioTest
Raut_Ratio2Skip
        moveq   #2,d1
Raut_RatioTest
        move.w  (a5)+,d0
        cmp.b   uo_AutoRatio(a2),d1
        beq.b   Raut_Sk
        rts
Raut_Gosub
        move.l  (a5)+,d0
        move.l  a5,-(sp)
        move.l  d0,a5
        bsr     ExeAuto
        move.l  (sp)+,a5
        rts

Functions
aut_end addq.l  #8,sp
        rts
aut_xa1 addq.w  #1,d4
        rts
aut_xs1 subq.w  #1,d4
        rts
aut_Xa1 addq.w  #1,d6
        rts
aut_Xs1 subq.w  #1,d6
        rts
aut_ya1 addq.w  #1,d5
        rts
aut_ys1 subq.w  #1,d5
        rts
aut_Ya1 addq.w  #1,d7
        rts
aut_Ys1 subq.w  #1,d7
        rts
aut_Mxy move.w  d4,d0
        move.w  d5,d1
        bra     move
aut_MxY move.w  d4,d0
        move.w  d7,d1
        bra     move
aut_MXy move.w  d6,d0
        move.w  d5,d1
        bra     move
aut_MXY move.w  d6,d0
        move.w  d7,d1
        bra     move
aut_Dxy move.w  d4,d0
        move.w  d5,d1
        bra     draw
aut_DxY move.w  d4,d0
        move.w  d7,d1
        bra     draw
aut_DXy move.w  d6,d0
        move.w  d5,d1
        bra     draw
aut_DXY move.w  d6,d0
        move.w  d7,d1
        bra     draw
aut_Gre move.b  #UIKCOL_GREY,d3
        bra     setapen
aut_Bla move.b  #UIKCOL_BLACK,d3
        bra     setapen
aut_Whi move.b  #UIKCOL_WHITE,d3
        bra     setapen
aut_Lig move.b  #UIKCOL_LIGHT,d3
        bra     setapen
aut_BoxPen move.b  uo_BoxPen(a2),d3
        bra     setapen
aut_BPe bra     setPenbpen
aut_POn bra     setpaton
aut_POf bra     setpatoff
aut_Rec move.w  d4,d0
        move.w  d5,d1
        move.w  d6,d2
        move.w  d7,d3
aut_rec cmp.w   d0,d2
        blt.b   .end
        cmp.w   d1,d3
        blt.b   .end
        bra     rectfill
.end    rts
aut_MxXy move.w d4,d0
        add.w   d6,d0
        lsr.w   #1,d0
        move.w  d5,d1
        bra     move
aut_MxXY move.w  d4,d0
        add.w   d6,d0
        lsr.w   #1,d0
        move.w  d7,d1
        bra     move
aut_MyYx move.w  d4,d0
        move.w  d5,d1
        add.w   d7,d1
        lsr.w   #1,d1
        bra     move
aut_MyYX move.w  d6,d0
        move.w  d5,d1
        add.w   d7,d1
        lsr.w   #1,d1
        bra     move
aut_DxXy move.w  d4,d0
        add.w   d6,d0
        lsr.w   #1,d0
        move.w  d5,d1
        bra     draw
aut_DxXY move.w  d4,d0
        add.w   d6,d0
        lsr.w   #1,d0
        move.w  d7,d1
        bra     draw
aut_DyYx move.w  d4,d0
        move.w  d5,d1
        add.w   d7,d1
        lsr.w   #1,d1
        bra     draw
aut_DyYX move.w  d6,d0
        move.w  d5,d1
        add.w   d7,d1
        lsr.w   #1,d1
        bra     draw
aut_fillpaton
        move.l  uo_Pattern(a2),a0
        bra     setfillpat
aut_fillpatoff
        suba.l  a0,a0
        bra     setfillpat
aut_Era move.b  uo_EraseColor(a2),d3
        bra     setapen
aut_xMI add.w   d6,d4
        lsr.w   #1,d4
        rts
aut_XMI add.w   d4,d6
        lsr.w   #1,d6
        rts
aut_yMI add.w   d7,d5
        lsr.w   #1,d5
        rts
aut_YMI add.w   d5,d7
        lsr.w   #1,d7
        rts
aut_xAn add.w   (a5)+,d4
        rts
aut_XAn add.w   (a5)+,d6
        rts
aut_yAn add.w   (a5)+,d5
        rts
aut_YAn add.w   (a5)+,d7
        rts
aut_xSn sub.w   (a5)+,d4
        rts
aut_XSn sub.w   (a5)+,d6
        rts
aut_ySn sub.w   (a5)+,d5
        rts
aut_YSn sub.w   (a5)+,d7
        rts
aut_LookSkip
        bra     Raut_LookSkip
aut_NotLookSkip
        bra     Raut_NotLookSkip
aut_Skip
        bra     Raut_Skip
aut_NotLookPatOn
        bra     Raut_NotLookPatOn
aut_LookPatOn
        bra     Raut_LookPatOn
aut_NotWhiteSkip
        bra     Raut_NotWhiteSkip
aut_Ratio1Skip
        bra     Raut_Ratio1Skip
aut_Ratio2Skip
        bra     Raut_Ratio2Skip
aut_Gosub
        bra     Raut_Gosub
aut_Loo move.b  #UIKCOL_LOOK,d3
        bra     setapen
aut_Imp move.b  #UIKCOL_IMP,d3
        bra     setapen
EndFunctions

***********************************************
* fonctions elementaires pour calcul uniquement
*
***********************************************

SimFunctions
sim_end addq.l  #8,sp
        rts
sim_xa1 addq.w  #1,d4
        rts
sim_xs1 subq.w  #1,d4
        rts
sim_Xa1 addq.w  #1,d6
        rts
sim_Xs1 subq.w  #1,d6
        rts
sim_ya1 addq.w  #1,d5
        rts
sim_ys1 subq.w  #1,d5
        rts
sim_Ya1 addq.w  #1,d7
        rts
sim_Ys1 subq.w  #1,d7
        rts
sim_Mxy rts
        ds.b    6
sim_MxY rts
        ds.b    6
sim_MXy rts
        ds.b    6
sim_MXY rts
        ds.b    6
sim_Dxy rts
        ds.b    6
sim_DxY rts
        ds.b    6
sim_DXy rts
        ds.b    6
sim_DXY rts
        ds.b    6
sim_Gre rts
        ds.b    6
sim_Bla rts
        ds.b    6
sim_Whi rts
        ds.b    6
sim_Lig rts
        ds.b    6
sim_BoxPen rts
        ds.b    6
sim_BPe rts
        ds.b    2
sim_POn rts
        ds.b    2
sim_POf rts
        ds.b    2
sim_Rec rts
        ds.b    20
sim_MxXy rts
        ds.b    10
sim_MxXY rts
        ds.b    10
sim_MyYx rts
        ds.b    10
sim_MyYX rts
        ds.b    10
sim_DxXy rts
        ds.b    10
sim_DxXY rts
        ds.b    10
sim_DyYx rts
        ds.b    10
sim_DyYX rts
        ds.b    10
sim_fillpaton
        rts
        ds.b    6
sim_fillpatoff
        rts
        ds.b    4
sim_Era rts
        ds.b    6
sim_xMI add.w   d6,d4
        lsr.w   #1,d4
        rts
sim_XMI add.w   d4,d6
        lsr.w   #1,d6
        rts
sim_yMI add.w   d7,d5
        lsr.w   #1,d5
        rts
sim_YMI add.w   d5,d7
        lsr.w   #1,d7
        rts
sim_xAn add.w   (a5)+,d4
        rts
sim_XAn add.w   (a5)+,d6
        rts
sim_yAn add.w   (a5)+,d5
        rts
sim_YAn add.w   (a5)+,d7
        rts
sim_xSn sub.w   (a5)+,d4
        rts
sim_XSn sub.w   (a5)+,d6
        rts
sim_ySn sub.w   (a5)+,d5
        rts
sim_YSn sub.w   (a5)+,d7
        rts
sim_LookSkip
        bra     Raut_LookSkip
sim_NotLookSkip
        bra     Raut_NotLookSkip
sim_Skip
        bra     Raut_Skip
sim_NotLookPatOn
        rts
        nop
sim_LookPatOn
        rts
        nop
sim_NotWhiteSkip
        bra     Raut_NotWhiteSkip
sim_Ratio1Skip
        bra     Raut_Ratio1Skip
sim_Ratio2Skip
        bra     Raut_Ratio2Skip
sim_Gosub
        bra     Raut_Gosub
sim_Loo rts
        ds.b    6
sim_Imp rts
        ds.b    6
EndSimFunctions

****************************************************************
*
        SECTION data,DATA
*
****************************************************************/

SpaceString
    dc.b    $20,0


        END
