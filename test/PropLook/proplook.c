/****************************************************************
 *
 *      File:      PropLook.c
 *      Project:   UIK example code
 *
 *      Created:   03/04/93     Jean-Michel Forgeas
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
#include "uiki:uikdrawauto.h"
#include "uiki:uikmacros.h"
#include "uiki:uikglobal.h"
#include "uiki:uikcolors.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objproplift.h"
#include "uiki:objwindow.h"

#define TEXT_WindowTitle    1
#define TEXT_ScreenTitle    2

#define ALIGN8(a) (((a)+7) & 0xfffffff8)
#define ALIGN16(a) (((a)+15) & 0xfffffff0)


/****** Imported ************************************************/

extern ULONG DOSBase;


/****** Exported ***********************************************/

struct UIKBase *UIKBase;
struct UIKGlobal *UIK;


/****** Statics ************************************************/

static UBYTE *LangEnglish[] =
    {
    "",
    "PropLook",
    "PropLook Copyright © 1992 Jean-Michel Forgeas",
    0
    };

static UBYTE *LangOther[] =
    {
    "",
    "PropLook",
    "PropLook Copyright © 1992 Jean-Michel Forgeas",
    0
    };

static UWORD PropAuto[] =
    {
    x_ADD_N, 2, X_SUB_N, 2,
    y_ADD_1, Y_SUB_1,
    END_AUTO
    };

static UWORD PatVertProp[] = {
    0x0240,
    0xFE7F,
    0xFE7F,
    0xFE7F,
    0xE247,
    0xFE7F,
    0xFE7F,
    0xFE7F,
    };

static UWORD PatHorizProp[] = {
    0xFFFF,
    0xFFFE,
    0xFFFE,
    0xFFFE,
    0xFEFE,
    0xFEFE,
    0xFFFF,
    0x0000,
    0x0000,
    0xFFFF,
    0xFEFE,
    0xFEFE,
    0xFFFE,
    0xFFFE,
    0xFFFE,
    0xFFFF,
    };

#include "uiki:uikgfx.h"

static UWORD chip BrVertData[5][2][1] =
   {
   0xfffc,    /* ##############.. */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0xfffc,    /* ##############.. */

   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0xfffc,    /* ##############.. */
   0xfffc,    /* ##############.. */
   };
struct UIKBitMap UBM_BrVert = { &BrVertData[4][0][0], UIKBMAP_MASKED|UIKBMAP_CONTIG,
    14, 2,2, 0,4,0, &BrVertData[0][0][0], &BrVertData[1][0][0], &BrVertData[2][0][0], &BrVertData[3][0][0], 0, 0, 0, 0 };

static UWORD chip BrHorizData[5][14][1] =
   {
   0x8000,    /* #............... */
   0x8000,    /* #............... */
   0x8000,    /* #............... */
   0x8000,    /* #............... */
   0x8000,    /* #............... */
   0x8000,    /* #............... */
   0x8000,    /* #............... */
   0x8000,    /* #............... */
   0x8000,    /* #............... */
   0x8000,    /* #............... */
   0x8000,    /* #............... */
   0x8000,    /* #............... */
   0x8000,    /* #............... */
   0x8000,    /* #............... */

   0x4000,    /* .#.............. */
   0x4000,    /* .#.............. */
   0x4000,    /* .#.............. */
   0x4000,    /* .#.............. */
   0x4000,    /* .#.............. */
   0x4000,    /* .#.............. */
   0x4000,    /* .#.............. */
   0x4000,    /* .#.............. */
   0x4000,    /* .#.............. */
   0x4000,    /* .#.............. */
   0x4000,    /* .#.............. */
   0x4000,    /* .#.............. */
   0x4000,    /* .#.............. */
   0x4000,    /* .#.............. */

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
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0xc000,    /* ##.............. */
   0xc000,    /* ##.............. */
   0xc000,    /* ##.............. */
   0xc000,    /* ##.............. */
   0xc000,    /* ##.............. */
   0xc000,    /* ##.............. */
   0xc000,    /* ##.............. */
   0xc000,    /* ##.............. */
   0xc000,    /* ##.............. */
   0xc000,    /* ##.............. */
   0xc000,    /* ##.............. */
   0xc000,    /* ##.............. */
   0xc000,    /* ##.............. */
   0xc000,    /* ##.............. */
   };
struct UIKBitMap UBM_BrHoriz = { &BrHorizData[4][0][0], UIKBMAP_MASKED|UIKBMAP_CONTIG,
    2, 2,14, 0,4,0, &BrHorizData[0][0][0], &BrHorizData[1][0][0], &BrHorizData[2][0][0], &BrHorizData[3][0][0], 0, 0, 0, 0 };


