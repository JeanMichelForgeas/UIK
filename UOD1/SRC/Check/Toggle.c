/****************************************************************
 *
 *      Project:   UIK
 *      Function:  objet CheckToggle
 *
 *      Created:   18/03/93     Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikbase.h"
#include "uiki:uikglobal.h"
#include "uiki:uikobj.h"
#include "uiki:uikmacros.h"
#include "uiki:uikgfx.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objchecktoggle.h"


/****** Macros **************************************************/


/****** Imported ************************************************/

extern struct UIKBitMap * __far UIKBitMapTable[];
extern struct UIKBitMap __far UBM_PicCheck;
extern struct UIKBitMap __far UBM_PicImage1;
extern struct UIKBitMap __far UBM_PicAltImage1;
extern struct UIKBitMap __far UBM_PicImage2;
extern struct UIKBitMap __far UBM_PicAltImage2;


/****** Exported ************************************************/

struct ExecBase * __far SysBase;
struct UIKBase * __far UIKBase;
struct GfxBase * __far GfxBase;
struct IntuitionBase * __far IntuitionBase;


/****** Statics *************************************************/

struct TagItem __far boolmap[] =
    {
    { UIKTAG_OBJCheckFl_Radio,  UIKF_CHECK_RADIO },
    { TAG_END }
    };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/


/*------- Object entries ----------------------------------------*/

