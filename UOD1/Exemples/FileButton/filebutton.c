/****************************************************************
 *
 *      File:      filebutton.c
 *      Project:   Essai de l'objet UIKObj_FileButton
 *
 *      Created:   22/02/93     Jean-Michel Forgeas
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

#include "uiki:objwfilebut.h"

#define TEXT_WindowTitle    1
#define TEXT_ScreenTitle    2
#define TEXT_OpenFile       3
#define TEXT_File           4
#define TEXT_Directory      5
#define TEXT_Points         6
#define TEXT_Standard       7
#define TEXT_Icons          8
#define TEXT_OpenDir        9


/****** Imported ************************************************/

extern ULONG __far DOSBase;


/****** Exported ***********************************************/

struct UIKBase * __far UIKBase;


/****** Statics ************************************************/

static struct UIKGlobal * __far UIK;

static UBYTE * __far LangEnglish[] =
    {
    "",
    "FileButton Test",
    "FileButton Test 1.1 Copyright © 1993 J-M Forgeas",
    "File to Open",
    "File",
    "Directory",
    "...",
    "Standard",
    "Icons",
    "Directory choice",
    0
    };

static UBYTE * __far LangOther[] =
    {
    "",
    "FileButton Test",
    "FileButton Test 1.1 Copyright © 1993 J-M Forgeas",
    "Fichier à ouvrir",
    "Fichier",
    "Répertoire",
    "...",
    "Standard",
    "Icônes",
    "Choix de répertoire",
    0
    };

#include "uiki:uikgfx.h"

static UWORD chip BrushData[5][4][1] =
   {
   0xffc0,    /* ##########...... */
   0x8040,    /* #........#...... */
   0x8040,    /* #........#...... */
   0xffc0,    /* ##########...... */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0xffc0,    /* ##########...... */
   0x8040,    /* #........#...... */
   0x8040,    /* #........#...... */
   0xffc0,    /* ##########...... */
   };
struct UIKBitMap __far UBM_Brush = { &BrushData[4][0][0], UIKBMAP_MASKED|UIKBMAP_CONTIG,
    10, 2,4, 0,4,0, &BrushData[0][0][0], &BrushData[1][0][0], &BrushData[2][0][0], &BrushData[3][0][0], 0, 0, 0, 0 };

struct UIKBitMap * __far UIKBitMapTable[] = {
    &UBM_Brush,
    0
    };

