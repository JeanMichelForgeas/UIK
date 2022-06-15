        OPT O+,W-
        OPT d+
*****************************************************************
*
*       Project:    MemTrack
*       Function:   Alloc/Free memory attached to the task
*
*       Project:    MemSp
*       Function:   Alloc/Free memory and remember its length
*
*       Created:    28/07/90    Jean-Michel Forgeas
*
*****************************************************************


******* Includes ************************************************

        INCLUDE "exec/types.i"
        INCLUDE "exec/lists.i"
        INCLUDE "exec/tasks.i"
        INCLUDE "exec/memory.i"
        INCLUDE "lib/exec_lib.i"

STACKNEED equ     ML_SIZE+ME_SIZE


******* Imported ************************************************

        XREF    _AsmAllocPooled
        XREF    _AsmFreePooled
        XREF    _MWAsmAllocPooled
        XREF    _MWAsmFreePooled


******* Exported ************************************************

        XDEF    _UIK_MemTAlloc
        XDEF    _UIK_MemTFree
        XDEF    _UIK_MemSpAlloc
        XDEF    _UIK_MemSpFree
        XDEF    _UIK_MemSpLength
        XDEF    _UIK_MemSpResize
        XDEF    _UIK_MemCopy
        ;XDEF    _UIK_MemPLength  ( APTR mem )(A0)
        ;XDEF    _UIK_MemPAlloc   ( APTR pool, ULONG size )(A0,D0)
        ;XDEF    _UIK_MemPResize  ( APTR pool, APTR old, ULONG newsize )(A0/A1,D0)
        ;XDEF    _UIK_MemPFree    ( APTR mem )(A1)


*****************************************************************
*
                    SECTION     text,CODE
*
*****************************************************************


*********************************************
*   memoryBlock = UIK_MemTAlloc( Size, Reqs )
*       d0                       d0    d1
*********************************************
_UIK_MemTAlloc
    movem.l d0-d1/a6,-(sp)
    move.l  4.w,a6

    ;------ set up a MemList structure and AllocEntry() it
    sub.w   #STACKNEED,sp                       reserve stack space
    move.w  #1,ML_NUMENTRIES(sp)
    move.l  d1,ML_SIZE+ME_REQS(sp)              requested type
    addq.l  #4,d0
    move.l  d0,ML_SIZE+ME_LENGTH(sp)            requested size+4
    move.l  sp,a0
    jsr     _LVOAllocEntry(a6)                  allocate all that
    btst    #31,d0                              have it ?
    beq.b   .alloc_ok
    moveq   #0,d0
    bra.b   .end

.alloc_ok
    move.l  d0,-(sp)                            save new memlist

    ;------ attach memory to the task
    suba.l  a1,a1
    jsr     _LVOFindTask(a6)
    move.l  d0,a0
    lea     TC_MEMENTRY(a0),a0                  get the task's memlist

    tst.l   (a0)                                if just a task, memlist
    bne.b   .list_ok                            is not initialized
    NEWLIST a0                                  so we do
.list_ok                                   ;    a0 = task's memlist
    move.l  (sp),a1                             a1 = new memlist
    jsr     _LVOAddTail(a6)                     attach

    ;------ final values
    move.l  (sp)+,a0                            a0 = new memlist
    move.l  ML_SIZE+ME_ADDR(a0),a1              a1 = memory block
    move.l  a0,(a1)+                            remember memlist and set
                                           ;    start of user block += 4
    move.l  a1,d0                               return( user block )
.end
    add.w   #STACKNEED,sp                       restore stack pointer
    movem.l (sp)+,d1/a0/a6
    rts


***********************************************
*   UIK_MemTFree( memoryBlock )
*                 a0
***********************************************
_UIK_MemTFree
    move.l  a0,d0
    beq.b   .end

    move.l  a6,-(sp)
    move.l  4.w,a6

    move.l  -4(a0),d0                           memlist address
    move.l  d0,a1
    REMOVE                                      remove it from the task's list

    move.l  d0,a0
    jsr     _LVOFreeEntry(a6)                   free all that

    move.l  (sp)+,a6
.end
    rts


***************************************************************************
*   extern UBYTE * __asm UIK_MemSpResize( register __a0 UBYTE *old,
*                                            register __d0 ULONG newsize,
*                                               register __d1 ULONG reqs );
***************************************************************************

_UIK_MemSpResize
        movem.l d0-d1/a0/a6,-(sp)
        move.l  4.w,a6                  ExecBase

        bsr.b   _UIK_MemSpAlloc
        move.l  d0,-(sp)                valeur de retour
        beq.b   .end

        move.l  12(sp),d1                if (old == 0)  normal, pas de premier buffer
        beq.b   .end                    ...goto end

        move.l  d1,a0
        move.l  d0,a1
        move.l  -4(a0),d0               d0 = oldsize + 4
        subq.l  #4,d0                   d0 = oldsize
        cmp.l   4(sp),d0                if (newsize < oldsize)
        ble.b   .copy
        move.l  4(sp),d0                ...d0 = newsize
