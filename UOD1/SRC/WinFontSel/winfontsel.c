/****************************************************************
 *
 *      Project:   UIK
 *      Function:  objet WinFontSel
 *
 *      Created:   27/03/93     Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikbase.h"
#include "uiki:uikglobal.h"
#include "uiki:uikobj.h"
#include "uiki:uikmacros.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objwinfontsel.h"


/****** Macros **************************************************/


/****** Imported ************************************************/



/****** Exported ************************************************/

ULONG __far SysBase;
struct UIKBase * __far UIKBase;
struct DOSBase * __far DOSBase;


/****** Statics *************************************************/

static struct TagItem __far TagBoolMap[] =
    {
    { UIKTAG_WFOSFl_PropWidth,  UIKF_WFOS_WPROP },
    { UIKTAG_WFOSFl_FixedWidth, UIKF_WFOS_WFIXED },
    { TAG_DONE }
    };

static struct WinFontSelPrefs __far DefaultPrefs =
    {
    { 100, 20, 300, 120 },
    };


/****************************************************************
 *
 *      Utility
 *
 ****************************************************************/

void read_env( struct UIKObjWinFontSel *wfs )
{
  ULONG file, old;

    wfs->Prefs = DefaultPrefs;

    old = UIK_DisableDOSReq();
    if (file = Open( "ENV:UIK/Objects/ObjWinFontSel.prefs", MODE_OLDFILE ))
        {
        Read( file, &wfs->Prefs, sizeof(struct WinFontSelPrefs) );
        Close( file );
        }
    UIK_EnableDOSReq( 0, old );

    wfs->OldPrefs = wfs->Prefs;
}

//--------------------------------------------------------------

static ULONG lo_GetFIB( UBYTE *name, struct FilrInfoBlock *fib )
{
  ULONG lock, rc=0;

    if (lock = Lock( name, ACCESS_READ ))
        {
        rc = Examine( lock, fib );
        UnLock( lock );
        }
    return( rc );
}

static void lo_DirCreate( UBYTE *name, struct FileInfoBlock *fib )
{
  ULONG lock, ok=0, old;

    old = UIK_DisableDOSReq();
    if (lo_GetFIB( name, fib ))
        {
        if (fib->fib_DirEntryType > 0)
            ok = 1;  /* it is a directory */
        }
    if (! ok)
        {
        if (lock = CreateDir( name ))
            UnLock( lock );
        }
    UIK_EnableDOSReq( 0, old );
}

static void save_envfile( struct UIKObjWinFontSel *wfs, UBYTE *name )
{
  ULONG file;

    if (file = Open( name, MODE_NEWFILE ))
        {
        Write( file, &wfs->Prefs.Box, sizeof(struct WinFontSelPrefs) );
        Close( file );
        }
}

static void save_env( struct UIKObjWinFontSel *wfs )
{
  ULONG old, i, dif=0;
  UBYTE *p1, *p2;

    //-- if (wfs->OldPrefs == wfs->Prefs) return;
    for (i=0, p1=(UBYTE*)&wfs->Prefs, p2=(UBYTE*)&wfs->OldPrefs; i < sizeof(struct WinFontSelPrefs); i++)
        {
        if (*p1++ != *p2++) { dif = 1; break; }
        }
    if (dif == 0) return;

    //-- save
    old = UIK_DisableDOSReq();
    save_envfile( wfs, "ENV:UIK/Objects/ObjWinFontSel.prefs" );
    save_envfile( wfs, "ENVARC:UIK/Objects/ObjWinFontSel.prefs" );
    UIK_EnableDOSReq( 0, old );
}

/****************************************************************
 *
 *      Callbacks
 *
 ****************************************************************/

static void OKFunc( struct UIKObj *fso, struct TextFont *font, UBYTE style )
{
  struct UIKObjWinFontSel *wfs = (struct UIKObjWinFontSel *) fso->Parent->Parent;

    if (wfs->okfunc) (*wfs->okfunc)( wfs, font, style );
}

static void CancelFunc( struct UIKObj *fso )
{
  struct UIKObjWinFontSel *wfs = (struct UIKObjWinFontSel *) fso->Parent->Parent;

    if (wfs->cancelfunc) (*wfs->cancelfunc)( wfs );
}


/****************************************************************
 *
 *      Methodes
 *
 ****************************************************************/

