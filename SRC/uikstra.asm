    OPT O+ ,W-
        OPT d+
****************************************************************
*
*       Project:    UIK
*       Function:   string manipulation
*
*       Created:    17/05/90    Jean-Michel Forgeas
*
****************************************************************

        IDNT    strings

******* Includes ************************************************


******* Imported ************************************************


******* Exported ************************************************

        XDEF    _StrNCSub


****************************************************************
*
*       Routines
*
****************************************************************/

        SECTION text,CODE


**********************************************************************
*   len = StcCpy( dest, source, len )
*                 d0    a0      a1
**********************************************************************

    XDEF    _StcCpy

_StcCpy:
;         movem.l  d6-d7/a2-a3,-(sp)
;         movea.l  d0,a3         ; a3=dest
;         movea.l  a0,a2         ; a2=src
;         move.l   a1,d7         ; d7=len
;L2:      tst.b    (a0)+
;         bne.s    L2
;         subq.w   #1,a0         ; mot etendu a 32 bits avant sub
;         suba.l   a2,a0
;         move.l   a0,d6         ; longeur chaine source (sans 0)
;         cmp.l    d7,d6
;         bcs.s    L3            ; dst<src : Pas de debordement
;         move.l   d7,d6         ; longeur totale (avec 0)
;         subq.l   #1,d6
;L3:      move.l   d6,d0         ; d0=len (sans 0)
;         movea.l  a2,a0         ; a0=src
;         movea.l  a3,a1         ; a1=dest
;         bra.s    L5
;
;L4:      move.b   (a0)+,(a1)+
;L5:      subq.l   #1,d0
;         bcc.s    L4
;         clr.b    (a3,d6.l)
;         move.l   d6,d0
;         addq.l   #1,d0         ; +1 pour le 0 final
;         movem.l  (sp)+,d6-d7/a2-a3
;         rts

; autre methode

*   len = StcCpy( dest, source, len )
*                 d0    a0      a1


        move.l  a1,d1           ; d1=len
        movea.l d0,a1           ; a1=dest
        move.l  a0,d0

.Loop:
        subq.l  #1,d1
        bcs.s   .Overflow       ; debordement vers negatif
        move.b  (a0)+,(a1)+
        bne.s   .Loop           ; pas de 0 dans src

        ; en sortie de boucle : a0 pointe sur le 1er caractere non recopie
        ; si overflow
.End:
        suba.l  d0,a0
        move.l  a0,d0           ; len (avec 0)

        rts

.Overflow:
        clr.b   -1(a1)
        bra.s   .End


**********************************************************************
*   s+, num = Str2Num( s )
*   a0  d0             a0
**********************************************************************

    XDEF    _Str2Num

_Str2Num:
        movem.l d2/d3,-(sp)
        moveq   #0,d0           ; start with zero
        move.l  a0,d1
        beq     errorzero

        moveq   #0,d1           ; clear upper bytes
        moveq   #10,d3          ; assume base of 10
        moveq   #' ',d2
skipespace:
        cmp.b   (a0)+,d2
        beq.s   skipespace      ; skip space
        moveq   #31,d1          ; bit de signe pour un LONG
.minus:
        cmpi.b  #'-',-(a0)      ; check for minus sign
        bne.s   .plus
        bset    d1,d3           ; show minus sign
        bra.s   .found
.plus
        cmpi.b  #'+',(a0)       ; check for plus sign
        bne.b   initialStr
        bclr    d1,d3           ; show plus sign
.found
        addq.w  #1,a0           ; skip plus sign

initialStr:
        move.b  (a0)+,d1        ; get characters from start
        cmpi.b  #'0',d1
        beq.s   initialStr      ; chuck initial zeros
        cmpi.b  #'x',d1         ; check for hex forms
        beq.s   initialHex
        cmpi.b  #'X',d1         ; check for hex forms
        beq.s   initialHex
        cmpi.b  #'$',d1
        beq.s   initialHex
        bra.s   bodyConvert

bodyStr:
        move.b  (a0)+,d1        ; get next character
bodyConvert:
        cmpi.b  #'0',d1
        bcs.s   bodyDone        ; fin de la conversion
        cmpi.b  #'9',d1
        bhi.s   perhapsHex
        subi.b  #'0',d1         ; ou andi.b #$0f,d1 (meme #cycles meme #octets)
        bra.s   calc

perhapsHex:
        cmp.w   #10,d3
        beq.s   bodyDone        ; base 10 : fin de conversion
        andi.b  #%11011111,d1   ; lower case to upper case
        cmpi.b  #'A',d1
        bcs.s   bodyDone        ; fin de conversion
        cmpi.b  #'F',d1
        bhi.s   bodyDone        ; fin de conversion
        subi.b  #'A'-10,d1

