    OPT O+,W-
        OPT d+
****************************************************************
*
*       Project:    UIK
*       Function:   uik objects
*
*       Created:    11/03/91    Jean-Michel Forgeas
*
****************************************************************

        IDNT    objgada


******* Includes ************************************************

        INCDIR  "include:"

        INCLUDE "exec/memory.i"
        INCLUDE "lib/exec_lib.i"
        INCLUDE "lib/layers_lib.i"
        INCLUDE "lib/locale_lib.i"
        INCLUDE "exec/execbase.i"

        INCLUDE "uiki:uikobj.i"
        INCLUDE "uiki:uikbase.i"
        INCLUDE "uiki:uikmacros.i"
        INCLUDE "uiki:uikglobal.i"
        INCLUDE "uiki:uiktag.i"
        INCLUDE "uiki:objrequest.i"


******* Imported ************************************************

        XREF    _UIKBase
        XREF    _LayersBase
        XREF    _LocaleBase

        XREF    _IntUIK_GetRastPort
        XREF    _StdGadInit
        XREF    _StopUIKGadget
        XREF    _StdGadResize
        XREF    _StdGadRefresh
        XREF    _IntUIK_PushBox
        XREF    _IntUIK_PopBox
        XREF    _IntUIK_DrawBox
        XREF    _IntUIK_DrawAuto
        XREF    _HdlUIK_VerifActGadget
        XREF    _UIK_nl_CrossList
        XREF    _UIK_AddObject
        XREF    _UIK_CauseObjHook
        XREF    _UIK_CauseObjLink
        XREF    _UIK_MemSpLength
        XREF    _UIK_MemSpAlloc
        XREF    _UIK_MemSpFree
        XREF    _StrLen
        XREF    _StrCpy


******* Exported ************************************************

        XDEF    _IntUIK_AddObjectTags
        XDEF    _UIK_CallObjectFunc
        XDEF    _UIK_InsideObj
        XDEF    _UIK_LockObjVars
        XDEF    _UIK_UnlockObjVars
        XDEF    LockObjVars
        XDEF    UnlockObjVars
        XDEF    _UIK_ObjTitle
        XDEF    _UIK_ObjAltTitle
        ;XDEF    _UIK_ChangeTitle
        ;XDEF    _UIK_ChangeAltTitle
        XDEF    _UIK_LangString
        XDEF    _UIK_ClipObject
        XDEF    _UIK_UnclipObject
        XDEF    _UIK_GetObjectValues
        XDEF    _UIK_SetObjectValues

        XDEF    _UIK_StdGadStart
        XDEF    _UIK_StdGadStop
        XDEF    _UIK_StdGadResize
        XDEF    _UIK_StdGadRefresh
        XDEF    _UIK_StdGadAct
        XDEF    _UIK_StdGadInact
        XDEF    _UIK_StdGadMove
        XDEF    _HdlUIK_StdGadAct
        XDEF    _HdlUIK_StdGadInact
        XDEF    _HdlUIK_StdGadMove
        XDEF    _UIK_DrawBox
        XDEF    _UIK_DrawAuto
        XDEF    _UIK_Select
        XDEF    _UIK_RefreshSons
        XDEF    _UIK_Refresh
        XDEF    _UIK_ObjDisable
        XDEF    _UIK_ObjEnable
        XDEF    _do_lockedrefresh
        XDEF    _UIK_RefreshWindow
        XDEF    _ExecuteClass
        XDEF    _ExecuteMask
        XDEF    _ExecuteLink
        XDEF    _UIK_ActivateKeyTank
        XDEF    _UIK_InactivateKeyTank



****************************************************************
*
*       Routines
*
****************************************************************/

        SECTION text,CODE


_IntUIK_AddObjectTags
    move.l  4(sp),a0
    move.l  8(sp),d0
    lea     12(sp),a1
    bra     _UIK_AddObject


*******************************************************
*   res = UIK_CallObjectFunc( obj, func, data1, data2 )
*   d0                        a1   d0    a0     d1
*******************************************************

