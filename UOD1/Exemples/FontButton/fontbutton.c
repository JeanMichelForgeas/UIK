/****************************************************************
 *
 *      File:      fontbutton.c
 *      Project:   Essai de l'objet UIKObj_FontButton
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

#include "uiki:objwfontbut.h"

#define TEXT_WindowTitle    1
#define TEXT_ScreenTitle    2
#define TEXT_OpenFont       3
#define TEXT_Font           4
#define TEXT_Other          5
#define TEXT_Points         6
#define TEXT_Standard       7
#define TEXT_Bad            8
#define TEXT_OpenDir        9

#ifdef LATTICE
int CXBRK(void) { return(0); }      /* disable SASC CTRLC handling */
int chkabort(void) { return(0); }   /* really */
#endif


/****** Imported ************************************************/

extern ULONG __far DOSBase;


/****** Exported ***********************************************/

struct UIKBase * __far UIKBase;


/****** Statics ************************************************/

static struct UIKGlobal * __far UIK;

static UBYTE * __far LangEnglish[] =
    {
    "",
    "FontButton Test",
    "FontButton Test 1.0 Copyright © 1993 J-M Forgeas",
    "Font to Open",
    "Font",
    "Other",
    "...",
    "Standard",
    "Bad",
    "Font choice",
    0
    };

static UBYTE * __far LangOther[] =
    {
    "",
    "FontButton Test",
    "FontButton Test 1.0 Copyright © 1993 J-M Forgeas",
    "Fonte à ouvrir",
    "Fonte",
    "Autre",
    "...",
    "Standard",
    "Mauvais",
    "Choix de Font",
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
static struct UIKBitMap __far UBM_Brush = { &BrushData[4][0][0], UIKBMAP_MASKED|UIKBMAP_CONTIG,
    10, 2,4, 0,4,0, &BrushData[0][0][0], &BrushData[1][0][0], &BrushData[2][0][0], &BrushData[3][0][0], 0, 0, 0, 0 };

struct UIKBitMap * __far UIKBitMapTable[] = {
    &UBM_Brush,
    0
    };

static struct TTextAttr __far tta = { "topaz.font", 11 };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void DisplayFont( struct UIKObjWFontBut *wfb, struct TTextAttr *tta )
{
    printf( "%ls %ld (%lx)\n", tta->tta_Name, tta->tta_YSize, tta->tta_Style );
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
    winheight = 5           /* top                                */
                + (sh * 3)  /* 3 strings auto-height (documented) */
                + 24        /* 1 string of height = 24            */
                + (2 * 3)   /* 3 inter-strings                    */
                + 5;        /* bottom                             */

    if (! (wo = UIK_AddObjectTags( "UIKObj_Window", UIK,
            UIKTAG_OBJ_LeftTop,             SETL(100,50),
            UIKTAG_OBJ_WidthHeight,         SETL(450,winheight),
            UIKTAG_OBJ_Title,               TEXT_WindowTitle,
            UIKTAG_OBJ_AltTitle,            TEXT_ScreenTitle,
            TAG_END ))) goto END_ERROR;
    top = 5;

    if (! (o = UIK_AddObjectTags( "UIKObj_WFontButton", wo,
            UIKTAG_OBJ_LeftTop,         W2L(80,top),
            UIKTAG_OBJ_WidthHeight,     W2L(290,0),
            UIKTAG_OBJ_Title,           TEXT_Standard,
            UIKTAG_OBJ_TitleFl_Left,    TRUE,
            UIKTAG_OBJ_ShortCutKey,     W2L(B2W('S',0),0),
            UIKTAG_WFOB_OKFunc,         DisplayFont,
            TAG_END ))) goto END_ERROR;
    top += sh + 2;

    if (! (o = UIK_AddObjectTags( "UIKObj_WFontButton", wo,
            UIKTAG_OBJ_LeftTop,         W2L(80,top),
            UIKTAG_OBJ_WidthHeight,     W2L(290,0),
            UIKTAG_OBJ_Title,           TEXT_Font,
            UIKTAG_OBJ_TitleFl_Left,    TRUE,
            UIKTAG_OBJ_ShortCutKey,     W2L(B2W('E',0),0),
            UIKTAG_WFOB_SelectorTitle,   TEXT_OpenFont,
            UIKTAG_WFOB_SelectorAltTitle,TEXT_ScreenTitle,
            UIKTAG_WFOB_OKFunc,          DisplayFont,
            UIKTAG_WFOB_ButtonTitle,     TEXT_Font,
            UIKTAG_WFOB_ButtonShortCut,  W2L(B2W('F',0),0),
            UIKTAG_WFOBFl_StringRight,   TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Right, TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Left, TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Bot, TRUE,
            TAG_END ))) goto END_ERROR;
    top += sh + 2;

    if (! (o = UIK_AddObjectTags( "UIKObj_WFontButton", wo,
            UIKTAG_OBJ_LeftTop,         W2L(80,top),
            UIKTAG_OBJ_WidthHeight,     W2L(290,24),
            UIKTAG_OBJ_Title,           TEXT_Other,
            UIKTAG_OBJ_TitleFl_Right,   TRUE,
            UIKTAG_OBJ_ShortCutKey,     W2L(B2W('T',0),0),
            UIKTAG_WFOB_SelectorTitle,   TEXT_OpenDir,
            UIKTAG_WFOB_SelectorAltTitle,TEXT_ScreenTitle,
            UIKTAG_WFOB_OKFunc,          DisplayFont,
            UIKTAG_WFOB_ButtonShortCut,  W2L(B2W('R',0),0),
            UIKTAG_WFOB_ButtonTitle,     TEXT_Other,
            UIKTAG_WFOB_ButtonAltTitle,  TEXT_Points,
            UIKTAG_WFOBFl_AutoHeight,    FALSE,
            UIKTAG_WFOB_HighlightFont,   &tta,
            UIKTAG_WFOBFl_ButtonNoPush,  TRUE,
            UIKTAG_WFOB_ButtonActInactPens, UIKCOLS(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_BLACK,UIKCOL_WHITE),
            UIKTAG_OBJ_Sz_AttachedFl_Right, TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Left, TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Bot,  TRUE,
            TAG_END ))) goto END_ERROR;
    top += 24 + 2;

    if (! (o = UIK_AddObjectTags( "UIKObj_WFontButton", wo,
            UIKTAG_OBJ_LeftTop,         W2L(80,top),
            UIKTAG_OBJ_WidthHeight,     W2L(290,0),
            UIKTAG_OBJ_Title,           TEXT_Bad,
            UIKTAG_OBJ_TitleFl_Right,   TRUE,
            UIKTAG_WFOB_ButtonTitle,    &UBM_Brush,
            UIKTAG_WFOB_FONTSPATH,      "SYS:ZOEUYZOIEUYZOIUEY",
            UIKTAG_WFOBFl_ButtonImage,  TRUE,
            UIKTAG_WFOBFl_ButtonNoPush, TRUE,
            UIKTAG_WFOB_ButtonWidth,    30,
            UIKTAG_OBJ_Sz_AttachedFl_Bot,   TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Right, TRUE,
            TAG_END ))) goto END_ERROR;

    if (! UIK_Start( UIK )) goto END_ERROR;

    UIK_Do( UIK, SIGBREAKF_CTRL_C );

  END_ERROR:
    Cleanup();
}
