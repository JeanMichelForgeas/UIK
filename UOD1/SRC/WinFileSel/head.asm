
        moveq   #20,d0
        rts

        XREF    _Germ
        dc.l    _Germ

        dc.b    0,'$VER: UIKObj_WinFileSel 1.8 (27.10.94)',0

        cnop    0,4


**********************************************************************
*       s+,     num     = my_StrToLong( s, pval )
*       a0      d0                                      a0 a1
**********************************************************************

                XDEF    _my_StrToLong

_my_StrToLong:
                movem.l 4(sp),a0-a1
                movem.l d2-d5,-(sp)
                moveq   #0,d0                   ; no char converted
                moveq   #0,d5                   ; no pre-char
                moveq   #0,d4                   ; start with value zero
                move.l  a0,d1
                beq             errorzero

                moveq   #0,d1                   ; clear upper bytes
                moveq   #10,d3                  ; assume base of 10
                bra.s   testspace
skipspace:
                addq.l  #1,d5                   ; one more pre-char
testspace:
                move.b  (a0)+,d2
                cmp.b   #' ',d2                 ;
                beq.s   skipspace               ; skip space
                cmp.b   #9,d2
                beq.s   skipspace               ; skip tab char

                moveq   #31,d1                  ; bit de signe pour     un LONG
.minus:
                cmpi.b  #'-',-(a0)              ; check for     minus sign
                bne.s   .plus
                bset    d1,d3                   ; show minus sign
                addq.l  #1,d5                   ; one more pre-char
                bra.s   .found
.plus
                cmpi.b  #'+',(a0)               ; check for     plus sign
                bne.b   initialStr
                bclr    d1,d3                   ; show plus     sign
                addq.l  #1,d5                   ; one more pre-char
.found
                addq.w  #1,a0                   ; skip plus     sign

initialStr:
                move.b  (a0)+,d1                ; get characters from start
                cmpi.b  #'0',d1
                bne.b   .testhex
                addq.l  #1,d0                   ; one more char converted
                bra.s   initialStr              ; chuck initial zeros
.testhex
                cmpi.b  #'x',d1                 ; check for     hex     forms
                beq.s   initialHex
                cmpi.b  #'X',d1                 ; check for     hex     forms
                beq.s   initialHex
                cmpi.b  #'$',d1
                beq.s   initialHex
                bra.s   bodyConvert

bodyStr:                ;----------------------------------
                move.b  (a0)+,d1                ; get next character
bodyConvert:
                cmpi.b  #'0',d1
                bcs.s   bodyDone                ; fin de la     conversion
                cmpi.b  #'9',d1
                bhi.s   perhapsHex
                subi.b  #'0',d1                 ; ou andi.b     #$0f,d1 (meme #cycles meme #octets)
                bra.s   calc

perhapsHex:
                cmp.w   #10,d3
                beq.s   bodyDone                ; base 10 :     fin     de conversion
                andi.b  #%11011111,d1   ; lower case to upper case
                cmpi.b  #'A',d1
                bcs.s   bodyDone                ; fin de conversion
                cmpi.b  #'F',d1
                bhi.s   bodyDone                ; fin de conversion
                subi.b  #'A'-10,d1

calc:
                addq.l  #1,d0                   ; one more char converted
                move.w  d4,d2                   ; save lsw of collection
                swap    d4
                mulu    d3,d4                   ; multiply up msw of collection
                swap    d4
                clr.w   d4                              ; throw away any overflow (!)
                mulu    d3,d2                   ; multiply up ls2 of collection
                add.l   d2,d4
                add.l   d1,d4
                bra.s   bodyStr

initialHex:             ;------------------------------------
                move.w  #16,d3                  ; show base     of 16, preserving minus
                addq.l  #1,d5                   ; one more pre-char
                bra.s   bodyStr

bodyDone:               ;-----------------------------------
                tst.l   d3
                bpl.s   returnNum
                neg.l   d4
returnNum:
                tst.b   -1(a0)                  ; stay on char 0 (end of string)
                bne.b   errorzero
                subq.w  #1,a0
errorzero
                tst.l   d0
                bne.b   .ok
                moveq   #-1,d0                  ; return(-1) if no char converted
.ok             move.l  d4,(a1)
                add.l   d5,d0                   ; total num     chars
                movem.l (sp)+,d2-d5
                rts

