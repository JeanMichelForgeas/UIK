/****************************************************************
 *
 *      Project:   UIK
 *      Function:  objet WinFileSel
 *
 *      Created:   25/02/93     Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikbase.h"
#include "uiki:uikglobal.h"
#include "uiki:uikobj.h"
#include "uiki:uikmacros.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objwinfilesel.h"


/****** Macros **************************************************/


/****** Imported ************************************************/

extern ULONG my_StrToLong( UBYTE *, ULONG * );


/****** Exported ************************************************/

ULONG __far SysBase;
struct UIKBase * __far UIKBase;
struct DOSBase * __far DOSBase;


/****** Statics *************************************************/

static struct TagItem __far TagBoolMap[] =
    {
    { UIKTAG_WFS_AssignLst,         UIK_WFS_ASSIGNLIST },
    { UIKTAG_WFSFl_ReturnReturn,    UIK_WFS_RETURNRETURN },
    { UIKTAG_WFSFl_OK_IfNotExists,  UIK_WFS_OK_IFNOTEXISTS },
    { UIKTAG_WFSFl_DirsOnly,        UIK_WFS_DIRSONLY },
    { UIKTAG_WFSFl_ShowIcons,       UIK_WFS_SHOWICONS },
    { UIKTAG_WFSFl_StringsTop,      UIK_WFS_STRINGSTOP },
    { UIKTAG_WFSFl_LeavePath,       UIK_WFS_LEAVEPATH },
    { TAG_DONE }
    };

static struct WinFileSelPrefs __far DefaultPrefs =
    {
    { 100, 20, 300, 120 },
    W2L(FS_DEVLIST_MINWIDTH,FS_DIRLIST_MINWIDTH),
    0,  // StringsTop
    };


/****************************************************************
 *
 *      Utility
 *
 ****************************************************************/

//static void verif_stringtop( struct UIKObjWinFileSel *wfs, UBYTE *str )
//{
//    if (wfs->Prefs.StringsTop)
//        kprintf( "!!! WinFileSel->StringTop = %ld !!! --> %ls\n", wfs->Prefs.StringsTop, str );
//}