_UIK_CallObjectFunc
    move.l  d1,-(sp)                data2
    move.l  a0,-(sp)                data1
    move.l  a1,-(sp)                object
    beq.b   .err
    exg     d0,d1                   d1 = offset

    move.l  uo_ExtentedVectors(a1),d0
    beq.b   .end                    pas de table de fonctions
    move.l  d0,a0                   a0 = fonctions

    move.l  uo_ObjExt(a1),a1
    move.l  uoe_LastFunc(a1),d0
    cmp.l   d1,d0                   if (last - offset >= 0) ok
    bcc.b   .ok
    moveq   #0,d0
    bra.b   .end
.ok
    move.l  0(a0,d1.l),d0
    beq.b   .end                    pas de fonction
    move.l  d0,a0

    jsr     (a0)                    appel de la fonction objet
.end
    add.w   #12,sp
    rts
.err
    moveq   #0,d0
    bra.b   .end


*************************************
*   in = UIK_InsideObj( obj, xy )
*   d0                  a0   a1
*************************************

_UIK_InsideObj
        movem.l d2-d5,-(sp)
        move.l  uo_rq(a0),d1                d1 = requester
        exg.l   d1,a1                       indicateurs non affectes, d1=xy
        beq.b   .win                        si pas de requester
        sub.l   rq_LeftEdge(a1),d1          xy += LeftTop
.win
        moveq   #0,d0                       assume outside
        movem.w uo_Box+box_Left(a0),d2-d5
        sub.w   d3,d1
        bmi.b   .outside
        sub.w   d5,d1
        bpl.b   .outside
        swap    d1
        sub.w   d2,d1
        bmi.b   .outside
        sub.w   d4,d1
        bpl.b   .outside
        moveq   #1,d0                       return inside
.outside
        movem.l (sp)+,d2-d5
        rts


*************************************
*   UIK_LockObjVars( obj )
*   UIK_UnlockObjVars( obj )
*                      a0
*************************************

LockObjVars
        move.l  8(sp),a0
_UIK_LockObjVars
        move.l  uo_SigSemaphore(a0),d1     uop_Semaphore = uol_Semaphore
        beq.b   .end

        exg     d1,a0                      a0=semaphore, d1=obj
        move.l  a6,-(sp)
        move.l  4.w,a6
        jsr     _LVOObtainSemaphore(a6)    preserve all registers
        move.l  (sp)+,a6
        exg     d1,a0                      a0=obj, d1=semaphore
.end
        tst.b   uo_Status(a0)
        rts


UnlockObjVars
        move.l  4(sp),a0
_UIK_UnlockObjVars
        move.l  uo_SigSemaphore(a0),d1
        beq.b   .end

        exg     d1,a0                      a0=semaphore, d1=obj
        move.l  a6,-(sp)
        move.l  4.w,a6
        jsr     _LVOReleaseSemaphore(a6)   preserve all registers
        move.l  (sp)+,a6
        exg     d1,a0                      a0=obj, d1=semaphore
.end
        rts


*************************************
*   bool = UIK_ClipObject( obj )
*   d0                     a1
*************************************
_UIK_ClipObject
        move.l  a1,a0
        bsr.b   _UIK_LockObjVars
        tst.b   uo_Status(a1)
        bne.b   .end                    ;---- if Z set -> object not started

        btst.b  #BOXB_CLIPPED,uo_Flags+3(a1)
        bne.b   .ok
        btst.b  #TITLEB_CLIPPED,uo_TitleFlags+3(a1)
        beq.b   .end
.ok
        move.l  uo_Region(a1),d0
        beq.b   .end
        addq.b  #1,uo_ClipCount(a1)
        bne.b   .end
        move.l  uo_rp(a1),d0
        beq.b   .end
        movem.l a1/a6,-(sp)
        move.l  d0,a0
        move.l  (a0),a0                     a0 = Layer
        move.l  uo_Region(a1),a1            a1 = Region
        move.l  _LayersBase,a6
        jsr     _LVOInstallClipRegion(a6)   install new clip
        movem.l (sp)+,a1/a6
        move.l  d0,uo_SpareRegion(a1)
.end
        move.l  a1,a0
        bsr.b   _UIK_UnlockObjVars
        rts


