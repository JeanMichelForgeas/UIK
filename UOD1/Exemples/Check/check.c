/****************************************************************
 *
 *      File:      check.c
 *      Project:   Essai des objets Check et Radio
 *
 *      Created:   18/03/93     Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include <libraries/dos.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikglobal.h"
#include "uiki:uikcolors.h"
#include "uiki:uikmacros.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objradioexclude.h"
#include "uiki:objchecktoggle.h"

#define TEXT_WindowTitle    1
#define TEXT_ScreenTitle    2
#define TEXT_CheckToggle    3
#define TEXT_RadioExclude   4


/****** Imported ************************************************/

extern ULONG __far DOSBase;


/****** Exported ***********************************************/

struct UIKBase * __far UIKBase;


/****** Statics ************************************************/

static struct UIKGlobal * __far UIK;

static UBYTE * __far LangEnglish[] =
    {
    "",
    "Check Test",
    "Check Test 1.0 Copyright © 1993 J-M Forgeas, All Rights Reserved",
    "CheckToggle",
    "RadioExclude",
    0
    };

static UBYTE * __far LangOther[] =
    {
    "",
    "Check Test",
    "Check Test 1.0 Copyright © 1993 J-M Forgeas, tous droits réservés",
    "CheckToggle",
    "RadioExclude",
    0
    };

/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void print_event( struct UIKObj *obj, UBYTE *eventname )
{
    printf( "%ls  ", eventname );
}

static void Cleanup()
{
    if (UIK)            UIK_Remove( UIK );
    if (UIKBase)        CloseLibrary( UIKBase );
    exit(0);
}

