    OPT O+,W-
        OPT O1+                 ; Tells when a branch could be optimised to short
        OPT i+                  ; Tells when '#' is probably missing
        OPT d+
****************************************************************
*
*       Project:    UIK
*       Function:   obj gadget
*
*       Created:    11/03/91    Jean-Michel Forgeas
*
****************************************************************

        IDNT    objgada


******* Includes ************************************************

        INCDIR  "include:"

        INCLUDE "uiki:uikobj.i"
        INCLUDE "uiki:uikglobal.i"


******* Imported ************************************************

        XREF    _IntUIK_PushBox
        XREF    _IntUIK_PopBox
        XREF    _UIK_InsideObj


******* Exported ************************************************

        XDEF    _HdlUIK_VerifActGadget


****************************************************************
*
*       Routines
*
****************************************************************/

        SECTION text,CODE


****************************************************
*   HdlUIK_VerifActGadget()
*   installée par AddGadget()
*   A cause du bug sous 2.0 qui met le flag SELECTED
*   apres le putmsg() de Intuition, ne pas utiliser
*   cette routine dans un champ ActVector.
****************************************************

_HdlUIK_VerifActGadget
    move.l  4(sp),a0
    btst    #7,uog_Gad+gg_Flags+1(a0)       selected ?
    bne.b   .sel
    jmp     _IntUIK_PopBox(pc)
.sel
    jmp     _IntUIK_PushBox(pc)

    END

;_HdlUIK_VerifActGadget
;    movem.l a2/a3,-(sp)
;    lea     _IntUIK_PushBox(pc),a2
;    lea     _IntUIK_PopBox(pc),a3
;
;    move.l  12(sp),a0
;    btst    #7,uog_Gad+gg_Flags+1(a0)       selected ?
;    bne.b   .sel
;    exg     a2,a3
;.sel
;    move.l  im_MouseX(a3),a1
;    bsr     _UIK_InsideObj
;    tst.b   d0
;    beq.b   .outside
;    exg     a2,a3
;.outside
;    move.l  a0,-(sp)
;    jsr     (a2)
;    move.l  (sp)+,a0
;.end
;    movem.l (sp)+,a2/a3
;    rts