void read_env( struct UIKObjWinFileSel *wfs )
{
  struct WinFileSelPrefs *p = &wfs->Prefs;
  ULONG file, old, len, val, val1, val2;
  UBYTE *buf=0, *ptr;
  LONG num;

    wfs->Prefs = DefaultPrefs;
    if (wfs->Flags & UIK_WFS_STRINGSTOP) p->StringsTop = 1;
//verif_stringtop( wfs, "read_env 1" );

    old = UIK_DisableDOSReq();
    if (file = Open( "ENV:UIK/Objects/ObjWinFileSel.prefs", MODE_OLDFILE ))
        {
        len = Seek( file, Seek( file, 0, OFFSET_END ), OFFSET_BEGINNING );
        if (ptr = buf = UIK_MemSpAlloc( len+2, MEMF_ANY|MEMF_CLEAR))
            {
//verif_stringtop( wfs, "read_env 2" );
            Read( file, buf, len );
            if (len == 28 && (buf[0] & 0xf0) == 0)
                {
                CopyMem( buf, p, 28 ); // compatibilité -> conversion nouveau format par save_envfile
                }
            else{
//verif_stringtop( wfs, "read_env 3" );
                if ((num = my_StrToLong( ptr, &val )) > 0)
                    {
                    p->Box.Left = val;
                    if ((num = my_StrToLong( ptr+=num, &val )) > 0)
                        {
                        p->Box.Top = val;
                        if ((num = my_StrToLong( ptr+=num, &val )) > 0)
                            {
                            p->Box.Width = val;
                            if ((num = my_StrToLong( ptr+=num, &val )) > 0)
                                {
                                p->Box.Height = val;
//verif_stringtop( wfs, "read_env 4" );
                                if ((num = my_StrToLong( ptr+=num, &val1 )) > 0)
                                    {
                                    if ((num = my_StrToLong( ptr+=num, &val2 )) > 0)
                                        {
                                        p->DevFileWidth = (val1<<16)|val2;
//ptr += num;
//if (*(ptr+1) != '0') kprintf( "read_env 5 : buf=%lx, ptr=%lx, *ptr=%lx\n", buf, ptr+1, *(ptr+1) );
//ptr -= num;
                                        if ((num = my_StrToLong( ptr+=num, &val )) > 0)
                                            {
//if (val != 0) kprintf( "read_env 6 : val=%ld\n", val );
                                            p->StringsTop = val;
//verif_stringtop( wfs, "read_env 7" );
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            UIK_MemSpFree( buf );
            }
        Close( file );
        if (p->StringsTop) wfs->Flags |= UIK_WFS_STRINGSTOP; else wfs->Flags &= ~UIK_WFS_STRINGSTOP;
        }
    UIK_EnableDOSReq( 0, old );

    wfs->OldPrefs = wfs->Prefs;
//verif_stringtop( wfs, "read_env 8" );
}

//--------------------------------------------------------------

static ULONG lo_GetFIB( UBYTE *name, struct FileInfoBlock *fib )
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

static void save_envfile( struct UIKObjWinFileSel *wfs, UBYTE *name )
{
  struct WinFileSelPrefs *p = &wfs->Prefs;
  ULONG file;
  UBYTE buf[200];

    if (file = Open( name, MODE_NEWFILE ))
        {
        sprintf( buf, "%ld %ld %ld %ld %ld %ld %ld",
            p->Box.Left, p->Box.Top, p->Box.Width, p->Box.Height,
            (p->DevFileWidth >> 16), (p->DevFileWidth & 0xffff),
            (ULONG)p->StringsTop );
        Write( file, buf, StrLen(buf) );
        //Write( file, &wfs->Prefs.Box, sizeof(struct WinFileSelPrefs) );
        Close( file );
        }
}

static void save_env( struct UIKObjWinFileSel *wfs )
{
  ULONG old, i, dif=0;
  UBYTE *p1, *p2;

//verif_stringtop( wfs, "save_env" );
    //-- if (wfs->OldPrefs == wfs->Prefs) return;
    for (i=0, p1=(UBYTE*)&wfs->Prefs, p2=(UBYTE*)&wfs->OldPrefs; i < sizeof(struct WinFileSelPrefs); i++)
        {
        if (*p1++ != *p2++) { dif = 1; break; }
        }
    if (dif == 0) return;

    //-- save
    old = UIK_DisableDOSReq();
    save_envfile( wfs, "ENV:UIK/Objects/ObjWinFileSel.prefs" );
    save_envfile( wfs, "ENVARC:UIK/Objects/ObjWinFileSel.prefs" );
    UIK_EnableDOSReq( 0, old );
}

/****************************************************************
 *
 *      Callbacks
 *
 ****************************************************************/

static void OKFunc( struct UIKObj *fso, UBYTE *totname )
{
  struct UIKObjWinFileSel *wfs = (struct UIKObjWinFileSel *) fso->Parent->Parent;

//verif_stringtop( wfs, "OKFunc 1" );
    if (wfs->okfunc) (*wfs->okfunc)( wfs, totname );
//verif_stringtop( wfs, "OKFunc 2" );
}

static void CancelFunc( int nil, struct UIKObj *fso )
{
  struct UIKObjWinFileSel *wfs = (struct UIKObjWinFileSel *) fso->Parent->Parent;

//verif_stringtop( wfs, "CancelFunc 1" );
    if (wfs->cancelfunc) (*wfs->cancelfunc)( 0, wfs );
//verif_stringtop( wfs, "CancelFunc 2" );
}


/****************************************************************
 *
 *      Methodes
 *
 ****************************************************************/

static void close_window( struct UIKObjWindow *wo, struct UIKObjWinFileSel *wfs )
{
//verif_stringtop( wfs, "close_window 1" );
    if (wfs->cancelfunc) (*wfs->cancelfunc)( 0, wfs );
    if (wfs->Obj.Status == UIKS_STARTED) UIK_Stop( wfs );
//verif_stringtop( wfs, "close_window 2" );
}

static void update_size( struct UIKObjWindow *wo, struct UIKObjWinFileSel *wfs )
{
  struct Window *w = wo->Obj.w;

//verif_stringtop( wfs, "update_size 1" );
    wfs->Prefs.Box.Left   = w->LeftEdge;
    wfs->Prefs.Box.Top    = w->TopEdge;
    wfs->Prefs.Box.Width  = w->Width;
    wfs->Prefs.Box.Height = w->Height;
    wfs->Prefs.DevFileWidth = W2L(wfs->FS->DevObjList->Obj.Box.Width,wfs->FS->DirObjList->Obj.Box.Width);
    if (wo->WinFlags & UIKW_AUTOADJUSTSIZE)
        {
        wfs->Prefs.Box.Width  -= (wo->LeftBorderWidth + wo->RightBorderWidth);
        wfs->Prefs.Box.Height -= (wo->TopBorderHeight + wo->BottomBorderHeight);
        }
//verif_stringtop( wfs, "update_size 2" );
}

/****************************************************************
 *
 *      Init/Cleanup
 *
 ****************************************************************/

static void GenericRemove( struct UIKObjWinFileSel *wfs )
{
    UIK_MemSpFree( wfs->InitPath );
    UIK_MemSpFree( wfs->Pattern );

    lo_DirCreate( "ENV:UIK", &wfs->FIB );
    lo_DirCreate( "ENV:UIK/Objects", &wfs->FIB );
    lo_DirCreate( "ENVARC:UIK", &wfs->FIB );
    lo_DirCreate( "ENVARC:UIK/Objects", &wfs->FIB );
    save_env( wfs );

    if (wfs->DOSBase) CloseLibrary( wfs->DOSBase );
    if (wfs->UIKBase) CloseLibrary( wfs->UIKBase );
}

static int InstallWindow( struct UIKObjWinFileSel *wfs )
{
    if (! (wfs->WO = (struct UIKObjWindow *) UIK_AddObjectTags( "UIKObj_Window", wfs,
            UIKTAG_OBJ_LeftTop,         W2L(wfs->Prefs.Box.Left,wfs->Prefs.Box.Top),
            UIKTAG_OBJ_WidthHeight,     W2L(wfs->Prefs.Box.Width,wfs->Prefs.Box.Height),
            UIKTAG_OBJWindow_MinimumWidthHeight, W2L(WINFS_STDWIDTH,WINFS_STDHEIGHT),
            UIKTAG_OBJ_LangMaster,      wfs->Obj.ObjExt->Lang->Master,
            UIKTAG_OBJ_Title,           wfs->Obj.Title,
            UIKTAG_OBJ_AltTitle,        wfs->Obj.AltTitle,
            UIKTAG_OBJ_Title_Flags,     wfs->Obj.TitleFlags,
            UIKTAG_OBJWindowFl_AutoShrink, TRUE,
            TAG_DONE ))) goto END_ERROR;
    if (! UIK_AddVector( wfs->WO, (void*)update_size, IDCMP_NEWSIZE|IDCMP_CHANGEWINDOW, (ULONG)wfs )) goto END_ERROR;
    if (! UIK_AddVector( wfs->WO, (void*)close_window, IDCMP_CLOSEWINDOW, (ULONG)wfs )) goto END_ERROR;

    if (wfs->WO->ShrunkWidth)
        {
        WORD devwidth, filewidth;
        wfs->Prefs.Box.Width = wfs->WO->Obj.Box.Width - (wfs->WO->LeftBorderWidth + wfs->WO->RightBorderWidth);
        devwidth = FS_DEVLIST_MINWIDTH;
        filewidth = wfs->Prefs.Box.Width - (FS_DEVLIST_MINWIDTH + FS_EXTRA_WIDTH + 12);
        if (filewidth < FS_DIRLIST_MINWIDTH) filewidth = FS_DIRLIST_MINWIDTH;
        wfs->Prefs.DevFileWidth = W2L(devwidth,filewidth);
        }
    if (wfs->WO->ShrunkHeight)
        {
        wfs->Prefs.Box.Height = wfs->WO->Obj.Box.Height - (wfs->WO->TopBorderHeight + wfs->WO->BottomBorderHeight);
        if (wfs->Prefs.Box.Height < FS_MIN_HEIGHT) wfs->Prefs.Box.Height = FS_MIN_HEIGHT;
        }

    if (! (wfs->FS = (struct UIKObjFileSelector *) UIK_AddObjectTags( "UIKObj_FileSelector", wfs->WO,
            UIKTAG_OBJ_LeftTop,     W2L(6,3),
            UIKTAG_OBJ_WidthHeight, W2L(wfs->Prefs.Box.Width-12,wfs->Prefs.Box.Height-6),
            UIKTAG_OBJ_Sz_Attached_Flags, UO_ATTACHED_TOP|UO_ATTACHED_BOT|UO_ATTACHED_LEFT|UO_ATTACHED_RIGHT,
            UIKTAG_FS_Flags,        wfs->Flags,
            UIKTAG_FS_OKFUNC,       wfs->okfunc ? OKFunc : 0,
            UIKTAG_FS_CANCELFUNC,   wfs->cancelfunc ? CancelFunc : 0,
            UIKTAG_FS_INITPATH,     wfs->InitPath,
            UIKTAG_FS_DEVFILES,     wfs->Prefs.DevFileWidth,
            UIKTAG_FS_PATTERN,      wfs->Pattern,
            UIKTAG_FSFl_StringsTop, wfs->Prefs.StringsTop ? TRUE : FALSE,
            TAG_DONE ))) goto END_ERROR;

    return(1);

  END_ERROR:
    return(0);
}

static int GenericAdd( struct UIKObjWinFileSel *wfs, struct TagItem *taglist )
{
  UBYTE *ptr;

    SysBase = (*(ULONG*)4);
    if (! (wfs->UIKBase = UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (wfs->DOSBase = DOSBase = (struct DOSBase *) OpenLibrary( DOSNAME, 0 ))) goto END_ERROR;

    wfs->Flags = UIK_GetTagData( UIKTAG_WFS_Flags, UIK_WFS_LEAVEPATH, taglist );
    wfs->Flags = (ULONG) UIK_PackBoolTags( (ULONG)wfs->Flags, taglist, TagBoolMap );

    read_env( wfs );

    wfs->okfunc = (void*) UIK_GetTagData( UIKTAG_WFS_OKFunc, 0, taglist );
    wfs->cancelfunc = (void*) UIK_GetTagData( UIKTAG_WFS_CancelFunc, 0, taglist );
    wfs->Prefs.DevFileWidth = UIK_GetTagData( UIKTAG_WFS_DevFiles, wfs->Prefs.DevFileWidth, taglist );

    ptr = (UBYTE*) UIK_GetTagData( UIKTAG_WFS_InitPath, 0, taglist );
    if (!ptr) ptr = "";
    if (! (wfs->InitPath = UIK_MemSpAlloc( 1+StrLen(ptr), MEMF_ANY ))) goto END_ERROR;
    StrCpy( wfs->InitPath, ptr );

    ptr = (UBYTE*) UIK_GetTagData( UIKTAG_WFS_Pattern, 0, taglist );
    if (!ptr) ptr = "#?";
    if (! (wfs->Pattern = UIK_MemSpAlloc( 1+StrLen(ptr), MEMF_ANY ))) goto END_ERROR;
    StrCpy( wfs->Pattern, ptr );

    if (! InstallWindow( wfs )) goto END_ERROR;

    return(1);

  END_ERROR:
    return(0);
}

static int GenericStart( struct UIKObjWinFileSel *wfs )
{
    // faire un UIK_Start() soi-même au lieu de laisser faire celui en cours
    // permet d'agir si la routine de Start de la fenêtre ne marche pas, et de
    // la redimensionner afin qu'elle puisse tenir dans un écran plus petit.

    if (! UIK_Start( wfs->WO )) goto END_ERROR;
    /*
        {
        UIK_Remove( wfs->WO );
        DefaultPrefs.StringsTop = wfs->Prefs.StringsTop;
        wfs->Prefs = DefaultPrefs;
        if (! InstallWindow( wfs )) goto END_ERROR;
        if (! UIK_Start( wfs->WO )) goto END_ERROR;
        }
    */
    return(1);

  END_ERROR:
    /*
    UIK_Remove( wfs->WO );
    wfs->WO = 0;
    */
    return(0);
}

static struct TagItem __far GermTagList[] =
{
   { UIKTAG_GEN_VersionRevision, W2L(1,8) },
   { TAG_END }
};

struct UIKObjGerm __far Germ = {
    "UIKObj_WinFileSel",
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
    WINFS_STDWIDTH,     /*UWORD   MinWidth; */
    WINFS_STDHEIGHT,    /*UWORD   MinHeight; */
    W2L(WINFS_STDWIDTH,WINFS_STDHEIGHT), /*ULONG   DefaultWidthHeight; */
    UIKID_WINFILESEL,   /*UWORD   ObjId; */
    sizeof(struct UIKObjWinFileSel),   /*UWORD   ObjSize; */
    BOX_DISPLAYABLE, /* flags par defaut */
    };
