    OPT O+ ,W-
        OPT d+
****************************************************************
*
*       Project:    UIK
*       Function:   Tags processing
*
*       Created:    05/02/91    Jean-Michel Forgeas
*       Assembler:  DevPac II
*
*       This code is CopyRight © 1991 Jean-Michel Forgeas
*       I hope it is compatible with corresponding function
*       calls from Commodore in utility.library (2.0).
*
****************************************************************

        IDNT    tag

******* Includes ************************************************

        INCDIR  "include:"

        INCLUDE "lib/exec_lib.i"
        INCLUDE "exec/memory.i"
        INCLUDE "uiki:uiktag.i"


******* Imported ************************************************


******* Exported ************************************************

        XDEF    _UIK_ApplyTagItems
        XDEF    _UIK_ApplyFindTag
        XDEF    _UIK_NextTagItem
        XDEF    _UIK_GetUserTag
        XDEF    _UIK_FindTagItem
        XDEF    _UIK_GetTagData
        XDEF    _UIK_SetTagData
        XDEF    _UIK_TagInArray
        XDEF    _UIK_FilterTagItems
        XDEF    _UIK_PackBoolTags
        XDEF    _UIK_FilterTagChanges
        XDEF    _UIK_MapTags
        XDEF    _UIK_AllocateTagItems
        XDEF    _UIK_CloneTagItems
        XDEF    _UIK_RefreshTagItemClones
        XDEF    _UIK_CopyTagItems
        XDEF    _UIK_FreeTagItems
        XDEF    _UIK_HowManyTagItems
        XDEF    _UIK_JoinTagItems


**************************************************************************
**************************************************************************
*       Routines
**************************************************************************
**************************************************************************

        SECTION text,CODE


**************************************************************************
*   FuncRet = ApplyTagItems( TagList, Func, FuncParm, TestRet, TestValue )
*   d0                       a0       a1    d2        d0       d1
*   modifies: d0 d1 a0 a1
**************************************************************************

_UIK_ApplyTagItems
        movem.l d3-d4/a2-a3,-(sp)
        move.l  a0,a2               a2 = TagList, a3 = Func
        move.l  a1,a3
        move.l  d0,d3               d2 = FuncParm, d3 = TestRet, d4 = TestValue
        move.l  d1,d4
        move.l  a2,d0
        beq.b   .end                if (TagList == 0) return(0)
        bra.b   .loop

.next   addq.w  #8,a2
.loop   move.l  a2,a0
        bsr.b   _UIK_GetUserTag     next tag
        beq.b   .end                if end of list goto end
        move.l  a0,a2               a0 = TagItem
        move.l  d2,d0               d0 = FuncParm
        jsr     (a3)                Func( struct TagItem *a0, ULONG FuncParm d0 );
        tst.l   d3
        beq.b   .next               if (TestRet == 0) continue
        cmp.l   d0,d4
        bne.b   .next               if (retcode != TestValue) continue;
.end
        movem.l (sp)+,d3-d4/a2-a3
        rts


**************************************************************************
*   struct TagItem  *NextTagItem( struct TagItem **TagItemPtr )
*   A0/D0                                          A0
*   modifies: d0 a0 a1
**************************************************************************

_UIK_NextTagItem
        move.l  (a0),a1
        exg     a0,a1               a0 = &TagItem[0]
        move.l  a0,d0               if (TagList == 0) return(0)
        beq.b   .end
        addq.w  #8,a0
        bsr.b   _UIK_GetUserTag
        move.l  d0,(a1)
        ;move.l  a0,d0
.end    rts


**************************************************************************
*   struct TagItem  *GetUserTag( struct TagItem *TagItem )
*   A0/D0                                        A0
*   modifies: d0 a0
**************************************************************************

* This routine (GetUserTag) returns the next Tag into a0 after
* processing the system Tags.
* If the end of TagList is reached, the Z flag is 1, else
* we are on a valid Tag. So a usual test after calling this
* routine can be: 'beq.b .end'

more    move.l  4(a0),a0            adress of next tag is in ti_Data
        bra.b   _UIK_GetUserTag     have it
