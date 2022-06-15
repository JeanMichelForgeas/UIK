    OPT O+,W-
*****************************************************************
*
*       Project:    UIKPrefs
*       Function:   Mode par défaut
*
*       Created:    27/08/92    Jean-Michel Forgeas
*
*****************************************************************


******* Includes ************************************************

        INCLUDE "uiki:uikbase.i"
        INCLUDE "uiki:uik_lib.i"
        INCLUDE 'i:lib/graphics_lib.i'
        INCLUDE 'i:lib/intuition_lib.i'
        INCLUDE 'i:graphics/view.i'
        INCLUDE 'i:graphics/displayinfo.i'
        INCLUDE 'i:intuition/screens.i'


******* Imported ************************************************

        XREF    _UIKBase
        XREF    _GfxBase
        XREF    _IntuitionBase


******* Exported ************************************************

        XDEF    _SetDefaultMode


*****************************************************************


*****************************************************************
*
                    SECTION     text,CODE
*
*****************************************************************


********************************************
*   SetDefaultMode( struct UIKPrefs *prefs )
*                           pile
********************************************
_SetDefaultMode
    move.l  4(sp),a0
    movem.l d2-d3/a2-a3/a6,-(sp)
    move.l  a0,a3
    move.l  #V_HIRES,d2                  d2=modes, d3=width/height
    move.l  #$028000c8,d3                640/200
    move.l  _UIKBase,a6
    jsr     _LVOUIK_IsSystemVersion2(a6)
    bne.b   .vp2
.vp1                                     ;--------
    sub.w   #90,sp
    move.l  sp,a2
    move.l  sp,a0
    sub.l   a1,a1
    move.l  #84,d0
    move.l  #WBENCHSCREEN,d1
    move.l  _IntuitionBase,a6
    jsr     _LVOGetScreenData(a6)
    add.w   #90,sp
    tst.l   d0
    beq.b   .finvm
    moveq   #0,d2
    move.w  sc_ViewPort+vp_Modes(a2),d2
    move.l  sc_Width(a2),d3
    bra.b   .finvm
.vp2                                     ;--------
    lea     wbname(pc),a0
    move.l  _IntuitionBase,a6
    jsr     _LVOLockPubScreen(a6)
    tst.l   d0
    beq.b   .finvm
    move.l  d0,a2
    move.l  sc_Width(a2),d3
    lea     sc_ViewPort(a2),a0
    move.l  _GfxBase,a6
    jsr     _LVOGetVPModeID(a6)
    cmp.l   #INVALID_ID,d0
    beq.b   .cl
    move.l  d0,d2
.cl sub.l   a0,a0
    move.l  a2,a1
    move.l  _IntuitionBase,a6
    jsr     _LVOUnlockPubScreen(a6)

.finvm                                  ;--------
    movem.l d2-d3,uikp_DefScreen_ModeId(a3)
    movem.l (sp)+,d2-d3/a2-a3/a6
    rts

wbname  dc.b    'Workbench',0



*****************************************************************
        END