/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void DisplayFile( struct UIKObjWFileBut *wfb, UBYTE *name )
{
    printf( "'%ls'\n", name );
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
  UWORD winheight, top, sh;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (UIK = (struct UIKGlobal *) UIK_AddObjectTags( "UIK", 0,
            UIKTAG_GEN_LangEnglish,    (ULONG) LangEnglish,
            UIKTAG_GEN_LangOther,      (ULONG) LangOther,
            TAG_END ))) goto END_ERROR;

    UIK_InvertBlackWhite( UIK, UIKBitMapTable, 1 );

    sh = UIKBase->DefaultFont->tf_YSize + 5;
    winheight = 5           /* top                               */
                + (sh * 3)  /* 3 string auto-height (documented) */
                + 24        /* 1 string of height = 24           */
                + (2 * 3)   /* 3 inter-strings                   */
                + 5;        /* bottom                            */

    if (! (wo = UIK_AddObjectTags( "UIKObj_Window", UIK,
            UIKTAG_OBJ_LeftTop,             SETL(100,50),
            UIKTAG_OBJ_WidthHeight,         SETL(460,winheight),
            UIKTAG_OBJ_Title,               TEXT_WindowTitle,
            UIKTAG_OBJ_AltTitle,            TEXT_ScreenTitle,
            TAG_END ))) goto END_ERROR;
    top = 5;

    if (! (o = UIK_AddObjectTags( "UIKObj_WFileButton", wo,
            UIKTAG_OBJ_LeftTop,         W2L(80,top),
            UIKTAG_OBJ_WidthHeight,     W2L(290,0),
            UIKTAG_OBJ_Title,           TEXT_Standard,
            UIKTAG_OBJ_TitleFl_Left,    TRUE,
            UIKTAG_OBJ_ShortCutKey,     W2L(B2W('S',0),0),
            UIKTAG_WFB_OKFunc,         DisplayFile,
            TAG_END ))) goto END_ERROR;
    top += sh + 2;

    if (! (o = UIK_AddObjectTags( "UIKObj_WFileButton", wo,
            UIKTAG_OBJ_LeftTop,         W2L(80,top),
            UIKTAG_OBJ_WidthHeight,     W2L(290,0),
            UIKTAG_OBJ_Title,           TEXT_File,
            UIKTAG_OBJ_TitleFl_Left,    TRUE,
            UIKTAG_OBJ_ShortCutKey,     W2L(B2W('E',0),0),
            UIKTAG_WFB_SelectorTitle,   TEXT_OpenFile,
            UIKTAG_WFB_SelectorAltTitle,TEXT_ScreenTitle,
            UIKTAG_WFB_ButtonTitle,     TEXT_File,
            UIKTAG_WFB_ButtonShortCut,  W2L(B2W('F',0),0),
            UIKTAG_WFBFl_StringRight,   TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Right, TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Left, TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Bot, TRUE,
            UIKTAG_WFB_OKFunc,         DisplayFile,
            TAG_END ))) goto END_ERROR;
    top += sh + 2;

    if (! (o = UIK_AddObjectTags( "UIKObj_WFileButton", wo,
            UIKTAG_OBJ_LeftTop,         W2L(80,top),
            UIKTAG_OBJ_WidthHeight,     W2L(290,24),
            UIKTAG_OBJ_Title,           TEXT_Directory,
            UIKTAG_OBJ_TitleFl_Right,   TRUE,
            UIKTAG_OBJ_ShortCutKey,     W2L(B2W('T',0),0),
            UIKTAG_WFB_SelectorTitle,   TEXT_OpenDir,
            UIKTAG_WFB_SelectorAltTitle,TEXT_ScreenTitle,
            UIKTAG_WFB_ButtonShortCut,  W2L(B2W('R',0),0),
            UIKTAG_WFB_ButtonTitle,     TEXT_Directory,
            UIKTAG_WFB_ButtonAltTitle,  TEXT_Points,
            UIKTAG_WFBFl_AutoHeight,    FALSE,
            UIKTAG_WFBFl_DirsOnly,      TRUE,
            UIKTAG_WFBFl_ButtonNoPush,  TRUE,
            UIKTAG_WFB_ButtonActInactPens, UIKCOLS(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_BLACK,UIKCOL_WHITE),
            UIKTAG_OBJ_Sz_AttachedFl_Right, TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Left, TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Bot, TRUE,
            UIKTAG_WFB_OKFunc,         DisplayFile,
            TAG_END ))) goto END_ERROR;
    top += 24 + 2;

    if (! (o = UIK_AddObjectTags( "UIKObj_WFileButton", wo,
            UIKTAG_OBJ_LeftTop,         W2L(80,top),
            UIKTAG_OBJ_WidthHeight,     W2L(290,0),
            UIKTAG_OBJ_Title,           TEXT_Icons,
            UIKTAG_OBJ_TitleFl_Right,   TRUE,
            UIKTAG_OBJ_ShortCutKey,     W2L(B2W('I',0),0),
            UIKTAG_WFB_ButtonTitle,     &UBM_Brush,
            UIKTAG_WFB_Pattern,         "#?.info",
            UIKTAG_WFBFl_ButtonImage,   TRUE,
            UIKTAG_WFBFl_ShowIcons,     TRUE,
            UIKTAG_WFBFl_ButtonNoPush,  TRUE,
            UIKTAG_WFB_ButtonWidth,     30,
            UIKTAG_OBJ_Sz_AttachedFl_Bot, TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Right, TRUE,
            UIKTAG_WFB_OKFunc,         DisplayFile,
            TAG_END ))) goto END_ERROR;

    if (! UIK_Start( UIK )) goto END_ERROR;

    UIK_Do( UIK, SIGBREAKF_CTRL_C );

  END_ERROR:
    Cleanup();
}