skip    addq.w  #8,a0               skip 2 tags (this & the next)
next    addq.w  #8,a0               next tag
_UIK_GetUserTag
        move.l  (a0),d0             d0 = tag
        beq.b   .end                if (tag == TAG_DONE) return(0) Z flag set !
        subq.l  #1,d0
        beq.b   next                if (tag == TAG_IGNORE) continue
        subq.l  #1,d0
        beq.b   more                if (tag == TAG_MORE) goto more
        subq.l  #1,d0
        beq.b   skip                if (tag == TAG_SKIP) goto skip
.ok
        move.l  a0,d0               ok, found a valid Tag.
.end    rts                         Z == 1 ou 0 !!!


************************************************************************
*   struct TagItem  *FindTagItem( Tag TagVal, struct TagItem *TagList );
*                                     D0                      A0
*   modifies: d0 a0
************************************************************************

_UIK_FindTagItem
        move.l  d1,-(sp)
        move.l  a0,d1               if (TagList == 0) return(0)
        beq.b   .error

        move.l  d0,d1               d1 = Tag to find
        bra.b   .loop

.next   addq.w  #8,a0
.loop
        cmp.l   (a0),d1             if (tag == Tag to find) return
        beq.b   .end
        cmp.l   #TAG_SKIP,(a0)
        bhi.b   .next
        bsr.b   _UIK_GetUserTag     next tag
        bne.b   .loop
.error
        moveq   #0,d0               if end of list
        bra.b   .ret
.end
        move.l  a0,d0
.ret    move.l  (sp)+,d1
        tst.l   d0
        rts


**************************************************************************
*   ULONG GetTagData( Tag TagVal, ULONG Default, struct TagItem *TagList )
*                         D0            D1                       A0
*   modifies: d0 a0
**************************************************************************

_UIK_GetTagData
        bsr.b   _UIK_FindTagItem
        beq.b   .end                if (not found) return( Default )
        move.l  4(a0),d1            return( ti_Data )
.end    move.l  d1,d0
        rts


************************************************************************************
*   struct TagItem *SetTagData( Tag TagVal, ULONG newdata, struct TagItem *TagList )
*                         D0            D1                       A0
*   modifies: d0 a0
************************************************************************************

_UIK_SetTagData
        bsr.b   _UIK_FindTagItem
        beq.b   .end                if (not found) return(0)
        move.l  d1,4(a0)            ti_Data = newdata
        move.l  a0,d0               return( &TagItem )
.end    rts


**************************************************************************
*   ApplyFindTag( ATagList, FTagList, Perform )
*                 A0        A1        D0
*   modifies:
**************************************************************************

_UIK_ApplyFindTag
        movem.l a2/a3,-(sp)
        move.l  d0,a3               a3 = Perform
        move.l  a0,d0               if (ATagList == 0) return( a0 )
        beq.b   .end
        move.l  a0,a2
        bra.b   .loop

.next   addq.w  #8,a0
.loop
        bsr.b   _UIK_GetUserTag     next tag
        beq.b   .end                if end of list goto end
.ok
        ;---UIK_FindTagItem( Tag TagVal, struct TagItem *FTagList ); (d0/a0)
        move.l  a0,a2
        move.l  (a0),d0             d0 = Tag to find into...
        move.l  a1,a0               a0 = début FTagList
        bsr.b   _UIK_FindTagItem    d0 = a0=found or 0=notfound
        exg     a0,a2               a0 = ATagList, a2 = FTagList
        pea     .next(pc)
        jmp     (a3)
.end
        movem.l (sp)+,a2/a3
        rts


**************************************************************************
*   FilterTagItems( TagList, TagArray, Logic )
*                   A0       A1        D0
*   modifies: d0 d1 a0
**************************************************************************

_UIK_FilterTagItems
        move.l  d2,-(sp)
        moveq   #0,d2               d2 = number = 0
        move.l  a0,d1               if (TagList == 0) return(0)
        beq.b   .end
        move.l  d0,d1               d1 = logic
        bra.b   .loop

