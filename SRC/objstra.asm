   OPT O+,W-
        OPT d+
****************************************************************
*
*       Project:    UIK
*       Function:   string gadget
*
*       Created:    17/05/90    Jean-Michel Forgeas
*
****************************************************************

        IDNT    objstra


******* Includes ************************************************

        INCDIR  "include:"

        INCLUDE "lib/graphics_lib.i"
        INCLUDE "lib/intuition_lib.i"
        INCLUDE "exec/types.i"
        INCLUDE "intuition/intuition.i"
        INCLUDE "uiki:uikglobal.i"
        INCLUDE "uiki:uikmacros.i"
        INCLUDE "uiki:uikbox.i"
        INCLUDE "uiki:uikbase.i"
        INCLUDE "uiki:objstring.i"


******* Imported ************************************************

        XREF    _IntUIK_DrawObjStrText

        XREF    _UIK_nl_PasteToNode
        XREF    _UIK_nl_CutFromNode
        XREF    _UIK_LockObjVars
        XREF    _UIK_UnlockObjVars
        XREF    _Str2Num
        XREF    _BufResize
        XREF    _BufCut
        XREF    _BufPaste
        XREF    _BufCopy
        XREF    _BufSet

        XREF    _UIK_ActivateKeyTank
        XREF    _UIK_InactivateKeyTank
        XREF    _IntUIK_PushBox
        XREF    _IntUIK_PopBox

        XREF    _UIKBase
        XREF    _IntuitionBase


******* Exported ************************************************

        XDEF    _UIK_OSt_Get
        XDEF    _UIK_OSt_Set
        XDEF    _UIK_OSt_Cut
        XDEF    _UIK_OSt_Paste
        XDEF    _UIK_CharWidth
        XDEF    _UIK_StrWidth

        XDEF    _IntUIK_CalcObjStr

        XDEF    _Hdl_ActivateKeyTank
        XDEF    _Hdl_InactivateKeyTank
        XDEF    _HdlUIK_ActivateStrGad
        XDEF    _HdlUIK_NewKey2StrGad
        XDEF    _HdlUIK_DigitalInt
        XDEF    _HdlUIK_DigitalHex



        SECTION text,CODE


*******************************************
*   buf = UIK_OSt_Get( obj, plen )
*   d0                 a0   a1
*******************************************
_UIK_OSt_Get
        bsr     _UIK_LockObjVars
        move.l  a1,d0
        beq.b   .nolen
        move.l  uos_NumChars(a0),(a1)
.nolen
        move.l  uos_Buffer(a0),d0
        bsr     _UIK_UnlockObjVars
        rts


*******************************************
*   buf = UIK_OSt_Set( obj, src, len )
*   d0                 a0   a1   d0
*******************************************

_UIK_OSt_Set
        movem.l d3/a2-a4,-(sp)
        move.l  a0,a2                   a2 = obj
        move.l  a1,a4                   a4 = src
        move.l  d0,d3                   d3 = len
        bsr     _UIK_LockObjVars

        move.l  a4,a0                   UBYTE *src
        move.l  d3,d0                   ULONG len
        move.l  uos_Buffer(a2),a1
        bsr     _BufSet
        tst.l   d0
        beq.b   .end                    si erreur -> fin

        move.l  d0,a3
        bsr.b   actualize
        move.l  a3,d0
.end
        move.l  a2,a0
        bsr     _UIK_UnlockObjVars
        movem.l (sp)+,d3/a2-a4
        rts

actualize
        move.l  -nubu_CurSize(a3),d3
        move.l  uos_MaxChars(a2),d0     maximum ?
        beq.b   .nolimit
        cmp.l   d3,d0                   max - len
        bcc.l   .nolimit                if (reste >= 0) ok
        move.l  d0,d3                   len = max
.nolimit
        move.l  d3,uos_NumChars(a2)
        move.l  a3,uos_Buffer(a2)
        clr.b   0(a3,d3.l)

        cmp.l   uos_CursorPos(a2),d3
        bcc.b   .disp
        move.l  d3,uos_CursorPos(a2)
