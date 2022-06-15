    OPT O+,W-
        OPT d+
*****************************************************************
*
*       Project:    UIK
*       Function:   UIKList management
*
*       Created:    10/07/91    Jean-Michel Forgeas
*
*****************************************************************


******* Includes ************************************************

        INCDIR  "include:"

        INCLUDE "exec/memory.i"
        INCLUDE "lib/exec_lib.i"

        INCLUDE "uiki:uiklist.i"
        INCLUDE "uiki:uikbuf.i"
        INCLUDE "uiki:uikmacros.i"


******* Imported ************************************************

        XREF    _IntUIK_ExtendList
        XREF    _UIK_MemCopy

        XREF    _BufAlloc
        XREF    _BufAllocP
        XREF    _BufResize
        XREF    _BufFree
        XREF    _BufLength
        XREF    _BufCut
        XREF    _BufCopy
        XREF    _BufPaste

        XREF    _AsmAllocPooled
        XREF    _AsmFreePooled
        XREF    _AsmCreatePool
        XREF    _AsmDeletePool


******* Exported ************************************************

        XDEF    _UIK_nl_ListPtr
        XDEF    _UIK_nl_ListNodeNum
        XDEF    _UIK_nl_NodeBuf
        XDEF    _UIK_nl_NodeBufLen
        XDEF    _UIK_nl_CrossList
        XDEF    _UIK_nl_FreeList
        XDEF    _UIK_nl_AllocNode
        XDEF    _UIK_nl_FreeNodes
        XDEF    _UIK_nl_FindNode


*****************************************************************


*****************************************************************
*
                    SECTION     text,CODE
*
*****************************************************************


**************************************************************************
* struct UIKList * __asm UIK_nl_ListPtr( register __a0 struct UIKList *L )
*{
*    while (L->BlockSize == ~0) L = (struct UIKList *) L->UNode;
*    return( L );
*}
**************************************************************************
_UIK_nl_ListPtr
    moveq   #-1,d0
.loop
    cmp.l   uikl_BlockSize(a0),d0
    bne.b   .end
    move.l  uikl_UNode(a0),a0
    bra.b   .loop
.end
    move.l  a0,d0
    rts


**************************************************************************
*ULONG __asm UIK_nl_ListNodeNum( register __a0 struct UIKList *L )
*{
*    return( (UIK_nl_ListPtr( L ))->NodeNum );
*}
**************************************************************************
_UIK_nl_ListNodeNum
    moveq   #-1,d0
.loop
    cmp.l   uikl_BlockSize(a0),d0
    bne.b   .end
    move.l  uikl_UNode(a0),a0
    bra.b   .loop
.end
    move.l  uikl_NodeNum(a0),d0
    rts


**************************************************************************
*UBYTE * __asm UIK_nl_NodeBuf( register __a0 struct UIKList *L,
*                                               register __d0 LONG index )
*{
*    return( (UIK_nl_ListPtr( L ))->UNode[index].Buf );
*}
**************************************************************************
_UIK_nl_NodeBuf
    moveq   #-1,d1
.loop
    cmp.l   uikl_BlockSize(a0),d1
    bne.b   .ok
    move.l  uikl_UNode(a0),a0
    bra.b   .loop
.ok
    move.l  uikl_UNode(a0),a0
    asl.l   #3,d0
    add.l   d0,a0               &L->UNode[index]
    move.l  (a0),d0             L->UNode[index].Buf
    rts


**************************************************************************
*ULONG __asm UIK_nl_NodeBufLen( register __a0 struct UIKList *L,
*                                               register __d0 LONG index )
**************************************************************************
_UIK_nl_NodeBufLen
    bsr     _UIK_nl_NodeBuf
    move.l  d0,a0
    bsr     _BufLength
    rts


**************************************************************************
*int __asm UIK_nl_CrossList(
*register __a0 struct UIKList *L,
*register __a1 int (*func)(),
*register __d0 int check,
*register __d1 ULONG parm )
*{
*  register LONG ind=-1;
*  register struct UIKNode *node;
*  ULONG /*nodenum,*/ r;
*
*    L = UIK_nl_ListPtr( L );
*    while (++ind < (/*nodenum =*/ L->NodeNum)) /* L->NodeNum peut etre modifie par *func */
*        {
*        node = &L->UNode[ind];
*        r = (*func)( node->Buf, parm, ind, L, node->Buf, node );
*                                 d1   d2,  d3,    a0,     a1
*        if (check && !r) return(0);
*        /*if (nodenum == L->NodeNum) ind++;*/
*        }
*    return(1);
*}
**************************************************************************
_UIK_nl_CrossList
    movem.l d2-d4/a2-a3,-(sp)
    sub.l   #24,sp      reserve pour l'appel de a2
    move.l  d1,4(sp)    met le parm à sa place
    moveq   #-1,d2      d2 = ind
    move.l  d0,d4       d4 = check
    move.l  a1,a2       a2 = func
    lea     4(sp),a3

    moveq   #-1,d0
