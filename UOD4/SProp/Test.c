/****************************************************************
 *
 *      Project:   UIK
 *      Function:  Test d'objets externes à la librairie
 *
 *      Created:   02/08/91    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikmacros.h"
#include "uiki:uikglobal.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"
#include "uiki:uikgfx.h"

#include "uiki:objwindow.h"
#include "uiki:objsproplift.h"

#include <stdio.h>

/****** Imported ************************************************/

extern ULONG DOSBase;

/****** Exported ************************************************/

struct UIKBase *UIKBase;
struct IntuitionBase *IntuitionBase;
struct GfxBase *GfxBase;


/****** Statics *************************************************/

static struct UIKGlobal *UIK;
struct UIKObjWindow *wo;
struct UIKObj *sprop;

static UBYTE *LangEnglish[] =
{
    "",
    "UIKObj_SPropLift",
    "UIKObj_SPropLift test, ©1993 DENIS Frédéric / \"The Software Laboratory\"",
    NULL
};

static UBYTE *LangOther[] =
{
    "",
    "UIKObj_SPropLift",
    "UIKObj_SPropLift test, ©1993 DENIS Frédéric / \"The Software Laboratory\"",
    NULL
};

static struct TagItem wtl[] = {  /* window */
    { UIKTAG_OBJ_LeftTop,       SETL(0,20) },
    { UIKTAG_OBJ_WidthHeight,   SETL(250,250) },
    { UIKTAG_OBJ_Title,         1 },
    { UIKTAG_OBJ_AltTitle,      2 },
    { UIKTAG_OBJ_ActInactPens,  UIKCOLS(UIKCOL_LIGHT,UIKCOL_GREY,UIKCOL_BLACK,UIKCOL_BLACK) },
    { UIKTAG_OBJWindow_MinimumWidthHeight, SETL(650,250) },
    { UIKTAG_OBJWindowFl_With_Zoom, FALSE },
    { TAG_END } };

/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void Cleanup()
{
   if (UIK)            UIK_Remove( UIK );
   if (GfxBase)        CloseLibrary( GfxBase );
   if (UIKBase)        CloseLibrary( UIKBase );
   if (IntuitionBase)  CloseLibrary( IntuitionBase );
   exit(0);
}

