        OPT O+,W-
        OPT d+
*****************************************************************
*
*       Project:    UIKBuffer
*       Function:   Alloc/Free memory blocks
*
*       Created:    01/04/92    Jean-Michel Forgeas
*
*****************************************************************


******* Includes ************************************************

        INCDIR  "I:"

        INCLUDE "exec/types.i"
        INCLUDE "exec/memory.i"
        INCLUDE "lib/exec_lib.i"

        INCLUDE "uiki:uikbuf.i"
        INCLUDE "uiki:uikmacros.i"


******* Imported ************************************************

        XREF    _UIK_MemCopy
        XREF    _MWAsmAllocPooled
        XREF    _MWAsmFreePooled
        XREF    _AsmAllocPooled
        XREF    _AsmFreePooled
        XREF    _AsmCreatePool
        XREF    _AsmDeletePool


******* Exported ************************************************

        XDEF    _BufGetUDatas
        XDEF    _BufSetUDatas
        XDEF    _BufAlloc
        XDEF    _BufAllocP
        XDEF    _BufClone
        XDEF    _BufCloneS
        XDEF    _BufTruncate
        XDEF    _BufResize
        XDEF    _BufResizeS
        XDEF    _BufFree
        XDEF    _BufFreeS
        XDEF    _BufLength
        XDEF    _BufCut
        XDEF    _BufCutS
        XDEF    _BufCopy
        XDEF    _BufCopyS
        XDEF    _BufPaste
        XDEF    _BufPasteS
        XDEF    _BufSet
        XDEF    _BufSetS
        XDEF    _VBufPrintf


*****************************************************************
*
                    SECTION     text,CODE
*
*****************************************************************


*******************************************************************
*   ULONG ulong = BufGetUDatas( UIKBUF *buf, UWORD *pw )
*                                      a0          a1
*******************************************************************

_BufGetUDatas
        moveq   #0,d0
        cmp.l   a1,d0
        beq.b   .noword
        move.w  -nubu_UserUWord(a0),(a1)
.noword
        move.l  -nubu_UserULong(a0),d0
        rts


********************************************************************
*   void BufSetUDatas( UIKBUF *buf, ULONG uw, ULONG ul )
*                              a0         d0        d1
********************************************************************

_BufSetUDatas
        move.w  d0,-nubu_UserUWord(a0)
        move.l  d1,-nubu_UserULong(a0)
        rts


*******************************************************************
*   void BufTruncate( UIKBUF *old, ULONG newsize )
*                             a0           d0
*******************************************************************

_BufTruncate
        move.l  a0,d1
        beq.b   .end
        cmp.l   -nubu_CurSize(a0),d0
        bhi.b   .end
        move.l  d0,-nubu_CurSize(a0)    actualize to newsize
        add.l   d0,a0
        clr.b   (a0)
.end    rts


*******************************************************************
*   UIKBUF *BufAllocP( APTR pool, ULONG size, ULONG blksize )
*                           a0          d0          d1
*******************************************************************

_BufAllocP
        movem.l d2-d7/a6,-(sp)
        move.l  a0,d7               d7 = pool
        move.l  d0,d4               d4 = asked size
        move.l  d1,d2               d2 = block size
        moveq   #-1,d5              d5 = memreqs == -1 ==> Pool id
        moveq   #ubup_SIZE,d6
        move.l  4.w,a6

        bsr     ComputeNewSize      d0 = totsize = ComputeNewSize( d4.l, d2.l )
        move.l  d0,d3               d3 = totsize
        add.l   d6,d0               sizeof(struct) + totsize
        jsr      _AsmAllocPooled
        tst.l   d0
        beq.b   end_alloc           if no memory...

        move.l  d0,a0               a0 = address
        move.l  d7,(a0)+            a0 = address + sizeof(APTR Pool) pour ubu_...
        bra.b   common_alloc