*************************************
*   UIK_UnclipObject( obj )
*                     a1
*************************************
_UIK_UnclipObject
        move.l  a1,a0
        bsr     _UIK_LockObjVars
        tst.b   uo_Status(a1)
        bne.b   .end               ;---- if Z set -> object not started

        btst.b  #BOXB_CLIPPED,uo_Flags+3(a1)
        bne.b   .ok
        btst.b  #TITLEB_CLIPPED,uo_TitleFlags+3(a1)
        beq.b   .end
.ok
        move.l  uo_Region(a1),d0
        beq.b   .end
        moveq   #-1,d0
        move.b  uo_ClipCount(a1),d1
        cmp.b   d0,d1
        beq.b   .end
        subq.b  #1,d1
        move.b  d1,uo_ClipCount(a1)
        cmp.b   d0,d1
        bne.b   .end
        move.l  uo_rp(a1),d0
        beq.b   .end
        movem.l a1/a6,-(sp)
        move.l  d0,a0
        move.l  (a0),a0                     a0 = Layer
        move.l  uo_SpareRegion(a1),a1       a1 = Region
        move.l  _LayersBase,a6
        jsr     _LVOInstallClipRegion(a6)   install old clip
        movem.l (sp)+,a1/a6
        clr.l   uo_SpareRegion(a1)
.end
        move.l  a1,a0
        bsr     _UIK_UnlockObjVars
        rts


*****************************************
* str = UIK_ObjTitle( obj )
* d0                  a0
*****************************************
_UIK_ObjTitle
        move.l  a0,d0
        beq.b   title_end
        move.l  uo_Title(a0),d0
        bra.b   common_objtitle


*****************************************
* str = UIK_ObjAltTitle( obj )
* d0                     a0
*****************************************
_UIK_ObjAltTitle
        move.l  a0,d0
        beq.b   title_end
        move.l  uo_AltTitle(a0),d0

common_objtitle
        btst.b  #TITLEB_ADDR,uo_TitleFlags+3(a0)
        bne.b   title_end
        btst.b  #TITLEB_IMAGE,uo_TitleFlags+3(a0)
        beq.b   _UIK_LangString
title_end
        rts


*****************************************
* str = UIK_LangString( obj, index )
* d0                    a0   d0
*****************************************
_UIK_LangString
        move.l  d0,d1
        beq.b   .end

        add.l   d1,d1                   ----------c'est du texte
        add.l   d1,d1                   d1 *= 4
        move.l  uo_ObjExt(a0),a0
        move.l  uoe_Lang(a0),a0
        move.l  ula_Master(a0),a0
        move.l  uo_ObjExt(a0),a0
        move.l  uoe_Lang(a0),a0         a0 = UIKLang
        move.l  ula_Langage(a0),d0
        beq.b   .nul
        move.l  d0,a1
        move.l  0(a1,d1.l),d0           d0 = chaine dans le langage
.nul
        move.l  _UIKBase,a1
        move.l  lb_UIKPrefs(a1),a1
        btst    #UIKLANGB_LOCALE,uikp_PrefsFlags+1(a1)
        beq.b   .end

        tst.l   ula_Catalog(a0)         y-t-il un catalogue?  non: retourne d0
        beq.b   .end
        move.l  d0,a1                   a1 = defaultString
        move.l  d1,d0
        lsr.l   #2,d0                   d0 = stringNum = index /4
        move.l  ula_Catalog(a0),a0      a0 = catalog
        move.l  a6,-(sp)
        move.l  _LocaleBase,a6
        jsr     _LVOGetCatalogStr(a6)   string = GetCatalogStr(catalog,stringNum,defaultString); (A0/D0/A1)
        move.l  (sp)+,a6
.end
        rts



