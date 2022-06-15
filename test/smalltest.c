/****************************************************************
 *
 *      File:      SmallTest.c
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
#include "uiki:uikcolors.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objwindow.h"

#define TEXT_WindowTitle    1
#define TEXT_ScreenTitle    2
#define TEXT_GAD1           3
#define TEXT_GAD2           4


/****** Imported ************************************************/


/****** Exported ***********************************************/

struct UIKBase * __far UIKBase;
struct UIKGlobal * __far UIK;


/****** Statics ************************************************/

static UBYTE * __far LangEnglish[] =
    {
    "",
    "ShortCut",
    "ShortCut Copyright © 1992 Jean-Michel Forgeas",
    "_Iconify",
    "_Quit",
    0
    };

static UBYTE * __far LangOther[] =
    {
    "",
    "ShortCut",
    "ShortCut Copyright © 1992 Jean-Michel Forgeas",
    "_Iconifier",
    "_Quitter",
    0
    };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void quit()
{
    Signal( FindTask(0), SIGBREAKF_CTRL_C );
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
  struct UIKObj *g1, *g2, *ga, *gb;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (UIK = (struct UIKGlobal *) UIK_AddObjectTags( "UIK", 0,
             UIKTAG_GEN_LangEnglish,    (ULONG) LangEnglish,
             UIKTAG_GEN_LangOther,      (ULONG) LangOther,
             UIKTAG_GEN_Catalog,        (ULONG) "uik/shortkey.catalog",
             TAG_END ))) goto END_ERROR;

    /*------------------ avant UIK_Start() -------------------*/

    if (! (wo1 = UIK_AddObjectTags( "UIKObj_Window", UIK,
            UIKTAG_OBJ_LeftTop,         SETL(180,30),
            UIKTAG_OBJ_WidthHeight,     SETL(260,70),
            /*
            UIKTAG_OBJ_Title,           TEXT_WindowTitle,
            UIKTAG_OBJ_AltTitle,        TEXT_ScreenTitle,
            */
            UIKTAG_OBJWindowFl_With_Size,FALSE,
            UIKTAG_OBJWindowFl_With_Icon,FALSE,
            UIKTAG_OBJWindowFl_With_Close,FALSE,
            UIKTAG_OBJWindowFl_With_Depth,FALSE,
            UIKTAG_OBJ_ShortCutKey,     B2L('X',0,0,0),
            TAG_END ))) goto END_ERROR;
    if (! UIK_AddVector( wo1, (void*)UIK_Iconify, RAWKEY, 0 )) goto END_ERROR;

    if (! (g1 = UIK_AddObjectTags( "UIKObj_Boolean", wo1,
            UIKTAG_OBJ_LeftTop,         SETL(40,15),
            UIKTAG_OBJ_WidthHeight,     SETL(77,23),
            UIKTAG_OBJ_Title,           TEXT_GAD1,
            UIKTAG_OBJ_TitleUnderscore, '_',
            UIKTAG_OBJ_BoxFl_Filled,    TRUE,
            TAG_END ))) goto END_ERROR;
    if (! UIK_AddVector( g1, (void*)UIK_Iconify, GADGETUP, 0 )) goto END_ERROR;

    if (! (g2 = UIK_AddObjectTags( "UIKObj_Boolean", wo1,
            UIKTAG_OBJ_LeftTop,         SETL(136,15),
            UIKTAG_OBJ_WidthHeight,     SETL(77,23),
            UIKTAG_OBJ_Title,           TEXT_GAD2,
            UIKTAG_OBJ_TitleUnderscore, '_',
            UIKTAG_OBJ_BoxFl_Filled,    TRUE,
            TAG_END ))) goto END_ERROR;
    if (! UIK_AddVector( g2, (void*)quit, GADGETUP, 0 )) goto END_ERROR;

    if (! UIK_Start( UIK )) goto END_ERROR;

    UIK_Do( UIK, SIGBREAKF_CTRL_C );

  END_ERROR:
    Cleanup();
}