calc:
        move.w  d0,d2           ; save lsw of collection
        swap    d0
        mulu    d3,d0           ; multiply up msw of collection
        swap    d0
        clr.w   d0              ; throw away any overflow (!)
        mulu    d3,d2           ; multiply up ls2 of collection
        add.l   d2,d0
        add.l   d1,d0
        bra.s   bodyStr

initialHex:
        move.w  #16,d3          ; show base of 16, preserving minus
        bra.s   bodyStr

bodyDone:
        tst.l   d3
        bpl.s   returnNum
        neg.l   d0
returnNum:
        tst.b   -1(a0)
        bne.b   errorzero
        subq.w  #1,a0
errorzero
        movem.l (sp)+,d2/d3
        rts


**********************************************************************
*   len = Hex2Str( hex, buf )
*   d0             d0   a0
**********************************************************************
ConvertHex:
        move.l  d2,-(sp)
        moveq   #8-1,d2         ;Eight numbers in a longword
.loop   rol.l   #4,d0           ;Shift nibble
        move.l  d0,d1
        andi.b  #$0f,d1         ;Mask nibble
        cmpi.b  #9,d1
        bls.s   .001
        addq.b  #7,d1           ;Adjust for 'A'-'F'
.001    addi.b  #$30,d1         ;Convert to ascii
        move.b  d1,(a0)+        ;Store ascii
        dbra    d2,.loop
        move.l  (sp)+,d2
        rts


**********************************************************************
*   len = StrB2C( bs, cs )
*   d0            a0  a1
**********************************************************************

    XDEF    _StrB2C

_StrB2C:
        moveq   #0,d0
        move.b  (a0)+,d0
        move.w  d0,d1           ; pas besoin de .l mais pas de gain en #cycles
        bra.s   2$
1$:
        move.b  (a0)+,(a1)+
2$:
        dbf     d1,1$           ; d1 est -- en tant que word
        clr.b   (a1)
        rts                     ; d0=len (sans 0)


**********************************************************************
*   cmp = StrCmp( s1, s2 )
*   d0            a0  a1
*   1    s1 > s2
*   0    s1 = s2
*  -1    s2 > s1
**********************************************************************

        XDEF    _StrCmp

_StrCmp:
        move.b  (a1)+,d0
        beq.s   1$              ; second string is done
        cmp.b   (a0)+,d0
        beq.s   _StrCmp         ; still equal
        bhi.s   2$              ; second string is greater
3$:
        moveq   #1,d0           ; first string is greater
        rts
1$:
        tst.b   (a0)            ; is first string done?
        bne.s   3$              ; first string is greater
        moveq   #0,d0           ; strings are equal
        rts
2$:
        moveq   #-1,d0          ; second string is greater
        rts


**********************************************************************
*   StrCat( dest, source )
*           a0    a1
**********************************************************************

        XDEF    _StrCat

_StrCat:
        tst.b   (a0)+
        bne.s   _StrCat
        subq.w  #1,a0           ; pas besoin de .l mais pas de gain en cycles et en octets


**********************************************************************
*   StrCpy( dest, source )
*           a0     a1
**********************************************************************

        XDEF    _StrCpy

_StrCpy:
        move.b  (a1)+,(a0)+
        bne.b   _StrCpy
        move.l  a0,d0           ; a0 et a1 pointent sur le caractere
        rts                     ;   apres le 0 final


**********************************************************************
*   StrLen( string )
*           a0
**********************************************************************

        XDEF   _StrLen

_StrLen
        move.l  a0,d0
.loop   tst.b   (a0)+
        bne.s   .loop
        suba.l d0,a0
        subq.w #1,a0
        move.l a0,d0
        rts


**********************************************************************
*   endstr = UIK_StrToUpper( str )
*   d0                       a0
**********************************************************************

        XDEF   _UIK_StrToUpper

_UIK_StrToUpper
        move.b  (a0),d0
        bsr.s   _UIK_ToUpper
        move.b  d0,(a0)+
        bne.s   _UIK_StrToUpper
        move.l  a0,d0           ; retourne fin de chaine+1
        rts


**********************************************************************
*   ch = UIK_ToUpper( ch )
*   d0                d0
**********************************************************************

        XDEF   _UIK_ToUpper

