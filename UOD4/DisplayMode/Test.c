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

#include "uiki:objwindow.h"
#include "uiki:objdisplaymode.h"

#include <stdio.h>

/****** Imported ************************************************/

extern ULONG DOSBase;

/****** Exported ************************************************/

struct UIKBase *UIKBase;
struct IntuitionBase *IntuitionBase;
struct GfxBase *GfxBase;


/****** Statics *************************************************/

static void okfunc();
static void cancelfunc();

static struct UIKGlobal *UIK;
struct UIKObjWindow *wo;
struct UIKObj *dm;
struct UIKPrefs Prefs;
struct ModeInfo mi;

static UBYTE *LangEnglish[] =
{
    "",
    "Mode selector",
    "DisplayMode editor v0.70, ©1992 DENIS Frédéric / \"ß release\"",
    "Display modes",
    " Properties ",
    "Min. overscan",
    "Text overscan",
    "Gfx. overscan",
    "Max. overscan",
    "Custom size",
    "Palette",
    "Max. colors",
    "Supports H.A.M.",
    "Supports EXTRA HALFBRITE",
    " Colors ",
    "Width :",
    "Height :",
};

static UBYTE *LangOther[] =
{
    "",
    "Sélecteur de mode",
    "DisplayMode editor v0.70, ©1992 DENIS Frédéric / \"ß release\"",
    "Mode d'affichage",
    " Propriétés ",
    "OvScan min.",
    "OvScan texte",
    "OvScan graph.",
    "OvScan max.",
    "Autre taille",
    "Palette",
    "Max. couleurs",
    "Accepte le H.A.M.",
    "Accepte l'EXTRA HALFBRITE",
    " Couleurs ",
    "Largeur :",
    "Hauteur :",
};

static struct TagItem wtl[] = {  /* window */
    { UIKTAG_OBJ_LeftTop,       SETL(0,2) },
    { UIKTAG_OBJ_WidthHeight,   SETL(574,165) },
    { UIKTAG_OBJ_Title,         1 },
    { UIKTAG_OBJ_AltTitle,      2 },
    { UIKTAG_OBJ_ActInactPens,  UIKCOLS(UIKCOL_LIGHT,UIKCOL_GREY,UIKCOL_BLACK,UIKCOL_BLACK) },
    { UIKTAG_OBJWindow_MinimumWidthHeight, SETL(500,165) },
    { UIKTAG_OBJWindowFl_With_Zoom, FALSE },
    { UIKTAG_OBJWindowFl_With_Close, FALSE },
    { TAG_END } };

static struct TagItem dmtl[] = {  /* display mode selector */
    { UIKTAG_OBJ_LeftTop,           SETL(10,5) },
    { UIKTAG_OBJ_WidthHeight,       SETL(554,155) },
    { UIKTAG_OBJ_BoxFl_TranslateLeftTop, TRUE },
    { UIKTAG_ObjDM_OKFunc,         (ULONG)okfunc },
    { UIKTAG_ObjDM_CancelFunc,     (ULONG)cancelfunc },
    { UIKTAG_ObjDM_ModeInfo,       (ULONG)&mi },
    { TAG_END }
    };

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

static void okfunc( struct UIKObjDisplayMode *dm, struct ModeInfo *mi, ULONG UserData)
{
   Prefs.DefScreen_Depth = mi->Depth;
   Prefs.DefScreen_ModeId  = mi->vpm.ModeId;
   Prefs.DefScreen_Width = mi->Width;
   Prefs.DefScreen_Height = mi->Height;
   UIK_SetPrefs(&Prefs,sizeof(struct UIKPrefs));
   printf("Mode is : \"%ls\"\n",mi->Name);
   printf("done!!!\n");
   UIK_Remove( dm );
   Signal(FindTask(NULL),SIGBREAKF_CTRL_C);
}

static void cancelfunc( struct UIKObjDisplayMode *dm, ULONG UserData)
{
   printf("Cancel!\n");
   UIK_Remove( dm );
   Signal(FindTask(NULL),SIGBREAKF_CTRL_C);
}

void main()
{

   if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) exit(20);
   if (! (IntuitionBase = (struct Intuition *) OpenLibrary( "intuition.library", 0 ))) exit(20);
   if (! (GfxBase = (struct GfxBase *) OpenLibrary( "graphics.library", 0 ))) exit(20);
   if (! (UIK = (struct UIKGlobal *) UIK_Install( LangEnglish, LangOther ))) goto END_ERROR;

   UIK_GetPrefs(&Prefs,sizeof(struct UIKPrefs));
   mi.Depth = Prefs.DefScreen_Depth;
   mi.vpm.ModeId = Prefs.DefScreen_ModeId;
   mi.Width = Prefs.DefScreen_Width;
   mi.Height = Prefs.DefScreen_Height;

   if (! (wo = (struct UIKObjWindow *) UIK_AddObject( "UIKObj_Window", UIK, wtl ))) goto END_ERROR;
   if (! (dm = UIK_AddObject( "UIKObj_DisplayMode", wo, dmtl ))) goto END_ERROR;

   if (! UIK_Start( UIK )) goto END_ERROR;

   UIK_Do( UIK, SIGBREAKF_CTRL_C );

   END_ERROR: Cleanup();
}