.disp
        move.l  a2,a0
        bsr     FitObjStr

        move.l  a2,-(sp)
        bsr     _IntUIK_DrawObjStrText
        addq.w  #4,sp
.Str2Num
        move.l  uos_LongIntResult(a2),d0
        beq.b   .ok
        move.l  a3,a0
        bsr     _Str2Num
        move.l  uos_LongIntResult(a2),a0
        move.l  d0,(a0)
.ok
        rts


*******************************************
*   buf = UIK_OSt_Cut( obj, start, len )
*   d0                 a0   d0     d1
*******************************************
_UIK_OSt_Cut
        movem.l d3/a2-a3,-(sp)
        move.l  a0,a2                   a2 = obj
        move.l  d1,d3
        bsr     _UIK_LockObjVars

        move.l  uos_Buffer(a2),a0
        move.l  d3,d1
        bsr     _BufCut
        tst.l   d0
        beq.b   .end

        move.l  d0,a3
        bsr.b   actualize
        move.l  a3,d0
.end
        move.l  a2,a0
        bsr     _UIK_UnlockObjVars
        movem.l (sp)+,d3/a2-a3
        rts


***********************************************
*   buf = UIK_OSt_Paste( obj, buf, start, len )
*   d0                   a0   a1    d0    d1
***********************************************
_UIK_OSt_Paste
        movem.l d3/a2-a3,-(sp)
        move.l  a0,a2                   a2 = obj
        move.l  a1,a3                   a3 = src
        move.l  d1,d3                   d3 = len
        bsr     _UIK_LockObjVars        d0 préservé

        move.l  a3,a0
        move.l  uos_Buffer(a2),a1
        move.l  d0,d1             start
        move.l  d3,d0             len
        bsr     _BufPaste
        tst.l   d0
        beq.b   .end

        add.l   d3,uos_CursorPos(a2)
        move.l  d0,a3
        bsr     actualize
        move.l  a3,d0
.end
        move.l  a2,a0
        bsr     _UIK_UnlockObjVars
        movem.l (sp)+,d3/a2-a3
        rts


**********************************
*   FitObjStr( obj )
*                     a0
**********************************
FitObjStr
        movem.l d2-d5/a2-a3/a5-a6,-(sp)
        move.l  a0,a5
        move.l  _UIKBase,a6
        move.l  uos_CharOffset(a5),d4           d4 = 1er affiché
        beq.s   .end
        move.l  uos_Buffer(a5),a2
        move.l  -nubu_CurSize(a2),d5            d5 = buflen
        beq.s   .end

        move.l  uo_Region(a5),d0
        beq.s   .end
        move.l  d0,a0
        move.w  rg_bounds+ra_MaxX(a0),d3        d3 = maxx
        sub.w   rg_bounds+ra_MinX(a0),d3        d3 = maxx - minx = width - 1

        move.l  d5,d2                           d2 = buflen = charnum
        ;sub.l   d4,d2                           d2 = numchars après début d'affichage
        add.l   d5,a2
        subq.w  #1,a2                           a2 = adresse du dernier char
        moveq   #0,d4                           d4 = compteur de chars
        move.l  uo_ObjExt(a5),a0
        move.l  uoe_Font(a0),a3                 a3 = fonte de l'objet
.loop
        tst.l   d2
        beq.b   .placedeb
        move.l  a3,a0
        move.b  (a2),d0
        bsr     _UIK_CharWidth
        sub.w   d0,d3
        bcs.b   .place
        addq.l  #1,d4
        subq.l  #1,d2
        subq.l  #1,a2
        bra.b   .loop
.placedeb
        moveq   #0,d5
        bra.s   .put
.place
        sub.l   d4,d5                           CharOffset = buflen - nbr visibles
        cmp.l   uos_CharOffset(a5),d5
        bcc.s   .end
