/****************************************************************
 *
 *      File:      wmenu.c
 *      Project:   semblant de menu par des gadgets
 *
 *      Created:   04-02-91     Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include <stdio.h>

#ifdef LATTICE
int CXBRK(void) { return(0); }
int chkabort(void) { return(0); }
#endif
#ifdef SAS
int CXBRK    ( void ) { return(0); }
int _CXBRK   ( void ) { return(0); }
int chkabort ( void ) { return(0); }
#endif

#include <libraries/dos.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikglobal.h"
#include "uiki:uikcolors.h"
#include "uiki:uikmacros.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objwindow.h"

#define TEXT_WindowTitle    1
#define TEXT_ScreenTitle    2

#define TEXT_MenuTitle      3
#define TEXT_MenuSequence   4
#define TEXT_MenuKeyboard   5


/****** Imported ************************************************/

extern ULONG DOSBase;


/****** Exported ***********************************************/

struct UIKBase *UIKBase;
struct GfxBase *GfxBase;
struct IntuitionBase *IntuitionBase;

struct ObjFrExe *ObjFrExe;

UBYTE *LangEnglish[] =
    {
    "",
    "WMenu",
    "WMenu 1.0 Copyright © 1991 J-M Forgeas",
    "WMenu",
    "Sequences",
    "Keyboard",
    0
    };

UBYTE *LangOther[] =
    {
    "",
    "WMenu",
    "WMenu 1.0 Copyright © 1991 J-M Forgeas",
    "WMenu",
    "Séquences",
    "Clavier",
    0
    };


/****** Statics ************************************************/

static struct UIKGlobal *UIK;

static struct TagItem TL_Window[] = {
    { UIKTAG_OBJ_LeftTop,       30 },
    { UIKTAG_OBJ_WidthHeight,   0 },
    { UIKTAG_OBJ_Title,         TEXT_MenuTitle },
    { UIKTAG_OBJ_AltTitle,      TEXT_ScreenTitle },
    { UIKTAG_OBJ_ActInactPens,  UIKCOL_LIGHT<<24 | UIKCOL_GREY<<16 | UIKCOL_WHITE<<8 | UIKCOL_BLACK },
    { UIKTAG_OBJWindowFl_With_Icon, FALSE },
    { UIKTAG_OBJWindowFl_With_Size, FALSE },
    { TAG_END } };

static struct TagItem TL_Toggle[] = {
    { UIKTAG_OBJ_LeftTop,       0 },
    { UIKTAG_OBJ_WidthHeight,   0 },
    { UIKTAG_OBJ_Title,         0 },
    { UIKTAG_OBJ_BoxFl_Filled,  TRUE },
    { UIKTAG_OBJ_ActInactPens,  UIKCOL_WHITE<<24 | UIKCOL_GREY<<16 | UIKCOL_BLACK<<8 | UIKCOL_WHITE },
    { UIKTAG_OBJ_BoxFl_NoPush,  TRUE },
    { TAG_END } };

/*-----------window Sequences */
static struct TagItem TL_SubWin[] = {
    { UIKTAG_OBJ_LeftTop,       0 },
    { UIKTAG_OBJ_WidthHeight,   0 },
    { UIKTAG_OBJ_Title,         TEXT_WindowTitle },
    { UIKTAG_OBJ_AltTitle,      TEXT_ScreenTitle },
    { UIKTAG_OBJ_ActInactPens,  UIKCOL_LIGHT<<24 | UIKCOL_GREY<<16 | UIKCOL_WHITE<<8 | UIKCOL_BLACK },
    { UIKTAG_OBJWindowFl_With_Icon, FALSE },
    { UIKTAG_OBJWindowFl_With_Size, FALSE },
    { TAG_END } };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void StartStopWindow( struct UIKObjGad *to, struct UIKObj *wo )
{
    if (! (to->Obj.Flags & BOX_OVER))
        {
        if (wo->Status != UIKS_STARTED) UIK_Start( wo );
        }
    else{
        if (wo->Status == UIKS_STARTED) UIK_Stop( wo );
        }
}