void main()
{

   if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) exit(20);
   if (! (IntuitionBase = (struct Intuition *) OpenLibrary( "intuition.library", 0 ))) exit(20);
   if (! (GfxBase = (struct GfxBase *) OpenLibrary( "graphics.library", 0 ))) exit(20);
   if (! (UIK = (struct UIKGlobal *) UIK_AddObjectTags( "UIK", 0,
             UIKTAG_GEN_LangEnglish,    (ULONG) LangEnglish,
             UIKTAG_GEN_LangOther,      (ULONG) LangOther,
             TAG_END ))) goto END_ERROR;

   if (! (wo = (struct UIKObjWindow *) UIK_AddObject( "UIKObj_Window", UIK, wtl ))) goto END_ERROR;
   if (! (sprop = UIK_AddObjectTags( "UIKObj_SPropLift", wo,
                                 UIKTAG_OBJ_LeftTop,                 SETL(10,10),
                                 UIKTAG_OBJ_BoxFl_TranslateLeftTop,  TRUE,
                                 UIKTAG_OBJSplFl_Vertical,           TRUE,
                                 UIKTAG_OBJSplFl_Stops,              TRUE,
                                 UIKTAG_OBJSplFl_Up,                 TRUE,
                                 UIKTAG_OBJSplFl_Down,               TRUE,
                                 UIKTAG_OBJSplFl_Both,               TRUE,
                                 TAG_END))) goto END_ERROR;
   if (! (sprop = UIK_AddObjectTags( "UIKObj_SPropLift", wo,
                                 UIKTAG_OBJ_LeftTop,                 SETL(40,10),
                                 UIKTAG_OBJ_BoxFl_TranslateLeftTop,  TRUE,
                                 UIKTAG_OBJSplFl_Vertical,           TRUE,
                                 UIKTAG_OBJSplFl_Stops,              TRUE,
                                 UIKTAG_OBJSplFl_Down,               TRUE,
                                 UIKTAG_OBJSplFl_Both,               TRUE,
                                 TAG_END))) goto END_ERROR;
   if (! (sprop = UIK_AddObjectTags( "UIKObj_SPropLift", wo,
                                 UIKTAG_OBJ_LeftTop,                 SETL(70,10),
                                 UIKTAG_OBJ_BoxFl_TranslateLeftTop,  TRUE,
                                 UIKTAG_OBJSplFl_Vertical,           TRUE,
                                 UIKTAG_OBJSplFl_Stops,              TRUE,
                                 UIKTAG_OBJSplFl_Up,                 TRUE,
                                 UIKTAG_OBJSplFl_Both,               TRUE,
                                 TAG_END))) goto END_ERROR;
   if (! (sprop = UIK_AddObjectTags( "UIKObj_SPropLift", wo,
                                 UIKTAG_OBJ_LeftTop,                 SETL(100,10),
                                 UIKTAG_OBJ_BoxFl_TranslateLeftTop,  TRUE,
                                 UIKTAG_OBJSplFl_Vertical,           TRUE,
                                 UIKTAG_OBJSplFl_Stops,              TRUE,
                                 UIKTAG_OBJSplFl_Up,                 TRUE,
                                 UIKTAG_OBJSplFl_Down,               TRUE,
                                 UIKTAG_OBJSpl_VArrowSize,            15,
                                 TAG_END))) goto END_ERROR;
   if (! (sprop = UIK_AddObjectTags( "UIKObj_SPropLift", wo,
                                 UIKTAG_OBJ_LeftTop,                 SETL(130,10),
                                 UIKTAG_OBJ_BoxFl_TranslateLeftTop,  TRUE,
                                 UIKTAG_OBJSplFl_Vertical,           TRUE,
                                 UIKTAG_OBJSplFl_Stops,              TRUE,
                                 UIKTAG_OBJSpl_VStopsSize,             15,
                                 UIKTAG_OBJ_Sz_AttachedFl_Right,     TRUE,
                                 //UIKTAG_OBJ_Sz_AttachedFl_Left,      TRUE,
                                 TAG_END))) goto END_ERROR;

   if (! (sprop = UIK_AddObjectTags( "UIKObj_SPropLift", wo,
                                 UIKTAG_OBJ_LeftTop,                 SETL(10,100),
                                 UIKTAG_OBJ_WidthHeight,             SETL(150,15),
                                 UIKTAG_OBJ_BoxFl_TranslateLeftTop,  TRUE,
                                 UIKTAG_OBJSplFl_Vertical,           FALSE,
                                 UIKTAG_OBJSplFl_Stops,              TRUE,
                                 UIKTAG_OBJSplFl_Up,                 TRUE,
                                 UIKTAG_OBJSplFl_Down,               TRUE,
                                 UIKTAG_OBJSplFl_Both,               TRUE,
                                 TAG_END))) goto END_ERROR;
   if (! (sprop = UIK_AddObjectTags( "UIKObj_SPropLift", wo,
                                 UIKTAG_OBJ_LeftTop,                 SETL(10,130),
                                 UIKTAG_OBJ_WidthHeight,             SETL(150,22),
                                 UIKTAG_OBJ_BoxFl_TranslateLeftTop,  TRUE,
                                 UIKTAG_OBJSplFl_Vertical,           FALSE,
                                 UIKTAG_OBJSplFl_Stops,              TRUE,
                                 UIKTAG_OBJSplFl_Down,               TRUE,
                                 UIKTAG_OBJSplFl_Both,               TRUE,
                                 UIKTAG_OBJSpl_HArrowSize,           15,
                                 UIKTAG_OBJSpl_HStopsSize,           20,
                                 TAG_END))) goto END_ERROR;
   if (! (sprop = UIK_AddObjectTags( "UIKObj_SPropLift", wo,
                                 UIKTAG_OBJ_LeftTop,                 SETL(10,160),
                                 UIKTAG_OBJ_WidthHeight,             SETL(100,22),
                                 UIKTAG_OBJ_BoxFl_TranslateLeftTop,  TRUE,
                                 UIKTAG_OBJSplFl_Vertical,           FALSE,
                                 UIKTAG_OBJSplFl_Stops,              TRUE,
                                 UIKTAG_OBJSplFl_Up,                 TRUE,
                                 UIKTAG_OBJSplFl_Both,               TRUE,
                                 TAG_END))) goto END_ERROR;
   if (! (sprop = UIK_AddObjectTags( "UIKObj_SPropLift", wo,
                                 UIKTAG_OBJ_LeftTop,                 SETL(10,190),
                                 UIKTAG_OBJ_WidthHeight,             SETL(100,22),
                                 UIKTAG_OBJ_BoxFl_TranslateLeftTop,  TRUE,
                                 UIKTAG_OBJSplFl_Vertical,           FALSE,
                                 UIKTAG_OBJSplFl_Stops,              TRUE,
                                 UIKTAG_OBJSplFl_Up,                 TRUE,
                                 UIKTAG_OBJSplFl_Down,               TRUE,
                                 TAG_END))) goto END_ERROR;
   if (! (sprop = UIK_AddObjectTags( "UIKObj_SPropLift", wo,
                                 UIKTAG_OBJ_LeftTop,                 SETL(10,220),
                                 UIKTAG_OBJ_WidthHeight,             SETL(100,22),
                                 UIKTAG_OBJ_BoxFl_TranslateLeftTop,  TRUE,
                                 UIKTAG_OBJSplFl_Vertical,           FALSE,
                                 UIKTAG_OBJSplFl_Stops,              TRUE,
                                 TAG_END))) goto END_ERROR;
   if (! UIK_Start( UIK )) goto END_ERROR;

   UIK_Do( UIK, SIGBREAKF_CTRL_C );

   END_ERROR: Cleanup();
}

