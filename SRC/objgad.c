/****************************************************************
 *
 *      Project:   UIK
 *      Function:  gadgets
 *
 *      Created:   12/05/90    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikglobal.h"
#include "uiki:uikbox.h"
#include "uiki:uikmacros.h"
#include "uiki:uikcolors.h"
#include "uiki:uikdrawauto.h"
#include "lci:uik_protosint.h"

#include "uiki:objscreen.h"
#include "uiki:objwindow.h"


/****** Macros **************************************************/


/****** Imported ************************************************/

extern void IntUIK_PushBox(), IntUIK_PopBox(), IntUIK_DrawBox();
extern void HdlUIK_VerifActGadget(), UIK_VerifInactGadget();

extern void __asm HdlUIK_StdGadAct(), HdlUIK_StdGadInact(), HdlUIK_StdGadMove();

extern struct UIKBase * __far UIKBase;
extern struct DOSBase * __far DOSBase;
extern struct GfxBase * __far GfxBase;
extern struct LayersBase * __far LayersBase;
extern struct IntuitionBase * __far IntuitionBase;

extern struct BitMap __far UBM_Zoom;
extern struct BitMap __far UBM_ZoomOn;
extern struct BitMap __far UBM_Icon;
extern struct BitMap __far UBM_IconOn;
extern struct BitMap __far UBM_Depth;
extern struct BitMap __far UBM_DepthOn;
extern struct BitMap __far UBM_Close;
extern struct BitMap __far UBM_CloseOn;
extern struct BitMap __far UBM_Up;
extern struct BitMap __far UBM_Down;
extern struct BitMap __far UBM_Right;
extern struct BitMap __far UBM_Left;
extern struct BitMap __far UBM_OK;
extern struct BitMap __far UBM_Cancel;

extern UWORD __far AutoDrawBox_Over[], __far AutoDrawBox_Light[];


/****** Exported ************************************************/


/****** Statics *************************************************/

static struct TagItem __far LightGermTagList[] =
    {
    UIKTAG_GEN_ActInactPens,    UIKCOLS(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_BLACK,UIKCOL_WHITE),
    UIKTAG_GEN_BoxAutomate,     (ULONG) AutoDrawBox_Over,
    UIKTAG_GEN_BoxAltAutomate,  (ULONG) AutoDrawBox_Light,
    TAG_END
    };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void add_boolinfo( struct UIKObjGad *gobj, struct TagItem *taglist )
{
  ULONG mask;

    if (mask = UIK_GetTagData( UIKTAG_OBJ_HitMask, 0, taglist ))
        {
        struct BoolInfo *boolinfo;
        if (boolinfo = (struct BoolInfo *) UIK_MemSpAlloc( sizeof(struct BoolInfo), MEMF_ANY|MEMF_CLEAR ))
            {
            boolinfo->Flags = BOOLMASK;
            boolinfo->Mask = (UWORD*) mask;
            gobj->Gad.SpecialInfo = (APTR) boolinfo;
            gobj->Gad.Activation |= GACT_BOOLEXTEND;
            }
        }
}

int StdGadInit( struct UIKObjGad *obj )
{
  UWORD position;
  struct Gadget *gad;

    if (obj->Obj.Flags & BOX_NOACT)
        {
        obj->Obj.ExceptActVector = obj->Obj.ExceptInaVector = obj->Obj.ExceptMovVector = 0;
        }
    if (obj->Obj.rq) obj->Gad.GadgetType |= REQGADGET;

    if (obj->Obj.Id >= UIKID_DEPTGAD && obj->Obj.Id <= UIKID_DRAGGAD) position = (USHORT)~0;
    else{
        for (position=0, gad=obj->Obj.w->FirstGadget; gad && (gad->GadgetType & SYSGADGET); position++, gad=gad->NextGadget);
        }
    AddGList( obj->Obj.w, &obj->Gad, position, 1, obj->Obj.rq );
    return(1);
}

void StdGadRefresh( struct UIKObjGad *obj )
{
    IntUIK_DrawBox( obj );
}

void StdGadResize( struct UIKObjGad *obj )
{
    obj->Gad.LeftEdge = obj->Obj.Box.Left;
    obj->Gad.TopEdge  = obj->Obj.Box.Top;
    obj->Gad.Width    = obj->Obj.Box.Width;
    obj->Gad.Height   = obj->Obj.Box.Height;
}