*******************************************************************
*   UIKBUF *BufAlloc( ULONG blksize, ULONG size, ULONG reqs )
*                             a0            d0          d1
*******************************************************************

ResizeNew
        ;sub.l   a0,a0          pas besoin puisque déjà à 0
        move.l  #MEMF_ANY|MEMF_CLEAR,d1
_BufAlloc
        movem.l d2-d7/a6,-(sp)      (d7 pour BufAllocP)
        move.l  a0,d2               d2 = block size
        move.l  d0,d4               d4 = asked size
        move.l  d1,d5               d5 = mem reqs
        moveq   #ubu_SIZE,d6
        move.l  4.w,a6

        bsr.b   ComputeNewSize      d0 = totsize = ComputeNewSize( d4.l, d2.l )
        move.l  d0,d3               d3 = totsize
        add.l   d6,d0               sizeof(struct) + totsize
        move.l  d5,d1               d1 = mem reqs
        jsr     _LVOAllocMem(a6)
        tst.l   d0
        beq.b   end_alloc           if no memory...

        move.l  d0,a0               a0 = address
common_alloc
        movem.l d3-d5,(a0)          ubu_TotSize ubu_CurSize ubu_MemReqs
        move.w  d2,ubu_BlkSize(a0)
        moveq   #0,d1
        move.w  d1,ubu_UserUWord(a0)
        move.l  d1,ubu_UserULong(a0)

        add.l   d6,d0               d0 = client buffer = address + (ubu_SIZE ou ubup_SIZE)
        move.l  d0,a0               user buffer
        clr.b   (a0)                zeroend buffer
        clr.b   0(a0,d4.l)          zeroend buffer+ubu_CurSize
end_alloc
        movem.l (sp)+,d2-d7/a6
        rts


*******************************************************************

_UIK_MulU1632_32 ; (d0:32 x d2:16 => d0:32)  ne touche pas à d2
    move.l    d3,-(sp)
    move.l    d2,d1
    move.l    d0,d3
    swap      d1
    swap      d3
    mulu      d0,d1
    mulu      d2,d3
    mulu      d2,d0
    add.w     d3,d1
    swap      d1
    clr.w     d1
    add.l     d1,d0
    move.l    (sp)+,d3
    rts

_UIK_DivU3216_32 ; (d0:32 / d2:16 => d0:32)  ne touche pas à d2
    move.l    d2,d1
    swap      d0
    move.w    d0,d1
    beq.s     .1
    divu      d2,d1
    move.w    d1,d0
.1  swap      d0
    move.w    d0,d1
    divu      d2,d1
    move.w    d1,d0
    swap      d1        ; d1 == reste
    rts

*******************************************************************
* modifies only d2 and d0 !!!

ComputeNewSize  ; d0 = ComputeNewSize( d4.l=askedsize, d2.l=blksize )
        tst.w   d2
        bne.b   .blkok
        moveq   #16,d2
.blkok
        addq.l  #3,d2
        and.l   #$fffffffc,d2       blksize long aligned
        move.l  d4,d0
        add.l   d2,d0
        cmp.l   #$ffff,d0
        bhi.s   .long
        divu    d2,d0
        mulu    d2,d0
        rts     ; d0 is always at least equal to askedsize + 1
.long
        bsr.s   _UIK_DivU3216_32        ; divu    d2,d0
        bsr.s   _UIK_MulU1632_32        ; mulu    d2,d0
        rts


*******************************************************************
*   UIKBUF *BufResize( UIKBUF *old, ULONG newsize )
*                              a0           d0
*******************************************************************

_BufResizeS
        move.l  a0,-(sp)
        move.l  (a0),a0
        bsr.b   _BufResize
        move.l  (sp)+,a0
        tst.l   d0
        beq.b   .end
        move.l  d0,(a0)
.end    rts