static int AddCheck( struct UIKObjCheckToggle *cobj, struct TagItem *taglist)
{
    SysBase = (struct ExecBase *) *((ULONG*)4);
    if (! (cobj->IntuitionBase = IntuitionBase = (struct IntuitionBase *) OpenLibrary( "intuition.library", 0 ))) goto END_ERROR;
    if (! (cobj->GfxBase = GfxBase = (struct GfxBase *) OpenLibrary( "graphics.library", 0 ))) goto END_ERROR;
    if (! (cobj->UIKBase = UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;

    UIK_InvertBlackWhite( cobj->Obj.UIK, UIKBitMapTable, 1 );

    cobj->Flags = UIK_GetTagData( UIKTAG_OBJCheck_Flags, 0, taglist );
    cobj->Flags = UIK_PackBoolTags( cobj->Flags, taglist, boolmap );

    cobj->Image1     = (struct UIKBitMap *) UIK_GetTagData( UIKTAG_OBJCheck_Image1,    0, taglist );
    cobj->AltImage1  = (struct UIKBitMap *) UIK_GetTagData( UIKTAG_OBJCheck_AltImage1, (ULONG)&UBM_PicCheck, taglist );
    cobj->Image2     = (struct UIKBitMap *) UIK_GetTagData( UIKTAG_OBJCheck_Image2,    (ULONG)cobj->Image1, taglist );
    cobj->AltImage2  = (struct UIKBitMap *) UIK_GetTagData( UIKTAG_OBJCheck_AltImage2, (ULONG)cobj->AltImage1, taglist );
    if (cobj->Flags & UIKF_CHECK_RADIO)
        {
        cobj->Image1     = &UBM_PicImage1;
        cobj->AltImage1  = &UBM_PicAltImage1;
        cobj->Image2     = &UBM_PicImage2;
        cobj->AltImage2  = &UBM_PicAltImage2;
        if (! UIK_FindTagItem( UIKTAG_OBJ_BoxAutomate, taglist )) cobj->Obj.Automate = 0;
        if (! UIK_FindTagItem( UIKTAG_OBJ_BoxAltAutomate, taglist )) cobj->Obj.AltAutomate = 0;
        cobj->Obj.ActBoxPen = cobj->Obj.InactBoxPen;
        }

    cobj->Gad.LeftEdge = cobj->Obj.Box.Left;
    cobj->Gad.TopEdge  = cobj->Obj.Box.Top;
    cobj->Gad.Width    = cobj->Obj.Box.Width;
    cobj->Gad.Height   = cobj->Obj.Box.Height;
    cobj->Gad.Flags      = GADGHNONE;
    cobj->Gad.Activation = FOLLOWMOUSE | RELVERIFY | GADGIMMEDIATE;
    cobj->Gad.GadgetType = BOOLGADGET;
    cobj->Gad.UserData = (APTR) cobj; /* le champ UserData doit toujours pointer */
                                      /* vers l'objet auquel appartient ce gadget */
   return(1);

 END_ERROR:
   return(0);
}

static int StartCheck( struct UIKObjCheckToggle *cobj )
{
    cobj->Gad.Flags = (cobj->Obj.Flags & BOX_OVER) ? GADGHNONE : GADGHNONE|SELECTED;
    cobj->Gad.Activation |= TOGGLESELECT;
    UIK_StdGadStart( cobj );
    return(1);
}

static void StopCheck( struct UIKObjCheckToggle *cobj )
{
    UIK_StdGadStop( cobj );
}

static void RemoveCheck( struct UIKObjCheckToggle *cobj )
{
    if (cobj->UIKBase) CloseLibrary( cobj->UIKBase );
    if (cobj->GfxBase) CloseLibrary( cobj->GfxBase );
    if (cobj->IntuitionBase) CloseLibrary( cobj->IntuitionBase );
}

static void RefreshImage( struct UIKObjCheckToggle *cobj )
{
  struct UIKBitMap *ubm = (cobj->Obj.Flags & BOX_OVER) ?
            (cobj->Obj.AutoRatio == 1 ? cobj->Image1 : cobj->Image2) :
            (cobj->Obj.AutoRatio == 1 ? cobj->AltImage1 : cobj->AltImage2);

    if (cobj->Obj.Status == UIKS_STARTED)
        if (ubm)
            BltMaskBitMapRastPort(
                UIK2BM(ubm), 0, 0,
                //((SysBase->ThisTask == cobj->Obj.UIK->Task) ? cobj->Obj.rp : cobj->Obj.irp),
                UIK_GetRastPort( cobj ),
                cobj->Obj.Box.Left+((cobj->Obj.Box.Width-ubm->Width)/2),
                cobj->Obj.Box.Top+((cobj->Obj.Box.Height-ubm->Rows)/2),
                ubm->Width, ubm->Rows,
                0xe0, ubm->Mask );
}

static void RefreshCheck( struct UIKObjCheckToggle *cobj )
{
    UIK_StdGadRefresh( cobj );
    RefreshImage( cobj );
}

static void ResizeCheck( struct UIKObjCheckToggle *cobj )
{
   UIK_StdGadResize( cobj );
}

static void __asm ActCheck( register __a1 struct UIKObjCheckToggle *cobj )
{
    if (cobj->Obj.Flags & BOX_OVER) UIK_StdGadAct( cobj );
    else UIK_StdGadInact( cobj );
    RefreshImage( cobj );
}

static void __asm InactCheck( register __a1 struct UIKObjCheckToggle *cobj )
{
    if (cobj->Gad.Flags & SELECTED) UIK_StdGadAct( cobj );
    else UIK_StdGadInact( cobj );
    RefreshImage( cobj );
}

static void __asm MoveCheck( register __a1 struct UIKObjCheckToggle *cobj, register __a3 struct IntuiMessage *imsg )
{
    UIK_StdGadMove( cobj, imsg ); /* redessine la boite si elle change d'état */
    RefreshImage( cobj );
}

/*------ Objects functions --------------------------------------*/

void SelectFunc( struct UIKObjCheckToggle *cobj )
{
  UWORD position;

    if (cobj)
        {
        position = RemoveGadget( cobj->Obj.w, &cobj->Gad );
        UIK_LockObjVars( cobj );
        cobj->Gad.Flags |= SELECTED;
        cobj->Obj.Flags &= ~BOX_OVER;
        RefreshCheck( cobj );
        UIK_UnlockObjVars( cobj );
        AddGList( cobj->Obj.w, &cobj->Gad, position, 1, cobj->Obj.rq );
        }
}

void UnselectFunc( struct UIKObjCheckToggle *cobj )
{
  UWORD position;

    if (cobj)
        {
        position = RemoveGadget( cobj->Obj.w, &cobj->Gad );
        UIK_LockObjVars( cobj );
        cobj->Gad.Flags &= ~(SELECTED);
        cobj->Obj.Flags |= BOX_OVER;
        RefreshCheck( cobj );
        UIK_UnlockObjVars( cobj );
        AddGList( cobj->Obj.w, &cobj->Gad, position, 1, cobj->Obj.rq );
        }
}

static ULONG __far FuncTab[] =
{
   (ULONG) SelectFunc,
   (ULONG) UnselectFunc,
};

/*------ Joy select ---------------------------------------------*/

static int SelectCheck( struct UIKObjCheckToggle *obj, ULONG class )
{
    if (obj && (class & GADGETUP))
        {
        if (obj->Gad.Flags & SELECTED) UnselectFunc( obj );
        else SelectFunc( obj );
        }
    return(1);
}

/*------ Germ ---------------------------------------------------*/

static struct TagItem __far GermTagList[] =
{
   { UIKTAG_GEN_VersionRevision, W2L(1,3) },
   { UIKTAG_GEN_LastFuncId,     UIKFUNC_Check_LastCmd },
   { UIKTAG_GEN_ActInactPens,   UIKCOLS(UIKCOL_GREY,UIKCOL_GREY,UIKCOL_WHITE,UIKCOL_BLACK) },
   { TAG_END }
};

struct UIKObjGerm __far Germ = {
    "UIKObj_CheckToggle",
    AddCheck,           /*int     (*AddVector)(); */
    StartCheck,         /*int     (*StartVector)(); */
    StopCheck,          /*void    (*StopVector)(); */
    RemoveCheck,        /*void    (*RemoveVector)(); */
    RefreshCheck,       /*void    (*RefreshVector)(); */
    ResizeCheck,        /*void    (*ResizeVector)(); */
    ActCheck,           /*void    (*ExceptActVector)(); */
    InactCheck,         /*void    (*ExceptInaVector)(); */
    MoveCheck,          /*void    (*ExceptMovVector)(); */
    NULL,               /*void    (*ExceptKeyVector)(); */
    NULL,               /*int     (*StartAfterVector)(); */
    SelectCheck,        /*int     (*SelectVector)(); */
    FuncTab,            /*ULONG   *ExtentedVectors; */
    GermTagList,        /*struct TagItem *GermTagList; */
    8,                  /*UWORD   MinWidth; */
    4,                  /*UWORD   MinHeight; */
    SETL(OBJCHECK_BOX_STDWIDTH,OBJCHECK_BOX_STDHEIGHT), /*ULONG DefaultWidthHeight; */
    UIKID_CHECKTOGGLE,                 /*UWORD   ObjId; */
    sizeof(struct UIKObjCheckToggle),   /*UWORD   ObjSize; */
    BOX_DISPLAYABLE|BOX_INNERZONE | BOX_NOPUSH | BOX_OVER | BOX_FILLED | BOX_SEMAPHORE | BOX_TRANSLATELEFTTOP | BOX_ZONEABLE,
    };