**********************************************
* str = UIK_ChangeTitle( obj, english, other )
* d0                     a0      d0     d1
**********************************************
;_UIK_ChangeTitle
;        lea     uo_Title(a0),a1
;        bra.b   common_changetitle
;
;
*************************************************
* str = UIK_ChangeAltTitle( obj, english, other )
* d0                        a0      d0     d1
*************************************************
;_UIK_ChangeAltTitle
;        lea     uo_AltTitle(a0),a1
;
;common_changetitle
;        movem.l d0/d1/d2/d3/a2-a5,-(sp)
;        btst.b  #TITLEB_ADDR,uo_TitleFlags+3(a0)
;        beq.b   .tables
;
;        move.l  a1,a3
;        move.l  (sp),a5                 a5 = new title
;        bsr.b   change_title
;        bra.b   .end
;.tables
;        move.l  uo_UIK(a0),a2           a2 = uik
;        move.l  (a1),d2                 d2 = title_num
;        add.l   d2,d2
;        add.l   d2,d2                   d2 *= 4
;        beq.b   .end
;
;        move.l  uike_LangEnglish(a2),a4
;        move.l  (sp),a5                 a5 = english new title
;        bsr.b   change_title
;        beq.b   .end
;
;        move.l  uike_LangEnglish(a2),a4
;        move.l  4(sp),a5                a5 = other new title
;        bsr.b   change_title
;.end
;        addq.l  #4,sp
;        movem.l (sp)+,d1/d2/d3/a2-a5
;        rts
;
;
;change_title
;        move.l  a4,d0
;        beq.b   end
;        lea     0(a4,d2.l),a3           a3 = pointe sur le pointeur de la chaine dans le langage
;have_old
;        move.l  a5,d0
;        beq.b   end
;
;        move.l  (a3),a0
;        bsr     _UIK_MemSpLength
;        move.l  d0,d3                   d3 = longueur de l'ancienne + 1
;
;        move.l  a5,a0
;        bsr     _StrLen
;        addq.l  #1,d0                   d0 = longueur de la nouvelle chaine + 1
;
;        cmp.l   d0,d3
;        bcc.b   .copy
;
;        move.l  #MEMF_ANY!MEMF_CLEAR,d1
;        bsr     _UIK_MemSpAlloc
;        beq.b   end
;        move.l  (a3),a1                 échange les pointeurs
;        move.l  d0,(a3)
;        bsr     _UIK_MemSpFree          libére l'ancien
;.copy
;        cmp.l   a0,a1                   sont-ce le même ?
;        beq.b   end
;        move.l  (a3),a0                 dest
;        move.l  a5,a1                   source
;        bsr     _StrCpy                 copy...
;end
;        rts


********************************************
*   d0 = UIK_GetObjectValues( obj, taglist )
*                             a0   a1
********************************************
_UIK_GetObjectValues

********************************************
*   d0 = UIK_SetObjectValues( obj, taglist )
*                             a0   a1
********************************************
_UIK_SetObjectValues
    moveq   #0,d0
    rts


*************************************
*   UIK_StdGadStart( obj )
*                    a0
*************************************
_UIK_StdGadStart
    lea     _StdGadInit(pc),a1
    bra.b   CallStack


*************************************
*   UIK_StdGadStop( obj )
*                   a0
*************************************
_UIK_StdGadStop
    lea     _StopUIKGadget(pc),a1
    bra.b   CallStack


*************************************
*   UIK_StdGadResize( obj )
*                     a0
*************************************
_UIK_StdGadResize
    lea     _StdGadResize(pc),a1
    bra.b   CallStack


*************************************
*   UIK_StdGadRefresh( obj )
*                      a0
*************************************
_UIK_StdGadRefresh
    lea     _StdGadRefresh(pc),a1
    bra.b   CallStack


*************************************
*   UIK_StdGadAct( obj )
*                  a0
*************************************
_HdlUIK_StdGadAct
    move.l  a1,a0
_UIK_StdGadAct
    lea     _IntUIK_PushBox(pc),a1
    ;pea     _UIK_RefreshSons(pc)
    bra.b   CallStack


*************************************
*   UIK_StdGadInact( obj )
*                    a0
*************************************
_HdlUIK_StdGadInact
    move.l  a1,a0
_UIK_StdGadInact
    lea     _IntUIK_PopBox(pc),a1
    ;pea     _UIK_RefreshSons(pc)
    bra.b   CallStack


*************************************
*   UIK_StdGadMove( obj, imsg )
*                   a0   a3
*************************************
_HdlUIK_StdGadMove
    move.l  a1,a0
_UIK_StdGadMove
    lea     _HdlUIK_VerifActGadget(pc),a1
    ;pea     _UIK_RefreshSons(pc)
    bra.b   CallStack


*************************************
*   UIK_DrawBox( obj )
*                a0
*************************************
_UIK_DrawBox
    lea     _IntUIK_DrawBox(pc),a1
    bra.b   CallStack