static void close_window( struct UIKObjWindow *wo, struct UIKObjWinFontSel *wfs )
{
    if (wfs->cancelfunc) (*wfs->cancelfunc)( wfs );
    if (wfs->Obj.Status == UIKS_STARTED) UIK_Stop( wfs );
}

static void update_size( struct UIKObjWindow *wo, struct UIKObjWinFontSel *wfs )
{
  struct Window *w = wo->Obj.w;

    wfs->Prefs.Box.Left   = w->LeftEdge;
    wfs->Prefs.Box.Top    = w->TopEdge;
    wfs->Prefs.Box.Width  = w->Width;
    wfs->Prefs.Box.Height = w->Height;
    if (wo->WinFlags & UIKW_AUTOADJUSTSIZE)
        {
        wfs->Prefs.Box.Width  -= (wo->LeftBorderWidth + wo->RightBorderWidth);
        wfs->Prefs.Box.Height -= (wo->TopBorderHeight + wo->BottomBorderHeight);
        }
}

/****************************************************************
 *
 *      Init/Cleanup
 *
 ****************************************************************/

static void GenericRemove( struct UIKObjWinFontSel *wfs )
{
    UIK_MemSpFree( wfs->FontPath );

    lo_DirCreate( "ENV:UIK", &wfs->FIB );
    lo_DirCreate( "ENV:UIK/Objects", &wfs->FIB );
    lo_DirCreate( "ENVARC:UIK", &wfs->FIB );
    lo_DirCreate( "ENVARC:UIK/Objects", &wfs->FIB );
    save_env( wfs );

    if (wfs->DOSBase) CloseLibrary( wfs->DOSBase );
    if (wfs->UIKBase) CloseLibrary( wfs->UIKBase );
}

static int InstallWindow( struct UIKObjWinFontSel *wfs )
{
    if (! (wfs->WO = (struct UIKObjWindow *) UIK_AddObjectTags( "UIKObj_Window", wfs,
            UIKTAG_OBJ_LeftTop,         W2L(wfs->Prefs.Box.Left,wfs->Prefs.Box.Top),
            UIKTAG_OBJ_WidthHeight,     W2L(wfs->Prefs.Box.Width,wfs->Prefs.Box.Height),
            UIKTAG_OBJWindow_MinimumWidthHeight, W2L(WINFOS_STDWIDTH,WINFOS_STDHEIGHT),
            UIKTAG_OBJ_LangMaster,      wfs->Obj.ObjExt->Lang->Master,
            UIKTAG_OBJ_Title,           wfs->Obj.Title,
            UIKTAG_OBJ_AltTitle,        wfs->Obj.AltTitle,
            UIKTAG_OBJ_Title_Flags,     wfs->Obj.TitleFlags,
            TAG_DONE ))) goto END_ERROR;
    if (! UIK_AddVector( wfs->WO, (void*)update_size, IDCMP_NEWSIZE|IDCMP_CHANGEWINDOW, (ULONG)wfs )) goto END_ERROR;
    if (! UIK_AddVector( wfs->WO, (void*)close_window, IDCMP_CLOSEWINDOW, (ULONG)wfs )) goto END_ERROR;

    if (! (wfs->FS = (struct UIKObjFontSelector *) UIK_AddObjectTags( "UIKObj_FontSelector", wfs->WO,
            UIKTAG_OBJ_LeftTop,         W2L(6,3),
            UIKTAG_OBJ_WidthHeight,     W2L(wfs->Prefs.Box.Width-12,wfs->Prefs.Box.Height-6),
            UIKTAG_OBJ_LangMaster,      wfs->Obj.ObjExt->Lang->Master,
            UIKTAG_FOS_OKFUNC,          wfs->okfunc ? OKFunc : 0,
            UIKTAG_FOS_CANCELFUNC,      wfs->cancelfunc ? CancelFunc : 0,
            UIKTAG_OBJ_Sz_Attached_Flags, UO_ATTACHED_TOP|UO_ATTACHED_BOT|UO_ATTACHED_LEFT|UO_ATTACHED_RIGHT,
            UIKTAG_FOS_FONTSPATH,       wfs->FontPath,
            UIKTAG_FOS_Flags,           wfs->Flags,
            UIKTAG_FOS_HighlightFont,   wfs->HighLightTTA.tta_Name ? &wfs->HighLightTTA : 0,
            TAG_DONE ))) goto END_ERROR;

    return(1);

  END_ERROR:
    return(0);
}

