/****************************************************************
 *
 *      Project:   UIK
 *      Function:  routines de gestion de liste
 *
 *      Created:   30/05/90    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikglobal.h"
#include "lci:uik_protosint.h"

#include "uiki:objscreen.h"
#include "uiki:objlist.h"
#include "uiki:objproplift.h"


/****** Imported ************************************************/

extern void IntUIK_DrawBox();
extern void HdlUIK_ListSelect();
extern int SStrNCCmp();

extern struct LayersBase * __far LayersBase;
extern struct GfxBase * __far GfxBase;
extern struct UIKBase * __far UIKBase;

extern UWORD __far AutoDrawList[];
extern UWORD __far AutoDrawList_Under[];


/****** Exported ************************************************/

ULONG __asm UIK_OLi_NewTextPtr( register __a0 struct UIKObjList *lobj, register __a1 UBYTE *buf );


/****** Statics *************************************************/

static struct TagItem __far flags_boolmap[] =
    {
    { UIKTAG_OBJListFl_CenLeft,     UIKL_LEFT       },
    { UIKTAG_OBJListFl_CenJustify,  UIKL_JUSTIFY    },
    { UIKTAG_OBJListFl_CenCenter,   UIKL_CENTER     },
    { UIKTAG_OBJListFl_CenRight,    UIKL_RIGHT      },
    { UIKTAG_OBJListFl_ClickSel,    UIKL_CLICKSEL   },
    { UIKTAG_OBJListFl_SelDispl,    UIKL_SELDISPL   },
    { UIKTAG_OBJListFl_Sorted,      UIKL_SORTED     },
    { TAG_DONE }
    };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void GetVis( struct UIKObjList *lobj )
{
    lobj->Vis = lobj->Gad.Height / lobj->Obj.ObjExt->Font->tf_YSize;
}

static void RemoveObjList( struct UIKObjList *lobj )
{
    UIK_nl_FreeList( &lobj->ListText );
}

static void StopObjList( struct UIKObjList *lobj )
{
    if (lobj->ListFlags & UIKL_CLICKSEL) StopUIKGadget( lobj );
}

static void VerifPosNum( struct UIKObjList *lobj )
{
  int pos, n, v;

    pos = lobj->Pos; n = UIK_nl_ListNodeNum( &lobj->ListText ); v = lobj->Vis;
    if (pos > n - v)
        if ((pos = n - v) < 0)
            pos = 0;
    lobj->Pos = pos;
}

static void ResetPosNum( struct UIKObjList *lobj )
{
    VerifPosNum( lobj );
    if (lobj->FeedBack)
        UIK_OPr_ChangePropGadget( lobj->FeedBack, UIK_nl_ListNodeNum( &lobj->ListText ), lobj->Vis, lobj->Pos );
}

static void ResizeObjList( struct UIKObjList *lobj )
{
  struct Rectangle *rect = &lobj->Obj.ObjExt->InnerRect;

    lobj->Gad.LeftEdge = rect->MinX;
    lobj->Gad.TopEdge  = rect->MinY;
    lobj->Gad.Width    = rect->MaxX - rect->MinX + 1;
    lobj->Gad.Height   = rect->MaxY - rect->MinY + 1;
    GetVis( lobj );
    VerifPosNum( lobj );
    if (lobj->FeedBack)
        UIK_OPr_ChangePropNoDraw( lobj->FeedBack, UIK_nl_ListNodeNum( &lobj->ListText ), lobj->Vis, lobj->Pos );
}

static int StartObjList( struct UIKObjList *lobj )
{
    ResizeObjList( lobj );
    if (lobj->ListFlags & UIKL_CLICKSEL) return( StdGadInit( lobj ) );
    else return(1);
}

static void RefreshObjList( struct UIKObjList *lobj )
{
    IntUIK_DrawBox( lobj );
    UIK_OLi_DrawList( lobj );
}