*************************************
*   UIK_DrawAuto( obj, auto, xoffs, yoffs )
*                 a0    a1    d0     d1
*************************************
_UIK_DrawAuto
    movem.l d0-d1,-(sp)
    movem.l a0-a1,-(sp)
    jsr     _IntUIK_DrawAuto(pc)
    add.w   #16,sp
    rts

    nop

CallStack
    move.l  a0,-(sp)
    jsr     (a1)
    move.l  (sp)+,a0
    rts


*************************************
*   disable stuff
*
*************************************

    XREF    _IntUIK_DrawDisPat

_UIK_ObjEnable                ; ( register __a0 struct UIKObj *obj, register __d0 int n )
    move.l  d0,d1

;static void enable( struct UIKObj *obj, int n )
;{
;    if (obj && n)
;        {
;        do_lockedrefresh( obj );
;        if (obj->List.NodeNum) UIK_nl_CrossList( obj, (void*)enable, 0, --n );
;        }
;}
.enable
    move.l  a0,d0
    beq.b   .end
    tst.l   d1
    beq.b   .end

    bclr    #(BOXB_DISABLED-16),uo_Flags+1(a0)
    beq.b   .no
    bsr     _do_lockedrefresh       ; préserve d1 et a0
.no
    tst.l   uikl_NodeNum(a0)
    beq.b   .end
    lea     .enable(pc),a1
    moveq   #0,d0
    subq.l  #1,d1
    bsr     _UIK_nl_CrossList
.end
    rts

_UIK_ObjDisable                ; ( register __a0 struct UIKObj *obj, register __d0 int n )
    move.l  d0,d1

;static void disable( struct UIKObj *obj, int n )
;{
;    if (obj && n)
;        {
;        do_lockeddisable( obj );
;        if (obj->List.NodeNum) UIK_nl_CrossList( obj, (void*)disable, 0, --n );
;        }
;}
.disable
    move.l  a0,d0
    beq.b   .end
    tst.l   d1
    beq.b   .end

    bsr.b   _do_lockeddisable       ; préserve d1 et a0

    tst.l   uikl_NodeNum(a0)
    beq.b   .end
    lea     .disable(pc),a1
    moveq   #0,d0
    subq.l  #1,d1
    bsr     _UIK_nl_CrossList
.end
    rts

_do_lockeddisable
    btst    #(BOXB_CANTDISABLE-16),uo_Flags+1(a0)
    bne.b   .end
    bset    #(BOXB_DISABLED-16),uo_Flags+1(a0)
    bne.b   .end
    move.l  uo_RefreshVector(a0),d0 si pas de display, pas de ghost
    beq.b   .end
    movem.l d1/a0,-(sp)
    bsr     _UIK_LockObjVars
    bne.b   .no                     if (obj->Status != UIKS_STARTED) goto .no
    jsr     _IntUIK_DrawDisPat
    move.l  4(sp),a0                obj
.no
    bsr     _UIK_UnlockObjVars
    movem.l (sp)+,d1/a0
.end
    rts                             preserve d1 et a0


*************************************
*   UIK_RefreshSons( obj )
*                    a0
*************************************
    XREF    _UIK_nl_CrossList
_UIK_RefreshSons
    lea     .CrossRefresh(pc),a1
    moveq   #0,d0
    move.l  uo_rp(a0),d1
    bra     _UIK_nl_CrossList

;static void CrossRefresh( struct UIKObj *obj, struct RastPort *rp )
;{
;    if (obj->rp == rp)
;        {
;        do_lockedrefresh( obj );
;        if (obj->List.NodeNum) UIK_nl_CrossList( obj, (void*)CrossRefresh, 0, (int)rp );
;        }
;}
.CrossRefresh
    cmp.l   uo_rp(a0),d1
    bne.b   .end

    bsr.b   _do_lockedrefresh       ; préserve d1 et a0

    tst.l   uikl_NodeNum(a0)
    beq.b   .end
    lea     .CrossRefresh(pc),a1
    moveq   #0,d0
    bsr     _UIK_nl_CrossList
.end
    rts


*************************************
*   refresh stuff
*
*************************************