.loop
    cmp.l   uikl_BlockSize(a0),d0
    bne.b   .while
    move.l  uikl_UNode(a0),a0
    bra.b   .loop

.next
    move.l  uikl_UNode(a0),a1
    move.l  d2,d0
    asl.l   #3,d0
    add.l   d0,a1
    move.l  (a1),d3     d3 = node->Buf
    move.l  d3,(sp)
    exg     a0,d3
    movem.l d1/d2/d3/a0/a1,(a3)  node->Buf, parm, ind, L, node->Buf, node
    jsr     (a2)
    movem.l (a3),d1/d2/d3/a0/a1
    exg     a0,d3
    tst.l   d4
    beq.b   .while      if (check)
    tst.l   d0
    beq.b   .end
.while
    addq.l  #1,d2       ++ind
    cmp.l   (a0),d2     if ind < L->NodeNum
    bcs.b   .next       continue

.endok
    moveq   #1,d0
.end
    add.l   #24,sp      reserve pour l'appel de a2
    movem.l (sp)+,d2-d4/a2-a3
    rts


**************************************************************************
*void __asm UIK_nl_FreeList( register __a0 struct UIKList *L )
*{
*  register LONG ind=-1;
*  register struct UIKNode *node;
*
*    if (L->BlockSize != ~0)
*        if (L->UNode)
*            {
*            while (++ind < L->NodeNum)
*                {
*                node = &L->UNode[ind];
*                BufFree( node->Buf );
*                }
*            BufFree( L->UNode );
*            L->UNode = 0;
*            }
*    L->NodeNum = L->MaxNodes = 0;
*}
**************************************************************************
_UIK_nl_FreeList
    movem.l d2-d3/a2-a3,-(sp)
    moveq   #-1,d2                  d2 = ind
    move.l  a0,a2

    cmp.l   uikl_BlockSize(a2),d2
    beq.b   .end                    if (L->BlockSize != ~0)

    move.l  uikl_UNode(a2),d0
    beq.b   .end                    if (L->UNode)
    move.l  d0,a3                   a3 = node
    move.l  uikl_NodeNum(a2),d3     d3 = NodeNum

    move.l  -nubu_MemReqs(a3),d1
    cmp.l   d2,d1                   UIKBuf->MemReqs == -1   -->  uikbuf pooled
    bne.b   .while                  si oui, c'est une uiklist pooled : struct UIKPList
    bclr    #UIKPLB_POOLOWNER,uikpl_Flags+1(a2)
    beq.b   .while

    move.l  -nubup_Pool(a3),a0      si on est le owner du pool, ça va plus vite!
    move.l  a6,-(sp)
    move.l  4.w,a6
    jsr     _AsmDeletePool
    move.l  (sp)+,a6
    bra.b   .clear

    ;--------------
.next
    move.l  (a3),a1                 uikn_Buf
    bsr     _BufFree

    addq.w  #8,a3
.while
    addq.l  #1,d2
    cmp.l   d3,d2       if ind < L->NodeNum
    bcs.b   .next       continue

    move.l  uikl_UNode(a2),a1
    bsr     _BufFree
    ;--------------
.clear
    clr.l   uikl_UNode(a2)      L->UNode = 0

.end
    moveq   #0,d0
    move.l  d0,(a2)+            L->NodeNum = 0
    move.l  d0,(a2)             L->MaxNodes = 0
    movem.l (sp)+,d2-d3/a2-a3
    rts


