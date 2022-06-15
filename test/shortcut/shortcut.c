/****************************************************************
 *
 *      File:      ShortCut.c
 *      Project:   UIK example code
 *
 *      Created:   23-08-92     Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include <stdio.h>

#include <libraries/dos.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikmacros.h"
#include "uiki:uikglobal.h"
#include "uiki:uikcolors.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"
#include "uiki:objlist.h"
#include "uiki:objproplift.h"
#include "uiki:objwindow.h"

#define TEXT_WindowTitle    1
#define TEXT_ScreenTitle    2
#define TEXT_GAD1           3
#define TEXT_GAD2           4
#define TEXT_GAD3           5
#define TEXT_GAD4           6
#define TEXT_BOX1           7

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
    "ShortCut",
    "ShortCut Copyright © 1992 Jean-Michel Forgeas",
    "Gadget 1",
    "Gadget 2",
    "_AAAAAAAA",
    "B!BBBBBBB",
    "Type the underligned characters",
    0
    };

static UBYTE *LangOther[] =
    {
    "",
    "ShortCut",
    "ShortCut Copyright © 1992 Jean-Michel Forgeas",
    "Cellule 1",
    "Cellule 2",
    "_AAAAAAAA",
    "B!BBBBBBB",
    "Tapez les caractères soulignés",
    0
    };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void start_wo2( struct UIKObj *g, struct UIKObjWindow *wo2 )
{
    UIK_Start( wo2 );
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
            UIKTAG_OBJ_Title,           TEXT_WindowTitle,
            UIKTAG_OBJ_AltTitle,        TEXT_ScreenTitle,
            UIKTAG_OBJWindowFl_With_Size,FALSE,
            UIKTAG_OBJWindowFl_With_Icon,FALSE,
            TAG_END ))) goto END_ERROR;

    if (! (g1 = UIK_AddObjectTags( "UIKObj_Boolean", wo1,
            UIKTAG_OBJ_LeftTop,         SETL(40,15),
            UIKTAG_OBJ_WidthHeight,     SETL(77,23),
            UIKTAG_OBJ_Title,           TEXT_GAD1,
            UIKTAG_OBJ_ShortCutKey,     B2L('1',0,0,0),
            UIKTAG_OBJ_BoxFl_Filled,    TRUE,
            TAG_END ))) goto END_ERROR;
    if (! (g2 = UIK_AddObjectTags( "UIKObj_Boolean", wo1,
            UIKTAG_OBJ_LeftTop,         SETL(136,15),
            UIKTAG_OBJ_WidthHeight,     SETL(77,23),
            UIKTAG_OBJ_Title,           TEXT_GAD2,
            UIKTAG_OBJ_ShortCutKey,     B2L('2',0,0,0),
            UIKTAG_OBJ_BoxFl_Filled,    TRUE,
            TAG_END ))) goto END_ERROR;
    if (! UIK_AddObjectTags( "UIKObj_Box", wo1,
            UIKTAG_OBJ_LeftTop,         SETL(0,40),
            UIKTAG_OBJ_WidthHeight,     SETL(258,32),
            UIKTAG_OBJ_Title,           TEXT_BOX1,
            UIKTAG_OBJ_BoxFl_NotDrawn,  TRUE,
            TAG_END )) goto END_ERROR;

    if (! UIK_Start( UIK )) goto END_ERROR;

    /*------------------ après UIK_Start() -------------------*/

    if (! (wo2 = UIK_AddObjectTags( "UIKObj_Window", UIK,
            UIKTAG_OBJ_LeftTop,         SETL(200,60),
            UIKTAG_OBJ_WidthHeight,     SETL(260,70),
            UIKTAG_OBJWindowFl_With_Size,FALSE,
            UIKTAG_OBJWindowFl_With_Icon,FALSE,
            UIKTAG_OBJWindowFl_With_Depth,FALSE,
            TAG_END ))) goto END_ERROR;
    if (! (ga = UIK_AddObjectTags( "UIKObj_MutualExclude", wo2,
            UIKTAG_OBJ_LeftTop,         SETL(40,15),
            UIKTAG_OBJ_WidthHeight,     SETL(77,23),
            UIKTAG_OBJ_Title,           TEXT_GAD3,
            UIKTAG_OBJ_BoxFl_Filled,    TRUE,
            UIKTAG_OBJ_TitleUnderscore, '_',
            TAG_END ))) goto END_ERROR;
    if (! (gb = UIK_AddObjectTags( "UIKObj_MutualExclude", wo2,
            UIKTAG_OBJ_LeftTop,         SETL(136,15),
            UIKTAG_OBJ_WidthHeight,     SETL(77,23),
            UIKTAG_OBJ_Title,           TEXT_GAD4,
            UIKTAG_OBJ_BoxFl_Filled,    TRUE,
            UIKTAG_OBJ_TitleUnderscore, '!',
            TAG_END ))) goto END_ERROR;

    /*------------------ vecteurs -------------------*/

    if (! UIK_AddVector( g2, (void*) start_wo2, GADGETUP, (ULONG) wo2 )) goto END_ERROR;

    UIK_Do( UIK, SIGBREAKF_CTRL_C );

  END_ERROR:
    Cleanup();
}