static void Cleanup()
{
    if (UIK)            UIK_Remove( UIK );
    if (UIKBase)        CloseLibrary( UIKBase );
    if (GfxBase)        CloseLibrary( (struct Library *) GfxBase );
    if (IntuitionBase)  CloseLibrary( (struct Library *) IntuitionBase );
    exit(0);
}

void main()
{
  UWORD fontheight, gadgetheight, top;
  struct UIKObj *wo, *o1, *o2, *obj;

    if (! (IntuitionBase = (struct IntuitionBase *) OpenLibrary( "intuition.library", 0 ))) goto END_ERROR;
    if (! (GfxBase = (struct GfxBase *) OpenLibrary( "graphics.library", 0 ))) goto END_ERROR;
    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (UIK = (struct UIKGlobal *) UIK_AddObjectTags( "UIK", 0,
             UIKTAG_GEN_LangEnglish,    (ULONG) LangEnglish,
             UIKTAG_GEN_LangOther,      (ULONG) LangOther,
             TAG_END ))) goto END_ERROR;

    /*----------------------Menu */
    fontheight = UIKBase->DefaultFont->tf_YSize;
    gadgetheight = fontheight + 6;

    TL_Window[1].ti_Data = SETL(116, (2 * gadgetheight));
    if (! (wo = UIK_AddObject( "UIKObj_Window", UIK, TL_Window ))) goto END_ERROR;

    top = 0;
    TL_Toggle[0].ti_Data = SETL(0,top);
    TL_Toggle[1].ti_Data = SETL(116,gadgetheight);
    TL_Toggle[2].ti_Data = TEXT_MenuSequence;
    if (! (o1 = UIK_AddObject( "UIKObj_Toggle", wo, TL_Toggle ))) goto END_ERROR;

    top += gadgetheight;
    TL_Toggle[0].ti_Data = SETL(0,top);
    TL_Toggle[1].ti_Data = SETL(116,gadgetheight);
    TL_Toggle[2].ti_Data = TEXT_MenuKeyboard;
    if (! (o2 = UIK_AddObject( "UIKObj_Toggle", wo, TL_Toggle ))) goto END_ERROR;

    /*========================================*/
    if (! UIK_Start( UIK )) goto END_ERROR;
    /*========================================*/

    /*---------------------------Fenêtre 'Sequence' */
    TL_SubWin[0].ti_Data = SETL(120,0);
    TL_SubWin[1].ti_Data = SETL(390,150);
    TL_SubWin[2].ti_Data = TEXT_MenuSequence;
    if (! (obj = UIK_AddObject( "UIKObj_Window", o1, TL_SubWin ))) goto END_ERROR;
    if (! UIK_Link( obj, o1, 0, CLOSEWINDOW, 0 )) goto END_ERROR;
    if (! UIK_AddVector( o1, (void*)StartStopWindow, GADGETUP, (ULONG)obj )) goto END_ERROR;

    /*---------------------------Fenêtre 'Clavier' */
    TL_SubWin[0].ti_Data = SETL(0,160);
    TL_SubWin[1].ti_Data = SETL(600,30);
    TL_SubWin[2].ti_Data = TEXT_MenuKeyboard;
    if (! (obj = UIK_AddObject( "UIKObj_Window", o2, TL_SubWin ))) goto END_ERROR;
    if (! UIK_Link( obj, o2, 0, CLOSEWINDOW, 0 )) goto END_ERROR;
    if (! UIK_AddVector( o2, (void*)StartStopWindow, GADGETUP, (ULONG)obj )) goto END_ERROR;

    UIK_Do( UIK, SIGBREAKF_CTRL_C );

  END_ERROR:
    Cleanup();
}
