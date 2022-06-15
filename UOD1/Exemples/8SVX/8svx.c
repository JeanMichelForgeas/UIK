/****************************************************************
 *
 *      File:      Ask and play 8SVX sample
 *      Project:   UIK example code
 *
 *      Created:   23-08-92     Jean-Michel Forgeas
 *
 *      Utilise l'objet "UIKObj_WinFileSel"
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
#include "uiki:objwinfilesel.h"
#include "uiki:obj8svx.h"

#define TEXT_WindowTitle    1
#define TEXT_ScreenTitle    2
#define TEXT_GAD1           3
#define TEXT_FS             4
#define TEXT_BOX            5


/****** Imported ************************************************/


/****** Exported ***********************************************/

struct UIKBase * __far UIKBase;
struct UIKGlobal * __far UIK;

struct UIKObj * __far Up;
struct UIKObj * __far Down;
struct UIKObj * __far List;

UBYTE __far sound_buf[] = "                                                                                   ";

UWORD __far NullAuto[] = { END_AUTO };

/****** Statics ************************************************/

static UBYTE * __far LangEnglish[] =
    {
    "",
    "8SVXTest",
    "8SVXTest Copyright © 1992 Jean-Michel Forgeas",
    "Select a 8SVX file",
    "Sound choice",
    sound_buf,
    0
    };

static UBYTE * __far LangOther[] =
    {
    "",
    "8SVXTest",
    "8SVXTest Copyright © 1992 Jean-Michel Forgeas",
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

static void choix_son( struct UIKObj *g, struct UIKObj *wfs )
{
    UIK_Start( wfs ); /* WinFileSel */
}

static void FileSelect_Cancel( int nil, struct UIKObj *wfs )
{
    UIK_Stop( wfs );  /* WinFileSel */
}

static void FileSelect_OK( struct UIKObj *wfs, UBYTE *filename )
{
  struct UIKObj *box = (struct UIKObj *) wfs->UserULong1;
  struct UIKObj *o8svx = (struct UIKObj *) wfs->UserULong2;

    StrCpy( UIK_ObjTitle( box ), filename );
    UIK_Stop( wfs );  /* WinFileSel */
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
  struct UIKObj *wo;
  struct UIKObj *g1, *box;
  struct UIKObj *wfs, *o8svx;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (UIK = (struct UIKGlobal *) UIK_AddObjectTags( "UIK", 0,
             UIKTAG_GEN_LangEnglish,    (ULONG) LangEnglish,
             UIKTAG_GEN_LangOther,      (ULONG) LangOther,
             UIKTAG_GEN_Catalog,        (ULONG) "uik/8svxtest.catalog",
             TAG_END ))) goto END_ERROR;

    /*------------------ avant UIK_Start() -------------------*/

    if (! (wo = UIK_AddObjectTags( "UIKObj_Window", UIK,
            UIKTAG_OBJ_LeftTop,         SETL(5,30),
            UIKTAG_OBJ_WidthHeight,     SETL(260,70),
            UIKTAG_OBJ_Title,           TEXT_WindowTitle,
            UIKTAG_OBJ_AltTitle,        TEXT_ScreenTitle,
            UIKTAG_OBJWindowFl_With_Size,FALSE,
            UIKTAG_OBJWindowFl_With_Icon,FALSE,
            TAG_END ))) goto END_ERROR;

    if (! (g1 = UIK_AddObjectTags( "UIKObj_Boolean", wo,
            UIKTAG_OBJ_LeftTop,         SETL(18,15),
            UIKTAG_OBJ_WidthHeight,     SETL(225,23),
            UIKTAG_OBJ_Title,           TEXT_GAD1,
            UIKTAG_OBJ_ShortCutKey,     B2L('S',0,0,0),
            UIKTAG_OBJ_BoxFl_Filled,    TRUE,
            TAG_END ))) goto END_ERROR;

    if (! (box = UIK_AddObjectTags( "UIKObj_Box", wo,
            UIKTAG_OBJ_LeftTop,         SETL(0,38),
            UIKTAG_OBJ_WidthHeight,     SETL(258,31),
            UIKTAG_OBJ_Title,           (ULONG) TEXT_BOX,
            UIKTAG_OBJ_BoxFl_Filled,    TRUE,
            UIKTAG_OBJ_BoxAutomate,     (ULONG) NullAuto,
            TAG_END ))) goto END_ERROR;

    if (! (o8svx = UIK_AddObject( "UIKObj_8SVX", wo, 0 ))) goto END_ERROR;

    if (! UIK_Start( UIK )) goto END_ERROR;

    /*------------------ après UIK_Start() -------------------*/

    if (! (wfs = UIK_AddObjectTags( "UIKObj_WinFileSel", wo,
            UIKTAG_WFSFl_ReturnReturn,  TRUE,
            UIKTAG_WFS_OKFunc,          (ULONG) FileSelect_OK,
            UIKTAG_WFS_CancelFunc,      (ULONG) FileSelect_Cancel,
            UIKTAG_OBJ_UserValue1,      (ULONG) box,
            UIKTAG_OBJ_UserValue2,      (ULONG) o8svx,
            TAG_END ))) goto END_ERROR;

    /*------------------ vecteurs -------------------*/

    if (! UIK_AddVector( g1, (void*) choix_son, GADGETUP, (ULONG) wfs )) goto END_ERROR;

    UIK_Do( UIK, SIGBREAKF_CTRL_C );

  END_ERROR:
    Cleanup();
}