static int AddObjList( struct UIKObjList *lobj, struct TagItem *taglist )
{
  struct TextEntry *te;
  UBYTE *buf;
  struct Rectangle *rect;

    lobj->ListFlags = UIK_GetTagData( UIKTAG_OBJList_Flags, UIKL_LEFT, taglist );
    lobj->ListFlags = UIK_PackBoolTags( lobj->ListFlags, taglist, flags_boolmap );
    if (lobj->ListFlags & UIKL_SORTED) lobj->SortFunc = SStrNCCmp;

    if (UIKBase->UIKPrefs->PrefsFlags & UIKF_SYSLOOK)
        //if (! UIK_FindTagItem( UIKTAG_OBJ_BoxFl_Over, taglist ))
            {
            if (lobj->ListFlags & UIKL_CLICKSEL) lobj->Obj.Flags |= BOX_OVER;
            else lobj->Obj.Flags &= ~BOX_OVER; // enfonce si pas active
            }

    if (lobj->ListFlags & UIKL_CLICKSEL)
        {
        lobj->Obj.ExceptActVector = HdlUIK_ListSelect;

        {
        struct UIKObj *wo, *rpobj, *parentbox;

        wo = lobj->Obj.Parent;
        rpobj = 0;
        parentbox = 0;
        while (wo && wo->Id != UIKID_WINDOW)
            {
            if (wo->Id == UIKID_REQUEST && !rpobj)
                { rpobj = wo; break; }
            if (wo->Id == UIKID_BOX && !parentbox && (wo->Flags & BOX_ZONEABLE))
                { parentbox = wo; }
            wo = wo->Parent;
            }
        if (wo && !rpobj) rpobj = wo;
        if (! parentbox) parentbox = rpobj;
        if (UIK_AddZone( parentbox, lobj, 0, 0 )) lobj->Obj.Flags |= BOX_ZONEABLE;
        }
        }
    rect = &lobj->Obj.ObjExt->InnerRect;
    lobj->Gad.LeftEdge = rect->MinX;
    lobj->Gad.TopEdge  = rect->MinY;
    lobj->Gad.Width    = rect->MaxX - rect->MinX + 1;
    lobj->Gad.Height   = rect->MaxY - rect->MinY + 1;
    lobj->Gad.UserData = (APTR) lobj;
    lobj->Gad.Flags      = GADGHNONE;
    lobj->Gad.Activation = FOLLOWMOUSE | RELVERIFY | GADGIMMEDIATE;
    lobj->Gad.GadgetType = BOOLGADGET;

    GetVis( lobj );

    lobj->ListText.BlockSize = 10;

    *((ULONG*)&lobj->Obj.ActBoxPen) = UIK_GetTagData( UIKTAG_OBJ_ActInactPens, UIKCOL_GREY<<24 | UIKCOL_GREY<<16 | UIKCOL_BLACK<<8 | UIKCOL_BLACK, taglist );
    lobj->ListPen = (UBYTE) UIK_GetTagData( UIKTAG_OBJList_ListPen, UIKCOL_BLACK, taglist );
    lobj->SelectPen = (UBYTE) UIK_GetTagData( UIKTAG_OBJList_SelectPen, UIKCOL_WHITE, taglist );
    lobj->SelectTextPen = (UBYTE) UIK_GetTagData( UIKTAG_OBJList_SelectTextPen, UIKCOL_BLACK, taglist );
    if (lobj->Max = (UWORD) UIK_GetTagData( UIKTAG_OBJList_MaxLines, 0, taglist ))
        lobj->ListFlags |= UIKL_FIXED;
    lobj->SortFunc = (void *) UIK_GetTagData( UIKTAG_OBJList_SortFunc, (ULONG)SStrNCCmp, taglist );

    if (te = (struct TextEntry *) UIK_GetTagData( UIKTAG_OBJList_TextEntry, 0, taglist ))
        {
        UIK_OLi_NewText( lobj, te );
        }
    else if (buf = (UBYTE *) UIK_GetTagData( UIKTAG_OBJList_TextPtr, 0, taglist ))
        {
        UIK_OLi_NewTextPtr( lobj, buf );
        }

    lobj->Pos = (UWORD) UIK_GetTagData( UIKTAG_OBJList_TopPosition, 0, taglist );
    lobj->Sel = (UWORD) UIK_GetTagData( UIKTAG_OBJList_SelectedLine, -1, taglist );

    return(1);
}

static int SelectList( struct UIKObj *obj, ULONG class )
{
    if (obj)
        {
        if (class & (INTUITICKS|GADGETDOWN))
            {
            if (obj->UIK->IptButs & VBJOYF_FORW) UIK_OLi_MoveListSel( obj, -1 );
            else if (obj->UIK->IptButs & VBJOYF_BACK) UIK_OLi_MoveListSel( obj, 1 );
            }
        if ((class & GADGETUP) && obj->UIK->ProcButs & DIR_BUTTONS) return(0);
        }
    return(1);
}

