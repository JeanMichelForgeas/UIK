/****************************************************************
 *
 *      Project:   UIKPrefs
 *      Function:  gestion des menus
 *
 *      Created:   09/08/92    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikglobal.h"
#include "uiki:uikcolors.h"
#include "uiki:uikmacros.h"
#include "uiki:uikdrawauto.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objmenu.h"
#include "uiki:objfileselector.h"

#include "lci:vars.h"
#include "lci:textprefs.h"


/****** Imported ************************************************/

extern struct DOSBase * __far DOSBase;
extern struct GfxBase * __far GfxBase;
extern struct UIKBase * __far UIKBase;


/****** Exported ***********************************************/

extern VARS __near V; /* magic workaround */


/****** Statics ************************************************/

static UBYTE __far defaults_prefs[] =
    {
    0x00, 0x02, 0x01, 0x03, 0x07, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,

    0x08, 0x88, 0x0f, 0xff, 0x00, 0x00, 0x06, 0x9c,
    0x0c, 0xcc, 0x0f, 0xfb, 0x09, 0x86, 0x0e, 0xe0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0xf0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0xf0,

    0x00, 0x10,
    0x00, 0x08,

    0x74, 0x6f, 0x70, 0x61, 0x7a, 0x2e, 0x66, 0x6f,
    0x6e, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x03, 0x00, 0x00, 0x80, 0x00, 0x02, 0x80, 0x00, 0xc8, 0x00, 0x00,
    0x00, 0x03, 0x00, 0x32, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00,
    };

/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

//---------------------------------- FileSelector

static void FS_removed() { V.FS = 0; }

static void CancelFS() { UIK_Remove( V.FS ); }

static void AddFS( struct UIKObj *parent, void (*OKFunc)(), ULONG save )
{
  struct UIKObj *fs;
  UWORD ww, dl=95, fl=FS_DIRLIST_MINWIDTH+210, wh, fh;

    if (V.FS) goto END_ERROR;
    ww = dl + fl + FS_EXTRA_WIDTH;
    wh = 161;

    /*------------ requester ------------*/
    if (! (V.FS = UIK_AddObjectTags( "UIKObj_Request", parent,
            UIKTAG_OBJ_WidthHeight,   W2L(ww+20,wh),
            TAG_END ))) goto END_ERROR;

    /*----------- FileSelector ----------*/
    fh = wh - 10;
    if (fh < FS_MIN_HEIGHT) fh = FS_MIN_HEIGHT;

    if (! (fs = (struct UIKObjFileSelector *) UIK_AddObjectTags( "UIKObj_FileSelector", V.FS,
            UIKTAG_OBJ_LeftTop,         W2L(10,6),
            UIKTAG_OBJ_WidthHeight,     W2L(ww,fh),
            UIKTAG_FS_DEVFILES,         W2L(dl,fl),
            UIKTAG_FS_INITPATH,         (ULONG) V.PreName,
            UIKTAG_FS_OKFUNC,           (ULONG) OKFunc,
            UIKTAG_FS_CANCELFUNC,       (ULONG) CancelFS,
            UIKTAG_OBJ_UserRemoveFunc,  (ULONG) FS_removed,
            UIKTAG_FSFl_ReturnReturn,   TRUE,
            UIKTAG_FSFl_OK_IfNotExists, (save ? TRUE : FALSE),
            UIKTAG_OBJ_BoxFl_TranslateLeftTop, FALSE,
            TAG_DONE ))) goto END_ERROR;

    UIK_Start( V.FS );
    return;

  END_ERROR:
    CancelFS();
}

//----------------------------------

static void open_preset( struct UIKObj *obj, UBYTE *filename )
{
  ULONG file;

    if (file = Open( filename, MODE_OLDFILE ))
        {
        Read( file, &V.Prefs, sizeof(struct UIKPrefs) );
        Close( file );
        StrCpy( V.PreName, filename );
        UIK_Remove( V.WO );
        TestPrefs( obj );
        return;
        }
    CancelFS();
}
static void Project_Open( struct UIKObjMenu *mo, struct UIKMenuEntry *ume, struct MenuItem *item, UWORD menunum, UWORD itemnum, UWORD subnum )
{
    AddFS( mo, open_preset, 0 );
}

static void save_preset( struct UIKObj *obj, UBYTE *filename )
{
  ULONG file;

    if (file = Open( filename, MODE_NEWFILE ))
        {
        Write( file, &V.Prefs, sizeof(struct UIKPrefs) );
        Close( file );
        StrCpy( V.PreName, filename );
        }
    CancelFS();
}
static void Project_SaveAs( struct UIKObjMenu *mo, struct UIKMenuEntry *ume, struct MenuItem *item, UWORD menunum, UWORD itemnum, UWORD subnum )
{
    AddFS( mo, save_preset, 1 );
}