**************************************************************************
*UBYTE * __asm UIK_nl_AllocNode(
*register __a0 struct UIKList *L,
*register __d0 LONG ind,
*register __d1 LONG len,
*register __a1 WORD flags )
*{
*  register UBYTE *ptr;
*  register LONG nnum;
*
*    L = UIK_nl_ListPtr( L );
*    nnum = L->NodeNum;
*    if (ind > nnum || ind < 0) ind = nnum;
*
*    if (nnum+1 >= L->MaxNodes) if (! IntUIK_ExtendList( L )) return(0);
*    if (! (ptr = BufAlloc( 4, len, MEMF_CLEAR!MEMF_ANY ))) return(0);
*
*    UIK_MemCopy( (char*)&L->UNode[ind], (char*)&L->UNode[ind+1], UTABLESIZE(nnum - ind) );
*
*    L->UNode[ind].Buf = ptr;
*    L->UNode[ind].Reserved = len;
*    L->UNode[ind].Flags = flags;
*    L->NodeNum++;
*    return( ptr );
*}
**************************************************************************
_UIK_nl_AllocNode
    movem.l d2-d5/a2-a3,-(sp)
    move.l  d0,d2                   d2 = ind
    move.w  a1,d4                   d4 = flags
    move.l  d1,d5                   d5 = len

    moveq   #-1,d0
.loop
    cmp.l   uikl_BlockSize(a0),d0
    bne.b   .ok
    move.l  uikl_UNode(a0),a0
    bra.b   .loop

.ok
    tst.l   uikl_BlockSize(a0)
    bne.b   .blkok
    moveq   #10,d1
    move.l  d1,uikl_BlockSize(a0)
.blkok
    move.l  a0,a2
    move.l  (a2),d3                 d3 = NodeNum
    moveq   #-1,d0
    cmp.l   d2,d0
    beq.b   .last
    cmp.l   d2,d3                   if (ind > nnum) non signé
    bcc.b   .10
.last
    move.l  d3,d2
.10
    move.l  d3,d0
    addq.l  #1,d0
    cmp.l   uikl_MaxNodes(a2),d0
    bcs.b   .20                     if (nnum+1 >= L->MaxNodes)
    bsr     _IntUIK_ExtendList      ( L == a0 )
    tst.l   d0                      if (! IntUIK_ExtendList( L )) return(0)
    beq.b   .end
.20
    move.l  uikl_UNode(a2),a3
    cmp.l   #-1,-nubu_MemReqs(a3)   UIKBuf->MemReqs == -1   -->  uikbuf pooled
    bne.b   .std                    si oui, c'est une uiklist pooled : struct UIKPList

.pool       ;------ pooled buffer
    move.l  -nubup_Pool(a3),a0
    move.l  d5,d0
    moveq   #4,d1                       a0 = pool, d0 = size, d1 = blksize
    bsr     _BufAllocP                  if (! (ptr = BufAllocP( pool, len, 4 ))) return(0);
    moveq   #0,d5                       pour mettre reserved à 0 pour AZur, si pooled pour compatibilité
    bra.b   .test

.std        ;------ std Buffer
    move.l  d5,a0
    moveq   #4,d0
    exg     d0,a0
    move.l  #MEMF_CLEAR!MEMF_ANY,d1     a0 = blksize, d0 = size, d1 = reqs
    bsr     _BufAlloc                   if (! (ptr = BufAlloc( 4, len, MEMF_CLEAR!MEMF_ANY ))) return(0);

.test
    tst.l   d0
    beq.b   .end
    addq.l  #1,uikl_NodeNum(a2)     L->NodeNum++;
    move.l  d0,a2                   a2 = ptr

    move.l  d3,d0
    sub.l   d2,d0
    asl.l   #3,d0                   UTABLESIZE(nnum - ind)
    asl.l   #3,d2
    add.l   d2,a3
    move.l  a3,a0                   &L->UNode[ind]
    move.l  a3,a1
    addq.w  #8,a1                   &L->UNode[ind+1]
    bsr     _UIK_MemCopy            UIK_MemCopy( (char*)&L->UNode[ind], (char*)&L->UNode[ind+1], UTABLESIZE(nnum - ind) );

    move.l  a2,uikn_Buf(a3)         L->UNode[ind].Buf = ptr;
    move.w  d5,uikn_Reserved(a3)    L->UNode[ind].Reserved = len;   mis à 0 pour AZur si pooled
    move.w  d4,uikn_Flags(a3)       L->UNode[ind].Flags = flags;
    move.l  a2,d0                   return( ptr );
.end
    movem.l (sp)+,d2-d5/a2-a3
    rts