;void __asm UIK_Refresh( register __a0 struct UIKObj *obj, register __d0 int n )
;{
;    refresh( obj, n );
;}
_UIK_Refresh
    move.l  d0,d1

;static void refresh( struct UIKObj *obj, int n )
;{
;    if (obj && n)
;        {
;        do_lockedrefresh( obj );
;        if (obj->List.NodeNum) UIK_nl_CrossList( obj, (void*)refresh, 0, --n );
;        }
;}
.refresh
    move.l  a0,d0
    beq.b   .end
    tst.l   d1
    beq.b   .end

    bsr.b   _do_lockedrefresh       ; préserve d1 et a0

    tst.l   uikl_NodeNum(a0)
    beq.b   .end
    lea     .refresh(pc),a1
    moveq   #0,d0
    subq.l  #1,d1
    bsr     _UIK_nl_CrossList
.end
    rts

;void __asm do_lockedrefresh( register __a0 struct UIKObj *obj )
;{
;    if (obj->RefreshVector)
;        {
;        UIK_LockObjVars( obj );
;        if (obj->Status == UIKS_STARTED) (*obj->RefreshVector)( obj );
;        UIK_UnlockObjVars( obj );
;        }
;}
    XDEF    _do_lockedrefresh
_do_lockedrefresh
    move.l  uo_RefreshVector(a0),d0
    beq.b   .end
    move.l  d1,-(sp)
    bsr     _UIK_LockObjVars
    bne.b   .no                     ;if (obj->Status != UIKS_STARTED) goto .no
    move.l  a0,-(sp)
    move.l  d0,a1
    jsr     (a1)                    ; (*obj->RefreshVector)( obj );
    move.l  (sp),a0
    btst    #(BOXB_DISABLED-16),uo_Flags+1(a0)
    beq.b   .nopat
    bsr     _IntUIK_DrawDisPat
.nopat
    move.l  (sp)+,a0
.no
    bsr     _UIK_UnlockObjVars
    move.l  (sp)+,d1
.end                                ; preserve d1 et a0
    rts


;void __asm UIK_RefreshWindow( register __a0 struct UIKObj *obj )
;{
;  UZone *zo=0;
;
;    if (obj->UIK->ActiveZoneList == obj->ObjExt->ZoneList)
;        zo = zo_UnmarkZone( obj->UIK );
;    refresh_window( obj, obj->w );
;    if (zo) zo_MarkZone( obj->UIK, zo );
;}
    XREF    _zo_MarkZone
    XREF    _zo_UnmarkZone
_UIK_RefreshWindow
    movem.l a2-a3,-(sp)
    move.l  a0,a2
    suba.l  a3,a3

    move.l  uo_UIK(a2),a0
    move.l  uo_ObjExt(a2),a1
    move.l  uike_ActiveZoneList(a0),d0
    cmp.l   uoe_ZoneList(a1),d0
    bne.b   .10
    bsr     _zo_UnmarkZone
    move.l  d0,a3
.10
    move.l  a2,a0
    move.l  uo_w(a2),d1
    bsr.b   .refresh_window

    move.l  a3,d0
    beq.b   .end
    move.l  uo_UIK(a2),a0
    move.l  a3,a1
    bsr     _zo_MarkZone
.end
    movem.l (sp)+,a2-a3
    rts

;static void refresh_window( struct UIKObj *obj, struct Window *w )
;{
;    if (obj->w == w)
;        {
;        do_lockedrefresh( obj );
;        if (obj->List.NodeNum) UIK_nl_CrossList( obj, refresh_window, 0, (ULONG)w );
;        }
;}

.refresh_window
    cmp.l   uo_w(a0),d1
    bne.b   .return

    bsr   _do_lockedrefresh     @@@  ; préserve d1 et a0

    tst.l   uikl_NodeNum(a0)
    beq.b   .return
    lea     .refresh_window(pc),a1
    moveq   #0,d0
    bsr     _UIK_nl_CrossList
.return
    moveq   #1,d0
    rts


*************************************
*   UIK_Select( obj, eventsmask )
*               a0       d0
*************************************