_BufResize
        move.l  a0,d1
        beq     ResizeNew

        movem.l d2-d6/a2/a5/a6,-(sp)
        moveq   #0,d2
        move.w  -nubu_BlkSize(a0),d2    d2 = block size
        move.l  d0,d4                   d4 = asked size
        move.l  a0,a2                   a2 = old buffer

        bsr.b   ComputeNewSize          d0 = totsize = ComputeNewSize( d4.l, d2.l )
        cmp.l   -nubu_TotSize(a2),d0
        beq     .inplace
.resize
        ;------------------------------ resize stuff
        move.l  4.w,a6
        move.l  d0,d3                   d3 = new.totsize
        move.l  -nubu_MemReqs(a2),d5    d5 = mem reqs
        cmp.l   #-1,d5
        bne.b   .std
.pool   ;-------------
        moveq   #ubup_SIZE,d6           some inits...
        add.l   d6,d0                   sizeof(struct) + totsize
        move.l  -nubup_Pool(a2),a0
        jsr     _AsmAllocPooled
        tst.l   d0
        beq   .alloc_failed
        move.l  d0,a5                   a5 = new
        move.w  d2,ubup_BlkSize(a5)
        move.l  -nubup_Pool(a2),d2
        movem.l d2-d5,(a5)              ubup_Pool ubup_TotSize ubup_CurSize ubup_MemReqs
        bra.b   .com
.std    ;-------------
        moveq   #ubu_SIZE,d6            some inits...
        add.l   d6,d0                   sizeof(struct) + totsize
        move.l  d5,d1                   reqs
        jsr     _LVOAllocMem(a6)
        tst.l   d0
        beq   .alloc_failed
        move.l  d0,a5                   a5 = new
        move.w  d2,ubu_BlkSize(a5)
        movem.l d3-d5,(a5)              ubu_TotSize ubu_CurSize ubu_MemReqs
.com    ;-------------
        add.l   d6,a5                   a5 = client buffer = address + (ubu_SIZE ou ubup_SIZE)
        move.w  -nubu_UserUWord(a2),-nubu_UserUWord(a5)
        move.l  -nubu_UserULong(a2),-nubu_UserULong(a5)
        move.l  a2,a0                   old buf
        move.l  a5,a1                   new buf
        move.l  -nubu_CurSize(a0),d0    d0 = oldsize
        move.l  -nubu_CurSize(a5),d1    d1 = newsize
        cmp.l   d0,d1                   copysize = oldsize
        bcc.b   .copy                   if (newsize < oldsize)
        exg     d0,d1                   copysize = newsize
.copy                                   ;CopyMem( old, new, d0 )
        tst.l   d0
        beq.b   .free
        jsr     _LVOCopyMem(a6)
.free
        move.l  a2,a1
        bsr   _BufFree
        move.l  a5,d0
        bra.b   .zeroend
        ;------------------------------

.alloc_failed
        ;------ alloc failed, il askedsize < old.totsize we can adjust (let 0 to end)
        cmp.l   -nubu_TotSize(a2),d4    if (askedsize >= old.totsize) return(0)
        bcc.b   .end                    d0 = 0
.inplace
        move.l  d4,-nubu_CurSize(a2)    actualize to newsize
        move.l  a2,d0

.zeroend
        ;------ zeroend the buffer
        move.l  d0,a0
        add.l   -nubu_CurSize(a0),a0
        clr.b   (a0)

.end
        movem.l (sp)+,d2-d6/a2/a5/a6
.nold   rts


*******************************************************************
*   void BufFree( UIKBUF *mem );
*                         a1
*******************************************************************

_BufFreeS
        move.l  a1,-(sp)
        move.l  (a1),a1
        bsr.b   _BufFree
        move.l  (sp)+,a1
        moveq   #0,d0
        move.l  d0,(a1)
        rts

_BufFree
        move.l  a1,d0
        beq.b   .end
        move.l  a6,-(sp)
        move.l  4.w,a6

        move.l  -nubu_TotSize(a1),d0
        cmp.l   #-1,-nubu_MemReqs(a1)
        bne.b   .std
