/****************************************************************
 *
 *      File:      8SVX.c
 *      Project:   UIK example code
 *
 *      Created:   23-08-92     Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include <libraries/dos.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikmacros.h"
#include "uiki:uikglobal.h"
#include "uiki:uikdrawauto.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objwindow.h"
#include "uiki:objfileselector.h"
#include "uiki:obj8svx.h"

#define TEXT_WindowTitle    1
#define TEXT_ScreenTitle    2
#define TEXT_GAD1           3
#define TEXT_FS             4
#define TEXT_BOX            5

#ifdef LATTICE
int CXBRK(void) { return(0); }      /* disable SASC CTRLC handling */
int chkabort(void) { return(0); }   /* really */
#endif
#ifdef SAS
int CXBRK    ( void ) { return(0); }
int _CXBRK   ( void ) { return(0); }
int chkabort ( void ) { return(0); }
#endif


/****** Imported ************************************************/


/****** Exported ***********************************************/

struct UIKBase *UIKBase;
struct UIKGlobal *UIK;

UBYTE sound_buf[] = "                                                                                                                                                                                 ";

UWORD NullAuto[] = { END_AUTO };


/****** Statics ************************************************/

static UBYTE *LangEnglish[] =
    {
    "",
    "8SVX",
    "8SVX Copyright © 1992 Jean-Michel Forgeas",
    "Select a 8SVX file",
    "Sound choice",
    sound_buf,
    0
    };

static UBYTE *LangOther[] =
    {
    "",
    "8SVX",
    "8SVX Copyright © 1992 Jean-Michel Forgeas",
    "Sélectionnez un fichier 8SVX",
    "Choix d'un son",
    sound_buf,
    0
    };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void choix_son( struct UIKObj *g, struct UIKObjWindow *wo2 )
{
    UIK_Start( wo2 );
}

static void CancelFS( int nil, struct UIKObj *fs )
{
    UIK_Stop( fs->Parent );  /* wo2 */
}

static void OKFS( struct UIKObj *fs, UBYTE *filename )
{
  struct UIKObj *box = (struct UIKObj *) fs->UserULong1;
  struct UIKObj *o8svx = (struct UIKObj *) fs->UserULong2;

    StrCpy( UIK_ObjTitle( box ), filename );
    UIK_Stop( fs->Parent );  /* wo2 */
    UIK_Refresh( box, 1 );
    UIK_CallObjectFunc( o8svx, UIKFUNC_8SVX_StartDigit, (ULONG) filename, 0 );
}

static void Cleanup()
{
    if (UIK)     UIK_Remove( UIK );
    if (UIKBase) CloseLibrary( UIKBase );
    exit(0);
}

void main()
{
  struct UIKObj *wo1, *wo2;
  struct UIKObj *g1, *box;
  struct UIKObj *fs, *o8svx;
  UWORD ww, wh=140;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (UIK = (struct UIKGlobal *) UIK_AddObjectTags( "UIK", 0,
             UIKTAG_GEN_LangEnglish,    (ULONG) LangEnglish,
             UIKTAG_GEN_LangOther,      (ULONG) LangOther,
             UIKTAG_GEN_Catalog,        (ULONG) "uik/8svx.catalog", /* juste en cas */
             TAG_END ))) goto END_ERROR;

    /*------------------ Fenêtre principale -------------------*/

    if (! (wo1 = UIK_AddObjectTags( "UIKObj_Window", UIK,
            UIKTAG_OBJ_LeftTop,         SETL(5,30),
            UIKTAG_OBJ_WidthHeight,     SETL(260,70),
            UIKTAG_OBJ_Title,           TEXT_WindowTitle,
            UIKTAG_OBJ_AltTitle,        TEXT_ScreenTitle,
            UIKTAG_OBJWindowFl_With_Size,FALSE,
            TAG_END ))) goto END_ERROR;

    if (! (g1 = UIK_AddObjectTags( "UIKObj_Boolean", wo1,
            UIKTAG_OBJ_LeftTop,         SETL(10,15),
            UIKTAG_OBJ_WidthHeight,     SETL(242,23),
            UIKTAG_OBJ_Title,           TEXT_GAD1,
            UIKTAG_OBJ_ShortCutKey,     B2L('S',0,0,0),
            UIKTAG_OBJ_BoxFl_Filled,    TRUE,
            TAG_END ))) goto END_ERROR;

    if (! (box = UIK_AddObjectTags( "UIKObj_Box", wo1,
            UIKTAG_OBJ_LeftTop,         SETL(0,38),
            UIKTAG_OBJ_WidthHeight,     SETL(258,31),
            UIKTAG_OBJ_Title,           (ULONG) TEXT_BOX,
            UIKTAG_OBJ_BoxFl_Filled,    TRUE,
            UIKTAG_OBJ_BoxAutomate,     (ULONG) NullAuto,
            TAG_END ))) goto END_ERROR;

    /*------------------ Joueur d'échantillon -------------------*/

    if (! (o8svx = UIK_AddObject( "UIKObj_8SVX", wo1, 0 ))) goto END_ERROR;

    /*------------------ Fenêtre secondaire -------------------*/

    ww = FS_DEVLIST_MINWIDTH + FS_DIRLIST_MINWIDTH + FS_EXTRA_WIDTH + 20;

    if (! (wo2 = UIK_AddObjectTags( "UIKObj_Window", wo1,
            UIKTAG_OBJ_LeftTop,         SETL(270,30),
            UIKTAG_OBJ_WidthHeight,     SETL(ww+2*7,wh+2*3),
            UIKTAG_OBJ_Title,           TEXT_FS,
            UIKTAG_OBJ_AltTitle,        TEXT_ScreenTitle,
            UIKTAG_OBJ_BoxFl_StartSpecify, TRUE,
            TAG_END ))) goto END_ERROR;

    if (! (fs = UIK_AddObjectTags( "UIKObj_FileSelector", wo2,
            UIKTAG_OBJ_LeftTop,         W2L(7,3),
            UIKTAG_OBJ_WidthHeight,     W2L(ww,wh),
            UIKTAG_FS_DEVFILES,         W2L(FS_DEVLIST_MINWIDTH,FS_DIRLIST_MINWIDTH),
            UIKTAG_FSFl_ReturnReturn,   TRUE,
            UIKTAG_FS_OKFUNC,           (ULONG) OKFS,
            UIKTAG_FS_CANCELFUNC,       (ULONG) CancelFS,
            UIKTAG_OBJ_UserValue1,      (ULONG) box,
            UIKTAG_OBJ_UserValue2,      (ULONG) o8svx,
            TAG_END ))) goto END_ERROR;

    /*------------------ Vecteurs -------------------*/

    if (! UIK_AddVector( g1, (void*) choix_son, GADGETUP, (ULONG) wo2 )) goto END_ERROR;

    /*------------------ Démarrage -------------------*/

    if (! UIK_Start( UIK )) goto END_ERROR;

    UIK_Do( UIK, SIGBREAKF_CTRL_C );

  END_ERROR:
    Cleanup();
}