void StopUIKGadget( struct UIKObjGad *obj )
{
  struct UIKGlobal *uik=obj->Obj.UIK;

    if (obj->Obj.w)
        {
        RemoveGList( obj->Obj.w, &obj->Gad, 1 );
        if (! UIK_IsSystemVersion2())
            {
            while (uik->ActiveObj && (obj->Gad.Flags & SELECTED)) Delay(1);
            }
        }
}

int AddUIKGadget( struct UIKObjGad *gobj, struct TagItem *taglist )
{
    StdGadResize( gobj );
    gobj->Gad.Flags      = GADGHNONE;
    gobj->Gad.Activation = FOLLOWMOUSE | RELVERIFY | GADGIMMEDIATE;
    gobj->Gad.GadgetType = BOOLGADGET;
    gobj->Gad.UserData   = (APTR) gobj;
    add_boolinfo( gobj, taglist );
    return(1);
}

void RemUIKGadget( struct UIKObjGad *gobj )
{
    UIK_MemSpFree( gobj->Gad.SpecialInfo );
}

static void remvectors( struct UIKObjGad *obj )
{
    UIK_RemoveVector( obj, (struct UIKHook *)obj->Gad.MutualExclude );
    StopUIKGadget( obj );
}

static void set_title_image( struct UIKObj *obj, struct UIKBitMap *image )
{
    if (obj->Title == 0 && !(obj->Flags & (BOX_NDRAWN | BOX_CUSTOMDRAW | BOX_USEBITMAP)))
        {
        obj->TitleFlags = TITLE_IMAGE;
        obj->Title = (ULONG)image;
        }
}

/****************************************************************
 *
 *      Gadget Specific
 *
 ****************************************************************/

//============ UIKObj_Boolean

static int SelectNorm( struct UIKObj *obj, ULONG class )
{
    if (obj)
        {
        if (class & GADGETDOWN) IntUIK_PushBox( obj );
        else if (class & GADGETUP) IntUIK_PopBox( obj );
        }
    return(1);
}

static int SelectBool( struct UIKObj *obj, ULONG class )
{
    if (obj)
        {
        UBYTE mouse = (obj->UIK->InputMode == INPUTMODE_MOUSE) ? 1 : 0;

        if (class & GADGETDOWN)
            {
            IntUIK_PushBox( obj );
            if (! mouse)
                {
                Delay(1);
                IntUIK_PopBox( obj );
                }
            }
        else if (class & GADGETUP)
            {
            if (mouse) IntUIK_PopBox( obj );
            }
        }
    return(1);
}

struct UIKObjGerm __far BooleanGerm =
    {
    "UIKObj_Boolean",
    AddUIKGadget,               // AddVector
    StdGadInit,                 // StartVector
    StopUIKGadget,              // StopVector
    RemUIKGadget,               // RemoveVector
    StdGadRefresh,              // RefreshVector
    StdGadResize,               // ResizeVector
    IntUIK_PushBox,             // ExceptActVector
    IntUIK_PopBox,              // ExceptInaVector
    HdlUIK_VerifActGadget,      // ExceptMovVector
    NULL,                       // ExceptKeyVector
    0,                          // StartAfterVector
    SelectBool,                 // SelectVector
    0,                          // ExtentedVectors
    0,                          // GermTagList
    6,4,
    (40<<16)|18,
    UIKID_BOOLGAD,
    sizeof(struct UIKObjGad),
    BOX_DISPLAYABLE|BOX_INNERZONE|BOX_OVER | BOX_SEMAPHORE | BOX_TRANSLATELEFTTOP | BOX_ZONEABLE,
    };

//============ UIKObj_Up

static int StartUp( obj )
struct UIKObjGad *obj;
{
    set_title_image( obj, &UBM_Up );
    return( StdGadInit( obj ) );
}