_UIK_ToUpper
;        btst    #5,d0
;        beq.s   .end            ; bit 5 already clear
;
;        cmp.b   #$61,d0
;        bcs.s   .end            ; dest<src
;        cmp.b   #$7b,d0
;        bcs.s   .ok
;
;        cmp.b   #$e0,d0
;        bcs.s   .end
;        cmp.b   #$f7,d0
;        bcs.s   .ok
;        beq.s   .end
;
;        cmp.b   #$ff,d0
;        beq.s   .end
;.ok
;        andi.b  #%11011111,d0   ; clear bit 5
;.end
;        rts

; autre possibilite :

        movem.l d2/d3,-(sp)
        bsr.s   deCase
        movem.l (sp)+,d2/d3
        rts


**********************************************************************
*   cmp = StrNCCmp( s1, s2 )
*   d0              a0  a1
*   1    s1 > s2
*   0    s1 = s2
*  -1    s2 > s1
**********************************************************************

    XDEF    _SStrNCCmp
    XDEF    _StrNCCmp

_SStrNCCmp:
        movem.l 4(sp),a0/a1
_StrNCCmp:
        movem.l d2/d3,-(sp)
        moveq   #0,d0
snccStillEqual:
        move.b  (a1)+,d0
        beq.s   1$              ; second string is done
        bsr.s   deCase
        move.b  d0,d1           ; save decased byte
        move.b  (a0)+,d0
        bsr.s   deCase
        cmp.b   d0,d1
        beq.s   snccStillEqual  ; still equal
        bhi.s   2$              ; second string is greater (d1>d0)
3$:
        moveq   #1,d0           ; first string is greater
        bra.s   snccRts
1$:
        tst.b   (a0)            ; is first string done?
        bne.s   3$              ; first string is greater
        moveq   #0,d0           ; strings are equal
        bra.s   snccRts
2$:
        moveq   #-1,d0          ; second string is greater
snccRts:
        movem.l (sp)+,d2/d3
        rts

deCase:
        btst    #5,d0
        beq.s   noLC            ; bit 5 already clear

        move.b  d0,d2
        and.w   #7,d2
        moveq   #0,d3
        move.b  d0,d3
        lsr.w   #3,d3
        btst    d2,caseList(pc,d3.w)
        beq.s   noLC
        andi.b  #%11011111,d0   ; convert to upper case
noLC:
        rts


**********************************************************************
*   endstr = UIK_StrToLower( str )
*   d0                       a0
**********************************************************************

        XDEF   _UIK_StrToLower

_UIK_StrToLower
        move.b  (a0),d0
        bsr.b   _UIK_ToLower
        move.b  d0,(a0)+
        bne.s   _UIK_StrToLower
        move.l  a0,d0
        rts


**********************************************************************
*   ch = UIK_ToLower( ch )
*   d0                d0
**********************************************************************

        XDEF   _UIK_ToLower

_UIK_ToLower
;        btst    #5,d0
;        bne.s   .end            ; bit 5 already set
;
;        cmp.b   #$41,d0
;        bcs.b   .end
;        cmp.b   #$5b,d0
;        bcs.b   .ok
;
;        cmp.b   #$c0,d0
;        bcs.b   .end
;        cmp.b   #$d7,d0
;        bcs.s   .ok
;        beq.s   .end
;
;        cmp.b   #$df,d0
;        beq.s   .end
;.ok
;        ori.b   #%00100000,d0   ; set bit 5
;.end
;        rts

Case:
        btst    #5,d0
        bne.s   LC              ; bit 5 already set

        move.b  d0,d2
        and.w   #7,d2
        moveq   #0,d3
        move.b  d0,d3
        lsr.w   #3,d3
        btst    d2,caseList(pc,d3.w)
        beq.s   LC
        ori.b   #%00100000,d0   ; convert to upper case
LC:
        rts

caseList:
        dc.b    0,0                     ; Colonne 0
        dc.b    0,0                     ; Colonne 1
        dc.b    0,0                     ; Colonne 2
        dc.b    0,0                     ; Colonne 3
        dc.b    %11111110,%11111111     ; Colonne 4 (bit 5=0)
        dc.b    %11111111,%00000111     ; Colonne 5 (bit 5=0)
        dc.b    %11111110,%11111111     ; Colonne 6
        dc.b    %11111111,%00000111     ; Colonne 7
        dc.b    0,0                     ; Colonne 8
        dc.b    0,0                     ; Colonne 9
        dc.b    0,0                     ; Colonne A
        dc.b    0,0                     ; Colonne B
        dc.b    %11111111,%11111111     ; Colonne C (bit 5=0)
        dc.b    %11111110,%11111110     ; Colonne D (bit 5=0)
        dc.b    %11111111,%11111111     ; Colonne E
        dc.b    %01111110,%01111111     ; Colonne F