_UIK_Select
    move.l  d0,-(sp)
    move.l  a0,-(sp)
    move.l  a0,d1
    beq.b   .end                    ; if (! obj) return;
    btst    #(BOXB_DISABLED-16),uo_Flags+1(a0)
    bne     .end                    ; si != 0, objet disabled

    move.l  uo_SelectVector(a0),d1
    beq.b   .hook                   ; if (obj->SelectVector)
    btst    #BOXB_NOACT,uo_Flags+3(a0)
    bne.b   .hook
    move.l  d1,a1                   ;
    jsr     (a1)                    ;     rc = (*obj->SelectVector)( obj, eventsmask );
    tst.l   d0
    beq.b   .end                    ; if (rc)
.hook
    move.l  (sp),a0
    move.l  4(sp),d0
    bsr     _UIK_CauseObjLink       ;     UIK_CauseObjLink( obj, eventsmask );
    move.l  (sp),a0
    move.l  4(sp),d0
    bsr     _UIK_CauseObjHook       ;     UIK_CauseObjHook( obj, eventsmask );
.end
    addq.w  #8,sp
    rts


**************************************************************************
* void __asm ExecuteClass( register __a0 struct UIKHook *hook,
*                                           register __d1 ULONG class );
*{
*    if (hook->Mask & class)
*        if (hook->Vect)
*            (*(hook->Vect))( hook->Obj, hook->Parm, hook );
*}                              a0          d0        d1
**************************************************************************

_ExecuteClass
    and.l   uh_Mask(a0),d1      if (hook->Mask & class)
    beq   .end

    move.l  uh_Vect(a0),d0      if (hook->Vect)
    beq   .end

;   DBUG <'event=%lx, vect=%lx, '>,D1,D0
    move.l  uh_Parm(a0),a1
    exg     a1,d0               a1 = vect, d0 = parm
    move.l  a0,d1               d1 = hook
    move.l  uh_Obj(a0),a0       a0 = obj

;   movem.l   d0-d2/a0-a1,-(sp)
;   move.l    uo_RegGerm(a0),a1
;   move.l    uorg_Germ(a1),a1
;   DBUG <'obj=%lx, name=%ls, '>,A0,A1
;   move.l    uo_UIK(a0),a0
;   move.l    uike_IntuiMsg(a0),a1
;   DBUG <'uik=%lx, IntuiMsg=%lx',10>,A0,A1
;   XREF      TimeDelay
;   moveq     #1,d0       unit
;   moveq     #1,d1       sec
;   moveq     #0,d2       micros
;   jsr       TimeDelay
;   movem.l   (sp)+,d0-d2/a0-a1

    movem.l d0-d1,-(sp)
    move.l  a0,-(sp)
    jsr     (a1)                à la fois pile et registres
    add.w   #12,sp
.end
    rts


**************************************************************************
*void __asm ExecuteMask( register __a0 struct UIKHook *hook,
*                                           register __d1 ULONG events );
*{
*  ULONG *AdVect;
*  void (*temp)();
*
*    if (hook->Mask & events)
*        if (AdVect = (ULONG*)hook->Vect)
*            if (temp = (void *) *AdVect)
*                { *AdVect = 0; (*temp)( hook->Parm, hook->Mask & events, hook ); }
*}                                          d0            d1               a0
**************************************************************************

_ExecuteMask
    and.l   uh_Mask(a0),d1      if (d1 = (hook->Mask & events))
    beq.b   .end

    move.l  uh_Vect(a0),d0      if (AdVect = (ULONG*)hook->Vect)
    beq.b   .end

    move.l  d0,a1               if (temp = (void *) *AdVect)
    move.l  (a1),d0             d0 = temp
    beq.b   .end

    clr.l   (a1)                *AdVect = 0
    move.l  uh_Parm(a0),a1
    exg     a1,d0               a1 = temp, d0 = parm
    movem.l d0-d1/a0,-(sp)
    jsr     (a1)                à la fois pile et registres
    add.w   #12,sp
.end
    rts


**************************************************************************
*extern void __asm ExecuteLink( register __a0 struct UIKLink *link, register __d1 ULONG class );
*{
*  register struct UIKObj *obj, *dest;
*
*    if (link->Mask & class)
*       if (obj = link->Obj)
*            if (dest = link->Dest)
*                {
*                if (! (*(link->Vect))( obj, dest, class, link->Parm ))
*                    return(0);         a0    a1    d0        d1
*                if (dest->Link.NodeNum)
*                    return( UIK_nl_CrossList( &dest->Link, ExecuteLink, 1, class/*-1*/ ) );
*                }
*    return(1);
*}
**************************************************************************