.put
        move.l  d5,uos_CharOffset(a5)
.end
        movem.l (sp)+,d2-d5/a2-a3/a5-a6
        rts


**********************************
*   IntUIK_CalcObjStr( obj )
*                      a0
**********************************
_IntUIK_CalcObjStr
        movem.l d2-d5/a2-a3/a5-a6,-(sp)
        tst.b   uo_Status(a0)           Obj->Status == STARTED ?
        bne.b   .end                    si non: rien a faire avec

        move.l  a0,a5
        move.l  _UIKBase,a6

        move.l  uo_Region(a5),a0
        move.w  rg_bounds+ra_MaxX(a0),d3        d3 = maxx
        sub.w   rg_bounds+ra_MinX(a0),d3        d3 = maxx - minx = width - 1
        move.l  uos_CharOffset(a5),d4           d4 = 1er affiché
        move.l  uos_CursorPos(a5),d5            d5 = curseur

        cmp.l   d4,d5
        bcc.b   .sup
        move.l  d5,uos_CharOffset(a5)
        bra.b   .end
.sup
        move.l  uos_Buffer(a5),a2
        add.l   d5,a2                           a2 = adresse du curseur
        move.l  d5,d2
        sub.l   d4,d2                           d2 = nbr de chars à afficher
        moveq   #0,d4                           d4 = compteur de chars
        move.l  uo_ObjExt(a5),a0
        move.l  uoe_Font(a0),a3                 a3 = fonte de l'objet
.loop
        tst.l   d2
        beq.b   .end
        move.l  a3,a0
        move.b  (a2),d0
        bsr.b   _UIK_CharWidth
        sub.w   d0,d3
        bcs.b   .place
        addq.l  #1,d4
        subq.l  #1,d2
        subq.l  #1,a2
        bra.b   .loop
.place
        sub.l   d4,d5                           CharOffset = curseur - nbr visibles
        move.l  d5,uos_CharOffset(a5)
.end
        movem.l (sp)+,d2-d5/a2-a3/a5-a6
        rts


****************************************
*   len = UIK_StrWidth( font, str, len )
*   d0                  a0    a1   d0
****************************************
_UIK_StrWidth
        movem.l a2/a3/d2/d3,-(sp)
        move.l  a0,a2
        move.l  a1,a3
        move.l  d0,d2
        moveq   #0,d3
        bra.b   .next
.loop       move.b  (a3)+,d0
            move.l  a2,a0
            bsr.b   _UIK_CharWidth
            add.l   d0,d3
.next   subq.l  #1,d2
        bcc.b   .loop
        move.l  d3,d0
        movem.l (sp)+,a2/a3/d2/d3
        rts


**********************************
*   len = UIK_CharWidth( font, c )
*   d0                   a0   d0:8
**********************************
_UIK_CharWidth
        move.b  tf_HiChar(a0),d1                ; if (c>font->tf_HiChar
        cmp.b   d1,d0
        bhi.b   .higher                         ; inférieur non signé
        sub.b   tf_LoChar(a0),d0                ; || (c-=font->tf_LoChar)<0)
        bcc.b   .goodoffset                     ; retenue => supérieur non signé
.higher
        sub.b   tf_LoChar(a0),d1
        addq.b  #1,d1                           ; c=font->tf_HiChar-font->tf_LoChar+1;
        exg     d1,d0
.goodoffset                                     ; d0 = offset
        moveq   #0,d1
        move.b  d0,d1
        add.w   d1,d1                           ; offset sur des words => c*2
        move.w  tf_XSize(a0),d0                 ; d0 = font->tf_XSize;
        movem.l tf_CharSpace(a0),a0-a1          ; a0 = tf_CharSpace, a1 = tf_CharKern
.space
        cmpa.l  #0,a0
        beq.b   .kern
        move.w  0(a0,d1.w),d0                   ; ((SHORT *)font->tf_CharSpace)[c]
