/****************************************************************
 *
 *      File:      ListProp.c
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
#define TEXT_ListTitle      3
#define TEXT_ListEntry      4

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

extern ULONG DOSBase;


/****** Exported ***********************************************/

struct UIKBase *UIKBase;
struct UIKGlobal *UIK;

struct UIKObj *Up;
struct UIKObj *Down;
struct UIKObj *List;


/****** Statics ************************************************/

static UBYTE *LangEnglish[] =
    {
    "",
    "ListProp",
    "ListProp Copyright © 1992 Jean-Michel Forgeas",
    "The List",
    "Hello1\nHello2\nHello3\nHello4\nHello5\nHello6\nHello7\nHello8\nHello9\nHello10\nHello11\nHello12",
    0
    };

static UBYTE *LangOther[] =
    {
    "",
    "ListProp",
    "ListProp Copyright © 1992 Jean-Michel Forgeas",
    "La liste",
    "bonjour1\n"       /* tous les "bonjourN\n" sont concaténés par le compilateur */
        "bonjour2\n"
        "bonjour3\n"
        "bonjour4\n"
        "bonjour5\n"
        "bonjour6\n"
        "bonjour7\n"
        "bonjour8\n"
        "bonjour9\n"
        "bonjour10\n"
        "bonjour11\n"
        "bonjour12",
    0
    };

static UWORD PatProp[] = { 0xAAAA, 0x5555 };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void ResizeUpDown( struct UIKObjWindow *wo )
{
  int size;

    /*--------- élargir les flêches relativement à la liste ---------*/
    size = List->Box.Width;
    Down->Box.Left = Up->Box.Left + size/2;
    Up->Box.Width = Down->Box.Left - Up->Box.Left;
    Down->Box.Width = size - Up->Box.Width;

    (*Down->ResizeVector)( Down );
    (*Up->ResizeVector)( Up );
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
  struct UIKObj *oprop;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (UIK = (struct UIKGlobal *) UIK_AddObjectTags( "UIK", 0,
             UIKTAG_GEN_LangEnglish,    (ULONG) LangEnglish,
             UIKTAG_GEN_LangOther,      (ULONG) LangOther,
             TAG_END ))) goto END_ERROR;

    if (! (wo = UIK_AddObjectTags( "UIKObj_Window", UIK, /* fenêtre */
            UIKTAG_OBJ_LeftTop,       SETL(180,30),
            UIKTAG_OBJ_WidthHeight,   SETL(250,104),
            UIKTAG_OBJ_Title,         TEXT_WindowTitle,
            UIKTAG_OBJ_AltTitle,      TEXT_ScreenTitle,
            TAG_END ))) goto END_ERROR;

    if (! (List = UIK_AddObjectTags( "UIKObj_List", wo,  /* liste */
            UIKTAG_OBJ_LeftTop,       SETL(10,15),
            UIKTAG_OBJ_WidthHeight,   SETL(200,70),
            UIKTAG_OBJ_Title,         TEXT_ListTitle,
            UIKTAG_OBJ_TitleFl_Above, TRUE,
            UIKTAG_OBJListFl_ClickSel,FALSE,
            UIKTAG_OBJListFl_SelDispl,FALSE,
            UIKTAG_OBJList_TextPtr,   (ULONG) UIK_LangString( UIK, TEXT_ListEntry ),
            UIKTAG_OBJ_Sz_AttachedFl_Top,   TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Left,  TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Right, TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Bot,   TRUE,
            TAG_END ))) goto END_ERROR;

    if (! (oprop = UIK_AddObjectTags( "UIKObj_PropLift", wo, /* ascenceur */
            UIKTAG_OBJ_LeftTop,       SETL(215,15),
            UIKTAG_OBJ_WidthHeight,   SETL(20,70),
            UIKTAG_OBJ_BoxFl_Over,    FALSE,
            UIKTAG_OBJ_Sz_AttachedFl_Top,  TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Right,TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Bot,  TRUE,
            UIKTAG_OBJProp_Vertical,  TRUE,
            UIKTAG_OBJ_Pattern,       (ULONG)PatProp,
            UIKTAG_OBJ_PatternPower,  1,
            TAG_END ))) goto END_ERROR;

    if (! (Up = UIK_AddObjectTags( "UIKObj_Up", wo,     /* haut */
            UIKTAG_OBJ_LeftTop,       SETL(10,87),
            UIKTAG_OBJ_WidthHeight,   SETL(100,12),
            UIKTAG_OBJ_Sz_AttachedFl_Bot,  TRUE,
            UIKTAG_OBJ_BoxFl_Clipped,  TRUE,
            TAG_END ))) goto END_ERROR;
    if (! (Down = UIK_AddObjectTags( "UIKObj_Down", wo, /* bas */
            UIKTAG_OBJ_LeftTop,       SETL(110,87),
            UIKTAG_OBJ_WidthHeight,   SETL(100,12),
            UIKTAG_OBJ_Sz_AttachedFl_Bot,  TRUE,
            TAG_END ))) goto END_ERROR;

    /* On lie l'ascenceur à la liste */
    if (! UIK_Link( oprop, List, 0, MOUSEMOVE|GADGETDOWN, 0 )) goto END_ERROR;

    /* On lie les boutons Up et Down à l'ascenceur */
    if (! UIK_Link( Up, oprop, 0, GADGETDOWN|INTUITICKS|MOUSEMOVE, 0 )) goto END_ERROR;
    if (! UIK_Link( Down, oprop, 0, GADGETDOWN|INTUITICKS|MOUSEMOVE, 0 )) goto END_ERROR;

    /* On ajoute un vecteur à la fenêtre pour retailler les 2 boutons */
    if (! UIK_AddVector( wo, (void *)ResizeUpDown, NEWSIZE, 0 )) goto END_ERROR;

    if (! UIK_Start( UIK )) goto END_ERROR;

    UIK_Do( UIK, SIGBREAKF_CTRL_C );

  END_ERROR:
    Cleanup();
}