struct UIKObjGerm __far UpGerm =
    {
    "UIKObj_Up",
    AddUIKGadget,               // AddVector
    StartUp,                    // StartVector
    StopUIKGadget,              // StopVector
    RemUIKGadget,               // RemoveVector
    StdGadRefresh,              // RefreshVector
    StdGadResize,               // ResizeVector
    IntUIK_PushBox,             // ExceptActVector
    IntUIK_PopBox,              // ExceptInaVector
    HdlUIK_VerifActGadget,      // ExceptMovVector
    NULL,                       // ExceptKeyVector
    0,
    SelectNorm,
    0,
    LightGermTagList,
    11,7,
    (19<<16)|11,
    UIKID_UPGAD,
    sizeof(struct UIKObjGad),
    BOX_DISPLAYABLE|BOX_INNERZONE|BOX_OVER | BOX_FILLED | BOX_SEMAPHORE | BOX_TRANSLATELEFTTOP | BOX_ZONEABLE,
    };

//============

static int StartDown( obj )
struct UIKObjGad *obj;
{
    set_title_image( obj, &UBM_Down );
    return( StdGadInit( obj ) );
}

struct UIKObjGerm __far DownGerm =
    {
    "UIKObj_Down",
    AddUIKGadget,               // AddVector
    StartDown,                  // StartVector
    StopUIKGadget,              // StopVector
    RemUIKGadget,               // RemoveVector
    StdGadRefresh,              // RefreshVector
    StdGadResize,               // ResizeVector
    IntUIK_PushBox,             // ExceptActVector
    IntUIK_PopBox,              // ExceptInaVector
    HdlUIK_VerifActGadget,      // ExceptMovVector
    NULL,                       // ExceptKeyVector
    0,
    SelectNorm,
    0,
    LightGermTagList,
    11,7,
    (19<<16)|11,
    UIKID_DOWNGAD,
    sizeof(struct UIKObjGad),
    BOX_DISPLAYABLE|BOX_INNERZONE|BOX_OVER | BOX_FILLED | BOX_SEMAPHORE | BOX_TRANSLATELEFTTOP | BOX_ZONEABLE,
    };

//============

static int StartRight( obj )
struct UIKObjGad *obj;
{
    set_title_image( obj, &UBM_Right );
    return( StdGadInit( obj ) );
}

struct UIKObjGerm __far RightGerm =
    {
    "UIKObj_Right",
    AddUIKGadget,               // AddVector
    StartRight,                 // StartVector
    StopUIKGadget,              // StopVector
    RemUIKGadget,               // RemoveVector
    StdGadRefresh,              // RefreshVector
    StdGadResize,               // ResizeVector
    IntUIK_PushBox,             // ExceptActVector
    IntUIK_PopBox,              // ExceptInaVector
    HdlUIK_VerifActGadget,      // ExceptMovVector
    NULL,                       // ExceptKeyVector
    0,
    SelectNorm,
    0,
    LightGermTagList,
    10,7,
    (19<<16)|11,
    UIKID_RIGHTGAD,
    sizeof(struct UIKObjGad),
    BOX_DISPLAYABLE|BOX_INNERZONE|BOX_OVER | BOX_FILLED | BOX_SEMAPHORE | BOX_TRANSLATELEFTTOP | BOX_ZONEABLE,
    };

//============

static int StartLeft( obj )
struct UIKObjGad *obj;
{
    set_title_image( obj, &UBM_Left );
    return( StdGadInit( obj ) );
}

struct UIKObjGerm __far LeftGerm =
    {
    "UIKObj_Left",
    AddUIKGadget,               // AddVector
    StartLeft,                  // StartVector
    StopUIKGadget,              // StopVector
    RemUIKGadget,               // RemoveVector
    StdGadRefresh,              // RefreshVector
    StdGadResize,               // ResizeVector
    IntUIK_PushBox,             // ExceptActVector
    IntUIK_PopBox,              // ExceptInaVector
    HdlUIK_VerifActGadget,      // ExceptMovVector
    NULL,                       // ExceptKeyVector
    0,
    SelectNorm,
    0,
    LightGermTagList,
    10,7,
    (19<<16)|11,
    UIKID_LEFTGAD,
    sizeof(struct UIKObjGad),
    BOX_DISPLAYABLE|BOX_INNERZONE|BOX_OVER | BOX_FILLED | BOX_SEMAPHORE | BOX_TRANSLATELEFTTOP | BOX_ZONEABLE,
    };

//============

static struct TagItem __far OKGermTagList[] =
    {
    UIKTAG_GEN_ActInactPens,    UIKCOLS(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_GREY,UIKCOL_WHITE),
    UIKTAG_GEN_BoxAutomate,     (ULONG) AutoDrawBox_Over,
    UIKTAG_GEN_BoxAltAutomate,  (ULONG) AutoDrawBox_Light,
    UIKTAG_GEN_ShortCutKey,     B2L(0,0x44,0,0),
    TAG_END
    };