.kern
        cmpa.l  #0,a1
        beq.b   .end
        add.w   0(a1,d1.w),d0                   ; + ((SHORT *)font->tf_CharKern)[c]
.end
        ext.l   d0
        rts


*******************************************
*   HdlUIK_ActivateStrGad( obj, imsg, uik )
*                          a1   a3    a5
*******************************************
_HdlUIK_ActivateStrGad
        movem.l d2-d4/a2-a3,-(sp)
        move.w  im_MouseX(a3),d2
        move.l  a1,a3

        move.l  a1,a0
        bsr     _UIK_LockObjVars
        bne .end

        move.l  uo_rq(a3),d1                d0 = requester
        beq.b   .win                        si pas de requester
        move.l  d1,a0
        sub.w   rq_LeftEdge(a0),d2          offset du requester
.win
        move.l  uo_Region(a3),a0
        sub.w   rg_bounds+ra_MinX(a0),d2        d2 = x offset du premier char affiché

        move.l  uos_Buffer(a3),a2
        move.l  uos_CharOffset(a3),d3           d3 = numéro du char clické
        add.l   d3,a2                           a2 = adresse du premier char affiché
        move.l  uos_NumChars(a3),d4
        sub.l   d3,d4
.loop
        tst.l   d4
        beq.b   .place
        move.l  uo_ObjExt(a3),a0
        move.l  uoe_Font(a0),a0                 a0 = fonte de l'objet
        move.b  (a2)+,d0
        bsr     _UIK_CharWidth
        sub.w   d0,d2
        ble.b   .place
        addq.l  #1,d3
        subq.l  #1,d4
        bra.b   .loop
.place
        move.l  d3,uos_CursorPos(a3)

        move.l  uike_ActiveKeyTank(a5),a0
        cmp.l   a3,a0
        beq.b   .active
        bsr     _UIK_InactivateKeyTank
.active
        move.l  a3,-(sp)

        move.l  a3,uike_ActiveKeyTank(a5)
        bset.b  #UIKSTB_CURSORON,uos_StrFlags+3(a3)
        bne.b   .same

        move.l  uo_AltAutomate(a3),d0
        cmp.l   uo_Automate(a3),d0
        beq.b   .same
        bsr     _IntUIK_PushBox
.same   bsr     _IntUIK_DrawObjStrText

        addq.l  #4,sp
.end
        move.l  a3,a0
        bsr     _UIK_UnlockObjVars
        movem.l (sp)+,d2-d4/a2-a3
        rts


*********************************************
*   Hdl_ActivateKeyTank( obj )
*                        a0
*********************************************
_Hdl_ActivateKeyTank
        bset.b  #UIKSTB_CURSORON,uos_StrFlags+3(a0)
        bne.b   .end
        move.l  a0,-(sp)

        move.l  uos_ActiveFunc(a0),d0
        beq.s   .nofunc
        move.l  d0,a1
        jsr     (a1)
        move.l  (sp),a0
.nofunc
        move.l  uo_AltAutomate(a0),d0
        cmp.l   uo_Automate(a0),d0
        beq.b   .same
        bsr     _IntUIK_PushBox
.same   bsr     _IntUIK_DrawObjStrText
        addq.l  #4,sp
.end
        rts


*********************************************
*   Hdl_InactivateKeyTank( obj )
*                          a0
*********************************************
_Hdl_InactivateKeyTank
        bclr.b  #UIKSTB_CURSORON,uos_StrFlags+3(a0)
        beq.b   .end

        move.l  a0,-(sp)
        move.l  uos_InactiveFunc(a0),d0
        beq.s   .nofunc
        move.l  d0,a1
        jsr     (a1)
        move.l  (sp),a0
.nofunc
        move.l  uo_AltAutomate(a0),d0
        cmp.l   uo_Automate(a0),d0
        beq.b   .same
        bsr     _IntUIK_PopBox
.same   bsr     _IntUIK_DrawObjStrText
        addq.l  #4,sp