.next   addq.w  #8,a0
.loop   bsr     _UIK_GetUserTag     next tag
        beq.b   .end                if end of list goto end

        move.l  a0,-(sp)
        move.l  (a0),d0
        move.l  a1,a0
        bsr.b   _UIK_TagInArray     search tag in the area
        move.l  (sp)+,a0

        beq.b   .notfound           if (d0 == 0) goto notfound
        tst.b   d1                ; we found it
        bne.b   .and                but here is NOT
        bra.b   .not                but here is AND
.notfound                         ; not found in the area
        moveq   #1,d0
        tst.b   d1
        bne.b   .not
.and    move.l  d0,(a0)             here is AND
        bra.b   .next
.not    addq.l  #1,d2               here is NOT
        bra.b   .next
.end
        move.l  d2,d0
        move.l  (sp)+,d2
        rts


**************************************************************************
*   BOOL    TagInArray( Tag, TagArray )
*                       D0   A0
*   modifies: d0 a0
**************************************************************************

_UIK_TagInArray
        tst.l   (a0)
        beq.b   .error              if (tag == TAG_DONE) return(0)
        cmp.l   (a0)+,d0
        bne.b   _UIK_TagInArray
        moveq   #1,d0
.end    rts
.error  moveq   #0,d0
        rts


**************************************************************************
*   PackBoolTags( ULONG InitialFlags, struct TagItem *TagList,
*                           D0                            A0
*                                               struct TagItem *BoolMap );
*                                                               A1
*   modifies: d0 d1 a0
**************************************************************************

_UIK_PackBoolTags
        move.l  d0,d1               d1 = InitialFlags
        move.l  #.each,d0
        bsr.b   _UIK_ApplyFindTag
        move.l  d1,d0
        rts

        ;------ applied function
.each                              ;a0 = TagList, d0 = a2 = BoolMap
        beq.b   .end                d1 preserved !
        move.l  4(a2),d0            d0 = BoolMap->ti_Data
        tst.l   4(a0)               is TagList->ti_Data TRUE or FALSE ?
        beq.b   .false
.true   or.l    d0,d1               InitialFlags |= mask
        rts
.false  not.l   d0
        and.l   d0,d1               InitialFlags &= ~mask
.end    rts


**************************************************************************
*   FilterTagChanges( ChangeList, OldValues, Apply )
*                     A0          A1         D0
*   modifies:
**************************************************************************

_UIK_FilterTagChanges
        move.l  d0,d1               d1 = Apply
        move.l  #.each,d0
        jmp     _UIK_ApplyFindTag(pc)

        ;------ applied function
.each                              ;a0 = ChangeList, d0 = a2 = OldList, d1 preserved !
        beq.b   .nfound              if (not found) remove ChangeTag
.found
        move.l  4(a0),d0
        cmp.l   4(a2),d0            if (ChangeData == OldData) remove ChangeTag
        bne.b   .apply
.nfound moveq   #TAG_IGNORE,d0
        move.l  d0,(a0)
        rts
.apply  tst.l   d1                  Apply new values to OldValues?
        beq.b   .end                if (Apply == 1)
        move.l  4(a0),4(a2)             OldList->ti_Data = ChangeList->ti_Data
.end    rts


**************************************************************************
*   MapTags( TagList, MapList, IncludeMiss)
*            A0       A1       D0
*   modifies: d0 d1 a0
**************************************************************************

_UIK_MapTags
        move.l  d0,d1               d1 = IncludeMiss
        move.l  #.each,d0
        jmp     _UIK_ApplyFindTag(pc)

        ;------ applied function
.each                              ;a0 = TagList, d0 = a2 = MapList
        bne.b   .found              d1 preserved !
.notfound
        tst.l   d1                  include or miss ?
        bne.b   .end
        moveq   #TAG_IGNORE,d0      if (IncludeMiss==0) remove Tag
        move.l  d0,(a0)
        rts
.found  move.l  4(a2),(a0)          TagList->ti_Tag = MapList->ti_Data
.end    rts


**************************************************************************
*   TagList = AllocateTagItems( NumItems )
*                               D0
*   modifies: d0 d1 a0 a1
**************************************************************************