struct UIKBitMap *UIKBitMapTable[] = {
    &UBM_BrVert,
    &UBM_BrHoriz,
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
  struct UIKObjWindow *wo;
  struct UIKObj *oprop;
  WORD left, top;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (UIK = (struct UIKGlobal *) UIK_AddObjectTags( "UIK", 0,
             UIKTAG_GEN_LangEnglish,    (ULONG) LangEnglish,
             UIKTAG_GEN_LangOther,      (ULONG) LangOther,
             TAG_END ))) goto END_ERROR;

    UIK_InvertBlackWhite( UIK, UIKBitMapTable, 1 );

    if (! (wo = (struct UIKObjWindow *) UIK_AddObjectTags( "UIKObj_Window", UIK,
            UIKTAG_OBJ_LeftTop,         SETL(180,30),
            UIKTAG_OBJ_WidthHeight,     SETL(250,100),
            UIKTAG_OBJ_Title,           TEXT_WindowTitle,
            UIKTAG_OBJ_AltTitle,        TEXT_ScreenTitle,
            TAG_END ))) goto END_ERROR;

    /* Le pattern de l'intérieur du prop doit être alignée sur l'origine du
     * RastPort. UIK ajoute l'épaisseur des bords de la fenêtre aux origines
     * gauche et droite des objets.
     * Le pattern sera toujours dessinée à left + 2 et top + 1 car 2 est la
     * largeur du bord gauche du prop et 1 la hauteur du bord du haut (voir
     * l'automate PropAuto).
     * Pour pouvoir placer les objets exactement où l'on veut, il faudrait
     * décaler l'image du pattern à la fois en hauteur et en largeur, du
     * nombre de pixels en trop (modulo pattern). Cela nécessiterait d'allouer
     * à chaque fois de la mémoire "chip" pour dupliquer le pattern si on
     * veut ajouter plusieurs objets à des endroits (modulos) différents.
     */
    left = ALIGN16(20 + wo->LeftBorderWidth + 2) - wo->LeftBorderWidth - 2;
    top  = ALIGN8(10 + wo->TopBorderHeight + 1) - wo->TopBorderHeight - 1;
    if (! (oprop = UIK_AddObjectTags( "UIKObj_PropLift", wo,  /* vertical */
            UIKTAG_OBJProp_Vertical,    TRUE,
            UIKTAG_OBJ_LeftTop,         SETL(left,top),
            UIKTAG_OBJ_WidthHeight,     SETL(4+16,2+8*10),
            UIKTAG_OBJProp_Total,       100,
            UIKTAG_OBJProp_Visible,     1,
            UIKTAG_OBJ_BoxFl_Filled,    TRUE,
            UIKTAG_OBJ_BoxAutomate,     PropAuto,
            UIKTAG_OBJ_Pattern,         PatVertProp,
            UIKTAG_OBJ_PatternPower,    3,
            UIKTAG_OBJProp_KnobImage1,  &UBM_BrVert,
            UIKTAG_OBJProp_KnobSpImage1,&UBM_BrVert,
            UIKTAG_OBJProp_KnobImage2,  &UBM_BrVert,
            UIKTAG_OBJProp_KnobSpImage2,&UBM_BrVert,
            TAG_END ))) goto END_ERROR;

    left = ALIGN16(70 + wo->LeftBorderWidth + 2) - wo->LeftBorderWidth - 2;
    top  = ALIGN16(40 + wo->TopBorderHeight + 1) - wo->TopBorderHeight - 1;
    if (! (oprop = UIK_AddObjectTags( "UIKObj_PropLift", wo,  /* horizontal */
            UIKTAG_OBJ_LeftTop,         SETL(left,top),
            UIKTAG_OBJ_WidthHeight,     SETL(4+16*10,2+16),
            UIKTAG_OBJProp_Total,       100,
            UIKTAG_OBJProp_Visible,     1,
            UIKTAG_OBJ_BoxFl_Filled,    TRUE,
            UIKTAG_OBJ_BoxAutomate,     PropAuto,
            UIKTAG_OBJ_Pattern,         PatHorizProp,
            UIKTAG_OBJ_PatternPower,    4,
            UIKTAG_OBJProp_KnobImage1,  &UBM_BrHoriz,
            UIKTAG_OBJProp_KnobSpImage1,&UBM_BrHoriz,
            UIKTAG_OBJProp_KnobImage2,  &UBM_BrHoriz,
            UIKTAG_OBJProp_KnobSpImage2,&UBM_BrHoriz,
            TAG_END ))) goto END_ERROR;

    if (! UIK_Start( UIK )) goto END_ERROR;

    UIK_Do( UIK, SIGBREAKF_CTRL_C );

  END_ERROR:
    Cleanup();
}