.end
        rts


*********************************************
*   HdlUIK_NewKey2StrGad( obj, imsg, uik )
*                         a1   a3    a5
*********************************************
_HdlUIK_NewKey2StrGad
        movem.l d2-d5/a1-a3/a6,-(sp)
        move.l  a1,a2               a2 = object

        move.l  a1,a0
        bsr     _UIK_LockObjVars

        move.l  uos_Buffer(a2),a6           a6 = buffer

        ;------ examine nothing or UP
        movem.w im_Code(a3),d3-d4           d4 = im_Qualifier
        move.b  d3,d0
        cmp.b   #$60,d3
        bcs.b   .perhapscmd
        cmp.b   #$68,d3                      touche spéciale sans caractère
        bcs    .end                          = CapsLock, ctrl, shft, alt, Amiga

        ;------ examine if command or buffer key
.perhapscmd
        btst    #IECODEB_UP_PREFIX,d3       touche relevée ?
        bne     .end

        ;------ empile l'adresse de retour
        pea     .redisplay(pc)                      POUR faire des simples branch
        move.l  uos_CursorPos(a2),d5                d5 = cursor

        move.l  uos_StrFlags(a2),d0
        btst    #UIKSTB_NOFILTER,d0
        bne     .ins

        btst    #IEQUALIFIERB_LCOMMAND,d4   touche Amiga ?
        bne     .other
        btst    #IEQUALIFIERB_RCOMMAND,d4
        bne     .other
        btst    #IEQUALIFIERB_CONTROL,d4
        bne     .other
.bsp
        cmp.b   #$41,d3            --------- bsp          d3 = im_Code
        bne.b   .del
        tst.l   d5
        beq     .ignore                 if cursor == 0 rien
        btst    #IEQUALIFIERB_LSHIFT,d4
        bne.b   .bs
        btst    #IEQUALIFIERB_RSHIFT,d4
        beq.b   .bns
.deldebutbuf
.bs     moveq   #0,d0                   start = 0
        move.l  d5,d1                   len = cursor = depuis le début
        moveq   #0,d5                   cursor = 0
        bra.b   .cut
.delleftchar
.bns    subq.l  #1,d5                   cursor--
        move.l  d5,d0                   start = cursor
        moveq   #1,d1                   len = 1
        bra.b   .cut
.del
        cmp.b   #$46,d3            --------- del
        bne.b   .ent
        cmp.l   uos_NumChars(a2),d5       if curs >= numchars rien
        bcc     .ignore
.d
        btst    #IEQUALIFIERB_LSHIFT,d4
        bne.b   .ds
        btst    #IEQUALIFIERB_RSHIFT,d4
        beq.b   .dns
.delfinbuf
.ds     move.l  d5,d0                   start
        moveq   #-1,d1                  len = -1 = jusqu'à la fin
        bra.b   .cut
.delchar
.dns    move.l  d5,d0                   start
        moveq   #1,d1                   len
.cut
        move.l  a6,a0                   buf
        bsr     _BufCut
        tst.l   d0
        beq     .ignore
        move.l  d0,a6
        move.l  -nubu_CurSize(a6),uos_NumChars(a2)
        move.l  a2,a0
        bsr     FitObjStr
        bra     .buffer_processing
.ent
        cmp.b   #$43,d3            --- ent
        beq.b   .doret
.ret
        cmp.b   #$44,d3            --- ret
        bne.b   .esc
.doret
        bsr     savebuf
        bra     .ignore
.esc
        cmp.b   #$45,d3            --- esc
        bne.b   .left
        bra     .ignore
.left
        cmp.b   #$4f,d3            --- left
        bne.b   .right
        tst.l   d5
        beq     .ignore                   if cursor == 0 rien
        btst    #IEQUALIFIERB_LSHIFT,d4
        bne.b   .ls
        btst    #IEQUALIFIERB_RSHIFT,d4
        beq.b   .lns