_UIK_AllocateTagItems
        move.l  a6,-(sp)
        addq.l  #1,d0               + 1 for TAG_DONE
        lsl.l   #3,d0               d0 = size
        addq.l  #4,d0               d0 = size + 4 to remember size

        move.l  #MEMF_ANY!MEMF_CLEAR,d1
        move.l  d0,-(sp)
        move.l  4.w,a6
        jsr     _LVOAllocMem(a6)
        exg     d0,a0               a0 = address
        move.l  (sp)+,d0

        move.l  a0,d1               save result address
        beq.b   .end                if no memory...

        move.l  d0,(a0)             remember size into the first long word
        addq.l  #4,d1               d1 point to the next

        subq.l  #8,d0               d0 = size - 8 = last TagItem offset
        add.l   d0,a0               a0 = last TagItem address
        moveq   #TAG_DONE,d0
        move.l  d0,(a0)             set last TagItem to TAG_DONE
.end
        move.l  d1,d0               return TagList address
        move.l  (sp)+,a6
        rts


**************************************************************************
*   NewTagList = CloneTagItems( TagList )
*                               A0
*   modifies: d0 d1 a0 a1
**************************************************************************

_UIK_CloneTagItems
        move.l  a0,-(sp)
        bsr.b   _UIK_HowManyTagItems    d0 = nombre de Tags dans TagList

        bsr.b   _UIK_AllocateTagItems
        move.l  (sp),a0             a0 = TagList
        move.l  d0,(sp)
        beq.b   .end                si pas de memoire return(0)

        move.l  d0,a1               a1 = NewTagList
        bsr.b   _UIK_CopyTagItems
.end
        move.l  (sp)+,d0
        rts


**************************************************************************
*   RefreshTagItemClones( CloneTagItems, OriginalTagItems )
*                         A0             A1
*   modifies: d0 a0 a1
**************************************************************************

_UIK_RefreshTagItemClones
        exg     a0,a1

**************************************************************************
*   CopyTagItems( OriginalTagItems, CloneTagItems )
*                 A0                A1
*   modifies: d0 a0 a1
**************************************************************************

_UIK_CopyTagItems
        move.l  a0,d0               if (STagList == 0) return(0)
        beq.b   .end

.loop
        cmp.l   #TAG_SKIP,(a0)
        bhi.b   .ok
        bsr     _UIK_GetUserTag     get a valid tag from a0
        beq.b   .end                if end of list...
.ok
        move.l  (a0)+,(a1)+         copy ti_Tag
        move.l  (a0)+,(a1)+         copy ti_Data
        bra.b   .loop
.end
        rts


**************************************************************************
*   FreeTagItems( TagList )
*                 A0
*   modifies: d0 d1 a0 a1
**************************************************************************

_UIK_FreeTagItems
        move.l  a0,d0
        beq.b   .end
        subq.w  #4,a0
        move.l  a0,a1
        move.l  (a0),d0
        move.l  a6,-(sp)
        move.l  4.w,a6
        jsr     _LVOFreeMem(a6)
        move.l  (sp)+,a6
.end    rts


**************************************************************************
*   HowManyTagItems( TagList )
*                    A0
*   modifies: d0 a0 a0
**************************************************************************

_UIK_HowManyTagItems
        moveq   #0,d1
.loop
        cmp.l   #TAG_SKIP,(a0)
        bhi.b   .ok
        bsr     _UIK_GetUserTag     get a valid tag from a0
        beq.b   .end
.ok
        addq.l  #1,d1
        addq.w  #8,a0
        bra.b   .loop
.end
        move.l  d1,d0
        rts


**************************************************************************
*   JoinTagItems( TagList1, TagList2 )
*                 A0        A1
*   modifies: d0 d1 a0 a1
**************************************************************************

_UIK_JoinTagItems
        bsr.b   _UIK_HowManyTagItems    a0 = dernier Tag dans TagList
        moveq   #TAG_MORE,d0
        move.l  d0,(a0)+
        move.l  a1,(a0)
        rts


**************************************************************************
        END