_ExecuteLink
    movem.l d2/a2,-(sp)
    move.l  d1,d2               d2 = class
    moveq   #1,d0               return(1)

    and.l   li_Mask(a0),d1      if (link->Mask & class)
    beq.b   .end

    move.l  li_Dest(a0),d1
    beq.b   .end
    tst.l   li_Obj(a0)
    beq.b   .end

    move.l  li_Vect(a0),a2
    move.l  li_Parm(a0),a1
    move.l  li_Obj(a0),a0       a0 = obj
    move.l  d2,d0               d0 = class
    exg     d1,a1               d1 = parm, a1 = dest
    movem.l d0-d1,-(sp)
    movem.l a0-a1,-(sp)
    jsr     (a2)                if (! (*(link->Vect))( obj, dest, class, link->Parm ))
    move.l  4(sp),a1            restore a1 = dest
    add.w   #16,sp
    tst.l   d0
    beq.b   .end                return(0);

    lea     uo_Link(a1),a0      a0 = &dest->Link
    tst.l   uikl_NodeNum(a0)    if (dest->Link.NodeNum)
    beq.b   .end

    lea     _ExecuteLink(pc),a1 a1 = func
    moveq   #1,d0
    moveq   #-1,d1      return( UIK_nl_CrossList( &dest->Link, ExecuteLink, 1, -1 ) );
    ;move.l  d2,d1      return( UIK_nl_CrossList( &dest->Link, ExecuteLink, 1, class/*-1*/ ) );
    bsr     _UIK_nl_CrossList
.end
    movem.l (sp)+,d2/a2
    rts


**************************************************************************
*   void UIK_ActivateKeyTank( struct UIKObj *obj )(a0)
**************************************************************************

_UIK_ActivateKeyTank
        move.l  a0,d0
        beq     .error
        btst    #(BOXB_DISABLED-16),uo_Flags+1(a0)
        bne     .end                    ; si != 0, objet disabled

        move.l  uo_UIK(a0),a1
        cmp.l   uike_ActiveKeyTank(a1),a0   déjà actif?
        beq.b   .error

        move.l  a3,-(sp)
        bsr     _UIK_LockObjVars
        move.l  a1,a3

        move.l  a0,-(sp)                    inactive l'autre
        move.l  uike_ActiveKeyTank(a3),a0
        bsr.b   _UIK_InactivateKeyTank
        move.l  (sp)+,a0

        move.l  uo_ObjExt(a0),a1
        move.l  uoe_ActKeyTank(a1),d0       fonction d'activation?
        beq.b   .end                            sinon fin...
        move.l  d0,a1
        move.l  a0,-(sp)
        jsr     (a1)
        move.l  (sp)+,a0
.end
        move.l  a0,uike_ActiveKeyTank(a3)   nouvel actif
        bsr     _UIK_UnlockObjVars
        move.l  (sp)+,a3
.error  moveq   #1,d0   ; ok
        rts


**************************************************************************
*   void UIK_InactivateKeyTank( struct UIKObj *obj )(a0)
**************************************************************************

_UIK_InactivateKeyTank
        move.l  a0,d0
        beq     .error
        bsr     _UIK_LockObjVars            ne touche pas a0

        move.l  uo_UIK(a0),a1
        move.l  uike_ActiveKeyTank(a1),d0   il y a un actif?
        beq     .end                            sinon fin...
        cmp.l   d0,a0                       est-ce obj?
        bne.b   .end                            sinon fin...

        clr.l   uike_ActiveKeyTank(a1)      plus d'actif

        move.l  uo_ObjExt(a0),a1
        move.l  uoe_InactKeyTank(a1),d0     fonction d'inactivation?
        beq.b   .end                            sinon fin...
        move.l  d0,a1
        move.l  a0,-(sp)
        jsr     (a1)
        move.l  (sp)+,a0
.end
        bsr     _UIK_UnlockObjVars          a0 = obj
.error  rts



    END