void main( int argc )
{
  struct UIKObj *wo, *o;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (UIK = (struct UIKGlobal *) UIK_AddObjectTags( "UIK", 0,
            UIKTAG_GEN_LangEnglish,    (ULONG) LangEnglish,
            UIKTAG_GEN_LangOther,      (ULONG) LangOther,
            TAG_END ))) goto END_ERROR;

    if (! (wo = UIK_AddObjectTags( "UIKObj_Window", UIK,
            UIKTAG_OBJ_LeftTop,             SETL(200,100),
            UIKTAG_OBJ_WidthHeight,         SETL(300,100),
            UIKTAG_OBJ_Title,               TEXT_WindowTitle,
            UIKTAG_OBJ_AltTitle,            TEXT_ScreenTitle,
            TAG_END ))) goto END_ERROR;

    /*--------------- test du "Toggle" */

    if (! (o = UIK_AddObjectTags( "UIKObj_CheckToggle", wo,
            UIKTAG_OBJ_LeftTop,         W2L(20,10),
            UIKTAG_OBJ_WidthHeight,     SETL(22,15),
            UIKTAG_OBJ_Title,           TEXT_CheckToggle,
            UIKTAG_OBJ_TitleFl_Right,   TRUE,
            UIKTAG_OBJ_ShortCutKey,     W2L(B2W('C',0),0),
            UIKTAG_OBJ_ActInactPens,    UIKCOLS(UIKCOL_GREY,UIKCOL_GREY,UIKCOL_BLACK,UIKCOL_BLACK),
            TAG_END ))) goto END_ERROR;
    if (! UIK_AddVector( o, (void*)print_event, IDCMP_GADGETUP, (ULONG)"GADGETUP" )) goto END_ERROR;
    if (! UIK_AddVector( o, (void*)print_event, IDCMP_GADGETDOWN, (ULONG)"GADGETDOWN" )) goto END_ERROR;
    if (! UIK_AddVector( o, (void*)print_event, IDCMP_MOUSEMOVE, (ULONG)"MOUSEMOVE" )) goto END_ERROR;
    if (! UIK_AddVector( o, (void*)print_event, IDCMP_INTUITICKS, (ULONG)"INTUITICKS" )) goto END_ERROR;

    if (! (o = UIK_AddObjectTags( "UIKObj_CheckToggle", wo,
            UIKTAG_OBJ_LeftTop,         W2L(20,30),
            UIKTAG_OBJ_WidthHeight,     SETL(22,15),
            UIKTAG_OBJ_Title,           TEXT_CheckToggle,
            UIKTAG_OBJ_TitleFl_Right,   TRUE,
            UIKTAG_OBJ_ShortCutKey,     W2L(B2W('T',0),0),
            TAG_END ))) goto END_ERROR;

    if (! (o = UIK_AddObjectTags( "UIKObj_CheckToggle", wo,
            UIKTAG_OBJ_LeftTop,         W2L(20,50),
            UIKTAG_OBJ_WidthHeight,     SETL(22,15),
            UIKTAG_OBJ_Title,           TEXT_CheckToggle,
            UIKTAG_OBJ_TitleFl_Right,   TRUE,
            UIKTAG_OBJ_ShortCutKey,     W2L(B2W('H',0),0),
            UIKTAG_OBJCheckFl_Radio,    TRUE,
            TAG_END ))) goto END_ERROR;
    if (! (o = UIK_AddObjectTags( "UIKObj_CheckToggle", wo,
            UIKTAG_OBJ_LeftTop,         W2L(20,66),
            UIKTAG_OBJ_WidthHeight,     SETL(22,15),
            UIKTAG_OBJ_Title,           TEXT_CheckToggle,
            UIKTAG_OBJ_TitleFl_Right,   TRUE,
            UIKTAG_OBJ_ShortCutKey,     W2L(B2W('E',0),0),
            UIKTAG_OBJCheckFl_Radio,    TRUE,
            TAG_END ))) goto END_ERROR;
    if (! (o = UIK_AddObjectTags( "UIKObj_CheckToggle", wo,
            UIKTAG_OBJ_LeftTop,         W2L(20,82),
            UIKTAG_OBJ_WidthHeight,     SETL(22,15),
            UIKTAG_OBJ_Title,           TEXT_CheckToggle,
            UIKTAG_OBJ_TitleFl_Right,   TRUE,
            UIKTAG_OBJ_ShortCutKey,     W2L(B2W('K',0),0),
            UIKTAG_OBJCheckFl_Radio,    TRUE,
            TAG_END ))) goto END_ERROR;

    /*--------------- test du "Exclude" */

    if (! (o = UIK_AddObjectTags( "UIKObj_RadioExclude", wo,
            UIKTAG_OBJ_LeftTop,         W2L(150,10),
            UIKTAG_OBJ_WidthHeight,     SETL(22,15),
            UIKTAG_OBJ_Title,           TEXT_RadioExclude,
            UIKTAG_OBJ_TitleFl_Right,   TRUE,
            UIKTAG_OBJRadioFl_Check,    TRUE,
            TAG_END ))) goto END_ERROR;

    if (! (o = UIK_AddObjectTags( "UIKObj_RadioExclude", wo,
            UIKTAG_OBJ_LeftTop,         W2L(150,30),
            UIKTAG_OBJ_WidthHeight,     SETL(22,15),
            UIKTAG_OBJ_Title,           TEXT_RadioExclude,
            UIKTAG_OBJ_TitleFl_Right,   TRUE,
            UIKTAG_OBJRadioFl_Check,    TRUE,
            UIKTAG_OBJ_BoxFl_NoPush,    FALSE,
            UIKTAG_OBJ_ActInactPens,    UIKCOLS(UIKCOL_LIGHT,UIKCOL_GREY,UIKCOL_BLACK,UIKCOL_BLACK),
            TAG_END ))) goto END_ERROR;

    if (! (o = UIK_AddObjectTags( "UIKObj_RadioExclude", wo,
            UIKTAG_OBJ_LeftTop,         W2L(150,50),
            UIKTAG_OBJ_WidthHeight,     SETL(22,15),
            UIKTAG_OBJ_Title,           TEXT_RadioExclude,
            UIKTAG_OBJ_TitleFl_Right,   TRUE,
            UIKTAG_OBJ_ShortCutKey,     W2L(B2W('X',0),0),
            UIKTAG_OBJ_ActInactPens,    UIKCOLS(UIKCOL_LIGHT,UIKCOL_GREY,UIKCOL_BLACK,UIKCOL_BLACK),
            TAG_END ))) goto END_ERROR;
    if (! (o = UIK_AddObjectTags( "UIKObj_RadioExclude", wo,
            UIKTAG_OBJ_LeftTop,         W2L(150,66),
            UIKTAG_OBJ_WidthHeight,     SETL(22,15),
            UIKTAG_OBJ_Title,           TEXT_RadioExclude,
            UIKTAG_OBJ_TitleFl_Right,   TRUE,
            UIKTAG_OBJ_ShortCutKey,     W2L(B2W('L',0),0),
            TAG_END ))) goto END_ERROR;
    if (! (o = UIK_AddObjectTags( "UIKObj_RadioExclude", wo,
            UIKTAG_OBJ_LeftTop,         W2L(150,82),
            UIKTAG_OBJ_WidthHeight,     SETL(22,15),
            UIKTAG_OBJ_Title,           TEXT_RadioExclude,
            UIKTAG_OBJ_TitleFl_Right,   TRUE,
            UIKTAG_OBJ_ShortCutKey,     W2L(B2W('U',0),0),
            TAG_END ))) goto END_ERROR;

    if (! UIK_Start( UIK )) goto END_ERROR;

    UIK_Do( UIK, SIGBREAKF_CTRL_C );

  END_ERROR:
    Cleanup();
}
