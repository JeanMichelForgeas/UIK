/****************************************************************
 *
 *      File:      MultI.c
 *      Project:   UIK example code
 *
 *      Created:   06-10-92     Jean-Michel Forgeas
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
#include <exec/execbase.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikgfx.h"
#include "uiki:uikmacros.h"
#include "uiki:uikglobal.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objwindow.h"

#define TEXT_WindowTitle    1
#define TEXT_ScreenTitle    2
#define TEXT_DessinTitle    3


/****** Imported ************************************************/

extern struct UIKBitMap * __far UIKBitMapTable[];


/****** Exported ***********************************************/

struct UIKBase * UIKBase;
struct UIKGlobal * UIK;


/****** Statics ************************************************/

static UBYTE *LangEnglish[] =
    {
    "",
    "Multi",
    "Multi Copyright © 1992 Jean-Michel Forgeas",
    "Drawings by Philippe Ducalet",
    0
    };

static UBYTE *LangOther[] =
    {
    "",
    "Multi",
    "Multi Copyright © 1992 Jean-Michel Forgeas",
    "Dessins de Philippe Ducalet",
    0
    };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void Cleanup()
{
    if (UIK)     UIK_Remove( UIK );
    if (UIKBase) CloseLibrary( UIKBase );
    exit(0);
}

void main()
{
  struct UIKObj *wo;
  struct UIKObj *obj;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;

    UIK_InvertBlackWhite( UIK, UIKBitMapTable, 1 );

    if (! (UIK = (struct UIKGlobal *) UIK_AddObjectTags( "UIK", 0,
             UIKTAG_GEN_LangEnglish,    (ULONG) LangEnglish,
             UIKTAG_GEN_LangOther,      (ULONG) LangOther,
             TAG_END ))) goto END_ERROR;

    if (! (wo = UIK_AddObjectTags( "UIKObj_Window", UIK, /* fenêtre */
            UIKTAG_OBJ_LeftTop,         SETL(180,30),
            UIKTAG_OBJ_WidthHeight,     SETL(214,120),
            UIKTAG_OBJ_Title,           TEXT_WindowTitle,
            UIKTAG_OBJ_AltTitle,        TEXT_ScreenTitle,
            TAG_END ))) goto END_ERROR;

    if (! (obj = UIK_AddObjectTags( "UIKObj_Toggle", wo,  /* toggle */
            UIKTAG_OBJ_LeftTop,         SETL(9,7),
            UIKTAG_OBJ_WidthHeight,     SETL(84,86),
            UIKTAG_OBJ_TitleFl_Image,   TRUE,
            UIKTAG_OBJ_Title,           (ULONG) &UIKBitMapTable[0],
            UIKTAG_OBJ_MultiTitleDelay, (((struct ExecBase *)*((ULONG*)4))->VBlankFrequency/10),
            UIKTAG_OBJ_MultiTitleNum,   10,
            TAG_END ))) goto END_ERROR;

    if (! (obj = UIK_AddObjectTags( "UIKObj_Toggle", wo,  /* toggle */
            UIKTAG_OBJ_LeftTop,         SETL(120,7),
            UIKTAG_OBJ_WidthHeight,     SETL(84,86),
            UIKTAG_OBJ_TitleFl_Image,   TRUE,
            UIKTAG_OBJ_Title,           (ULONG) &UIKBitMapTable[0],
            UIKTAG_OBJ_MultiTitleDelay, 0,
            UIKTAG_OBJ_MultiTitleNum,   10,
            TAG_END ))) goto END_ERROR;

    if (! (obj = UIK_AddObjectTags( "UIKObj_Box", wo,  /* box */
            UIKTAG_OBJ_LeftTop,         SETL(6,103),
            UIKTAG_OBJ_WidthHeight,     SETL(1,8),
            UIKTAG_OBJ_BoxFl_NotDrawn,  TRUE,
            UIKTAG_OBJ_Title,           TEXT_DessinTitle,
            UIKTAG_OBJ_TitleFl_Right,   TRUE,
            TAG_END ))) goto END_ERROR;

    if (! UIK_Start( UIK )) goto END_ERROR;

    UIK_Do( UIK, SIGBREAKF_CTRL_C );

  END_ERROR:
    Cleanup();
}