static int StartOK( obj )
struct UIKObjGad *obj;
{
    set_title_image( obj, &UBM_OK );
    return( StdGadInit( obj ) );
}

struct UIKObjGerm __far OKGerm =
    {
    "UIKObj_OK",
    AddUIKGadget,               // AddVector
    StartOK,                    // StartVector
    StopUIKGadget,              // StopVector
    RemUIKGadget,               // RemoveVector
    StdGadRefresh,              // RefreshVector
    StdGadResize,               // ResizeVector
    IntUIK_PushBox,             // ExceptActVector
    IntUIK_PopBox,              // ExceptInaVector
    HdlUIK_VerifActGadget,      // ExceptMovVector
    NULL,                       // ExceptKeyVector
    0,
    SelectNorm,
    0,
    OKGermTagList,
    18, 10,
    (40<<16)|12,
    UIKID_OKGAD,
    sizeof(struct UIKObjGad),
    BOX_DISPLAYABLE|BOX_INNERZONE|BOX_OVER | BOX_FILLED | BOX_SEMAPHORE | BOX_TRANSLATELEFTTOP | BOX_ZONEABLE,
    };

//============

static struct TagItem __far CancelGermTagList[] =
    {
    UIKTAG_GEN_ActInactPens,    UIKCOLS(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_GREY,UIKCOL_WHITE),
    UIKTAG_GEN_BoxAutomate,     (ULONG) AutoDrawBox_Over,
    UIKTAG_GEN_BoxAltAutomate,  (ULONG) AutoDrawBox_Light,
    UIKTAG_GEN_ShortCutKey,     B2L(0,0x45,0,0),
    TAG_END
    };

static int StartCancel( struct UIKObjGad *obj )
{
    set_title_image( obj, &UBM_Cancel );
    return( StdGadInit( obj ) );
}

struct UIKObjGerm __far CancelGerm =
    {
    "UIKObj_Cancel",
    AddUIKGadget,               // AddVector
    StartCancel,                // StartVector
    StopUIKGadget,              // StopVector
    RemUIKGadget,               // RemoveVector
    StdGadRefresh,              // RefreshVector
    StdGadResize,               // ResizeVector
    IntUIK_PushBox,             // ExceptActVector
    IntUIK_PopBox,              // ExceptInaVector
    HdlUIK_VerifActGadget,      // ExceptMovVector
    NULL,                       // ExceptKeyVector
    0,
    SelectNorm,
    0,
    CancelGermTagList,
    18, 10,
    (40<<16)|12,
    UIKID_CANCELGAD,
    sizeof(struct UIKObjGad),
    BOX_DISPLAYABLE|BOX_INNERZONE|BOX_OVER | BOX_FILLED | BOX_SEMAPHORE | BOX_TRANSLATELEFTTOP | BOX_ZONEABLE,
    };

//============

extern void __asm ZoneUpdateStart( register __a0 struct UIKObj *obj );
extern void __asm ZoneUpdateEnd( register __a0 struct UIKObj *obj );

static int CrossMExUnsel( struct UIKObj *obj, struct UIKObj *sel )
{
    if ((obj->Id == UIKID_MEXGAD) && (obj != sel) && !(obj->Flags & BOX_OVER))
        {
        ZoneUpdateStart( sel );
        IntUIK_PopBox( obj );
        ZoneUpdateEnd( sel );
        UIK_RefreshSons( sel );
        //UIK_nl_CrossList( sel, (void*)CrossRefresh, 0, (ULONG)sel->rp );
        return(0); /* stop scan */
        }
    return(1); /* continue scan */
}

int HdlUIK_OME_ActiveMExObj( struct UIKObjGad *sel )
{
    if (sel)
        {
        UIK_LockObjVars( sel );
        UIK_nl_CrossList( sel->Obj.Parent, CrossMExUnsel, 1, (ULONG)sel );
        IntUIK_PushBox( sel );
        UIK_UnlockObjVars( sel );
        UIK_RefreshSons( sel );
        //UIK_nl_CrossList( sel, (void*)CrossRefresh, 0, (ULONG)sel->Obj.rp );
        }
    return(1);
}