static struct TagItem __far GermTagList[] =
    {
    UIKTAG_GEN_BoxAutomate,     (ULONG) AutoDrawList,
    UIKTAG_GEN_BoxAltAutomate,  (ULONG) AutoDrawList_Under,
    TAG_END
    };

struct UIKObjGerm __far ListGerm =
    {
    "UIKObj_List",
    AddObjList,                 // AddVector
    StartObjList,               // StartVector
    StopObjList,                // StopVector
    RemoveObjList,              // RemoveVector
    RefreshObjList,             // RefreshVector
    ResizeObjList,              // ResizeVector
    NULL,                       // ExceptActVector
    NULL,                       // ExceptInaVector
    NULL,                       // ExceptMovVector
    NULL,                       // ExceptKeyVector
    0,                          // StartAfterVector
    SelectList,                 // SelectVector
    0,                          // ExtentedVectors
    GermTagList,                // GermTagList
    8,7,
    (60<<16)|60,
    UIKID_LIST,
    sizeof(struct UIKObjList),
    BOX_DISPLAYABLE|BOX_INNERZONE|BOX_SEMAPHORE | BOX_FILLED | BOX_CLIPPED | BOX_TRANSLATELEFTTOP,
    };

/*---------------------- text funcs -------------------*/

void UIK_OLi_ScrollListFrom( struct UIKObjList *lo, ULONG abslen, LONG len, LONG from )
{
  struct RastPort *rp = lo->Obj.rp;
  struct Rectangle *r = &lo->Obj.Region->bounds;
  LONG start, x, a=0, b=0, offs, miny, height;
  UWORD fh = lo->Obj.ObjExt->Font->tf_YSize;

    UIK_LockObjVars( lo );
    if (abslen && lo->Obj.Status == UIKS_STARTED)
        {
        if (from == -1)
            from = UIK_nl_ListNodeNum( &lo->ListText ) - 1;

        offs = from - lo->Pos;
        /*if (offs > lo->Vis + 1) return;
        else*/ if (offs < 0) offs = 0;

        start = len > 0 ? lo->Pos + lo->Vis - len : lo->Pos + offs;
        miny = r->MinY + offs * fh;
        x = abslen * fh;
        if ((height = r->MaxY - miny + 1 - x) > 0)
            {
            UBYTE mask = rp->Mask;

            rp->Mask = lo->Obj.sc ? ((struct UIKObjScreen *)lo->Obj.sc->UserData)->Mask : UIKBase->UIKPrefs->Mask;

            if (len > 0) { a = x; abslen++; } else b = x;
            ClipBlit( rp, r->MinX, miny + a, rp, r->MinX, miny + b,
                    r->MaxX-r->MinX+1, height, 0xC0 );

            rp->Mask = mask;
            }
        UIK_OLi_DrawListFrom( lo, start, abslen );
        }
    UIK_UnlockObjVars( lo );
}

static int __asm copystring( register __a0 UBYTE *src, register __a1 UBYTE *dest,
                             register __d0 int len, register __d1 struct UIKObjList *lobj )
{
    if (dest && len) CopyMem( src, dest, len );
    ResetPosNum( lobj );
    return( (int)dest );
}

int __asm UIK_OLi_ChangeLine( register __a0 struct UIKObjList *lobj, register __a1 UBYTE *str,
                          register __d0 LONG line, register __d1 WORD flags )
{
  int rc, len;
  UBYTE *ptr;
  struct UIKList *ul = UIK_nl_ListPtr( &lobj->ListText );
  struct UIKNode *node = &ul->UNode[line];

    UIK_LockObjVars( lobj );
    if ((str == 0) || (str == node->Buf))
        {
        if (flags != 0xffffffff) node->Flags = flags;
        rc = (int) node->Buf;
        }
    else{
        len = StrLen( str );
        ptr = UIK_nl_ReAllocNode( &lobj->ListText, line, len, flags );
        rc = copystring( str, ptr, len, lobj );
        }
    UIK_UnlockObjVars( lobj );
    return( rc );
}