**************************************************************************
*void __asm UIK_nl_FreeNodes(
*register __a0 struct UIKList *L,
*register __d0 LONG from,
*register __d1 LONG to )
*{
*  register struct UIKNode *node, *startnode;
*  register int n;
*  register LONG ind, nnum;
*
*    L = UIK_nl_ListPtr( L );
*    if (! (nnum = L->NodeNum)) return;
*    if (from < 0) from = nnum - 1;
*    if (from >= nnum) return;
*    if (to >= nnum || to < 0) to = nnum - 1;
*    if (to < from) return;
*
*    startnode = node = &L->UNode[ind=from];
*    while (ind <= to)
*        {
*        BufFree( node->Buf );
*        node = &L->UNode[++ind];
*        }
*    n = UTABLESIZE(nnum - 1 - to);
*    if (n) _UIK_MemCopy( (char*)node, (char*)startnode, n );
*    L->NodeNum -= (to - from + 1);
*}
**************************************************************************
_UIK_nl_FreeNodes
    movem.l d2-d4/a2-a3/a5,-(sp)

    moveq   #-1,d2
.loop
    cmp.l   uikl_BlockSize(a0),d2
    bne.b   .ok
    move.l  uikl_UNode(a0),a0
    bra.b   .loop
.ok
    move.l  a0,a2
    move.l  (a2),d3
    beq.b   .end                    if (! (d3 = L->NodeNum)) return;

    cmp.l   d0,d2
    bne.b   .10
    move.l  d3,d0
    subq.l  #1,d0                   if (from < 0) from = nnum - 1;
.10
    cmp.l   d3,d0
    bcc.b   .end                    if (from >= nnum) return;

    cmp.l   d3,d1
    bcc.b   .to                     if (to >= nnum
    cmp.l   d1,d2
    bne.b   .tofrom                 || to < 0)
.to
    move.l  d3,d1
    subq.l  #1,d1                   to = nnum - 1;

.tofrom
    cmp.l   d0,d1
    bcs.b   .end                    if (to < from) return;


    move.l  d0,d2                   d2 = from
    move.l  d1,d3                   d3 = to
    move.l  d0,d4                   d4 = ind=from
    move.l  uikl_UNode(a2),a3
    lsl.l   #3,d0
    add.l   d0,a3                   node = &L->UNode[ind=from];
    move.l  a3,a5                   startnode = node
    bra.b   .while

.next
    move.l  (a3),a1                 node->Buf
    bsr     _BufFree                BufFree( node->Buf );
.nofree
    addq.w  #8,a3
    addq.l  #1,d4                   node = &L->UNode[++ind];
.while
    cmp.l   d4,d3       while (ind <= to)
    bcc.b   .next       continue

    move.l  (a2),d0
    subq.l  #1,d0
    sub.l   d3,d0
    lsl.l   #3,d0               n = UTABLESIZE(nnum - 1 - to);
    beq.b   .nocopy             if (n)
    move.l  a3,a0
    move.l  a5,a1
    bsr     _UIK_MemCopy        CopyMemQuick( (char*)node, (char*)startnode, n );
.nocopy
    sub.l   d2,d3
    addq.l  #1,d3
    sub.l   d3,(a2)             L->NodeNum -= (to - from + 1);
.end
    movem.l (sp)+,d2-d4/a2-a3/a5
    rts


**************************************************************************
*LONG __asm UIK_nl_FindNode(
*register __a0 struct UIKList *L,
*register __d0 UBYTE *buf )
*{
*  register LONG ind=-1;
*
*    L = UIK_nl_ListPtr( L );
*    for (;;)
*        {
*        if (++ind >= L->NodeNum) return(-1);
*        if (L->UNode[ind].Buf == buf) break;
*        }
*    return( ind );
*}
**************************************************************************
_UIK_nl_FindNode
    move.l  d2,-(sp)
    moveq   #-1,d1
    exg     d0,d1               d0 = ind, d1 = buf
.loop
    cmp.l   uikl_BlockSize(a0),d0
    bne.b   .ok
    move.l  uikl_UNode(a0),a0
    bra.b   .loop
.ok
    move.l  (a0),d2             d2 = NodeNum
    move.l  uikl_UNode(a0),a1
    subq.w  #8,a1
.for
    addq.l  #1,d0
    cmp.l   d2,d0
    bcc.b   .err
    addq.w  #8,a1
    cmp.l   uikn_Buf(a1),d1
    bne.b   .for
    bra.b   .end
.err
    moveq   #-1,d0
.end
    move.l  (sp)+,d2
    rts

*****************************************************************
        END