void __asm UIK_OME_ActiveMExObj( register __a0 struct UIKObjGad *sel )
{
    HdlUIK_OME_ActiveMExObj( sel );
}

void __asm UIK_OME_InactiveMExObj( register __a0 struct UIKObjGad *sel )
{
    if (sel)
        {
        IntUIK_PopBox( sel );
        }
}

static int StartMutualExclude( obj )
struct UIKObjGad *obj;
{
    obj->Obj.ExceptActVector = (void*) HdlUIK_OME_ActiveMExObj;
    //obj->Gad.Activation = FOLLOWMOUSE | RELVERIFY | GADGIMMEDIATE;
    //obj->Gad.GadgetType = BOOLGADGET;
    return( StdGadInit( obj ) );
}

struct UIKObjGerm __far MutualExcludeGerm =
    {
    "UIKObj_MutualExclude",
    AddUIKGadget,               // AddVector
    StartMutualExclude,         // StartVector
    StopUIKGadget,              // StopVector
    RemUIKGadget,               // RemoveVector
    StdGadRefresh,              // RefreshVector
    StdGadResize,               // ResizeVector
    (void*)HdlUIK_OME_ActiveMExObj,    // ExceptActVector
    NULL,                       // ExceptInaVector
    NULL,                       // ExceptMovVector
    NULL,                       // ExceptKeyVector
    0,
    HdlUIK_OME_ActiveMExObj,
    0,0,
    8,4,
    (40<<16)|18,
    UIKID_MEXGAD,
    sizeof(struct UIKObjGad),
    BOX_DISPLAYABLE|BOX_INNERZONE|BOX_OVER | BOX_SEMAPHORE | BOX_TRANSLATELEFTTOP | BOX_ZONEABLE,
    };

//============

static void ToggleAct( obj )
struct UIKObjGad *obj;
{
    if (obj->Obj.Flags & BOX_OVER) IntUIK_PushBox( obj );
    else IntUIK_PopBox( obj );
    UIK_RefreshSons( obj );
    //UIK_nl_CrossList( obj, (void*)CrossRefresh, 0, (ULONG)obj->Obj.rp );
    /*
    if (obj->Obj.Flags & BOX_OVER) obj->Obj.Flags &= ~BOX_OVER;
    else obj->Obj.Flags |= BOX_OVER;
    */
}

static void ToggleInact( obj )
struct UIKObjGad *obj;
{
    if (obj->Gad.Flags & SELECTED) IntUIK_PushBox( obj );
    else IntUIK_PopBox( obj );
    UIK_RefreshSons( obj );
    //UIK_nl_CrossList( obj, (void*)CrossRefresh, 0, (ULONG)obj->Obj.rp );
    /*
    if (obj->Gad.Flags & SELECTED) obj->Obj.Flags &= ~BOX_OVER;
    else obj->Obj.Flags |= BOX_OVER;
    */
}

void __asm UIK_OTo_SelectToggle( register __a0 struct UIKObjGad *obj )
{
  UWORD position;

    if (obj)
        {
        if (obj->Obj.Status == UIKS_STARTED) position = RemoveGadget( obj->Obj.w, &obj->Gad );
        UIK_LockObjVars( obj );
        obj->Gad.Flags |= SELECTED;
        obj->Obj.Flags &= ~BOX_OVER;
        IntUIK_DrawBox( obj );
        UIK_UnlockObjVars( obj );
        if (obj->Obj.Status == UIKS_STARTED)
            {
            AddGList( obj->Obj.w, &obj->Gad, position, 1, obj->Obj.rq );
            UIK_RefreshSons( obj );
            }
        }
}

void __asm UIK_OTo_UnselectToggle( register __a0 struct UIKObjGad *obj )
{
  UWORD position;

    if (obj && (obj->Obj.Status == UIKS_STARTED))
        {
        if (obj->Obj.Status == UIKS_STARTED) position = RemoveGadget( obj->Obj.w, &obj->Gad );
        UIK_LockObjVars( obj );
        obj->Gad.Flags &= ~(SELECTED);
        obj->Obj.Flags |= BOX_OVER;
        IntUIK_DrawBox( obj );
        UIK_UnlockObjVars( obj );
        if (obj->Obj.Status == UIKS_STARTED)
            {
            AddGList( obj->Obj.w, &obj->Gad, position, 1, obj->Obj.rq );
            UIK_RefreshSons( obj );
            }
        }
}

