/****************************************************************
 *
 *      File:      Palette.c
 *      Project:   UIK example code
 *
 *      Created:   25/08/93     Jean-Michel Forgeas
 *
 *      L'objet UIKObj_Palette testé est une réalisation commune
 *      de David Scrève, Frédéric Denis et Jean-Michel Forgeas.
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

#include <exec/types.h>
#include <intuition/screens.h>
#include <libraries/dos.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uiktree.h"
#include "uiki:uikmacros.h"
#include "uiki:uikglobal.h"
#include "uiki:uikcolors.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objscreen.h"
#include "uiki:objwindow.h"
#include "uiki:objpalette.h"
#include "uiki:objstring.h"

#define TEXT_WindowTitle    1
#define TEXT_PaletteTitle   2
#define TEXT_ScreenTitle    3
#define TEXT_GadgetTitle    4
#define TEXT_ColSelected    5
#define TEXT_ColModified    6
#define TEXT_ColorsOK       7
#define TEXT_ColorsCancel   8
#define TEXT_ColorsRestore  9
#define TEXT_PaletteStrings 10
#define TEXT_Toggle1Title   15
#define TEXT_StringTitle    16


/****** Imported ************************************************/


/****** Exported ***********************************************/

struct UIKBase *UIKBase;


/****** Statics ************************************************/

static void open_palette( struct UIKObj *obj );
static void color_ok( struct UIKObjPalette *op );
static void color_cancel( struct UIKObjPalette *op );
static void color_restore( struct UIKObjPalette *op );
static void color_modify( struct UIKObjPalette *op, struct UIKColorDef *ucd );
static void color_select( struct UIKObjPalette *op, ULONG color );

static struct UIKGlobal *UIK;
static struct UIKObj *ObjWin, *ObjGad, *ObjTog1, *ObjStr;
static struct UIKObj *ObjPalette;

static ULONG StringResult;

/*-------------------------- Langages -------------------------*/
static UBYTE *LangEnglish[] =
    {
    "",
    "Palette example",
    "Palette Object",
    "Palette example, Copyright © 1993 Jean-Michel Forgeas",
    "_Open palette",
    "Color selected",
    "Color modified",
    "Palette result: OK",
    "Palette result: Cancel",
    "Colors restored",
    "Colors",
    "R",
    "G",
    "B",
    "_Restore",
    "_Wheel",
    "_Colors",
    0
    };

static UBYTE *LangOther[] =
    {
    "",
    "Exemple Palette",
    "Objet Palette",
    "Exemple Palette, Copyright © 1993 Jean-Michel Forgeas",
    "_Ouvrir la palette",
    "Couleur sélectionnée",
    "Couleur modifiée",
    "Résultat Palette : OK",
    "Résultat Palette : Abandon",
    "Couleurs remises comme au début",
    "Couleurs",
    "R",
    "V",
    "B",
    "_Remettre",
    "_Roue",
    "_Couleurs",
    0
    };

/*--------------- TagLists pour l'ajout des objets ------------*/

/*------------ UIK */

static struct TagItem UIKTags[] = {
    UIKTAG_GEN_LangEnglish,    (ULONG) LangEnglish,
    UIKTAG_GEN_LangOther,      (ULONG) LangOther,
    UIKTAG_GEN_Catalog,        (ULONG) "uik/palette.catalog",
    TAG_END
    };

/*------------ Ecran */

static struct TagItem IntuitionScreenTags[] = {
    SA_SharePens,   TRUE,  /* --> pour la roue des couleurs - Intuition V39 (3.0) */
    SA_Interleaved, TRUE,  /* --> plus joli avec depth 8 - Intuition V39 (3.0) */
    TAG_END
    };

static struct TagItem ScreenTags[] = {
    UIKTAG_OBJ_Title,               TEXT_ScreenTitle,
    UIKTAG_OBJScreenFl_Public,      TRUE,
    UIKTAG_OBJScreenFl_MaxDepth,    TRUE,
    /* ne sera pris en compte que sous 3.0 */
    UIKTAG_OBJScreen_IntuiTags,     (ULONG)IntuitionScreenTags,
    TAG_END
    };

/*------------ Fenêtre */

static struct TagItem WindowTags[] = {
    UIKTAG_OBJ_LeftTop,         W2L(0,30),
    UIKTAG_OBJ_WidthHeight,     W2L(240,66),
    UIKTAG_OBJ_Title,           TEXT_WindowTitle,
    UIKTAG_OBJ_AltTitle,        TEXT_ScreenTitle,
    TAG_END
    };

/*------------ Bouton */

static struct TagItem GadgetTags[] = {
    UIKTAG_OBJ_LeftTop,         W2L(20,10),
    UIKTAG_OBJ_WidthHeight,     W2L(200,18),
    UIKTAG_OBJ_BoxFl_Filled,    TRUE,
    UIKTAG_OBJ_Title,           TEXT_GadgetTitle,
    UIKTAG_OBJ_TitleUnderscore, (ULONG)'_',
    TAG_END
    };

/*------------ Toggle 1 */