;caseList:
;        dc.b    0,0                     ; Colonne 0
;        dc.b    0,0                     ; Colonne 1
;        dc.b    0,0                     ; Colonne 2
;        dc.b    0,0                     ; Colonne 3
;        dc.b    %00000000,000000000     ; Colonne 4 (bit 5=0)
;        dc.b    %00000000,%00000000     ; Colonne 5 (bit 5=0)
;        dc.b    %11111110,%11111111     ; Colonne 6
;        dc.b    %11111111,%00000111     ; Colonne 7
;        dc.b    0,0                     ; Colonne 8
;        dc.b    0,0                     ; Colonne 9
;        dc.b    0,0                     ; Colonne A
;        dc.b    0,0                     ; Colonne B
;        dc.b    %00000000,%00000000     ; Colonne C (bit 5=0)
;        dc.b    %00000000,%00000000     ; Colonne D (bit 5=0)
;        dc.b    %11111111,%11111111     ; Colonne E
;        dc.b    %01111110,%01111111     ; Colonne F

*********************************************
*
*   ULONG __asm StrNCSub( register __a0 UBYTE *sub, register __a1 UBYTE *str );
*   ULONG __asm StrNCEqual( register __a0 UBYTE *sub, register __a1 UBYTE *str, register __d0 ULONG maxcmplen );
*
*********************************************

_StrNCSub
    movem.l d2/d3,-(sp)
.loop
    move.b  (a1)+,d0
    beq.b   .isok       total string is done
    bsr.b   deCase
    move.b  d0,d1

    move.b  (a0)+,d0    sub string
    beq.b   .ok
    bsr.b   deCase

    cmp.b   d0,d1
    beq.b   .loop       still equal
    bra.b   .err
.isok
    tst.b   (a0)        is first string done?
    beq.b   .ok
.err
    moveq   #0,d0
    bra.s   .en
.ok moveq   #1,d0
.en movem.l (sp)+,d2/d3
    rts

; _StrNCEqual
;     movem.l d2-d4,-(sp)
;     move.l  d0,d4       d4 = len
; .loop
;     move.b  (a1)+,d0
;     beq.b   .is1ok      total string is done
;     bsr.b   DeCase
;     move.b  d0,d1
;
;     move.b  (a0)+,d0    sub string
;     beq.b   .is2ok
;     bsr.b   DeCase
;
;     subq.l  #1,d4
;     cmp.b   d0,d1
;     beq.b   .loop       still equal
;     bra.b   .err
;
; .is2ok
;     tst.l   d4
;     beq.b   .ok
;     bra.b   .err
; .is1ok
;     tst.b   (a0)        is first string done?
;     beq.b   .ok
; .err
;     moveq   #0,d0
;     bra.s   .en
; .ok moveq   #1,d0
; .en movem.l (sp)+,d2-d4
;     rts


        END

**********************************************************************
*   cmp = ARG_Equal( name, cmdopt )
*   d0                a0     a1
*   ok : adresse du char après le signe '='
*   ok : -1 si pas de signe '='
*   erreur : 0
**********************************************************************

_ARG_Equal
        movem.l d2/d3,-(sp)
        moveq   #0,d0
snccStillEqual
        move.b  (a1)+,d0
        beq.s   2$              ; second string is done
        cmp.b   #'=',d0
        beq.s   1$              ; second string is done

        bsr     deCase
        move.b  d0,d1           ; save decased byte
        move.b  (a0)+,d0
        bsr     deCase
        cmp.b   d0,d1
        beq.s   snccStillEqual  ; still equal
3$
        moveq   #0,d0           ; first string is greater
        bra.s   snccRts
2$
        tst.b   (a0)            ; is first string done?
        bne.s   3$              ; no -> not equal
4$      moveq   #-1,d0          ; arg with no parm
        bra.s   snccRts
1$
        tst.b   (a0)            ; is first string done?
        bne.s   3$              ; no -> not equal
        tst.b   (a1)
        beq.b   4$
        move.l  a1,d0           ; arg with a parm
snccRts:
        movem.l (sp)+,d2/d3
        rts

;deCase:
;        btst    #5,d0
;        beq.s   noLC            ; bit 5 already clear
;
;        move.b  d0,d2
;        and.w   #7,d2
;        moveq   #0,d3
;        move.b  d0,d3
;        lsr.w   #3,d3
;        btst    d2,caseList(pc,d3.w)
;        beq.s   noLC
;        andi.b  #%11011111,d0   ; convert to upper case
;noLC:
;        rts