static int SelectToggle( struct UIKObjGad *obj, ULONG class )
{
    if (obj && (class & GADGETUP))
        {
        if (obj->Gad.Flags & SELECTED) UIK_OTo_UnselectToggle( obj );
        else UIK_OTo_SelectToggle( obj );
        }
    return(1);
}

static int StartToggle( struct UIKObjGad *obj )
{
    obj->Gad.Flags = (obj->Obj.Flags & BOX_OVER) ? GADGHNONE : GADGHNONE|SELECTED;
    obj->Gad.Activation |= TOGGLESELECT /*| FOLLOWMOUSE | RELVERIFY | GADGIMMEDIATE*/;
    return( StdGadInit( obj ) );
}

struct UIKObjGerm __far ToggleGerm =
    {
    "UIKObj_Toggle",
    AddUIKGadget,               // AddVector
    StartToggle,                // StartVector
    StopUIKGadget,              // StopVector
    RemUIKGadget,               // RemoveVector
    IntUIK_DrawBox,             // RefreshVector
    StdGadResize,               // ResizeVector
    ToggleAct,                  // ExceptActVector
    ToggleInact,                // ExceptInaVector
    HdlUIK_VerifActGadget,      // ExceptMovVector
    NULL,                       // ExceptKeyVector
    0,
    SelectToggle,
    0,0,
    8,4,
    (40<<16)|18,
    UIKID_TOGLGAD,
    sizeof(struct UIKObjGad),
    BOX_DISPLAYABLE|BOX_INNERZONE|BOX_OVER | BOX_SEMAPHORE | BOX_TRANSLATELEFTTOP | BOX_ZONEABLE,
    };

//============

static int StartWindowDragg( struct UIKObjGad *obj )
{
    obj->Gad.UserData = 0; // annule les messages pour cet objet car pas de GADGETUP
    obj->Gad.GadgetType = SYSGADGET | WDRAGGING;
    return( StdGadInit( obj ) );
}

struct UIKObjGerm __far WindowDraggGerm =
    {
    "UIKObj_WindowDragg",
    AddUIKGadget,               // AddVector
    StartWindowDragg,           // StartVector
    StopUIKGadget,              // StopVector
    RemUIKGadget,               // RemoveVector
    StdGadRefresh,              // RefreshVector
    StdGadResize,               // ResizeVector
    NULL,                       // ExceptActVector
    NULL,                       // ExceptInaVector
    NULL,                       // ExceptMovVector
    NULL,                       // ExceptKeyVector
    0,0,0,0,
    8,4,
    (10<<16)|10,
    UIKID_DRAGGAD,
    sizeof(struct UIKObjGad),
    BOX_DISPLAYABLE|BOX_INNERZONE|BOX_OVER | BOX_SEMAPHORE | BOX_TRANSLATELEFTTOP | BOX_CANTDISABLE,
    };

//============

static int StartWindowSize( struct UIKObjGad *obj )
{
/*
    obj->Obj.Box.Top    = obj->Gad.TopEdge  = obj->Obj.w->Height - obj->Obj.Box.Height;
    obj->Obj.Box.Width  = obj->Gad.Width    = obj->Obj.w->Width - obj->Obj.Box.Left;
*/
    obj->Gad.Activation = 0;
    obj->Gad.GadgetType = SIZING;
    return( StdGadInit( obj ) );
}

struct UIKObjGerm __far WindowSizeGerm =
    {
    "UIKObj_WindowSize",
    AddUIKGadget,               // AddVector
    StartWindowSize,            // StartVector
    StopUIKGadget,              // StopVector
    RemUIKGadget,               // RemoveVector
    StdGadRefresh,              // RefreshVector
    StdGadResize,               // ResizeVector
    NULL,                       // ExceptActVector
    NULL,                       // ExceptInaVector
    NULL,                       // ExceptMovVector
    NULL,                       // ExceptKeyVector
    0,0,0,0,
    8,4,
    (30<<16)|5,
    UIKID_SIZEGAD,
    sizeof(struct UIKObjGad),
    BOX_DISPLAYABLE|BOX_INNERZONE|BOX_OVER | BOX_FILLED | BOX_SEMAPHORE | BOX_TRANSLATELEFTTOP | BOX_CANTDISABLE,
    };