.godebut
.ls     moveq   #1,d5
.lns    subq.l  #1,d5                     cursor--
        rts             dépile pea
.right
        cmp.b   #$4e,d3            --- right
        bne.b   .up
        cmp.l   uos_NumChars(a2),d5       if curs >= numchars rien
        bcc     .ignore
        btst    #IEQUALIFIERB_LSHIFT,d4
        bne.b   .rs
        btst    #IEQUALIFIERB_RSHIFT,d4
        beq.b   .rns
.gofin
.rs     move.l  uos_NumChars(a2),d5
        rts             dépile pea
.rns    addq.l  #1,d5                     cursor++
        rts             dépile pea
.up
        cmp.b   #$4c,d3            --- up
        bne.b   .down
        bra     .ignore
.down
        cmp.b   #$4d,d3            --- down
        bne.b   .tab
        bra     .ignore
.tab
        cmp.b   #$42,d3            --- tab
        bne.b   .other
        bra     .ignore
.other
        move.l  _UIKBase,a1
        move.b  lb_KeySupp+uks_KeyBuffer(a1),d1 d1 = ANSI char
        moveq   #1,d0
        cmp.l   lb_KeySupp+uks_Length(a1),d0
        bne     .ins
        ;--------------------- CTRL key ?
        btst    #IEQUALIFIERB_CONTROL,d4
        beq.b   .amiga
        btst    #IEQUALIFIERB_LCOMMAND,d4
        bne     .ins

        cmp.b   #$01,d1     'A'
        beq.b   .godebut

        cmp.b   #$18,d1     'X'
        beq.b   .erase

        cmp.b   #$1a,d1     'Z'
        beq.b   .gofin

        cmp.b   #$08,d1     'H'
        beq     .delleftchar

        cmp.b   #$0b,d1     'K'
        beq     .delfinbuf

        cmp.b   #$15,d1     'U'
        beq     .deldebutbuf

        cmp.b   #$0d,d1     'M'
        beq     .doret

        cmp.b   #$17,d1     'W'
        bne     .ins

        move.l  d5,d0       start
        moveq   #0,d1       len
.space
        subq.l  #1,d0
        bcs     .deldebutbuf
        addq.l  #1,d1
        cmp.b   #' ',0(a6,d0.l)
        beq.b   .space
.loop
        subq.l  #1,d0
        bcs     .deldebutbuf
        cmp.b   #' ',0(a6,d0.l)
        beq.b   .loopend
        addq.l  #1,d1
        bra.b   .loop
.loopend
        addq.l  #1,d0
        move.l  d0,d5
        bra     .cut

        ;--------------------- AMIGA key ?
.amiga
        btst    #IEQUALIFIERB_RCOMMAND,d4
        beq.b   .ins

.cbcopykey
        cmp.b   #'c',d1
        bne.b   .cbpastekey
        bra     .ignore
.cbpastekey
        cmp.b   #'v',d1
        bne.b   .erasekey
        bra     .ignore

.erasekey
        cmp.b   #'x',d1
        bne.b   .undokey
.erase  bsr     savebuf
        move.l  a6,a0
        moveq   #0,d0
        bsr     _BufResize              efface le contenu du buffer
        bra.b   .finspecial
.undokey
        cmp.b   #'q',d1
        bne.b   .ins
        move.l  uos_UndoBuffer(a2),a0   from = UndoBuffer
        move.l  a6,a1                   to = Buffer
        moveq   #0,d0                   start = 0
        moveq   #-1,d1                  len = all
        bsr     _BufCopy
.finspecial
        tst.l   d0
        beq     .ignore
        move.l  d0,a6
        move.l  d0,a3
        bsr     actualize
        bra     .ignore