.pool
        moveq   #ubup_SIZE,d1
        sub.l   d1,a1                   a1 = struct address
        add.l   d1,d0                   d0 = size to free
        move.l  ubup_Pool(a1),a0        a0 = pool
        jsr     _AsmFreePooled
        bra.b   .endfree
.std
        moveq   #ubu_SIZE,d1
        sub.l   d1,a1                   a1 = struct address
        add.l   d1,d0                   d0 = size to free
        jsr     _LVOFreeMem(a6)
.endfree
        move.l  (sp)+,a6
.end    rts


*******************************************************************
*   ULONG BufLength( UIKBUF *mem )
*                            a0
*******************************************************************

_BufLength
        move.l  a0,d0
        beq.b   .end
        move.l  -nubu_CurSize(a0),d0
.end    rts


*******************************************************************
*   UIKBUF *BufClone( UIKBUF *old )
*                             a0
*******************************************************************

_BufCloneS
        move.l  a0,-(sp)
        move.l  (a0),a0
        bsr.b   _BufClone
        move.l  (sp)+,a0
        tst.l   d0
        beq.b   .end
        move.l  d0,(a0)
.end    rts

_BufClone
        moveq   #-1,d1          max len (unsigned)
        moveq   #0,d0           start from 0
        move.l  d0,a1           alloc buffer
        bra.b   _BufCopy

****************************************************************************
*   UIKBUF *BufCopy( UIKBUF *buf, UIKBUF *dest, ULONG start, ULONG len )
*                            a0            a1          d0          d1
****************************************************************************

_BufCopyS
        move.l  a1,-(sp)
        move.l  (a1),a1
        bsr.b   _BufCopy
        move.l  (sp)+,a1
        tst.l   d0
        beq.b   .end
        move.l  d0,(a1)
.end    rts

_BufCopy
        movem.l d2/a2-a3/a6,-(sp)
        exg     d0,d1
        move.l  d0,d2               d2 = len (unsigned)
        beq.b   .end                nothing to copy
        move.l  a0,d0
        beq.b   .end                no source buffer

        move.l  4.w,a6
        move.l  a0,a2
        add.l   d1,a2               a2 = start of copy

        move.l  -nubu_CurSize(a0),d0
        sub.l   d0,d1               d1 is now -(max number of copyable bytes)
        bcc.b   .error              nothing to copy (start was after buffer end)
        neg.l   d1
        cmp.l   d2,d1               if d2 < max, result is < 0
        bcc.b   .numok
        move.l  d1,d2               d2 = len to copy

.numok
        move.l  a1,a0               pass old buffer
        move.l  d2,d0               wanted size
        bsr     _BufResize          allocate or resize buffer
        tst.l   d0
        beq.b   .end                no memory...
        move.l  d0,a3

        move.l  a2,a0
        move.l  a3,a1
        move.l  d2,d0
        jsr     _LVOCopyMem(a6)

        move.l  a3,d0               return copy buffer
.end
        movem.l (sp)+,d2/a2-a3/a6
        rts
.error
        moveq   #0,d0
        bra.b   .end


***************************************************************************
*   UIKBUF *BufCut( UIKBUF *buf, ULONG start, ULONG len )
*                           a0          d0          d1
***************************************************************************

_BufCutS
        move.l  a0,-(sp)
        move.l  (a0),a0
        bsr.b   _BufCut
        move.l  (sp)+,a0
        tst.l   d0
        beq.b   .end
        move.l  d0,(a0)
.end    rts