//============

ULONG IsWindowFrontMost( struct Window *w )
{
  struct Screen *s = w->WScreen;
  ULONG frontmost;

    LockLayerInfo( &s->LayerInfo );
    if (! (frontmost = (s->LayerInfo.top_layer->Window == w)))
        {
        struct Layer *f, *l;
        for (f = l = w->RPort->Layer; f = f->front; )
            {
            if (f->bounds.MinX > l->bounds.MaxX || f->bounds.MaxX < l->bounds.MinX) continue;
            if (f->bounds.MinY > l->bounds.MaxY || f->bounds.MaxY < l->bounds.MinY) continue;
            break;
            }
        if (f == 0) frontmost = 1;
        }
    UnlockLayerInfo( &s->LayerInfo );
    return( frontmost );
}

static void DepthArrange( struct UIKObjGad *obj )
{
  struct Window *w = obj->Obj.w;
  struct IntuiMessage *imsg = obj->Obj.UIK->IntuiMsg;
  ULONG frontmost=0;

    if (w->Flags & WFLG_BACKDROP) return;
    else frontmost = IsWindowFrontMost( w );

    if (imsg)
        { if (imsg->Qualifier & (IEQUALIFIER_LSHIFT|IEQUALIFIER_RSHIFT)) frontmost = 1; }

    if (frontmost) WindowToBack( w ); else WindowToFront( w );
}

static int StartWindowDepth( struct UIKObjGad *obj )
{
    if (! (obj->Gad.MutualExclude = (ULONG) UIK_AddVector( obj, (void*)DepthArrange, GADGETUP, 0))) return(0);
    return( StdGadInit( obj ) );
}

struct UIKObjGerm __far WindowDepthGerm =
    {
    "UIKObj_WindowDepth",
    AddUIKGadget,               // AddVector
    StartWindowDepth,           // StartVector
    remvectors,                 // StopVector
    RemUIKGadget,               // RemoveVector
    StdGadRefresh,              // RefreshVector
    StdGadResize,               // ResizeVector
    IntUIK_PushBox,             // ExceptActVector
    IntUIK_PopBox,              // ExceptInaVector
    HdlUIK_VerifActGadget,      // ExceptMovVector
    NULL,                       // ExceptKeyVector
    0,0,0,0,
    15,8,
    (22<<16)|10,
    UIKID_DEPTGAD,
    sizeof(struct UIKObjGad),
    BOX_DISPLAYABLE|BOX_OVER | BOX_SEMAPHORE | BOX_TRANSLATELEFTTOP | BOX_ZONEABLE | BOX_CANTDISABLE,
    };

//============

static void ZoomArrange( struct UIKObj *obj )
{
    ZipWindowObj( obj->w->UserData, -1, 0 ); // passe d'une taille à l'autre
}

static int StartWindowZoom( obj )
struct UIKObjGad *obj;
{
    if (! (obj->Gad.MutualExclude = (ULONG) UIK_AddVector( obj, (void*)ZoomArrange, GADGETUP, 0))) return(0);
    return( StdGadInit( obj ) );
}

struct UIKObjGerm __far WindowZoomGerm =
    {
    "UIKObj_WindowZoom",
    AddUIKGadget,               // AddVector
    StartWindowZoom,            // StartVector
    remvectors,                 // StopVector
    RemUIKGadget,               // RemoveVector
    StdGadRefresh,              // RefreshVector
    StdGadResize,               // ResizeVector
    IntUIK_PushBox,             // ExceptActVector
    IntUIK_PopBox,              // ExceptInaVector
    HdlUIK_VerifActGadget,      // ExceptMovVector
    NULL,                       // ExceptKeyVector
    0,0,0,0,
    15,8,
    (22<<16)|10,
    UIKID_ZOOMGAD,
    sizeof(struct UIKObjGad),
    BOX_DISPLAYABLE|BOX_OVER | BOX_SEMAPHORE | BOX_TRANSLATELEFTTOP | BOX_ZONEABLE | BOX_CANTDISABLE,
    };

//============

static int StartWindowIconify( obj )
struct UIKObjGad *obj;
{
    if (! (obj->Gad.MutualExclude = (ULONG) UIK_AddVector( obj, (void*)UIK_Iconify, GADGETUP, 0))) return(0);
    return( StdGadInit( obj ) );
}