int __asm UIK_OLi_AddLineLen( register __a0 struct UIKObjList *lobj, register __a1 UBYTE *str,
           register __d0 LONG line, register __d1 WORD flags, register __d2 LONG len )
{
  UBYTE *ptr;

    UIK_LockObjVars( lobj );
    if (lobj->ListFlags & UIKL_SORTED)
        {
        struct UIKList *L = UIK_nl_ListPtr( &lobj->ListText );
        line = -1;
        while (++line < L->NodeNum)
            if ((*lobj->SortFunc)( L->UNode[line].Buf, str ) >= 0) break;
        }
    if (ptr = UIK_nl_AllocNode( &lobj->ListText, line, len, flags ))
        {
        register UWORD sel = lobj->Sel;
        if (len) CopyMem( str, ptr, len );
        if (sel != ~0 && sel >= ((ULONG)line)) lobj->Sel++;
        ResetPosNum( lobj );
        UIK_OLi_ScrollListFrom( lobj, 1, -1, line );
        }
    UIK_UnlockObjVars( lobj );
    return( (int)ptr );
}

int __asm UIK_OLi_AddLine( register __a0 struct UIKObjList *lobj, register __a1 UBYTE *str,
                       register __d0 LONG line, register __d1 WORD flags )
{
    return( UIK_OLi_AddLineLen( lobj, str, line, flags, StrLen( str ) ) );
}

static void __asm common_delete( register __a0 struct UIKObjList *lobj, register __d0 LONG line, register __d1 ULONG redraw )
{
  register UWORD sel;
  struct UIKList *ul = UIK_nl_ListPtr( &lobj->ListText );

    if (line >= 0 && line < ul->NodeNum)
        {
        UIK_LockObjVars( lobj );
        UIK_nl_FreeNodes( ul, line, line );
        sel = lobj->Sel;
        if (sel == line) { sel = -1; lobj->SelectLen = 0; }
        else if (sel != ~0 && sel > line) sel--;
        lobj->Sel = sel;
        ResetPosNum( lobj );
        if (redraw) UIK_OLi_DrawList( lobj );
        UIK_UnlockObjVars( lobj );
        }
}

void __asm UIK_OLi_DeleteLine( register __a0 struct UIKObjList *lobj, register __d0 LONG line )
{
    common_delete( lobj, line, 1 );
}

void __asm UIK_OLi_DelLine( register __a0 struct UIKObjList *lobj, register __d0 LONG line )
{
    common_delete( lobj, line, 0 );
}

void __asm UIK_OLi_UnselectList( register __a0 struct UIKObjList *lobj )
{
  register int sel;

    UIK_LockObjVars( lobj );
    if ((sel = lobj->Sel) != -1)
        {
        lobj->Sel = -1;
        lobj->SelectLen = 0;
        UIK_OLi_DrawLine( lobj, sel );
        }
    UIK_UnlockObjVars( lobj );
}

void __asm UIK_OLi_SelectList( register __a0 struct UIKObjList *lobj, register __d0 LONG line )
{
  LONG nsel, oldlen, oldsel;

    UIK_LockObjVars( lobj );
    oldlen = lobj->SelectLen;
    oldsel = lobj->Sel;

    UIK_OLi_UnselectList( lobj );
    nsel = UIK_OLi_TotLines( lobj ) - 1;
    if ((line < 0) || (line > nsel)) line = nsel;
    lobj->Sel = line;
    if (line == oldsel) lobj->SelectLen = oldlen;
    UIK_OLi_DrawLine( lobj, line );

    UIK_UnlockObjVars( lobj );
}

void __asm UIK_OLi_MoveListFrame( register __a0 struct UIKObjList *list, register __d0 LONG n )
{
  register int top;

    UIK_LockObjVars( list );
    top = list->Pos + n * list->Vis;
    if (top < 0) top = 0;
    else if (top >= UIK_OLi_TotLines( list )) top = -1;
    UIK_OLi_SetListTop( list, top );
    UIK_UnlockObjVars( list );
}