.copy                                   ;CopyMemQuick( old, new, d0 )
        tst.l   d0
        beq.b   .free
        jsr     _LVOCopyMem(a6)
.free
        move.l  12(sp),a1
        bsr.b   _UIK_MemSpFree

.end
        movem.l (sp)+,d0-d1/a0-a1/a6       return( d0==mem )
        rts


**************************************************************************
*   extern UBYTE * __asm UIK_MemSpAlloc( register __d0 ULONG size,
*   returns 0 if no memory                     register __d1 ULONG reqs );
**************************************************************************

_UIK_MemSpAlloc
        addq.l  #4,d0               d0 = size + 4 to remember size
        move.l  d0,-(sp)

        move.l  a6,-(sp)
        move.l  4.w,a6
        jsr     _LVOAllocMem(a6)
        move.l  (sp)+,a6
        move.l  d0,a0               a0 = address

        move.l  (sp)+,d1            restore size
        tst.l   d0                  test if alloc successful
        beq.b   .end                if no memory...

        move.l  d1,(a0)+            remember size into the firt long word
        move.l  a0,d0               return address of first byte to use
.end    rts


**************************************************************************
*   extern void __asm UIK_MemSpFree( register __a1 UBYTE *mem );
**************************************************************************

_UIK_MemSpFree
        move.l  a1,d0
        beq.b   .end
        move.l  -(a1),d0
        move.l  a6,-(sp)
        move.l  4.w,a6
        jsr     _LVOFreeMem(a6)
        move.l  (sp)+,a6
.end    rts


**************************************************************************
*   extern int __asm UIK_MemSpLength( register __a0 UBYTE *mem );
**************************************************************************

_UIK_MemSpLength
        move.l  a0,d0
        beq.b   .end
        move.l  -4(a0),d0
        subq.l  #4,d0
.end    rts


**************************************************************************
*   extern void __asm UIK_MemCopy( register __a0 UBYTE *src,
*                                       register __a1 UBYTE *dst
*                                            register __d0 ULONG len );
**************************************************************************

                ;   MOVMEM.A
                ;
                ;   (c)Copyright 1990, Matthew Dillon, All Rights Reserved

                ;section text,code

                ;   movmem(src, dst, len)   (ANSI)
                ;   bcopy(src, dst, len)    (UNIX)
                ;           A0  A1   D0     DICE-REG
                ;           A0  A1   D0     internal
                ;        4(sp) 8(sp) 12(sp)
                ;
                ;   The memory move algorithm is somewhat more of a mess
                ;   since we must do it either ascending or decending.

                ;xdef    _movmem
                ;xdef    _bcopy      ; UNIX
                ;xdef    @movmem
                ;xdef    @bcopy      ; UNIX


;_bcopy:
;_movmem:        move.l  4(sp),A0
;                move.l  8(sp),A1
;                move.l  12(sp),D0
;@bcopy:
;@movmem:

_UIK_MemCopy
                tst.l   d0
                beq.b   xbmend
                cmp.l   A0,A1           ;move to self
                beq.b   xbmend
                bls.b   xbmup
xbmdown         adda.l  D0,A0           ;descending copy
                adda.l  D0,A1
                move.w  A0,D1           ;CHECK WORD ALIGNED
                lsr.l   #1,D1
                bcs.b   xbmdown1
                move.w  A1,D1
                lsr.l   #1,D1
                bcs.b   xbmdown1
                cmp.l   #259,D0             ;chosen by calculation.
                bcs.b   xbmdown8

                move.l  D0,D1               ;overhead for bmd44: ~360
                divu    #44,D1
                bvs.b   xbmdown8            ;too big (> 2,883,540)
                movem.l D2-D7/A2-A6,-(sp)   ;use D2-D7/A2-A6 (11 regs)
                moveq   #44,D0
                bra.b   xbmd44b
xbmd44a         sub.l   D0,A0               ;8          total 214/44bytes
                movem.l (A0),D2-D7/A2-A6    ;12 + 8*11  4.86 cycles/byte
                movem.l D2-D7/A2-A6,-(A1)   ; 8 + 8*11
xbmd44b         dbf     D1,xbmd44a          ;10
                swap    D1                  ;D0<15:7> already contain 0
                move.w  D1,D0               ;D0 = remainder
                movem.l (sp)+,D2-D7/A2-A6

xbmdown8        move.w  D0,D1               ;D1<2:0> = #bytes left later
                lsr.l   #3,D0               ;divide by 8
                bra.b   xbmd8b
xbmd8a          move.l  -(A0),-(A1)         ;20         total 50/8bytes
                move.l  -(A0),-(A1)         ;20         = 6.25 cycles/byte
xbmd8b          dbf     D0,xbmd8a           ;10
                sub.l   #$10000,D0
                bcc.b   xbmd8a
                move.w  D1,D0               ;D0 = 0 to 7 bytes
                and.l   #7,D0
                bne.b   xbmdown1
xbmend
;                move.l  8(sp),D0
                rts