static int GenericAdd( struct UIKObjWinFontSel *wfs, struct TagItem *taglist )
{
  struct TTextAttr *tta;
  UBYTE *ptr;

    SysBase = (*(ULONG*)4);
    if (! (wfs->UIKBase = UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (wfs->DOSBase = DOSBase = (struct DOSBase *) OpenLibrary( DOSNAME, 0 ))) goto END_ERROR;

    read_env( wfs );

    wfs->Flags = UIK_GetTagData( UIKTAG_WFOS_Flags, (UIKF_FOS_WPROP|UIKF_FOS_WFIXED), taglist );
    wfs->Flags = (ULONG) UIK_PackBoolTags( (ULONG)wfs->Flags, taglist, TagBoolMap );

    wfs->okfunc = (void*) UIK_GetTagData( UIKTAG_WFOS_OKFUNC, 0, taglist );
    wfs->cancelfunc = (void*) UIK_GetTagData( UIKTAG_WFOS_CANCELFUNC, 0, taglist );

    ptr = (UBYTE*) UIK_GetTagData( UIKTAG_WFOS_FONTSPATH, (ULONG)"FONTS:", taglist );
    if (! (wfs->FontPath = UIK_MemSpAlloc( 1+StrLen(ptr), MEMF_ANY ))) goto END_ERROR;
    StrCpy( wfs->FontPath, ptr );

    if (tta = (struct TTextAttr *) UIK_GetTagData( UIKTAG_WFOS_HighlightFont, 0, taglist ))
        {
        wfs->HighLightTTA = *tta;
        }
    else wfs->HighLightTTA.tta_Name = 0;

    if (! InstallWindow( wfs )) goto END_ERROR;

    return(1);

  END_ERROR:
    return(0);
}

static int GenericStart( struct UIKObjWinFontSel *wfs )
{
    // faire un UIK_Start() soi-même au lieu de laisser faire celui en cours
    // permet d'agir si la routine de Start de la fenêtre ne marche pas, et de
    // la redimensionner afin qu'elle puisse tenir dans un écran plus petit.

    if (! UIK_Start( wfs->WO ))
        {
        UIK_Remove( wfs->WO );
        wfs->Prefs = DefaultPrefs;
        if (! InstallWindow( wfs )) goto END_ERROR;
        if (! UIK_Start( wfs->WO )) goto END_ERROR;
        }
    return(1);

  END_ERROR:
    UIK_Remove( wfs->WO );
    wfs->WO = 0;
    return(0);
}

static struct TagItem __far GermTagList[] =
{
   { UIKTAG_GEN_VersionRevision, W2L(1,5) },
   { TAG_END }
};

struct UIKObjGerm __far Germ = {
    "UIKObj_WinFontSel",
    GenericAdd,         /*int     (*AddVector)(); */
    GenericStart,       /*int     (*StartVector)(); */
    NULL,               /*void    (*StopVector)(); */
    GenericRemove,      /*void    (*RemoveVector)(); */
    NULL,               /*void    (*RefreshVector)(); */
    NULL,               /*void    (*ResizeVector)(); */
    NULL,               /*void    (*ExceptActVector)(); */
    NULL,               /*void    (*ExceptInaVector)(); */
    NULL,               /*void    (*ExceptMovVector)(); */
    NULL,               /*void    (*ExceptKeyVector)(); */
    NULL,               /*int     (*StartAfterVector)(); */
    NULL,               /*int     (*SelectVector)(); */
    NULL,               /*ULONG   *ExtentedVectors; */
    GermTagList,        /*struct TagItem *GermTagList; */
    WINFOS_STDWIDTH,    /*UWORD   MinWidth; */
    WINFOS_STDHEIGHT,   /*UWORD   MinHeight; */
    W2L(WINFOS_STDWIDTH,WINFOS_STDHEIGHT), /*ULONG   DefaultWidthHeight; */
    UIKID_WINFONTSEL,   /*UWORD   ObjId; */
    sizeof(struct UIKObjWinFontSel),   /*UWORD   ObjSize; */
    BOX_DISPLAYABLE, /* flags par defaut */
    };
