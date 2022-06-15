/****************************************************************
 *
 *      Project:   UIK
 *      Function:  routines de gestion des objets 'requester'
 *
 *      Created:   30/05/90    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikglobal.h"
#include "uiki:uikbox.h"
#include "uiki:uikcolors.h"
#include "lci:uik_protosint.h"

#include "uiki:objrequest.h"
#include "uiki:objwindow.h"


/****** Imported ************************************************/

extern void IntUIK_DrawBox();

extern ULONG __far DOSBase;
extern struct UIKBase * __far UIKBase;
extern struct GfxBase * __far GfxBase;
extern struct IntuitionBase * __far IntuitionBase;


/****** Exported ************************************************/


/****** Statics *************************************************/


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void DrawRequest( struct UIKObjReq *obj )
{
    if (! (obj->Obj.Flags & BOX_NDRAWN))
        {
        if (obj->ReqFlags & UIKREQ_BITMAPED)
            {
            struct BitMap *bm;
            if (bm = obj->CustomBitMap)
                BltBitMapRastPort( obj->CustomBitMap, 0,0, obj->Obj.rp, 0,0, bm->BytesPerRow<<3, bm->Rows, 0xc0 );
            }
        else IntUIK_DrawBox( obj );
        }
}

static int AddRequest( struct UIKObjReq *robj, struct TagItem *taglist )
{
    robj->ReqFlags = UIK_GetTagData( UIKTAG_OBJReq_Flags,0, taglist );

    if (robj->CustomBitMap = (struct BitMap *) UIK_GetTagData( UIKTAG_OBJReq_CustomBitMap, 0, taglist ))
        robj->ReqFlags |= UIKREQ_BITMAPED;
    robj->CustomAltBitMap = (struct BitMap *) UIK_GetTagData( UIKTAG_OBJReq_CustomAltBitMap, (ULONG) robj->CustomBitMap, taglist );

    if (! UIK_FindTagItem( UIKTAG_OBJ_LeftTop, taglist )) robj->ReqFlags |= UIKREQ_CENTERED;
    else *((ULONG*)&robj->OldLeft) = *((ULONG*)&robj->Obj.Box.Left);

    *((ULONG*)&robj->Obj.Box.Left) = 0;
    *((ULONG*)&robj->Obj.MinWidth) = *((ULONG*)&robj->Obj.Box.Width);

    return(1);
}

static int StartRequest( struct UIKObjReq *robj )
{
  register struct Requester *req = &robj->Req;
  register struct Window *w = robj->Obj.w;
  register struct UIKObjWindow *wo = (struct UIKObjWindow *) w->UserData;
  WORD left, top;
  int rc=0;

    if (! (w->Flags & WFLG_BACKDROP)) 
        {
        if (! IsWindowFrontMost( w )) // objgad.c
            WindowToFront( w );
        }
    ScreenToFront( w->WScreen );

    InitRequester( req );
    if (robj->ReqFlags & UIKREQ_CENTERED)
        {
        //left = w->Width/2 - robj->Obj.Box.Width/2;
        left = wo->LeftBorderWidth + (w->Width - wo->LeftBorderWidth - wo->RightBorderWidth) / 2 - robj->Obj.Box.Width / 2;
        top = wo->TopBorderHeight + (w->Height - wo->TopBorderHeight - wo->BottomBorderHeight) / 2 - robj->Obj.Box.Height / 2;
        }
    else{
        left = wo->LeftBorderWidth + robj->OldLeft;
        top = wo->TopBorderHeight + robj->OldTop;
        }
    if (left + robj->Obj.Box.Width > w->Width) left = w->Width - robj->Obj.Box.Width;
    if (left < 0) left = 0;
    if (top + robj->Obj.Box.Height > w->Height) top = w->Height - robj->Obj.Box.Height;
    if (top < 0) top = 0;
    robj->Req.LeftEdge = left;
    robj->Req.TopEdge  = top;
    robj->Req.Width    = robj->Obj.Box.Width;
    robj->Req.Height   = robj->Obj.Box.Height;
    robj->Req.Flags    = NOISYREQ;
    robj->Req.BackFill = UIK_GetColor( robj, robj->Obj.BoxPen );  // ((UBYTE*)&UIKBase->UIKPrefs->Colors)[robj->Obj.BoxPen];

    //*((ULONG*)&robj->Obj.Box.Left) = 0;

    ModifyIDCMP( w, w->IDCMPFlags & ~IDCMP_MENUVERIFY);
    Forbid();
    if (Request( req, w ))
        {
        if (robj->Obj.irp = (struct RastPort *) UIK_CloneRPort( req->ReqLayer->rp ))
            {
            req->ImageBMap = (struct BitMap *) robj;
            robj->Obj.rq = req;
            robj->Obj.rp = req->ReqLayer->rp;
            rc = 1;
            }
        }
    Permit();
    ModifyIDCMP( w, w->IDCMPFlags | IDCMP_MENUVERIFY);
    return( rc );
}

static void StopRequest( struct UIKObjReq *robj )
{
  register struct Requester *r = robj->Obj.rq;

    if (r)
        {
        Forbid();
        UIK_StripWindowMsg( robj );
        EndRequest( r, robj->Obj.w ); robj->Obj.rq = 0;
        Permit();
        UIK_FreeCloneRPort( robj->Obj.irp );
        }
}

static void RemoveRequest( struct UIKObjReq *robj )
{
    UIK_nl_FreeList( &robj->ShortCutList );
}

static int SelectFunc( struct UIKObj *or )
{
    if (or->w) ActivateWindow( or->w );
    return(1);
}

struct UIKObjGerm __far RequestGerm =
    {
    "UIKObj_Request",
    AddRequest,                 // AddVector
    StartRequest,               // StartVector
    StopRequest,                // StopVector
    RemoveRequest,              // RemoveVector
    DrawRequest,                // RefreshVector
    NULL,                       // ResizeVector
    NULL,                       // ExceptActVector
    NULL,                       // ExceptInaVector
    NULL,                       // ExceptMovVector
    NULL,                       // ExceptKeyVector
    0,
    SelectFunc,
    0,0,
    8,4, (250<<16)|60,
    UIKID_REQUEST,
    sizeof(struct UIKObjReq),
    BOX_DISPLAYABLE|BOX_OVER | BOX_SEMAPHORE | BOX_FILLED | BOX_CANTDISABLE,
    };