static void Project_Quit( struct UIKObjMenu *mo, struct UIKMenuEntry *ume, struct MenuItem *item, UWORD menunum, UWORD itemnum, UWORD subnum )
{
    Signal( FindTask(0), SIGBREAKF_CTRL_C );
}

static void replace_font()
{
  struct TextFont *font;

    if (font = UIK_OpenFont( V.Prefs.DefFontName, V.Prefs.DefFontHeight ))
        {
        CloseFont( V.UIKBase->DefaultFont );
        V.UIKBase->DefaultFont = font;
        UIK_Font2TTA( font, &V.UIKBase->DefaultFontAttr );
        }
}

static void Edit_Reset( struct UIKObjMenu *mo, struct UIKMenuEntry *ume, struct MenuItem *item, UWORD menunum, UWORD itemnum, UWORD subnum )
{
  int i;
  UBYTE *p;

    UIK_Remove( V.WO );
    for (i=0, p=(UBYTE *)&V.Prefs; i < sizeof(struct UIKPrefs); i++) *p++ = 0;
    for (i=0, p=(UBYTE *)&V.Prefs; i < sizeof(defaults_prefs); i++) *p++ = defaults_prefs[i];
    if (UIK_IsSystemVersion2())
        {
        UWORD w;
        UBYTE c = V.Prefs.Colors.Black;
        V.Prefs.Colors.Black = V.Prefs.Colors.White;
        V.Prefs.Colors.White = c;
        w = V.Prefs.ColorTable[1];
        V.Prefs.ColorTable[1] = V.Prefs.ColorTable[2];
        V.Prefs.ColorTable[2] = w;
        }
    replace_font();
    SetDefaultMode( &V.Prefs );
    TestPrefs( mo );
}

static void Edit_Last( struct UIKObjMenu *mo, struct UIKMenuEntry *ume, struct MenuItem *item, UWORD menunum, UWORD itemnum, UWORD subnum )
{
    UIK_Remove( V.WO );
    UIK_ReadArcPrefs();
    UIK_GetPrefs( &V.Prefs, sizeof(struct UIKPrefs) );
    replace_font();
    TestPrefs( mo );
}

static void Edit_Restore( struct UIKObjMenu *mo, struct UIKMenuEntry *ume, struct MenuItem *item, UWORD menunum, UWORD itemnum, UWORD subnum )
{
    UIK_Remove( V.WO );
    UIK_MemCopy( &V.OldPrefs, &V.Prefs, sizeof(struct UIKPrefs) );
    replace_font();
    TestPrefs( mo );
}

/*
static void Settings_Icons( struct UIKObjMenu *mo, struct UIKMenuEntry *ume, struct MenuItem *item, UWORD menunum, UWORD itemnum, UWORD subnum )
{
}
*/

static struct UIKMenuEntry __far EnglishMenu[] = {
    { 0,  0, MSG_PROJECT_MENU,                  NULL,           0,0,0,MENUENABLED },
    { 1,'O',      MSG_PROJECT_OPEN_MENU,        Project_Open,   0,0,0,ITEMENABLED },
    { 1,'A',      MSG_PROJECT_SAVEAS_MENU,      Project_SaveAs, 0,0,0,ITEMENABLED },
    { 1,  0,      ITEM_SEPARATION },
    { 1,'Q',      MSG_PROJECT_QUIT_MENU,        Project_Quit,   0,0,0,ITEMENABLED },
    { 0,  0, MSG_EDIT_MENU,                     NULL,           0,0,0,MENUENABLED },
    { 1,'D',      MSG_EDIT_RESET_MENU,          Edit_Reset,     0,0,0,ITEMENABLED },
    { 1,'L',      MSG_EDIT_LAST_MENU,           Edit_Last,      0,0,0,ITEMENABLED },
    { 1,'R',      MSG_EDIT_RESTORE_MENU,        Edit_Restore,   0,0,0,ITEMENABLED },
    /*
    { 0,  0, MSG_SETTINGS_MENU,                 NULL,           0,0,0,MENUENABLED },
    { 1,'I',      MSG_SETTINGS_CRICON_MENU,     Settings_Icons, 0,0,0,ITEMENABLED|MENUTOGGLE|CHECKIT },
    */
    { 0,0,0,0,0,0 },  // marqueur de fin: Title doit être à 0
    };

struct UIKObj *InstallMenu( struct UIKObj *parent )
{
    return( UIK_AddObjectTags( "UIKObj_Menu", parent,
                UIKTAG_Menu_Entry,  (ULONG) EnglishMenu,
                TAG_END ));
}
