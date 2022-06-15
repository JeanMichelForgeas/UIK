/****************************************************************
 *
 *      Project:   UIK
 *      Function:  gestion des objets
 *
 *      Created:   13/05/90    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikbase.h"
#include "uiki:uikglobal.h"
#include "uiki:uikbox.h"
#include "uiki:uikgfx.h"
#include "uiki:uikmacros.h"
#include "uiki:uikcolors.h"
#include "uiki:uikdrawauto.h"
#include "lci:uik_protosint.h"

#include "uiki:objscreen.h"
#include "uiki:objmenu.h"
#include "uiki:objjoystick.h"   objscreen  lkjkjlkjkj
#include "uiki:objwindow.h"
#include "uiki:objrequest.h"   lkjkjlkjkjobjscreenobjscreen
#include "uiki:objstring.h"

/* this macro lets us long-align structures on the stack */
#define D_S(type,name) char a_##name[sizeof(type)+3]; \
                       type *name = (type *)((LONG)(a_##name+3) & ~3);


/****** Macros **************************************************/


/****** Imported ************************************************/

extern LONG func_CBWrite( struct IOClipReq *clip, char *ptr, LONG len );
extern struct IOClipReq *func_CBWriteStart( LONG unit );
extern void func_CBWriteEnd( struct IOClipReq *clip, LONG len );
extern LONG func_CBRead( struct IOClipReq *clip, char *ptr, LONG len );
extern struct IOClipReq *func_CBReadStart( LONG unit, ULONG *plen );
extern void func_CBReadEnd( struct IOClipReq *clip );

extern struct Unit * __asm do_process_cmd( register __a0 struct IORequest *req );
extern void __asm do_process_warn( register __a0 struct IORequest *req );
extern void __asm do_send_warnremove( register __a0 struct UIKObj *obj );

extern void __asm Auto2InnerBox( register __a0 struct UIKObj *obj );
extern void IntUIK_DrawBox();
extern void __asm do_lockedrefresh( register __a0 struct UIKObj *obj );
extern struct UIKZoneList * __asm zo_InstallZoneList( register __a0 struct UIKGlobal *uik, register __a1 struct UIKZoneList *uzl );
extern void __asm zo_MarkZone( register __a0 struct UIKGlobal *uik, register __a1 UZone *zo );
extern UZone * __asm zo_UnmarkZone( register __a0 struct UIKGlobal *uik );
extern UBYTE * __asm circbuf_get( register __a1 struct UIKGlobal *uik );

extern struct IntuitionBase * __far IntuitionBase;
extern struct GfxBase * __far GfxBase;
extern struct UIKBase * __far UIKBase;
extern ULONG __far DOSBase;


/****** Exported ************************************************/

ULONG __far stdout;


/****** Statics *************************************************/

int FALSE_Remove();
int TRUE_Remove();

static UWORD __far EraseAutomate[] = { COL_ERASE, RECTFILL, END_AUTO };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

void ReplyIntuiMsg( struct Message *msg )
{
    if (msg->mn_ReplyPort) ReplyMsg( msg );
    else UIK_MemSpFree( msg );
}

struct RastPort *UIK_CloneRPort( register struct RastPort *rp )
{
  register struct RastPort *clone;

    if (! (clone = (struct RastPort *) UIK_MemSpAlloc( sizeof(struct RastPort), MEMF_ANY|MEMF_CLEAR ))) return(0);
    InitRastPort( clone );
    clone->Layer = rp->Layer;
    clone->BitMap = rp->BitMap;
    return( clone );
}

void UIK_FreeCloneRPort( struct RastPort *clone )
{
    UIK_MemSpFree( clone );
}

/*
static void RemoveMsgCode( register struct IntuiMessage *msg )
{
  register struct Window *w = msg->IDCMPWindow;
  register UWORD code = msg->Code;
  register struct IntuiMessage *succ;

    Forbid();
    msg = (struct IntuiMessage *)msg->ExecMessage.mn_Node.ln_Succ;
    while (succ = (struct IntuiMessage *)msg->ExecMessage.mn_Node.ln_Succ)
        {
        if (msg->IDCMPWindow == w)
            {
            if (msg->Code == code) { Remove( msg ); ReplyIntuiMsg( msg ); }
            //else break;
            }
        msg = succ;
        }
    Permit();
}
*/

static void StripListNodes( register struct List *list, register struct UIKObj *obj )
{
  register struct Window *w;
  register struct IntuiMessage *msg, *succ, *umsg;

    w = obj->w;
    msg = (struct IntuiMessage *) list->lh_Head;
    umsg = obj->UIK->IntuiMsg;
    while (succ = (struct IntuiMessage *)msg->ExecMessage.mn_Node.ln_Succ)
        {
        if (msg->IDCMPWindow == w)
            {
            Remove( msg ); ReplyIntuiMsg( msg );
            if (msg == umsg)
                {
                obj->UIK->IntuiMsg = 0;
                }
            }
        msg = succ;
        }
}

void UIK_StripWindowMsg( register struct UIKObj *obj )
{
    Forbid();
    StripListNodes( (struct List *) &obj->UIK->UPort.mp_MsgList, obj );
    StripListNodes( (struct List *) &obj->UIK->IMSList, obj );
    Permit();
}

void __asm UIK_StripWindowEvents( register __a0 struct UIKObj *obj, register __d0 ULONG class )
{
  struct List *list;
  struct Node *succ, *node;

    if (obj)
        {
        list = (struct List *) &obj->UIK->IMSList;
        node = list->lh_Head;
        while (succ = node->ln_Succ)
            {
            if (((struct IntuiMessage *)node)->IDCMPWindow == obj->w)
                {
                if (((struct IntuiMessage *)node)->Class & class)
                    {
                    Disable();
                    Remove( node );
                    Enable();
                    ReplyIntuiMsg( (struct Message *) node );
                    }
                }
            node = succ;
            }
        }
}

static struct Region * __asm GetObjRegion( register __a0 struct UIKObj *obj )
{
  struct Region *reg;

    if (reg = (struct Region *) NewRegion())
        {
        /*@@@
        struct Rectangle rect;
        rect.MinX = obj->Box.Left + 2;
        rect.MinY = obj->Box.Top + 1;
        rect.MaxX = obj->Box.Left + obj->Box.Width - 3;
        rect.MaxY = obj->Box.Top + obj->Box.Height - 2;
        if (obj->ActBoxPen == UIKCOL_WHITE && !(obj->Flags & BOX_OVER) && !(obj->Flags & BOX_NOPUSH)) { rect.MaxX--; rect.MaxY--; }
        if (! OrRectRegion( reg, &rect )) { DisposeRegion( reg ); reg = 0; }
        */
        if (! OrRectRegion( reg, &obj->ObjExt->InnerRect )) { DisposeRegion( reg ); reg = 0; }
        }
    return( reg );
}

int IntUIK_ChangeRegion( struct UIKObj *obj, int wantnew )
{
  struct Region *reg=0;

    if (!wantnew || (wantnew && (reg = GetObjRegion( obj ))))
        {
        if (obj->Region) DisposeRegion( obj->Region );
        obj->Region = reg;
        }
    return( (int)reg );
}

static void stopactive( obj )
register struct UIKObj *obj;
{
  struct UIKGlobal *uik = obj->UIK;

    if (obj == uik->ActiveObj)
        {
        if (obj->ExceptInaVector) (*obj->ExceptInaVector)( obj );
        uik->ActiveObj = 0;
        }
}

void __asm UIK_ShortCutRemove( register __a0 struct UIKObj *o )
{
  struct UIKList *list;
  struct UIKHook *hook;
  int ind;

    if (o)
        {
        if (list = o->ObjExt->SCList)
            if (hook = o->ObjExt->SCHook)
                if ((ind = UIK_nl_FindNode( list, (UBYTE *) hook )) >= 0)
                    UIK_nl_FreeNodes( list, ind, ind );
        *((ULONG*)&o->ObjExt->SCKey) = (ULONG)(o->ObjExt->SCList = (APTR)(o->ObjExt->SCHook = (APTR)0));
        }
}

static void CrossCleanup( struct UIKObj *obj, int free )
{
    if ((obj->Status == UIKS_REMOVED) || ((obj->Status == UIKS_STOPPED) && (free == 0))) return;

    if (obj->List.NodeNum) UIK_nl_CrossList( obj, (void*)CrossCleanup, 0, free );

    if (obj->Status == UIKS_ICONIFIED)
        {
        if (obj->Id == UIKID_WINDOW)
            {
            struct UIKObj *iconparent = obj->sc ? (struct UIKObj *) obj->sc->UserData : obj->UIK;
            LONG n = UIK_nl_FindNode( iconparent, (UBYTE *) obj->Icon );
            if (n >= 0) TRUE_Remove( obj->Icon );
            }
        obj->Icon = 0;
        obj->Status = UIKS_OLDICONIFIED;
        }

    if (obj->Status != UIKS_REMOVING &&
        obj->Status != UIKS_STOPPED  &&
        obj->Status != UIKS_ICONIFIED /*&& obj != obj->UIK*/)
        {
        UBYTE old = obj->Status == UIKS_OLDICONIFIED ? 1 : 0;

//if (obj->RegGerm) kprintf( "Stopping object %lx '%ls'\n", obj, obj->RegGerm->Germ->ObjName );
        zo_ObjStopping( obj );

        if ((obj->Flags & BOX_VISUALERASE) && (obj->Status != UIKS_STOPPING) && !old)
            {
            obj->Status = UIKS_STARTED;
            IntUIK_DrawAuto( obj, EraseAutomate, 0, 0 );
            }
        obj->Status = UIKS_STOPPED; // laisser avant l'appel de UserStopFunc pour éviter l'appel à l'infini
        stopactive( obj );
        if (obj->ObjExt->UserStopFunc)
            {
            APTR tmp = obj->UIK->RemoveVector;
            obj->UIK->RemoveVector = FALSE_Remove; //----------------------
            (*obj->ObjExt->UserStopFunc)( obj );
            obj->UIK->RemoveVector = tmp;  //----------------------
            }
        if (!old && obj->StopVector) (*obj->StopVector)( obj );

        IntUIK_ChangeRegion( obj, 0 ); // DisposeRegion() seulement
        }
    else{
        if (obj->Status != UIKS_REMOVING) obj->Status = UIKS_STOPPED;
        stopactive( obj );
        }

    if (free)
        {
//if (obj->RegGerm) kprintf( "Removing object %lx '%ls'\n", obj, obj->RegGerm->Germ->ObjName );

        obj->Status = UIKS_REMOVED; // laisser avant l'appel de UserRemoveFunc pour éviter l'appel à l'infini
        if (obj->ObjExt->UserRemoveFunc) (*obj->ObjExt->UserRemoveFunc)( obj );
        if (obj->RemoveVector) (*obj->RemoveVector)( obj );

        do_send_warnremove( obj ); // après l'appel éventuel de code de l'autre tâche

        if (obj->Flags & BOX_OPENEDFONT) CloseFont( obj->ObjExt->Font );
        UIK_ShortCutRemove( obj );
        zo_DeleteZoneList( obj );
        FreeLangage( obj );
        if ((obj->TitleFlags & (TITLE_ADDR | TITLE_IMAGE)) == TITLE_ADDR)
            {
            BufFree( (UBYTE*) obj->Title );
            BufFree( (UBYTE*) obj->AltTitle );
            }
        UIK_nl_FreeList( &obj->List );
        UIK_nl_FreeList( &obj->Hook );
        UIK_nl_FreeList( &obj->Link );
        if (obj->RegGerm) IntUIK_DecRegGermCounter( obj->RegGerm );
        }

    obj->sc = 0; obj->w = 0; obj->rq = 0; obj->rp = obj->irp = 0;
}

static void CrossIconify( obj, icon )
struct UIKObj *obj, *icon;
{
    if (obj->Status != UIKS_STARTED) return; /* on iconifie que les objets started */
    if (obj->List.NodeNum) UIK_nl_CrossList( obj, (void*)CrossIconify, 0, (ULONG)icon );
    // pas de UserStopFunc() car elle peut contenire remove...
    if (obj->StopVector) (*obj->StopVector)( obj );
    obj->Status = UIKS_ICONIFIED;
    stopactive( obj );
    obj->w = 0; obj->rq = 0; obj->rp = obj->irp = 0;
    obj->Icon = icon;
}

int do_init( struct UIKObj *obj, struct Window *w )
{
//if (obj->RegGerm) kprintf( "Starting object %lx '%ls'\n", obj '%ls', obj->RegGerm->Germ->ObjName );
    if (obj->Id == UIKID_SCREEN)
        {
        if (! (*obj->StartVector)( obj )) return(0);
        return(-1);  // pas de window, mais pour ne pas retourner 0
        }
    else{
        if (obj->Id == UIKID_WINDOW)
            {
            obj->sc = (struct Screen *) obj->Parent->sc;
            if (! (*obj->StartVector)( obj )) return(0);
            w = obj->w;
            }
        else if (obj->Parent) // si UIK, parent == 0
            {
            obj->sc = (struct Screen *) obj->Parent->sc;
            obj->rp = obj->Parent->rp;
            obj->irp = obj->Parent->irp;
            if (w && ((LONG)w != -1))
                {                        // si une window
                obj->w = w;
                obj->rq = obj->Parent->rq;
                }
            else w = (struct Window *) -1;

            if (obj->Flags & BOX_CLIPPED || obj->TitleFlags & TITLE_CLIPPED)
                {
                Auto2InnerBox( obj );  //@@@
                if (! IntUIK_ChangeRegion( obj, 1 )) return(0);
                }
            obj->Status = UIKS_STARTED;
            if (obj->StartVector)
                {
                if (! (*obj->StartVector)( obj ))
                    {
                    obj->Status = UIKS_STOPPED;
                    IntUIK_ChangeRegion( obj, 0 ); // DisposeRegion() seulement
                    return(0);
                    }
                }
            do_lockedrefresh( obj );
            }
        else{
            obj->Status = UIKS_STARTED;
            return(-1);  // UIK: pas de window, mais pour ne pas retourner 0
            }
        }
    return( (int) w );
}

static int CrossResize( struct UIKObj *obj, struct UIKObjWindow *wo )
{
  WORD woffs, hoffs;
  struct Rectangle *r = &obj->ObjExt->InnerRect;

    if ((obj->Id == UIKID_REQUEST)
     || (obj->Id == UIKID_SCREEN)
     || (obj->Id == UIKID_WINDOW && obj != wo)) return(1);

    woffs = wo->Obj.Box.Width - wo->NW.Width;
    hoffs = wo->Obj.Box.Height - wo->NW.Height;

    if (obj->ResizeFlags & UO_ATTACHED_RIGHT)
        {
        if (obj->ResizeFlags & UO_ATTACHED_LEFT)
            {
            if (obj->Box.Width + woffs < obj->MinWidth) woffs = obj->MinWidth - obj->Box.Width;
            obj->Box.Width += woffs; r->MaxX += woffs;
            }
        else { obj->Box.Left += woffs; r->MinX += woffs; r->MaxX += woffs; }
        }
    if (obj->ResizeFlags & UO_ATTACHED_BOT)
        {
        if (obj->ResizeFlags & UO_ATTACHED_TOP)
            {
            if (obj->Box.Height + hoffs < obj->MinHeight) hoffs = obj->MinHeight - obj->Box.Height;
            obj->Box.Height += hoffs; r->MaxY += hoffs;
            }
        else { obj->Box.Top += hoffs; r->MinY += hoffs; r->MaxY += hoffs; }
        }

    if (obj->Flags & BOX_CLIPPED || obj->TitleFlags & TITLE_CLIPPED)
        {
        //Auto2InnerBox( obj );  //@@@
        IntUIK_ChangeRegion( obj, 1 );
        }
    if (obj->ResizeVector) (*obj->ResizeVector)( obj );

    if (obj->List.NodeNum) return( UIK_nl_CrossList( obj, CrossResize, 1, (ULONG)wo ) );
    else return(1);
}

static int ResizeWindowObjects( struct UIKObjWindow *wo, struct Window *w )
{
    if (wo->Obj.Box.Width == w->Width && wo->Obj.Box.Height == w->Height) return(0);
    wo->Obj.Box.Width = w->Width;
    wo->Obj.Box.Height = w->Height;
    if (wo->Obj.ResizeVector) (*wo->Obj.ResizeVector)( wo );
    UIK_nl_CrossList( wo, CrossResize, 1, (ULONG)wo );
    wo->NW.Width = w->Width;
    wo->NW.Height = w->Height;
    return(1);
}

static int CrossInit( struct UIKObj *obj, struct Window *w )
{
    if (obj->Status == UIKS_STOPPED)
        {
        if (!(obj->Flags & BOX_STARTSPEC) || (obj == obj->UIK->StartSpec))
            {
//kprintf( "3 %ld\n", obj->Id );
            if (! (w = (struct Window *) do_init( obj, w ))) return(0);
//kprintf( "4\n" );

            if (obj->List.NodeNum)
                {
//kprintf( "1 %ld\n", obj->Id );
                if (! UIK_nl_CrossList( obj, (void*)CrossInit, 1, (ULONG)w )) return(0); /* want retcode checking */
//kprintf( "2\n" );
                }
            if (obj->StartAfterVector)
                {
                if (! obj->StartAfterVector( obj )) return(0);
                }
            }
        }
    return(1);
}

int __asm UIK_Start( register __a0 struct UIKObj *obj )
{
  struct Window *w=0;

    if (! obj) return(0);
    if (obj != obj->UIK) // obj->Parent et obj->Parent->w == 0 si UIK
        {
//kprintf( "Start: parent=%lx, Status=%ld\n", obj->Parent, obj->Parent->Status );
        if (obj->Parent->Status != UIKS_STARTED) return(0);
        w = obj->Parent->w;
        }
    obj->UIK->StartSpec = (obj->Flags & BOX_STARTSPEC ? obj : 0);
    return( CrossInit( obj, w ) );
}

/*-------------------------Iconify------------------------*/

static int CrossUniconify( struct UIKObj *obj, struct Window *w )
{
  int rc;

    if (obj->Status == UIKS_ICONIFIED && obj->Icon == obj->Parent->Icon)
        {
        if (! (w = (struct Window *) do_init( obj, w ))) return(0);
        rc = (obj->List.NodeNum) ? UIK_nl_CrossList( obj, (void*)CrossUniconify, 1, (ULONG)w ) : 1; /* want retcode checking */
        if (rc && obj->StartAfterVector) rc = obj->StartAfterVector( obj );
        if (rc) obj->Icon = 0;
        return(rc);
        }
    return(1); /* continue scan */
}

ULONG __asm UIK_Uniconify( register __a0 struct UIKObjWindow *icon )
{
  struct UIKObjWindow *uwo;
  struct UIKGlobal *uik;
  struct IntuiMessage *im;
  ULONG rc, flag=0;

    if (icon)
        {
        uwo = icon->UniconW;
        uik = icon->Obj.UIK;
        im = uik->IntuiMsg;
        if (icon->WinFlags & UIKW_ICONWINDOW)
            {
            if (im)
                {
                if (im->IDCMPWindow == icon->Obj.w)
                    {
                    if (im->Class == MOUSEBUTTONS) { if (im->Code == MENUDOWN) flag = 1; }
                    else if (im->Class == RAWKEY) { if ((uik->KeySupp.Length == 1) && (uik->KeySupp.KeyBuffer[0] == 'q')) flag = 1; }
                    }
                else flag = 1;
                }
            else flag = 1;

            if (flag)
                {
                uwo->IconLeftTop = *(ULONG*)&icon->Obj.w->LeftEdge;
                if (rc = do_init( uwo, 0 ))
                    {
                    rc = UIK_nl_CrossList( uwo, (void*)CrossUniconify, 1, rc ); /* w */
                    if (rc && uwo->Obj.StartAfterVector) rc = uwo->Obj.StartAfterVector( uwo );
                    }
                if (rc) { UIK_Remove( icon ); uwo->Obj.Icon = 0; return(1); }
                else CrossIconify( uwo, icon );
                }
            return(0);
            }
        else return(1);
        }
    return(1);
}

ULONG __asm UIK_UniconifyWindow( register __a0 struct UIKObjWindow *win )
{
  struct UIKObjWindow *uwo, *icon;
  ULONG rc;

    if (win)
        {
        if (icon = (struct UIKObjWindow *) win->Obj.Icon)
            {
            uwo = icon->UniconW;
            uwo->IconLeftTop = *(ULONG*)&icon->Obj.w->LeftEdge;
            if (rc = do_init( uwo, 0 ))
                {
                rc = UIK_nl_CrossList( uwo, (void*)CrossUniconify, 1, rc ); /* w */
                if (rc && uwo->Obj.StartAfterVector) rc = uwo->Obj.StartAfterVector( uwo );
                }
            if (rc) { UIK_Remove( icon ); uwo->Obj.Icon = 0; }
            else { CrossIconify( uwo, icon ); return(0); }
            }
        return(1);
        }
    return(0);
}

struct UIKObjWindow * __asm UIK_Iconify( register __a0 struct UIKObj *obj )
{
  register struct UIKObjWindow *icon, *wo;
  struct UIKObj *so;
  UWORD icw, ich;
  UBYTE *title;
  struct TextFont *font;
  struct TTextAttr TTA;
  ULONG left_top, master;

    if (!obj || (obj->Status != UIKS_STARTED || !obj->w)) goto ERROR0;

    wo = (struct UIKObjWindow *) obj->w->UserData;
    master = (ULONG) wo->Obj.ObjExt->Lang->Master;
    title = UIK_ObjTitle( wo );
    font = (struct TextFont *) IconFont( wo );
    UIK_Font2TTA( font, &TTA );
    ich = 3 + font->tf_YSize;
    icw = 10;
    if (title)
        {
        if (wo->Obj.TitleFlags & TITLE_IMAGE) icw += ((struct UIKBitMap*)title)->Width;
        else icw += UIK_StrWidth( font, title, StrLen( title ) );
        }
    UIK_AskIconPlace( wo, &left_top, icw );
    so = 0;
    if (obj->sc) so = (struct UIKObj *) obj->sc->UserData;
    if (! (icon = (struct UIKObjWindow *) IntUIK_AddObjectTags( "UIKObj_Window", (so ? so : obj->UIK),
            UIKTAG_OBJ_LeftTop, left_top,
            UIKTAG_OBJ_WidthHeight, SETL(icw,ich),
            UIKTAG_OBJ_Title, wo->Obj.Title,
            UIKTAG_OBJ_AltTitle, wo->Obj.AltTitle,
            UIKTAG_OBJ_ActInactPens, wo->IconColors, //UIKCOL_GREY<<24 | UIKCOL_GREY<<16 | UIKCOL_BLACK<<8 | UIKCOL_BLACK,
            UIKTAG_OBJ_Title_Flags, wo->Obj.TitleFlags,
            UIKTAG_OBJWindow_Flags, UIKW_ICONWINDOW | UIKW_DRAG,
            UIKTAG_OBJWindowFl_NoBlackPerimeter, TRUE,
            UIKTAG_OBJ_LangMaster, master,
            UIKTAG_OBJ_TTextAttr, &TTA,
            UIKTAG_OBJWindow_ScreenAddr, wo->Obj.w->WScreen /*ScrAdd*/,
            TAG_END ))) goto ERROR;
    if (! UIK_AddVector( icon, (void*)UIK_Uniconify, MOUSEBUTTONS|RAWKEY, 0 )) goto ERROR;
    if (! UIK_Start( icon )) goto ERROR;
    icon->UniconW = wo;

    CrossIconify( wo, icon );
    return( icon );

    ERROR:
    UIK_Remove( icon );
    ERROR0:
    return(0);
}

/*------------------------Remove/Stop---------------------*/
void CrossMark( struct UIKObj *obj, UBYTE status )
{
    if (obj->List.NodeNum) UIK_nl_CrossList( obj, (void*)CrossMark, 0, status );
    if (obj->Status == UIKS_STARTED ||
        obj->Status == UIKS_STOPPING ||
        obj->Status == UIKS_STOPPED)
        {  // sinon on va executer obj->StopVector deux fois !!!
        if (! (obj->Status == UIKS_STOPPED && status == UIKS_STOPPING))
            {
            obj->Status = status;
            obj->UIK->RemObj = 1;
            }
        }
}

int FALSE_Remove( struct UIKObj *obj )
{
    if (! obj) return(0);
    UIK_Stop( obj );
    CrossMark( obj, UIKS_REMOVING );
    return(1);
}

int TRUE_Remove( struct UIKObj *obj )
{
  register struct UIKGlobal *uik;
  register struct UIKObj *parent;
  int n = -1;

    if (! obj) return(0);

    uik = obj->UIK;
    parent = obj->Parent;

    /* dommage, ça fait planter...
    if (parent)
        {
        int n = UIK_nl_FindNode( parent, (UBYTE *) obj );
        if (n != -1) UIK_nl_FreeNodes( parent, n, n );
        }
    */

    if (parent) n = UIK_nl_FindNode( parent, (UBYTE *) obj );
    CrossCleanup( obj, 1 );
    if (n >= 0) UIK_nl_FreeNodes( parent, n, n );

    if (obj == uik) { Remove2( uik ); return(0); }
    return(1);
}

void __asm UIK_Stop( register __a0 struct UIKObj *obj )
{
    if (! obj) return;
    CrossCleanup( obj, 0 );
}

void __asm UIK_Remove( register __a0 struct UIKObj *obj )
{
    if (obj)
        if (obj->UIK)
            if (obj->UIK->RemoveVector)
                (*obj->UIK->RemoveVector)( obj );
}

/*-------------------------------------------------------------*/

struct UIKHook * __asm UIK_AddVector(
register __a0 struct UIKObj *obj,
register __a1 void (*vect)(),
register __d0 ULONG mask,
register __d1 ULONG parm )
{
  struct UIKHook *hook=0;

    if (obj)
        if (hook = (struct UIKHook *)UIK_nl_AllocNode( &obj->Hook, -1, sizeof(struct UIKHook), 0 ))
            {
            hook->Vect = vect;
            hook->Obj  = obj;
            hook->Mask = mask;
            hook->Parm = parm;
            if (obj == obj->UIK) ((struct UIKGlobal *)obj)->ExtMask |= mask;
            }
    return( hook );
}

static void __asm build_uik_mask( register __a0 struct UIKHook *hook, register __d1 struct UIKGlobal *uik )
{
    uik->ExtMask |= hook->Mask;
}

void __asm UIK_RemoveVector(
register __a1 struct UIKObj *obj,
register __a0 struct UIKHook *hook )
{
  int ind;

    if (obj && hook && (ind = UIK_nl_FindNode( &obj->Hook, (UBYTE *) hook )) >= 0)
        {
        struct UIKGlobal *uik = obj->UIK;
        UIK_nl_FreeNodes( &obj->Hook, ind, ind );
        if (obj == uik)
            {
            uik->ExtMask = 0;
            UIK_nl_CrossList( &obj->Hook, (void*)build_uik_mask, 0, (ULONG)uik );
            }
        }
}

void __asm UIK_RemAllVectors(
register __a1 struct UIKObj *obj )
{
    if (obj)
        {
        UIK_nl_FreeList( &obj->Hook );
        if (obj == obj->UIK)
            ((struct UIKGlobal *)obj)->ExtMask = 0;
        }
}

extern void __asm ExecuteClass( register __a0 struct UIKHook *hook, register __d1 ULONG class );
extern void __asm ExecuteShortCut( register __a0 struct UIKHook *hook, register __d1 ULONG events );
extern void __asm ExecuteMask( register __a0 struct UIKHook *hook, register __d1 ULONG events );
extern int __asm ExecuteLink( register __a0 struct UIKLink *link, register __d1 ULONG class );

void __asm UIK_CauseObjLink( register __a0 struct UIKObj *obj, register __d0 ULONG class )
{
    if (obj && !(obj->Flags & BOX_DISABLED))
        {
        obj->MouseIn = 1;
        UIK_nl_CrossList( &obj->Link, ExecuteLink, 1, class );
        }
}

void __asm UIK_CauseObjHook( register __a0 struct UIKObj *obj, register __d0 ULONG class )
{
    if (obj && !(obj->Flags & BOX_DISABLED))
        {
        obj->MouseIn = 1;
        UIK_nl_CrossList( &obj->Hook, (void*)ExecuteClass, 0, class );
        }
}

static void CrossTrueStop( obj )
struct UIKObj *obj;
{
    if (obj->Status != UIKS_STOPPING)
        {
        if (obj->List.NodeNum) UIK_nl_CrossList( obj, (void*)CrossTrueStop, 0, 0 );
        }
    else{
        UIK_Stop( obj );
        }
}

static int CrossTrueRemove( obj )
struct UIKObj *obj;
{
    if (obj->Status != UIKS_REMOVED)
        {
        if (obj->Status != UIKS_REMOVING)
            {
            if (obj->List.NodeNum) UIK_nl_CrossList( obj, (void*)CrossTrueRemove, 1, 0 );
            }
        else{
            if (TRUE_Remove( obj ) == 0) return(0); /* 0 -> arrêt de UIK !!! */
            }
        }
    return(1);
}

struct UIKObj * __asm win_or_req( register __a0 struct Window *w )
{
    return( (struct UIKObj *) (w->FirstRequest ? w->FirstRequest->ImageBMap : w->UserData) );
}

ULONG __asm FalseCrossList( register __a0 struct UIKGlobal *uik, register __d0 ULONG mask, register __d1 ULONG visit )
{
  APTR tmp = uik->RemoveVector;
  ULONG rc = 0;

    uik->RemoveVector = FALSE_Remove; //----------------------
    UIK_nl_CrossList( &uik->Obj.Hook, (void*)ExecuteMask, 0, mask );
    if (uik->RemObj)
        {
        CrossTrueStop( uik );
        if (uik->Obj.Status != UIKS_STARTED) goto END_ERROR; /* UIK stoppé */
        if (visit == 0)
            {
            if (CrossTrueRemove( uik ) == 0) goto END_ERROR;  /* UIK arrêté */
            }
        }
    rc = 1;

  END_ERROR:
    uik->RemoveVector = tmp; //----------------------
    return( rc );
}

//----------------------

struct _IsId {
    struct UIKObj   *After;
    struct UIKObj   *Res;
    UWORD           Id;
    UWORD           SearchFlag;
    };

static int CrossIsId( struct UIKObj *obj, struct _IsId *isid )
{
    if (isid->SearchFlag == 0)
        {
        if (isid->After == obj) isid->SearchFlag = 1;
        }
    else{
        if (obj->Id == isid->Id)
            {
            isid->Res = obj;
            return(0);
            }
        }
    if (obj->List.NodeNum)
        return( UIK_nl_CrossList( obj, (void*)CrossIsId, 1, (ULONG)isid ) );
    return(1);
}

struct UIKObj * __asm UIK_GetObject( register __a0 struct UIKObj *root, register __a1 struct UIKObj *after, register __d0 UWORD id )
{
  D_S(struct _IsId,isid);

    isid->After = after;
    isid->Res = 0;
    isid->Id = id;
    isid->SearchFlag = (after == 0) ? 1 : 0;

    if (root->List.NodeNum)
        UIK_nl_CrossList( root, (void*)CrossIsId, 1, (ULONG)isid );
    return( isid->Res );
}

//----------------------

struct KeyTankParms {
    struct UIKObj   *Parent;
    struct UIKObj   *Selected;
    struct UIKObj   *First;
    struct UIKObj   *Prev;
    struct UIKObj   *Found;
    struct RastPort *RP;
    UWORD           Id;
    UBYTE           Dir;
    UBYTE           Flag;
    };

static void CrossFindFirstTank( struct UIKObj *obj, struct KeyTankParms *ktp )
{
    if (!ktp->Found && obj->rp == ktp->RP)
        {
        if ((obj->Id == ktp->Id) && !(obj->Flags & BOX_DISABLED))
            {
            ktp->Found = obj;
            return;
            }
        UIK_nl_CrossList( obj, (void*)CrossFindFirstTank, 0, (ULONG)ktp );
        }
}

static void CrossFindTank( struct UIKObj *obj, struct KeyTankParms *ktp )
{
    if (!ktp->Found && obj->rp == ktp->RP)
        {
        if (obj->Id == ktp->Id && !(obj->Flags & BOX_DISABLED))
            {
            if (! ktp->First) ktp->First = obj;

            if (ktp->Dir == -1)
                {
                if (obj == ktp->Selected)
                    {
                    ktp->Found = ktp->Prev;
                    return;
                    }
                }
            else{
                if (ktp->Prev == ktp->Selected)
                    {
                    ktp->Found = obj;
                    return;
                    }
                }

            ktp->Prev = obj;
            }
        UIK_nl_CrossList( obj, (void*)CrossFindTank, 0, (ULONG)ktp );
        }
}

static int cross_findvalidobj( struct UIKObj *obj, struct UIKObj *findobj )
{
    if (obj == findobj && obj->Status != UIKS_REMOVING) return(0); // trouvé, on arrête là
    return( UIK_nl_CrossList( obj, (void*)cross_findvalidobj, 1, (ULONG) findobj ) );
}

ULONG __asm ProcessEvents( register __a0 struct UIKObj *o, register __d0 ULONG EXTEVENT, register __d1 ULONG windowflag, register __a1 ULONG visit )
{
  register ULONG CONT=1;
  register struct UIKGlobal *uik = o->UIK;
  APTR tmp_remove = uik->RemoveVector;
  struct UIKObj *gaddown = 0;
  //struct IntuiMessage *SaveMsg = uik->IntuiMsg;

    if (!o || (!uik->WindowNum && !windowflag)) return(0);

//@@kprintf( "UIK: ++++++++++++++++++++++++++\n" );
    if (visit)
        {
        EXTEVENT = uik->CmdMask | uik->SigMask | uik->ActiveMask | EXTEVENT | uik->ExtMask; // tout fait sortir
        if (visit == 1) CONT = (EXTEVENT & SetSignal( 0, 0 ));  // passe dans la boucle si signaux reçus
//@@kprintf( "UIK: visit... CONT=%ld\n", CONT );
        }

    while (CONT && (uik->Obj.Status == UIKS_STARTED))
        {
        register ULONG events, mask;

//@@@kprintf( "UIK: going to Wait()\n" );
        events = Wait( uik->CmdMask | uik->SigMask | uik->ActiveMask | EXTEVENT | uik->ExtMask );
//@@@kprintf( "UIK: Events received=%lx\n", events );

        mask = events & (uik->ExtMask & ~uik->ActiveMask);
        if (mask)
            {
//@@kprintf( "UIK: executing UIK vectors on events %lx\n", mask );
            //----------------------
            if (! FalseCrossList( uik, mask, visit )) goto RETURN0; /* UIK stoppé */
            //----------------------
            }

        if (events & uik->ActiveMask)
            {
            UBYTE cont=1, *ptr, buts, changes;
            void (*tmp)();

            while (cont)
                {
                Disable();
                if (ptr = circbuf_get( uik )) { buts = *ptr++; changes = *ptr; }
                Enable();
                if (ptr)
                    {
                    // il faut laisser ca avant d'appeler InputVector() car InputVector()
                    // peut faire un flush qui va modifier uik->IptButs et uik->IptChgs.
                    uik->IptButs = buts;
                    uik->IptChgs = changes;

//@@@kprintf( "UIK: executing ZoneList handling\n" );
                    if (tmp = uik->InputVector) (*tmp)( uik, buts, changes ); /* cyclage */
                    //----------------------
//@@@kprintf( "UIK: executing Inputick Vector\n" );
                    if (! FalseCrossList( uik, uik->ActiveMask, visit )) goto RETURN0; /* UIK stoppé */
                    //----------------------
                    }
                else cont = 0;
                }
            }

        if (CONT && events & uik->SigMask)
            {
//@@kprintf( "UIK: signal received for Intuition messages List %lx\n", events & uik->SigMask );
            while (TRUE)
                {
                register struct IntuiMessage *msg;

                Disable();
                msg = (struct IntuiMessage *) RemHead( (struct List *) &uik->IMSList );
                Enable();
                if (msg)
                    {
                    UBYTE exelinkhook=0, wrefresh=0;
                    register struct Window *w = msg->IDCMPWindow;
                    register struct UIKObj *obj=0;
                    struct UIKObjWindow *wo = (struct UIKObjWindow *) w->UserData;
                    struct UIKObjMenu *mo = 0;

                    ULONG xy = *((ULONG*)&msg->MouseX);
                    UWORD code = msg->Code;
                    UWORD qual = msg->Qualifier;
                    ULONG class = msg->Class;
                    struct UIKObj *gadobj = 0;

//@@kprintf( "UIK: message from Intuition %lx, class=%lx, code=%lx, qual=%lx\n", msg, class, code, qual );
                    if (uik->IntuiMsg) ReplyIntuiMsg( uik->IntuiMsg );
                    uik->IntuiMsg = msg;

                  DOSHORTCUT:
                    switch (class)
                        {
                        case MOUSEMOVE:
                            if (! (obj = uik->ActiveObj))
                                obj = win_or_req( w );
                            break;
                        case MOUSEBUTTONS:
                            //ReportMouse( (code & IECODE_UP_PREFIX) ? 0 : -1, w );
                            obj = 0;
                            if (code & IECODE_UP_PREFIX)
                                if (gaddown)
                                    if (gaddown->Flags & BOX_EXTGADUP)
                                        { obj = gaddown; class = GADGETUP; }
                            if (obj == 0) obj = win_or_req( w );
                            gaddown = 0;
                            break;
                        case RAWKEY:
                            UIK_DeadKeyConvert( &uik->KeySupp, msg, 0 );

                            if (obj = uik->ActiveKeyTank)
                                {
                                if (obj->Id == UIKID_STRGAD)
                                    {
                                    if (! (code & IECODE_UP_PREFIX))
                                        {
                                        UBYTE next=0;
                                        struct KeyTankParms ktp;

                                        switch (code)
                                            {
                                            case 0x4c: next = -1; break; // haut
                                            case 0x4d: next = 1; break; // bas
                                            case 0x42: next = qual & (IEQUALIFIER_LSHIFT|IEQUALIFIER_RSHIFT) ? -1 : 1; break; // tab

                                            case 0x43: // enter
                                            case 0x44: // return
                                            case 0x45: // escape
                                                if (!(qual & IEQUALIFIER_REPEAT))
                                                    if (obj) UIK_InactivateKeyTank( obj );
                                                break;
                                            default:
                                                if (!(qual & IEQUALIFIER_REPEAT) && (uik->KeySupp.Length == 1))
                                                    {
                                                    UBYTE ch = uik->KeySupp.KeyBuffer[0];
                                                    APTR io, mem;
                                                    ULONG len=0;
                                                    switch (ch)
                                                        {
                                                        case 0x0d:
                                                            if (obj) UIK_InactivateKeyTank( obj );
                                                            break;
                                                        case 'c':
                                                            if (qual & IEQUALIFIER_RCOMMAND)
                                                                {
                                                                if (mem = UIK_OSt_Get( obj, &len ))
                                                                    {
                                                                    if (len && (io = func_CBWriteStart( 0 )))
                                                                        {
                                                                        len = func_CBWrite( io, mem, len );
                                                                        func_CBWriteEnd( io, len );
                                                                        }
                                                                    }
                                                                }
                                                            break;
                                                        case 'v':
                                                            if (qual & IEQUALIFIER_RCOMMAND)
                                                                {
                                                                if (io = func_CBReadStart( 0, &len ))
                                                                    {
                                                                    if (len && (mem = UIK_MemSpAlloc( len, MEMF_ANY )))
                                                                        {
                                                                        if ((len = func_CBRead( io, mem, len )) > 0)
                                                                            {
                                                                            UIK_OSt_Paste( obj, mem, ((struct UIKObjStrGad *)obj)->CursorPos, len );
                                                                            }
                                                                        UIK_MemSpFree( mem );
                                                                        }
                                                                    func_CBReadEnd( io );
                                                                    }
                                                                }
                                                            ;
                                                            break;
                                                        default: break;
                                                        }
                                                    }
                                                break;
                                            }

                                        if (next)
                                            {
                                            ktp.Parent = win_or_req( w );
                                            ktp.Selected = obj;
                                            ktp.First = 0;
                                            ktp.Prev = 0;
                                            ktp.Found = 0;
                                            ktp.RP = ktp.Parent->rp;
                                            ktp.Id = obj->Id;
                                            ktp.Dir = next;
                                            UIK_nl_CrossList( ktp.Parent, (void*)CrossFindTank, 0, (ULONG)&ktp );
                                            if (! ktp.Found) ktp.Found = (next == 1 ? ktp.First : ktp.Prev);
                                            if (ktp.Found)
                                                {
                                                UIK_ActivateKeyTank( ktp.Found );
                                                break;
                                                }
                                            }
                                        }
                                    }
                                }
                            else{
                                struct UIKList *list;
                                LONG line=-1, found=0, active=0;
                                struct UIKObj *o;
                                WORD squal;

                                obj = win_or_req( w );

                                if (code == 0x42) // TAB active la première zone
                                    {
                                    struct KeyTankParms ktp;
                                    ktp.Parent = obj;
                                    ktp.Selected = 0;
                                    ktp.First = 0;
                                    ktp.Prev = 0;
                                    ktp.Found = 0;
                                    ktp.Id = UIKID_STRGAD;
                                    ktp.RP = ktp.Parent->rp;
                                    UIK_nl_CrossList( ktp.Parent, (void*)CrossFindFirstTank, 0, (ULONG)&ktp );
                                    if (ktp.Found)
                                        {
                                        UIK_ActivateKeyTank( ktp.Found );
                                        break;
                                        }
                                    }

                                list = (obj->Id == UIKID_REQUEST) ? &((struct UIKObjReq *)obj)->ShortCutList : &((struct UIKObjWindow *)obj)->ShortCutList;
                                if (list->NodeNum)
                                    {
                                    msg->Qualifier &= ~(IEQUALIFIER_CONTROL|IEQUALIFIER_LALT|IEQUALIFIER_RALT|IEQUALIFIER_LCOMMAND|IEQUALIFIER_RCOMMAND);
                                    UIK_DeadKeyConvert( &uik->SCutSupp, msg, 0 );
                                    msg->Qualifier = qual;

                                    while (++line < list->NodeNum)
                                        {
                                        struct UIKHook *hook = (struct UIKHook *) list->UNode[line].Buf;
                                        UBYTE ObjSCKey, ObjSCCode, ch = UIK_ToUpper( uik->SCutSupp.KeyBuffer[0] );
                                        UWORD ObjSCQual;

                                        o = hook->Obj;
                                        if (o->Id == UIKID_BOX) continue;
                                        if (o->Status != UIKS_STARTED) continue;
                                        if (o->Flags & BOX_DISABLED) continue;

                                        //--- Bufferize pour transformer en cas de Amiga+B ou V
                                        ObjSCKey  = o->ObjExt->SCKey;
                                        ObjSCCode = o->ObjExt->SCCode;
                                        ObjSCQual = o->ObjExt->SCQual;

                                        //--- Transforme pour OK ou Cancel
                                        if (qual & IEQUALIFIER_LCOMMAND) /* ne pas filtrer les autres pour permettre d'annuler avec un qualifier */
                                            {
                                            if (uik->SCutSupp.Length == 1)
                                                {
                                                if ((o->Id == UIKID_OKGAD && ch == 'V') || (o->Id == UIKID_CANCELGAD && ch == 'B'))
                                                    {
                                                    ObjSCKey = ch; ObjSCCode = 0; ObjSCQual = IEQUALIFIER_LCOMMAND;
                                                    }
                                                }
                                            }

                                        //--- Vérifie la touche
                                        if (ObjSCCode)
                                            {
                                            if ((code & 0x7f) != ObjSCCode) continue;
                                            }
                                        else{
                                            if ((uik->SCutSupp.Length != 1) || (ch != ObjSCKey)) continue;
                                            }

                                        squal = qual;
                                        if (squal & IEQUALIFIER_LALT || squal & IEQUALIFIER_RALT) squal |= IEQUALIFIER_LALT|IEQUALIFIER_RALT;
                                        squal &= (/*IEQUALIFIER_LSHIFT|IEQUALIFIER_RSHIFT |*/
                                                IEQUALIFIER_LALT|IEQUALIFIER_RALT
                                                | IEQUALIFIER_CONTROL
                                                | IEQUALIFIER_LCOMMAND|IEQUALIFIER_RCOMMAND );
                                        if (squal == ObjSCQual)
                                            {
                                            if ((code & IECODE_UP_PREFIX) && (!hook->Parm)) continue;
                                            if (hook->Obj->Id != UIKID_STRGAD) // car ActiveKeyTank est activé
                                                hook->Parm = (code & IECODE_UP_PREFIX) ? 0 : 1;
                                            found = 1;
                                            active = 1;
                                            break;
                                            }
                                        else if (hook->Parm && (code & IECODE_UP_PREFIX) /*&& gadobj == hook->Obj*/)
                                            {
                                            found = 1;
                                            //active = 0;
                                            break;
                                            }
                                        }

                                    if (found)
                                        {
                                        if (! (qual & IEQUALIFIER_REPEAT))
                                            {
                                            ULONG scl = (code & IECODE_UP_PREFIX) ? GADGETUP : GADGETDOWN;
                                            if (active)
                                                {
                                                class = scl; code = 0; qual = 0;
                                                gadobj = o;
                                                }
                                            if (scl == GADGETDOWN) gadobj = uik->ActiveObj = o;
                                            else uik->ActiveObj = 0;
                                            if (o->SelectVector && !(o->Flags & BOX_NOACT))
                                                {
                                                APTR tmp = uik->RemoveVector;
                                                uik->RemoveVector = FALSE_Remove; //----------------------
                                                (*o->SelectVector)( o, scl );
                                                uik->RemoveVector = tmp; //----------------------
                                                }
                                            if (active) goto DOSHORTCUT;
                                            }
                                        else if (active)
                                            {
                                            class = MOUSEMOVE; code = 0; qual = 0;
                                            gadobj = o;
                                            if (o->SelectVector && !(o->Flags & BOX_NOACT))
                                                {
                                                APTR tmp = uik->RemoveVector;
                                                uik->RemoveVector = FALSE_Remove; //----------------------
                                                (*o->SelectVector)( o, class );
                                                uik->RemoveVector = tmp; //----------------------
                                                }
                                            goto DOSHORTCUT;
                                            }
                                        }
                                    }
                                }
                            break;

                        case INTUITICKS:
                            if (obj = uik->ActiveObj) break;
                            obj = win_or_req( w ); //(struct UIKObj *) (w->FirstRequest ? w->FirstRequest->ImageBMap : w->UserData);
                            break;

                        case ACTIVEWINDOW:
                            obj = win_or_req( w ); //(struct UIKObj *) (w->FirstRequest ? w->FirstRequest->ImageBMap : w->UserData);
                            if (obj->ObjExt->ZoneList)
                                {
                                zo_InstallZoneList( uik, obj->ObjExt->ZoneList );
                                }
                            //zo_InstallZoneList( uik, 0 );
                            break;
                        case INACTIVEWINDOW:
                            obj = win_or_req( w ); //(struct UIKObj *) (w->FirstRequest ? w->FirstRequest->ImageBMap : w->UserData);
                            /* une zone éventuelle est restituée avant par le handler */
                            break;

                        case REQSET: case REQCLEAR:
                            obj = win_or_req( w ); //(struct UIKObj *) (w->FirstRequest ? w->FirstRequest->ImageBMap : w->UserData);

                            if (class == REQSET)
                                {
                                UWORD l = w->FirstRequest->LeftEdge+10+w->BorderRight, t = w->FirstRequest->TopEdge+10+w->BorderBottom;
                                WindowLimits( w, (l>w->MinWidth)?l:0, (t>w->MinHeight)?t:0, 0, 0 );
                                }
                            else
                                WindowLimits( w, ((struct UIKObjWindow*)obj)->NW.MinWidth, ((struct UIKObjWindow*)obj)->NW.MinHeight, 0, 0 );

                            if (obj->ObjExt->ZoneList)
                                {
                                zo_InstallZoneList( uik, obj->ObjExt->ZoneList );
                                }
                            break;

                        case DISKINSERTED: case DISKREMOVED:
                            obj = win_or_req( w ); //(struct UIKObj *) (w->FirstRequest ? w->FirstRequest->ImageBMap : w->UserData);
                            break;

                        case GADGETDOWN:
                            obj = gadobj ? gadobj : (struct UIKObj *) (((struct Gadget *)msg->IAddress)->UserData);
                            gaddown = obj;
                            break;
                        case GADGETUP:
                            obj = gadobj ? gadobj : (struct UIKObj *) (((struct Gadget *)msg->IAddress)->UserData);
                            gaddown = 0;
                            break;

                        case IDCMP_MENUPICK:
                        case IDCMP_MENUVERIFY:
                        case IDCMP_MENUHELP:   // pour celui-la il faudrait WA_MenuHelp à la fenêtre...
                            obj = mo = wo->WMenu;
                            break;
                        case SIZEVERIFY:
                            if (w->FirstGadget)
                                {
                                struct Gadget *gad = w->FirstGadget;
                                while (gad && (gad->GadgetType & GTYP_SYSGADGET)) gad = gad->NextGadget;
                                if (gad)
                                    { wo->GadgetList = gad; RemoveGList( w, gad, -1 ); }
                                else wo->GadgetList = 0;
                                }
                            obj = wo;
                            break;

                        case NEWSIZE:
                            wrefresh = ResizeWindowObjects( wo, w );
                            if (/*!w->FirstGadget &&*/ wo->GadgetList)
                                {
                                AddGList( w, wo->GadgetList, (UWORD)~0, -1, 0 );
                                wo->GadgetList = 0;
                                }
                            if (wrefresh) obj = wo;
                            break;

                        case CLOSEWINDOW:
                            obj = (struct UIKObj *) w->UserData;
                            if (((struct UIKObjWindow*)obj)->WinFlags & (UIKW_CLOSESTOP|UIKW_CLOSEREMOVE))
                                {
                                CrossMark( obj, UIKS_STOPPING );
                                exelinkhook = 1;
                                }
                            break;

                        case IDCMP_REFRESHWINDOW:
                        case IDCMP_CHANGEWINDOW:
                        case IDCMP_IDCMPUPDATE:
                        case IDCMP_GADGETHELP:
                        default:
                            obj = wo; //(struct UIKObj *) w->UserData;
                            break;
                        }

                    {
                    APTR tmp = uik->RemoveVector;
                    uik->RemoveVector = FALSE_Remove; //----------------------

                    if (!uik->FilterIE || (*uik->FilterIE)( obj, class, qual, code ))
                        {
                        if (mo && !(mo->Obj.Flags & BOX_DISABLED))
                            {
                            if (mo->MenuFunc)
                                (*mo->MenuFunc)( mo, wo, class, code );
                            }

                        if ((obj) && (obj->UIK == uik) && (obj->Status == UIKS_STARTED) && !(obj->Flags & BOX_DISABLED))
                            {
                            exelinkhook = 1;
                            }
                        if (exelinkhook)
                            {
                            obj->MouseIn = (obj == gadobj) ? 1 : UIK_InsideObj( obj, xy );
                            *((ULONG*)&obj->MouseX) = xy;
                            if (obj->Link.NodeNum) UIK_nl_CrossList( &obj->Link, ExecuteLink, 1, class );
                            if (obj->Hook.NodeNum) UIK_nl_CrossList( &obj->Hook, (void*)ExecuteClass, 0, class );
                            }
                        }

                    if (uik->IntuiMsg) { ReplyIntuiMsg( uik->IntuiMsg ); uik->IntuiMsg = 0; }

                    if (wrefresh)
                        {
                        UIK_RefreshWindow( wo ); wrefresh = 0;
                        }

                    if (uik->RemObj) // peut être appelé plusieurs fois si visit != 0
                        {
                        CrossTrueStop( uik );
                        if (uik->Obj.Status != UIKS_STARTED) goto RETURN0; /* UIK stoppé */

                        if (class == CLOSEWINDOW && obj->Status != UIKS_REMOVING)
                            if (((struct UIKObjWindow*)obj)->WinFlags & UIKW_CLOSEREMOVE)
                                CrossMark( obj, UIKS_REMOVING );
                        if (CrossTrueRemove( uik ) == 0) goto RETURN0;  /* UIK arrêté */
                        }
                    uik->RemoveVector = tmp; //----------------------
                    }
                    }
                else{
                    break;
                    }
                }
            }

        if (events & uik->CmdMask)
            {
            struct IORequest *req;
            struct UIKObjExtension *ext;

//@@kprintf( "UIK: External command received\n" );
            while (req = (struct IORequest *) GetMsg( &uik->CmdPort ))
                {
                APTR tmp = uik->RemoveVector;
                uik->RemoveVector = FALSE_Remove; //----------------------

                if (req->io_Command & 0x8000)                   //--- demande de permission
                    {
                    do_process_warn( req );
                    }
                else{
                    ULONG ok=1;
                    if (req->io_Command == 0x00F6) // AddObject
                        {
                        if (cross_findvalidobj( uik, (struct UIKObj *) ((struct UIKArgRegs *)req->io_Device)->D0 ))
                            {
                            req->io_Unit = 0;
                            ok = 0;
                            }
                        }
                    if (ok && (req->io_Unit = do_process_cmd( req ))) //--- demande d'exécution
                        {
                        if (req->io_Command == 0x00F6) // AddObject
                            {
                            ext = ((struct UIKObj *)req->io_Unit)->ObjExt;
                            ext->IPMother = req->io_Message.mn_ReplyPort->mp_SigTask;
                            }
                        }
                    }

                uik->RemoveVector = tmp; //----------------------
                ReplyMsg( req );
                }
            }

        if (uik->RemObj /*&& (visit == 0)*/)
            {
//@@kprintf( "UIK: removing objects really (2)\n" );
            uik->RemObj = 0;
            if (CrossTrueRemove( uik ) == 0) goto RETURN0;  /* UIK arrêté */
            if (uik->Obj.Status != UIKS_STARTED) goto RETURN0; /* UIK stoppé */
            }
        if (!uik->WindowNum && !windowflag) CONT=0;

        if (events & EXTEVENT)
            {
            uik->RemoveVector = tmp_remove;
//@@kprintf( "UIK: ------------------------EXTEVENT visit=%ld\n", visit );
            return( events & EXTEVENT );
            }
        }
    RETURN0:
    uik->RemoveVector = tmp_remove;
//@@kprintf( "UIK: ------------------------RETURN0 visit=%ld\n", visit );
    return(0);
}

ULONG __asm UIK_Do( register __a0 struct UIKObj *o, register __d0 ULONG EXTEVENT )
{
    return( ProcessEvents( o, EXTEVENT, 0, 0 ) );
}

ULONG __asm UIK_Do2( register __a0 struct UIKObj *o, register __d0 ULONG EXTEVENT, register __d1 ULONG windowflag )
{
    return( ProcessEvents( o, EXTEVENT, windowflag, 0 ) );
}

ULONG __asm UIK_ProcessPendingEvents( register __a0 struct UIKObj *o, register __d0 ULONG EXTEVENT )
{
    return( ProcessEvents( o, EXTEVENT, 1, 1 ) ); // passe dans la boucle seulement si sig reçus
}

ULONG __asm UIK_WaitFirstEvents( register __a0 struct UIKObj *o, register __d0 ULONG EXTEVENT )
{
    return( ProcessEvents( o, EXTEVENT, 1, 2 ) ); // passe dans la boucle toujours
}