_BufCut
        movem.l d2-d4/a3/a6,-(sp)
        move.l  a0,a3               a3 = buffer
        exg     d0,d1
        move.l  d0,d2               d2 = len
        beq.b   .ok                 nothing to cut
        move.l  a0,d0
        beq.b   .end                no source buffer

        move.l  4.w,a6
        move.l  d1,d3                   d3 = start
        move.l  -nubu_CurSize(a3),d4    d4 = old CurSize

        ;------ cut what ?
        moveq   #0,d0               if nothing to cut
        move.l  d4,d1
        sub.l   d3,d1               d1 = max number of cutable bytes
        bls.b   .ok                 nothing to cut (start >= buffer end)
        cmp.l   d1,d2               if d2_len > d1_max, bad, result is > 0
        bcs.b   .cutlenok
        move.l  d1,d2               d2 = max len to cut
.cutlenok
        ;------ length to copy inplace
        move.l  d4,d0
        sub.l   d3,d0
        sub.l   d2,d0
        beq.b   .resize

        ;------ copy inplace
        move.l  a3,a1
        add.l   d3,a1               a1 = buffer + start
        move.l  a1,a0
        add.l   d2,a0               a0 = buffer + start + cutlen
        jsr     _LVOCopyMem(a6)
.resize
        ;------ resize the cut buffer
        move.l  a3,a0
        move.l  d4,d0
        sub.l   d2,d0
        bsr     _BufResize
.end
        movem.l (sp)+,d2-d4/a3/a6
        rts
.ok
        move.l  a3,d0
        bra.b   .end


************************************************************************
*   UIKBUF *BufPaste( UBYTE *src, ULONG len, UIKBUF *dest, ULONG start )
*                            a0         d0            a1          d1
************************************************************************

_BufPasteS
        move.l  a1,-(sp)
        move.l  (a1),a1
        bsr.b   _BufPaste
        move.l  (sp)+,a1
        tst.l   d0
        beq.b   .end
        move.l  d0,(a1)
.end    rts

_BufPaste
        movem.l d2-d4/a2-a3/a6,-(sp)
        move.l  a1,a3                   a3 = dest buffer
        move.l  d0,d2                   d2 = len
        beq.b   .ok                     nothing to copy
        move.l  a0,d0
        beq.b   .ok                     no source ptr
        move.l  a1,d0
        beq.b   .end                    no dest buffer

        move.l  4.w,a6
        move.l  a0,a2                   a2 = src ptr
        move.l  d1,d3                   d3 = start
        move.l  -nubu_CurSize(a3),d4    d4 = old CurSize

        ;------ compute newsize
        move.l  d4,d0                   d0 = pre-size
        cmp.l   d0,d3                   is start after end?
        bls     .sizeok                 .no, pre-size is ok
        move.l  d3,d0                   .yes, pre-size becomes equal to start

.sizeok
        ;------ resize the buffer
        add.l   d2,d0                   add len to pre-size
        move.l  a3,a0
        bsr     _BufResize
        tst.l   d0
        beq.b   .end
        move.l  d0,a3                   dest buffer is a new allocated buffer

        ;------ dig the hole
        move.l  d4,d0
        sub.l   d3,d0                   was start after end?   (old size - start)
        bls     .insert                 .yes, nothing to move
        move.l  a3,a0                   .yes, d0 = num to move to the right
        add.l   d3,a0
        move.l  a0,a1
        add.l   d2,a1
        bsr     _UIK_MemCopy

.insert
        ;------ insert source
        move.l  a2,a0
        move.l  a3,a1
        add.l   d3,a1
        move.l  d2,d0
        jsr     _LVOCopyMem(a6)
.ok
        move.l  a3,d0               return the copy buffer
.end
        movem.l (sp)+,d2-d4/a2-a3/a6
        rts


************************************************************************
*   UIKBUF *BufSet( UBYTE *src, ULONG len, UIKBUF *dest )
*                          a0         d0            a1
************************************************************************

_BufSet
        move.l  a1,d1
        beq.b   .error
        move.l  a0,d1
        beq.b   .zero

        moveq   #0,d1
        move.l  d1,-nubu_CurSize(a1)    set size to 0
        tst.l   d0
        beq.b   .zero
        bra.b   _BufPaste               paste to pos=0
