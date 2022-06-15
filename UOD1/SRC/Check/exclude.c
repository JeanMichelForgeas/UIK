/****************************************************************
 *
 *      Project:   UIK
 *      Function:  objet RadioExclude
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

#include "uiki:objradioexclude.h"


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


/****** Statics *************************************************/

struct TagItem __far boolmap[] =
    {
    { UIKTAG_OBJRadioFl_Check,  UIKF_RADIO_CHECK },
    { TAG_END }
    };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/


/*------- Object entries ----------------------------------------*/

static int AddRadio( struct UIKObjRadioExclude *cobj, struct TagItem *taglist)
{
    SysBase = (struct ExecBase *) *((ULONG*)4);
    if (! (cobj->GfxBase = GfxBase = (struct GfxBase *) OpenLibrary( "graphics.library", 0 ))) goto END_ERROR;
    if (! (cobj->UIKBase = UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;

    UIK_InvertBlackWhite( cobj->Obj.UIK, UIKBitMapTable, 1 );

    cobj->Flags = UIK_GetTagData( UIKTAG_OBJRadio_Flags, 0, taglist );
    cobj->Flags = UIK_PackBoolTags( cobj->Flags, taglist, boolmap );

    cobj->Image1     = (struct UIKBitMap *) UIK_GetTagData( UIKTAG_OBJRadio_Image1,    (ULONG)&UBM_PicImage1,    taglist );
    cobj->AltImage1  = (struct UIKBitMap *) UIK_GetTagData( UIKTAG_OBJRadio_AltImage1, (ULONG)&UBM_PicAltImage1, taglist );
    cobj->Image2     = (struct UIKBitMap *) UIK_GetTagData( UIKTAG_OBJRadio_Image2,    (ULONG)&UBM_PicImage2,    taglist );
    cobj->AltImage2  = (struct UIKBitMap *) UIK_GetTagData( UIKTAG_OBJRadio_AltImage2, (ULONG)&UBM_PicAltImage2, taglist );

    if (cobj->Flags & UIKF_RADIO_CHECK)
        {
        cobj->Image1     = 0;
        cobj->AltImage1  = &UBM_PicCheck;
        cobj->Image2     = cobj->Image1;
        cobj->AltImage2  = cobj->AltImage1;
        }
    else{
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

static void StopRadio( struct UIKObjRadioExclude *cobj )
{
    UIK_StdGadStop( cobj );
}

static void RemoveRadio( struct UIKObjRadioExclude *cobj )
{
    if (cobj->UIKBase) CloseLibrary( cobj->UIKBase );
    if (cobj->GfxBase) CloseLibrary( cobj->GfxBase );
}

static void RefreshImage( struct UIKObjRadioExclude *cobj )
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

static void RefreshRadio( struct UIKObjRadioExclude *cobj )
{
    UIK_StdGadRefresh( cobj );
    RefreshImage( cobj );
}

static void ResizeRadio( struct UIKObjRadioExclude *cobj )
{
   UIK_StdGadResize( cobj );
}

static int CrossMExUnsel( struct UIKObj *obj, struct UIKObj *sel )
{
    if ((obj->Id == UIKID_RADIOEXCLUDE) && (obj != sel) && !(obj->Flags & BOX_OVER))
        {
        UIK_StdGadInact( (struct UIKObjGad *) obj );
        RefreshImage( (struct UIKObjRadioExclude *) obj );
        return(0); /* stop scan */
        }
    return(1); /* continue scan */
}

static int Hdl_ActiveMExObj( struct UIKObjRadioExclude *cobj )
{
    if (cobj)
        {
        UIK_LockObjVars( cobj );
        UIK_nl_CrossList( cobj->Obj.Parent, CrossMExUnsel, 1, (ULONG)cobj );
        UIK_StdGadAct( cobj );
        RefreshImage( cobj );
        UIK_UnlockObjVars( cobj );
        }
    return(1);
}

static int StartRadio( struct UIKObjRadioExclude *cobj )
{
    cobj->Obj.ExceptActVector = (void*) Hdl_ActiveMExObj;
    UIK_StdGadStart( cobj );
    return(1);
}

/*------ Objects functions --------------------------------------*/

void SelectFunc( struct UIKObjRadioExclude *cobj )
{
    Hdl_ActiveMExObj( cobj );
}

static ULONG __far FuncTab[] =
{
   (ULONG) SelectFunc,
};

/*------ Germ ---------------------------------------------------*/

static struct TagItem __far GermTagList[] =
{
   { UIKTAG_GEN_VersionRevision, W2L(1,3) },
   { UIKTAG_GEN_LastFuncId,     UIKFUNC_Radio_LastCmd },
   { UIKTAG_GEN_ActInactPens,   UIKCOLS(UIKCOL_GREY,UIKCOL_GREY,UIKCOL_WHITE,UIKCOL_BLACK) },
   { TAG_END }
};

struct UIKObjGerm __far Germ = {
    "UIKObj_RadioExclude",
    AddRadio,           /*int     (*AddVector)(); */
    StartRadio,         /*int     (*StartVector)(); */
    StopRadio,          /*void    (*StopVector)(); */
    RemoveRadio,        /*void    (*RemoveVector)(); */
    RefreshRadio,       /*void    (*RefreshVector)(); */
    ResizeRadio,        /*void    (*ResizeVector)(); */
    (void*)Hdl_ActiveMExObj,   /*void    (*ExceptActVector)(); */
    NULL,               /*void    (*ExceptInaVector)(); */
    NULL,               /*void    (*ExceptMovVector)(); */
    NULL,               /*void    (*ExceptKeyVector)(); */
    NULL,               /*int     (*StartAfterVector)(); */
    Hdl_ActiveMExObj,   /*int     (*SelectVector)(); */
    FuncTab,            /*ULONG   *ExtentedVectors; */
    GermTagList,        /*struct TagItem *GermTagList; */
    8,                  /*UWORD   MinWidth; */
    4,                  /*UWORD   MinHeight; */
    SETL(OBJRADIO_BOX_STDWIDTH,OBJRADIO_BOX_STDHEIGHT), /*ULONG DefaultWidthHeight; */
    UIKID_RADIOEXCLUDE, /*UWORD   ObjId; */
    sizeof(struct UIKObjRadioExclude),   /*UWORD   ObjSize; */
    BOX_DISPLAYABLE|BOX_INNERZONE | BOX_NOPUSH | BOX_OVER | BOX_FILLED | BOX_SEMAPHORE | BOX_TRANSLATELEFTTOP | BOX_ZONEABLE,
    };
