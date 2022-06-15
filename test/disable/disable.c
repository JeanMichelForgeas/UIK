/****************************************************************
 *
 *      File:      disable.c
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

#define TEXT_WindowTitle    1
#define TEXT_ScreenTitle    2
#define TEXT_FS             3
#define TEXT_GAD2           4
#define TEXT_GAD1           5

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


/****** Statics ************************************************/

static UBYTE *LangEnglish[] =
    {
    "",
    "Disable",
    "Disable Copyright © 1992 Jean-Michel Forgeas",
    "Select a file",
    "_Files...",
    "_Enable",
    0
    };

static UBYTE *LangOther[] =
    {
    "",
    "Disable",
    "Disable Copyright © 1992 Jean-Michel Forgeas",
    "Sélection",
    "_Fichiers...",
    "_Enable",
    0
    };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void disable_func( struct UIKObj *obj, struct UIKObj *filesel )
{
  struct UIKObj *gad = (struct UIKObj *)filesel->UserULong1;

    if (obj->Flags & BOX_OVER)
        {
        UIK_ObjDisable( gad, -1 );
        }
    else{
        UIK_ObjEnable( gad, -1 );
        }
}

/*-------------------------------------------------------------*/

static void choix_fichier( struct UIKObj *g, struct UIKObjWindow *wo2 )
{
    UIK_Start( wo2 );
}

static void fichier_choisi( int nil, struct UIKObj *fs )
{
    UIK_Stop( fs->Parent );  /* wo2 */
}

/*-------------------------------------------------------------*/

void main()
{
  struct UIKObj *wo1, *wo2;
  struct UIKObj *g1, *g2;
  struct UIKObj *fs;
  UWORD ww, wh=140;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (UIK = (struct UIKGlobal *) UIK_AddObjectTags( "UIK", 0,
             UIKTAG_GEN_LangEnglish,    (ULONG) LangEnglish,
             UIKTAG_GEN_LangOther,      (ULONG) LangOther,
             UIKTAG_GEN_Catalog,        (ULONG) "uik/disable.catalog", /* juste en cas */
             TAG_END ))) goto END_ERROR;

    /*------------------ Fenêtre principale -------------------*/

    if (! (wo1 = UIK_AddObjectTags( "UIKObj_Window", UIK,
            UIKTAG_OBJ_LeftTop,         SETL(20,30),
            UIKTAG_OBJ_WidthHeight,     SETL(260,70),
            UIKTAG_OBJ_Title,           TEXT_WindowTitle,
            UIKTAG_OBJ_AltTitle,        TEXT_ScreenTitle,
            UIKTAG_OBJWindowFl_With_Size,FALSE,
            TAG_END ))) goto END_ERROR;

    if (! (g1 = UIK_AddObjectTags( "UIKObj_Toggle", wo1,
            UIKTAG_OBJ_LeftTop,         SETL(20,15),
            UIKTAG_OBJ_WidthHeight,     SETL(105,23),
            UIKTAG_OBJ_Title,           TEXT_GAD1,
            UIKTAG_OBJ_TitleUnderscore, '_',
            UIKTAG_OBJ_BoxFl_Filled,    TRUE,
            TAG_END ))) goto END_ERROR;

    if (! (g2 = UIK_AddObjectTags( "UIKObj_Boolean", wo1,
            UIKTAG_OBJ_LeftTop,         SETL(130,15),
            UIKTAG_OBJ_WidthHeight,     SETL(105,23),
            UIKTAG_OBJ_Title,           TEXT_GAD2,
            UIKTAG_OBJ_TitleUnderscore, '_',
            UIKTAG_OBJ_BoxFl_Filled,    TRUE,
            UIKTAG_OBJ_BoxFl_Disabled,  TRUE,
            TAG_END ))) goto END_ERROR;

    /*------------------ Fenêtre secondaire -------------------*/

    ww = FS_DEVLIST_MINWIDTH + FS_DIRLIST_MINWIDTH + FS_EXTRA_WIDTH + 20;

    if (! (wo2 = UIK_AddObjectTags( "UIKObj_Window", g2,
            UIKTAG_OBJ_LeftTop,         SETL(270,30),
            UIKTAG_OBJ_WidthHeight,     SETL(ww+2*7,wh+2*3),
            UIKTAG_OBJ_Title,           TEXT_FS,
            UIKTAG_OBJ_AltTitle,        TEXT_ScreenTitle,
            UIKTAG_OBJ_BoxFl_StartSpecify, TRUE,
            TAG_END ))) goto END_ERROR;

    if (! (fs = UIK_AddObjectTags( "UIKObj_FileSelector", wo2,
            UIKTAG_OBJ_LeftTop,         W2L(7,3),
            UIKTAG_OBJ_WidthHeight,     W2L(ww,wh),
            UIKTAG_OBJ_UserValue1,      g2,
            UIKTAG_FS_DEVFILES,         W2L(FS_DEVLIST_MINWIDTH,FS_DIRLIST_MINWIDTH),
            UIKTAG_FSFl_ReturnReturn,   TRUE,
            UIKTAG_FS_OKFUNC,           fichier_choisi,
            TAG_END ))) goto END_ERROR;

    /*------------------ Vecteurs -------------------*/

    if (! UIK_AddVector( g1, (void*) disable_func, GADGETUP, (ULONG) fs )) goto END_ERROR;
    if (! UIK_AddVector( g2, (void*) choix_fichier, GADGETUP, (ULONG) wo2 )) goto END_ERROR;

    /*------------------ Démarrage -------------------*/

    if (! UIK_Start( UIK )) goto END_ERROR;

    UIK_Do( UIK, SIGBREAKF_CTRL_C );

  END_ERROR:
    if (UIK)     UIK_Remove( UIK );
    if (UIKBase) CloseLibrary( UIKBase );
}