.error
        moveq   #0,d0
        rts
.zero
        move.l  a1,d0
        add.l   -nubu_CurSize(a1),a1
        clr.b   (a1)
        rts

_BufSetS
        move.l  a1,-(sp)
        move.l  (a1),a1
        bsr.b   _BufSet
        move.l  (sp)+,a1
        tst.l   d0
        beq.b   .end
        move.l  d0,(a1)
.end    rts


**************************************************************************
*   UIKBUF *VBufPrintf( UIKBUF **ubuf, ULONG start, UYTE *fmt, APTR *argv )
*                                a0          d0           a1         d1
**************************************************************************

_VBufPrintf
        movem.l d0-d1/a0-a3/a6,-(sp)
        move.l  a0,a3               a3 = &ubuf
        move.l  (a3),d0             ubuf ?
        bne.b   .bufok

        move.l  #64,a0
        moveq   #0,d0
        move.l  #MEMF_ANY,d1
        bsr     _BufAlloc
        tst.l   d0
        beq.b   .end
.bufok
        move.l  d0,(a3)             range ubuf
        move.l  d0,a2               a2 = ubuf
        movem.l (sp),d0-d1/a0-a1    reprend les paramètres

        cmp.l   -nubu_CurSize(a2),d0    is start after end?
        bls     .sizeok                 .no, start is ok
        move.l  -nubu_CurSize(a2),d0    .yes, start becomes equal to size
.sizeok
        move.l  d0,-(sp)            start
        move.l  a0,-(sp)            &ubuf
        move.l  sp,a3               a3 = struct { UIKBUF **ubuf, ULONG cursor }
        move.l  a1,a0               Get the FormatString pointer
        move.l  d1,a1               Get the pointer to the DataStream
        lea.l   stuffChar(pc),a2
        move.l  4.w,a6
        jsr     _LVORawDoFmt(a6)
        move.l  (sp)+,a0
        addq.w  #4,sp
        move.l  (a0),(sp)           retourne d0 = ubuf
.end
        movem.l (sp)+,d0-d1/a0-a3/a6
        rts

stuffChar               ; d0 = char, a3 = struct { UIKBUF **ubuf, ULONG cursor }
        movem.l d2-d4/a2,-(sp)
        move.l  (a3),a0                 a0 = &ubuf
        move.l  (a0),a2                 a2 = dest buffer
        move.l  4(a3),d3                d3 = start
        move.b  d0,d2                   d2 = char
        beq   .end                    pas le caractère 0
        move.l  -nubu_CurSize(a2),d4    d4 = pre-size
        move.l  d4,d0
        addq.l  #1,d0                   newsize = pre-size + 1

        ;------ resize the buffer
        cmp.l   -nubu_TotSize(a2),d0    newsize - tot < 0 ? ok : resize
        bcc.b   .resize
        move.l  d0,-nubu_CurSize(a2)
        clr.b   0(a2,d0.l)
        bra.b   .sizeok
.resize
        move.l  a2,a0
        bsr     _BufResize
        tst.l   d0
        beq.b   .end
        move.l  d0,a2                   dest buffer is a new allocated buffer
.sizeok
        ;------ dig the hole
        move.l  d4,d0
        sub.l   d3,d0                   was start after end?   (old size - start)
        bls     .insert                 .yes, nothing to move
        lea     0(a2,d3.l),a0
        lea     1(a2,d3.l),a1
        bsr     _UIK_MemCopy
.insert
        ;------ insert char
        move.b  d2,0(a2,d3.l)
        addq.l  #1,d3
.end
        move.l  (a3),a0                 a0 = &ubuf
        move.l  a2,(a0)                 range ubuf
        move.l  d3,4(a3)
        movem.l (sp)+,d2-d4/a2
        rts