void __asm UIK_OLi_MoveListSel( register __a0 struct UIKObjList *list, register __d0 LONG n )
{
  register int sel, nsel, old;
  register LONG first, last;

    UIK_LockObjVars( list );
    old = list->Sel;
    first = list->Pos;                                          // 1ere ligne visible
    last = first + list->Vis - 1;
    if ((nsel = UIK_OLi_TotLines( list ) - 1) < last) last = nsel;  // derniere visible
    sel = list->Sel;
    if (sel < first || sel > last)                              // hors fenêtre...
        {
        if (n > 0)
            {
            list->Sel = first;
            UIK_OLi_SetListTop( list, first );                      // début visible
            }
        else{
            list->Sel = last;
            UIK_OLi_SetListTop( list, (last - list->Vis + 1) );     // affiche début
            }
        }
    else{                                                       // dans fenêtre...
        nsel = sel + n;                                         // incrémente
        if (nsel < UIK_OLi_TotLines( list ) && nsel >= 0)           // ou reste là ?
            {
            LONG pos = list->Pos;
            if (nsel < pos) pos--;
            else if (nsel >= pos + list->Vis) pos++;
            if (pos != list->Pos)                               // glisse fenêtre
                {
                UIK_OLi_UnselectList( list );
                list->Pos = pos;
                UIK_OLi_ScrollListFrom( list, 1, n, pos );
                if (list->FeedBack) UIK_OPr_ChangePropGadget( list->FeedBack, -1, -1, pos );
                }
            UIK_OLi_SelectList( list, nsel );                       // recalcule & affiche
            }
        }
    if (old != list->Sel) list->SelectLen = 0;
    UIK_UnlockObjVars( list );
}

int __asm UIK_OLi_TotLines( register __a0 struct UIKObjList *lobj )
{
    return( (int) UIK_nl_ListNodeNum( &lobj->ListText ) );
}

ULONG __asm UIK_OLi_NewTextPtr( register __a0 struct UIKObjList *lobj, register __a1 UBYTE *buf )
{
  struct UIKList *ul = &lobj->ListText;
  ULONG rc=1;
  UBYTE *str, *ptr;

    if (ul->BlockSize == ~0) return(0);
    UIK_LockObjVars( lobj );
    lobj->Sel = -1;
    lobj->SelectLen = 0;
    UIK_nl_FreeList( ul );

    ptr = str = buf;
    while (*ptr)
        {
        while (*ptr && *ptr != '\n') ptr++;
        if (! UIK_OLi_AddLineLen( lobj, str, -1, 0, ptr-str )) { rc = 0; break; }
        if (*ptr) ptr++; // skip newline
        str = ptr;
        }

    UIK_UnlockObjVars( lobj );
    return( rc );
}

int __asm UIK_OLi_NewText( register __a0 struct UIKObjList *lobj, register __a1 struct TextEntry *te )
{
  struct UIKList *ul = &lobj->ListText;
  int rc = 1;

    if (ul->BlockSize == ~0) return(0);
    UIK_LockObjVars( lobj );
    lobj->Sel = -1;
    lobj->SelectLen = 0;
    UIK_nl_FreeList( ul );
    while (te->String)
        {
        if (! UIK_OLi_AddLine( lobj, te->String, -1, te->Flags )) { rc = 0; break; }
        te++;
        }
    UIK_UnlockObjVars( lobj );
    return( rc );
}

UBYTE * __asm UIK_OLi_LineBuffer( register __a0 struct UIKObjList *lobj
                             ,register __d0 LONG line )
{
  struct UIKList *ul = UIK_nl_ListPtr( &lobj->ListText );

    if (line < ul->NodeNum)
        return( ul->UNode[line].Buf );
    return(0);
}

void __asm UIK_OLi_ActualizeList( register __a0 struct UIKObjList *lobj )
{
  LONG nsel;

    UIK_LockObjVars( lobj );
    GetVis( lobj );
    ResetPosNum( lobj );
    nsel = UIK_OLi_TotLines( lobj ) - 1;
    if (lobj->Sel != -1)
        if (lobj->Sel > nsel)
            {
            lobj->Sel = -1;
            lobj->SelectLen = 0;
            }
    UIK_OLi_DrawList( lobj );
    UIK_UnlockObjVars( lobj );
}

void __asm UIK_OLi_SetListTop( register __a0 struct UIKObjList *lobj
                          ,register __d0 LONG line )
{
    UIK_LockObjVars( lobj );
    lobj->Pos = (line == -1) ? UIK_OLi_TotLines( lobj ) : line;
    UIK_OLi_ActualizeList( lobj );
    UIK_UnlockObjVars( lobj );
}

/*-----------------------------------------------------*/