.ins
        ;--------------------- prepare values
        move.l  _UIKBase,a1
        move.l  lb_KeySupp+uks_Length(a1),d2    d2 = input length
        beq     .ignore
        lea     lb_KeySupp+uks_KeyBuffer(a1),a1 a1 = input buffer
        ;------ call user hook
        move.l  uos_UserHook(a2),d0
        beq.b   .nohook
        move.l  a6,uos_Buffer(a2)
        move.l  d0,a0      userhook( d2,  d3,   d4,   d5,     a1,  a3,   a5,  a2,     a6 )
        jsr     (a0)                 len, code, qual, curpos, buf, imsg, uik, objstr, objbuf
        tst.l   d0
        beq.b   .ignore                 user abort
.hookok
        move.l  d0,a1                   take new src buffer
.nohook ;-------
        move.l  uos_MaxChars(a2),d0     maximum ?
        beq.b   .nolimit
        move.l  uos_NumChars(a2),d3
        add.l   d2,d3                   totlen
        sub.l   uos_MaxChars(a2),d3     reste = totlen - max
        bls.l   .nolimit                if (reste < 0) ok
        sub.l   d3,d2                   len -= reste
        move.l  a6,-(sp)
        move.l  _IntuitionBase,a6       do beep
        move.l  ib_ActiveScreen(a6),a0
        jsr     _LVODisplayBeep(a6)
        move.l  (sp)+,a6
.nolimit
        move.l  d2,d0                   d0 = len
        move.l  d5,d1                   d1 = start = curpos
        move.l  a6,a0                   a0 = UIKBuffer
        exg     a0,a1
        bsr     _BufPaste
        tst.l   d0
        beq.b   .ignore
        move.l  d0,a6
        add.l   d2,d5
        add.l   d2,uos_NumChars(a2)

.buffer_processing
        move.l  uos_LongIntResult(a2),d0
        beq.b   .noconv
        move.l  a6,a0
        bsr     _Str2Num
        move.l  uos_LongIntResult(a2),a0
        move.l  d0,(a0)
.noconv
        rts             dépile pea

.redisplay
        move.l  d5,uos_CursorPos(a2)
        move.l  a6,uos_Buffer(a2)
        move.l  a2,-(sp)
        bsr     _IntUIK_DrawObjStrText
        move.l  (sp)+,a1

.end
        move.l  a6,uos_Buffer(a2)
        move.l  a2,a0
        bsr     _UIK_UnlockObjVars
        movem.l (sp)+,d2-d5/a1-a3/a6
        rts

.ignore addq.l  #4,sp   this is used to unstack the pea.
        bra.b   .end

savebuf
        move.l  a6,a0                   from = Buffer
        move.l  uos_UndoBuffer(a2),a1   to = UndoBuffer
        moveq   #0,d0                   start = 0
        move.l  #-1,d1                  len = all
        bsr     _BufCopy
        tst.l   d0
        beq     .end
        move.l  d0,uos_UndoBuffer(a2)
.end    rts

; input string handlers

_HdlUIK_DigitalInt
        move.b  (a1),d0
        cmp.b   #',',d0
        beq.b   endok
        cmp.b   #'.',d0
        beq.b   endok
        cmp.b   #'-',d0
        beq.b   endok
        cmp.b   #'+',d0
        beq.b   endok
        cmp.b   #'0',d0
        blt.b   endno
        cmp.b   #'9',d0
        bgt.b   endno

endok   move.l  a1,d0
        rts
endno   moveq   #0,d0
        rts


_HdlUIK_DigitalHex
        move.b  (a1),d0
        cmp.b   #'-',d0
        beq.b   endok
        cmp.b   #'+',d0
        beq.b   endok
        cmp.b   #'$',d0
        beq.b   endok
        cmp.b   #'x',d0
        beq.b   endok
        cmp.b   #'0',d0
        blt.b   endno
        cmp.b   #'9',d0
        ble.b   endok
        cmp.b   #'A',d0
        blt.b   endno
        cmp.b   #'F',d0
        ble.b   endok
        cmp.b   #'a',d0
        blt.b   endno
        cmp.b   #'f',d0
        ble.b   endok
        bra.b   endno


*************************************
        END
*************************************