struct UIKObjGerm __far WindowIconifyGerm =
    {
    "UIKObj_WindowIconify",
    AddUIKGadget,               // AddVector
    StartWindowIconify,         // StartVector
    remvectors,                 // StopVector
    RemUIKGadget,               // RemoveVector
    StdGadRefresh,              // RefreshVector
    StdGadResize,               // ResizeVector
    IntUIK_PushBox,             // ExceptActVector
    IntUIK_PopBox,              // ExceptInaVector
    HdlUIK_VerifActGadget,      // ExceptMovVector
    NULL,                       // ExceptKeyVector
    0,0,0,0,
    15,8,
    (22<<16)|10,
    UIKID_ICONGAD,
    sizeof(struct UIKObjGad),
    BOX_DISPLAYABLE|BOX_OVER | BOX_SEMAPHORE | BOX_TRANSLATELEFTTOP | BOX_CANTDISABLE,
    };

//============

static int StartWindowClose( obj )
struct UIKObjGad *obj;
{
    obj->Gad.GadgetType = SYSGADGET | CLOSE;
    return( StdGadInit( obj ) );
}

static int SelectClose( struct UIKObj *obj, ULONG class )
{
    if (obj)
        {
        if (class & GADGETDOWN) IntUIK_PushBox( obj );
        else if (class == GADGETUP)
            {
            struct IntuiMessage *imsg = obj->UIK->IntuiMsg;
            int ok=0;

            IntUIK_PopBox( obj );

            if (imsg) // donc sélection avec touche et non pas avec joystick
                {
                ULONG squal = imsg->Qualifier;
                if (squal & IEQUALIFIER_LSHIFT || squal & IEQUALIFIER_RSHIFT) squal |= IEQUALIFIER_LSHIFT|IEQUALIFIER_RSHIFT;
                if (squal & IEQUALIFIER_LALT || squal & IEQUALIFIER_RALT) squal |= IEQUALIFIER_LALT|IEQUALIFIER_RALT;
                squal &=  (IEQUALIFIER_LSHIFT|IEQUALIFIER_RSHIFT
                    | IEQUALIFIER_LALT|IEQUALIFIER_RALT
                    | IEQUALIFIER_CONTROL
                    | IEQUALIFIER_LCOMMAND|IEQUALIFIER_RCOMMAND );
                if (squal == obj->ObjExt->SCQual)
                    ok = 1;
                }
            else ok = 1;

            if (ok)
                {
                struct UIKObj *wo = (struct UIKObj *) obj->w->UserData;
                if (((struct UIKObjWindow*)wo)->WinFlags & (UIKW_CLOSESTOP|UIKW_CLOSEREMOVE))
                    CrossMark( wo, UIKS_STOPPING );
                UIK_CauseObjLink( wo, CLOSEWINDOW );
                UIK_CauseObjHook( wo, CLOSEWINDOW );
                if (((struct UIKObjWindow*)wo)->WinFlags & UIKW_CLOSEREMOVE)
                    {
                    UIK_Stop( wo );
                    CrossMark( wo, UIKS_REMOVING );
                    }
                return(0);
                }
            }
        }
    return(1);
}

struct UIKObjGerm __far WindowCloseGerm =
    {
    "UIKObj_WindowClose",
    AddUIKGadget,               // AddVector
    StartWindowClose,           // StartVector
    StopUIKGadget,              // StopVector
    RemUIKGadget,               // RemoveVector
    StdGadRefresh,              // RefreshVector
    StdGadResize,               // ResizeVector
    IntUIK_PushBox,             // ExceptActVector
    IntUIK_PopBox,              // ExceptInaVector
    HdlUIK_VerifActGadget,      // ExceptMovVector
    NULL,                       // ExceptKeyVector
    0,                          // StartAfterVector
    SelectClose,                // SelectVector
    0,                          // ExtentedVectors
    0,                          // GermTagList
    15,8,
    (22<<16)|10,
    UIKID_CLOSGAD,
    sizeof(struct UIKObjGad),
    BOX_DISPLAYABLE|BOX_OVER | BOX_SEMAPHORE | BOX_TRANSLATELEFTTOP | BOX_ZONEABLE | BOX_CANTDISABLE,
    };