static struct TagItem Toggle1Tags[] = {
    UIKTAG_OBJ_LeftTop,         W2L(20,38),
    UIKTAG_OBJ_WidthHeight,     W2L(80,18),
    UIKTAG_OBJ_BoxFl_Filled,    TRUE,
    UIKTAG_OBJ_Title,           TEXT_Toggle1Title,
    UIKTAG_OBJ_TitleUnderscore, (ULONG)'_',
    TAG_END
    };

/*------------ String */

static struct TagItem StringTags[] = {
    UIKTAG_OBJ_LeftTop,         W2L(180,38),
    UIKTAG_OBJ_WidthHeight,     W2L(40,18),
    UIKTAG_OBJ_Title,           TEXT_StringTitle,
    UIKTAG_OBJ_TitleUnderscore, (ULONG)'_',
    UIKTAG_OBJ_TitleFl_Left,    TRUE,
    UIKTAG_OBJStr_ResultPointer,(ULONG)&StringResult,
    UIKTAG_OBJStr_TextPointer,  (ULONG)"10",
    UIKTAG_OBJStr_MaxChars,     3,
    UIKTAG_OBJStrFl_DigitalInt, TRUE,
    TAG_END
    };

/*------------ Palette */

static struct TagItem PaletteTags[] = {
    UIKTAG_OBJ_LeftTop,             W2L(300,100),
    UIKTAG_OBJ_BoxFl_StartSpecify,  TRUE,    /* ne sera pas starté au début */
    UIKTAG_OBJ_Title,               TEXT_PaletteTitle,
    UIKTAG_OBJ_AltTitle,            TEXT_ScreenTitle,
    UIKTAG_Palette_FirstTitleNum,   TEXT_PaletteStrings,
    UIKTAG_Palette_OKFunc,          (ULONG)color_ok,
    UIKTAG_Palette_CancelFunc,      (ULONG)color_cancel,
    UIKTAG_Palette_RestoreFunc,     (ULONG)color_restore,
    UIKTAG_Palette_ModifyFunc,      (ULONG)color_modify,
    UIKTAG_Palette_SelectFunc,      (ULONG)color_select,
    UIKTAG_Palette_EditColorNum,    256,
    UIKTAG_Palette_NoColorWheel,    TRUE,
    TAG_END
    };

/*------------------ Arborescence des objets -------------------*/

static struct UIKHookItem __far GadgetHooks[] =
    {
    { open_palette, IDCMP_GADGETUP },
    { 0 }
    };

static struct UIKTreeItem __far Arbo[] =
    {
    OBJECT( 0, &UIK, "UIK", UIKTags, 0, 0, 0 ),
        OBJECT( 1, NOSTORE, "UIKObj_Screen", ScreenTags, 0, 0, 0 ),
            OBJECT( 2, &ObjWin, "UIKObj_Window", WindowTags, 0, 0, 0 ),
                OBJECT( 3, &ObjGad, "UIKObj_Boolean", GadgetTags, 0, 0, GadgetHooks ),
                OBJECT( 3, &ObjTog1, "UIKObj_Toggle", Toggle1Tags, 0, 0, 0 ),
                OBJECT( 3, &ObjStr, "UIKObj_String", StringTags, 0, 0, 0 ),
    OBJEND
    };

/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void color_ok( struct UIKObjPalette *op )
{
    printf( "%ls\n", UIK_LangString( UIK, TEXT_ColorsOK ) );
    UIK_Remove( ObjPalette ); ObjPalette = 0;
}

static void color_cancel( struct UIKObjPalette *op )
{
    printf( "%ls\n", UIK_LangString( UIK, TEXT_ColorsCancel ) );
    UIK_Remove( ObjPalette ); ObjPalette = 0;
}

static void color_restore( struct UIKObjPalette *op )
{
    printf( "%ls\n", UIK_LangString( UIK, TEXT_ColorsRestore ) );
}

static void open_palette( struct UIKObj *obj )
{
    if (! ObjPalette)
        {
        PaletteTags[10].ti_Data = StringResult ? StringResult : 2;
        PaletteTags[11].ti_Data = (ObjTog1->Flags & BOX_OVER ? TRUE : FALSE);
        if (ObjPalette = UIK_AddObject( "UIKObj_Palette", ObjWin, PaletteTags ))
            UIK_Start( ObjPalette );
        }
}

static void color_modify( struct UIKObjPalette *op, struct UIKColorDef *ucd )
{
    printf( "%ls : colnum=%ld 32bits=R-%8.8lx,G-%8.8lx,B-%08.8lx, 12bits=%04.4lx\n", UIK_LangString( UIK, TEXT_ColModified ), ucd->ColorNum, ucd->B32Red, ucd->B32Green, ucd->B32Blue, ucd->B12Color );
}

static void color_select( struct UIKObjPalette *op, ULONG color )
{
    printf( "%ls : %ld\n", UIK_LangString( UIK, TEXT_ColSelected ), color );
}

void main()
{
    if (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))
        if (UIK_MakeUIKTree( Arbo, NULL, NULL ))
            if (UIK_Start( UIK ))
                UIK_Do( UIK, SIGBREAKF_CTRL_C );

  END_ERROR:
    if (UIK)     UIK_Remove( UIK );
    if (UIKBase) CloseLibrary( UIKBase );
    exit(0);
}