xbmd1a          move.b  -(A0),-(A1)         ;12         total 22/byte
xbmdown1                                    ;           = 22 cycles/byte
xbmd1b          dbf     D0,xbmd1a           ;10
                sub.l   #$10000,D0
                bcc.b   xbmd1a
;                move.l  8(sp),D0
                rts

xbmup           move.w  A0,D1               ;CHECK WORD ALIGNED
                lsr.l   #1,D1
                bcs.b   xbmup1
                move.w  A1,D1
                lsr.l   #1,D1
                bcs.b   xbmup1
                cmp.l   #259,D0             ;chosen by calculation
                bcs.b   xbmup8

                move.l  D0,D1               ;overhead for bmu44: ~360
                divu    #44,D1
                bvs.b   xbmup8              ;too big (> 2,883,540)
                movem.l D2-D7/A2-A6,-(sp)   ;use D2-D7/A2-A6 (11 regs)
                moveq   #44,D0
                bra.b   xbmu44b
xbmu44a         movem.l (A0)+,D2-D7/A2-A6   ;12 + 8*11  ttl 214/44bytes
                movem.l D2-D7/A2-A6,(A1)    ;8  + 8*11  4.86 cycles/byte
                add.l   D0,A1               ;8
xbmu44b         dbf     D1,xbmu44a          ;10
                swap    D1                  ;D0<15:7> already contain 0
                move.w  D1,D0               ;D0 = remainder
                movem.l (sp)+,D2-D7/A2-A6

xbmup8          move.w  D0,D1               ;D1<2:0> = #bytes left later
                lsr.l   #3,D0               ;divide by 8
                bra.b   xbmu8b
xbmu8a          move.l  (A0)+,(A1)+         ;20         total 50/8bytes
                move.l  (A0)+,(A1)+         ;20         = 6.25 cycles/byte
xbmu8b          dbf     D0,xbmu8a           ;10
                sub.l   #$10000,D0
                bcc.b   xbmu8a
                move.w  D1,D0               ;D0 = 0 to 7 bytes
                and.l   #7,D0
                bne.b   xbmup1
;                move.l  8(sp),D0
                rts

xbmu1a          move.b  (A0)+,(A1)+
xbmup1
xbmu1b          dbf     D0,xbmu1a
                sub.l   #$10000,D0
                bcc.b   xbmu1a
;                move.l  8(sp),D0
                rts


*****************************************************************
        END

**************************************************************************
*   extern int __asm UIK_MemPLength( register __a0 UBYTE *mem );
**************************************************************************

_UIK_MemPLength
        move.l  a0,d0
        beq.b   .end
        move.l  -4(a0),d0
        subq.l  #8,d0
.end    rts


***************************************************************************
*   extern UBYTE * __asm UIK_MemPResize( register __a0 APTR pool,
*                                          register __a1 UBYTE *old,
*                                            register __d0 ULONG newsize );
***************************************************************************

_UIK_MemPResize
        movem.l d0-d1/a1/a6,-(sp)
        move.l  4.w,a6                  ExecBase

        bsr.b   _UIK_MemPAlloc
        move.l  d0,-(sp)                valeur de retour
        beq.b   .end

        move.l  12(sp),d1                if (old == 0)  normal, pas de premier buffer
        beq.b   .end                    ...goto end

        move.l  d1,a0
        move.l  d0,a1
        move.l  -4(a0),d0               d0 = oldsize + 8
        subq.l  #8,d0                   d0 = oldsize
        cmp.l   4(sp),d0                if (newsize < oldsize)
        ble.b   .copy
        move.l  4(sp),d0                ...d0 = newsize
.copy;
        tst.l   d0
        beq.b   .free
        jsr     _LVOCopyMem(a6)
.free
        move.l  12(sp),a1
        bsr.b   _UIK_MemPFree

.end
        movem.l (sp)+,d0-d1/a0-a1/a6       return( d0==mem )
        rts


**************************************************************************
*   extern UBYTE * __asm UIK_MemPAlloc( register __a0 APTR pool,
*                                              register __d0 ULONG size );
**************************************************************************

_UIK_MemPAlloc
        addq.l  #8,d0               d0 = size + 4 to remember size

        movem.l d0/a0/a6,-(sp)
        move.l  4.w,a6
        jsr     _AsmAllocPooled
        movem.l (sp)+,d1/a0/a6      d1 = size+8, a0 = pool
        tst.l   d0                  test if alloc successful
        beq.b   .end                if no memory...

        move.l  d0,a1               a1 = address
        move.l  a0,(a1)+            address = pool
        move.l  d1,(a1)+            address+4 = size+8
        move.l  a1,d0               d0 = address+8
.end    rts


**************************************************************************
*   extern void __asm UIK_MemPFree( register __a1 UBYTE *mem );
**************************************************************************

_UIK_MemPFree
        move.l  a1,d0
        beq.b   .end
        move.l  -(a1),d0    size+8
        move.l  -(a1),a0    pool
        move.l  a6,-(sp)
        move.l  4.w,a6
        jsr     _AsmFreePooled
        move.l  (sp)+,a6
.end    rts
