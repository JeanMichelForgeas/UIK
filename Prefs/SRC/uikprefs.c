/****************************************************************
 *
 *      File:      uikprefs.c
 *      Project:   manages uik preferences
 *
 *      Created:   1990     Jean-Michel Forgeas
 *
 *      Note:      Ce code est linké avec rstartup.o qui est un
 *                 startup fourni par Commodore et permet de
 *                 faire un executable réentrant (pure, bit p)
 *                 que l'on peut rendre résident.
 *                 Comme stdout n'existe pas, il faut utiliser
 *                 fprintf(Output(),...) au lieu de printf(...).
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikglobal.h"
#include "uiki:uikcolors.h"
#include "uiki:uikmacros.h"
#include "uiki:uikdrawauto.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objwinhelp.h"
#include "uiki:objreqstring.h"
#include "uiki:objwindow.h"
#include "uiki:objscreen.h"
#include "uiki:objwinfontsel.h"
#include "uiki:objpalette.h"

#include "uiki:objdisplaymode.h"

#include "lci:vars.h"
#include "lci:objreqmode.h"
#include "lci:textprefs.h"

#include <graphics/gfxbase.h>
#include <clib/locale_protos.h>
#include <pragmas/locale_pragmas.h>

#define MSG_FONTREQ         40
#define MSG_INPUTREQ        41
#define MSG_HELP            55
#define MSG_FONTSEL_TITLE   56
#define MSG_LOOKGAD_TITLE   57
#define MSG_LOOKSYS_TITLE   58
#define MSG_LOOKUIK_TITLE   59
#define MSG_IMPGAD_TITLE    60

#define UIKLIB_MINVER   1
#define UIKLIB_MINREV   39


/****** Imported ************************************************/

extern struct Library * __far DOSBase;
struct GfxBase * __far GfxBase;
struct UIKBase * __far UIKBase;
struct Library * __far LocaleBase;
struct Library * __far IntuitionBase;

//extern struct UIKObjRegGerm __far ObjPaletteRegGerm;
extern struct UIKObjRegGerm __far ObjReqModeRegGerm;

extern struct UIKBitMap * __far UIKBitMapTable[];
extern struct UIKBitMap * __far UBM_Font;
extern struct UIKBitMap * __far UBM_Palette;
extern struct UIKBitMap * __far UBM_Mode;
extern struct UIKBitMap * __far UBM_Dims;


/****** Exported ***********************************************/

VARS __near V; /* magic workaround */

UBYTE versiontag[] = "\0$VER: UIKPrefs 2.6 (4.11.93)";

static UBYTE * __far DMTitle = "Mode";
static UBYTE * __far WTitle = "UIKPrefs 2.6";
UBYTE * __far STitle = "User Interface Kit, Copyright © JM Forgeas 1990-1993, Tous Droits Réservés";
static UBYTE * __far PWTitle = "UIK Palette";

static struct UIKPrefs __far TmpPrefs;


/****** Statics ************************************************/

static int StartObjects();

/*** pour creation du premier objet 'fenetre' ***/

static UBYTE * __far LangEnglish[] =
    {
    "",
    "Frame",
    "Localized",
    "Save",
    "Help",
    "Test",
    "Light",
    "Dark",
    "Special",
    "Medium",
    "UIKPrefs Help",
    " Language ",
    "English",
    "Other",
    " Aspect ",
    " Default screen ",
    "Workbench",
    "UIK",
    " Mode ",
    " Font ",
    " UIK screen ",
    "Use",
    "Cancel",
    "",
    "Colors",  /* palette */
    "R",
    "G",
    "B",
    "_Restore",
    "",
    "Project",  /* menu */
    "Open...",
    "Save As...",
    "Quit",
    "Edit",
    "Reset To Defauts",
    "Last Saved",
    "Restore",
    "Settings",
    "Create Icons?",
    "Enter font name point its height\nExample: Topaz.11",    /* requester de fonte */
    "Input modes",                   /* input modes */
    "Mouse",
    "Bouris",
    "JoyMouse",
    "1 Contact",
    "2 Contacts",
    "5 Contacts",
    "Automatic",
    "Holding",
    " Times ",
    "Frame speed",
    "Validation",
    "Unselect",
    "Frame thick",
    "With UIKPrefs you can change the visual aspect\n"     // aide
        "of the programs built around the functions of\n"
        "the library 'uik.library'.\n"
        "You can change a lot of things, like the default\n"
        "font, color values, color assigns, input mode,\n"
        "screen mode, language...\n"
        "Under 3.x the palette window uses the ColorWheel\n"
        "but if you prefer sliders press the alt or shift\n"
        "key while clicking on the palette gadget.\n"
        "This program has normaly a convex 3D look if the\n"
        "color of your Workbench allow it. If not, you\n"
        "have to re-assign them with the frame titled\n"
        "'Colors':\n"
        "The UIK interface has its own color registers.\n"
        "You combine each color square with a Workbench\n"
        "color by clicking on the color squares.\n"
        "For your modifications to be considered and\n"
        "saved, they have to be tested with the 'Test'\n"
        "button.\n"
        "MEDIUM is generaly the register used for the\n"
        "color of the background of the windows and of\n"
        "other objects. For example light grey.\n"
        "LIGHT and DARK produce the relief look, so it\n"
        "is likely to have nearly white and black for\n"
        "them.\n"
        "SPECIAL can be used to show a selection, a\n"
        "window activation, to draw, etc...\n"
        "FRAME is the color used to surround selectable\n"
        "areas, with one of the contact input modes. \n"
        "If the SPECIAL color is between MEDIUM and\n"
        "DARK, select the button at the right of the\n"
        "\"Test\" button, it will do nicer. Don't forget\n"
        "to validate with the \"Test\" button.\n"
        "The langage for the interface can be chosen\n"
        "between two, provided by the programmer.\n"
        "It is recommended to him to provide english\n"
        "and what he wants.\n"
        "Depending on system version, language catalogs\n"
        "can be taken into accounts.\n"
        "The windows of UIK applications can be opened\n"
        "on a separate screen instead of the system\n"
        "screen.\n"
        "Your preferences are saved either:\n"
        " by the way of APServer if present,\n"
        " or as \"ENV:uik/uik.prefs\"/\"ENVARC:uik/uik.prefs\",\n"
        " or as \"S:uik.prefs\",\n"
        "Shortcuts:\n"
        "1 Light color\n"
        "2 Dark color\n"
        "3 Special color\n"
        "4 Medium color\n"
        "5 Frame color\n"
        "6 Look color\n"
        "X Look color activation\n"
        "T Colors test\n"
        "W Workbench\n"
        "K UIK\n"
        "F Font\n"
        "M Selection mode\n"
        "C Palette\n"
        "D Display mode\n"
        "L Locale\n"
        "S Save\n"
        "U Use\n"
        "A Cancel\n"
        "O Look UIK/Sys\n",
    "Select a Font",
    "Look",
    "Sys Look",
    "UIK Look",
    "Important",
    0
    };

static UBYTE * __far LangOther[] =
    {
    "",
    "Cadre",
    "Local",
    "Sauvegarder",
    "Aide",
    "Test",
    "Clair",
    "Foncé",
    "Spécial",
    "Medium",
    "UIKPrefs Aide",
    " Langage ",
    "Anglais",
    "Autre",
    " Aspect ",
    " Ecran par défaut ",
    "Workbench",
    "UIK",
    " Mode ",
    " Fonte ",
    " Ecran UIK ",
    "Utiliser",
    "Annuler",
    "",
    "Couleurs",   /* palette */
    "R",
    "V",
    "B",
    "_Remettre",
    "",
    "Projet",     /* menu */
    "Ouvrir...",
    "Sauver en...",
    "Quitter",
    "Edition",
    "Valeurs par défaut",
    "Dernières valeurs sauvées",
    "Valeurs initiales",
    "Options",
    "Créer icônes?",
    "Entrer un nom de fonte point sa hauteur\nExemple: Topaz.11",  /* requester de fonte */
    "Modes d'entrée",               /* input modes */
    "Souris",
    "Bouris",
    "JoyMouse",
    "1 Contact",
    "2 Contacts",
    "5 Contacts",
    "Automatique",
    "Maintient",
    " Temps ",
    "Défilement",
    "Validation",
    "Relâchement",
    "Epaisseur du cadre",
    "Grâce à UIKPrefs vous pouvez changer l'aspect\n"          // help
        "visuel des programmes construits autour de la\n"
        "librairie 'uik.library'.\n"
        "Vous pouvez changer beaucoup de choses, comme la\n"
        "police par défaut, les valeurs des couleur, leur\n"
        "assignation, le mode d'entrée, le mode d'écran,\n"
        "le langage...\n"
        "Sous 3.0 la palette utilise la roue des couleurs\n"
        "mais si vous préférez les ascenceurs appuyez sur\n"
        "alt ou shift en même temps que sur le gadget qui\n"
        "ouvre la palette.\n"
        "Ce programme possède un aspect 3D convexe si les\n"
        "couleurs de votre Workbench le permettent. Sinon\n"
        "réassignez les couleurs de UIK grâce au cadre\n"
        "intitulé 'Couleurs':\n"
        "L'interface UIK comporte ses propres registres\n"
        "de couleur. On associe chaque carré de couleur\n"
        "à une couleur du Workbench en cliquant sur les\n"
        "carrés.\n"
        "Pour que ces modifications soient prises en\n"
        "compte et puissent être sauvegardées, elle\n"
        "doivent être testées avec le bouton 'Test'.\n"
        "MEDIUM est en général le registre utilisé pour\n"
        "la couleur du fond des fenêtres et des autres\n"
        "objets. Un gris clair par exemple.\n"
        "CLAIR et FONCE servent à donner l'impression de\n"
        "relief donc il est préférable de leur associer\n"
        "des teintes se rapprochant le plus du blanc et \n"
        "du noir.\n"
        "SPECIAL peut être utilisé pour montrer une\n"
        "sélection, l'activation d'une fenêtre, pour\n"
        "dessiner, etc...\n"
        "CADRE est la couleur de détourage des zones\n"
        "sélectionnables par un des modes d'entrée par\n"
        "contacts.\n"
        "Si la couleur SPECIAL est intermédiaire entre\n"
        "MEDIUM et FONCE, sélectionnez le bouton juste à\n"
        "droite de \"Test\", ce sera plus agréable.\n"
        "N'oubliez pas de valider avec le bouton \"Test\".\n"
        "Le langage choisi dans l'interface peut l'être\n"
        "parmis deux, fournis par le programmeur.\n"
        "Il lui est conseillé de fournir l'anglais plus\n"
        "le langage de son choix.\n"
        "Selon la version du système, des catalogues de\n"
        "langages suplémentaires peuvent être pris en\n"
        "compte.\n"
        "Les fenêtres des applications UIK peuvent être\n"
        "redirigées vers un écran autre que celui du\n"
        "système.\n"
        "Vos préférences sont sauvegardées soit:\n"
        " par APServer si il est present,\n"
        " ou en \"ENV:uik/uik.prefs\"/\"ENVARC:uik/uik.prefs\",\n"
        " ou en \"S:uik.prefs\",\n"
        "Raccourcis clavier :\n"
        "1 Couleur claire\n"
        "2 Couleur foncée\n"
        "3 Couleur spéciale\n"
        "4 Couleur medium\n"
        "5 Couleur du cadre\n"
        "6 Couleur \n"
        "X Activation de la couleur de surlignage\n"
        "T Test des couleurs\n"
        "W Workbench\n"
        "K UIK\n"
        "F Fonte\n"
        "M Mode de sélection\n"
        "C Palette\n"
        "D Mode d'affichage\n"
        "L Locale\n"
        "S Sauvegarder\n"
        "U Utiliser\n"
        "A Annuler\n"
        "O Look UIK/Sys\n",
    "Choix d'une Police",
    "Surligne",
    "Look Sys",
    "Look UIK",
    "Important",
    0
    };

static UWORD __far Automate[] = { END_AUTO };

static UWORD __far LookAuto[] = {
    COL_WHITE, MOVE_Xy, DRAW_xy, DRAW_xY,
    COL_BLACK, x_ADD_1, MOVE_xY, DRAW_XY, y_ADD_1, DRAW_Xy,
    Y_SUB_1, X_SUB_1,
    COL_PEN, RECTFILL,
    COL_BLACK,
    x_ADD_1, x_ADD_1, x_ADD_1, x_ADD_1,
    y_ADD_1, y_ADD_1, y_ADD_1,
    Y_SUB_1, Y_SUB_1,
    X_SUB_1, X_SUB_1,
    MOVE_xY, DRAW_XY, DRAW_Xy,
    COL_LOOK,
    X_SUB_1, Y_SUB_1,
    MOVE_xY, DRAW_XY, DRAW_Xy, X_SUB_1, MOVE_Xy, DRAW_XY,
    END_AUTO
    };
static UWORD __far AltLookAuto[] = {
    COL_WHITE, MOVE_Xy, DRAW_xy, DRAW_xY,
    COL_BLACK, x_ADD_1, MOVE_xY, DRAW_XY, y_ADD_1, DRAW_Xy,
    Y_SUB_1, X_SUB_1,
    COL_WHITE, RECTFILL,
    COL_BLACK,
    x_ADD_1, x_ADD_1, x_ADD_1, x_ADD_1,
    y_ADD_1, y_ADD_1, y_ADD_1,
    Y_SUB_1, Y_SUB_1,
    X_SUB_1, X_SUB_1,
    MOVE_xY, DRAW_XY, DRAW_Xy,
    COL_LOOK,
    X_SUB_1, Y_SUB_1,
    MOVE_xY, DRAW_XY, DRAW_Xy, X_SUB_1, MOVE_Xy, DRAW_XY,
    END_AUTO
    };

static struct TagItem __far IntuitionScreenTags[] = {
    SA_SharePens,   TRUE,  /* --> pour la roue des couleurs - Intuition V39 (3.0) */
    SA_Interleaved, TRUE,  /* --> pour l'exemple - Intuition V39 (3.0) */
    TAG_END
    };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void make_assign_libs( UBYTE *pathname )
{
  UBYTE buf[300];

    sprintf( buf, "Assign >NIL: fspt%lx: \"LIBS:\"", FindTask(0) );
    Execute( buf, 0, V.Nil);
    sprintf( buf, "Assign >NIL: LIBS: \"%ls\"", pathname );
    Execute( buf, 0, V.Nil);
}

static void remove_assign_libs()
{
  UBYTE buf[300];

    sprintf( buf, "Assign >NIL: LIBS: \"fspt%lx:\"", FindTask(0) );
    Execute( buf, 0, V.Nil );
    sprintf( buf, "Assign >NIL: fspt%lx:", FindTask(0) );
    Execute( buf, 0, V.Nil );
}

static void make_assign_path( UBYTE *pathname )
{
  UBYTE buf[300];

    sprintf( buf, "Assign >NIL: UIKInstall: \"%ls\"", pathname );
    Execute( buf, 0, V.Nil);
}

static void remove_assign_path()
{
    Execute( "Assign >NIL: UIKInstall:", 0, V.Nil );
}


static void StrInsert( UBYTE *buf, UBYTE *src )
{
  UBYTE tmp[256];

    __builtin_strcpy(tmp, buf); __builtin_strcpy(buf, src); strcat (buf, tmp);
}

int BuiltPathName( struct WBArg *wbarg, UBYTE *buf )
{
  LONG i, len, lock, oldlock;

    *buf = 0;
    lock = DupLock( wbarg->wa_Lock );
    while (lock)
        {
        if (Examine( lock, &V.FIB ))
            {
            if (buf[0]) StrInsert( buf, "/" );
            StrInsert( buf, V.FIB.fib_FileName );
            }
        lock = ParentDir( oldlock = lock );
        UnLock( oldlock );
        }

    for (i=0,len=__builtin_strlen(buf); i<len; i++) if (buf[i] == '/') break;
    buf[i] = ':'; buf[len+1] = 0;

    return( __builtin_strlen( buf ) );
}

static void Cleanup()
{
  VARS *vars;

    if (vars = (VARS *) __builtin_getreg( 12 )) // 12 = A4, qui point sur la structure des variables
        {
        if (V.UIKBase)
            {
            if (V.UIK)          UIK_Remove( V.UIK );
            if (V.LocaleBase)   CloseLibrary( V.LocaleBase );
            CloseLibrary( V.UIKBase );
            }
        if (V.GfxBase)      CloseLibrary( V.GfxBase );
        if (V.IntuitionBase) CloseLibrary( V.IntuitionBase );
        if (V.DLock)
            {
            CurrentDir( V.OldLock );
            UnLock( V.DLock );
            }
        if (V.Nil) Close( V.Nil );
        FreeMem( vars, sizeof(VARS) );
        }
    exit(0);
}

void main( int argc, char **argv )
{
  VARS *vars = (VARS *) AllocMem( sizeof(VARS), MEMF_PUBLIC | MEMF_CLEAR );
  UBYTE pathname[300];

    __builtin_putreg( 12, vars ); // A4 pointe maintenant sur la structure des variables
    if (! vars) goto END_ERROR;

    if (! (V.IntuitionBase = IntuitionBase = (struct Library *) OpenLibrary( "intuition.library", 0 ))) goto END_ERROR;
    if (! (V.GfxBase = GfxBase = (struct GfxBase *) OpenLibrary( "graphics.library", 0 ))) goto END_ERROR;

    if (! (V.Nil = Open( "NIL:", MODE_NEWFILE ))) goto END_ERROR;

    if (! argc)
        {
        BuiltPathName( ((struct WBStartup *)(argv))->sm_ArgList, pathname );
        make_assign_path( pathname );
        }

    if (DOSBase->lib_Version < 36)
        {
        if (! argc)
            {
            if (V.DLock = DupLock( ((struct WBStartup *)(argv))->sm_ArgList->wa_Lock ))
                {
                V.OldLock = CurrentDir( V.DLock );
                make_assign_libs( pathname );
                V.UIKBase = UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 );
                remove_assign_libs();
                }
            }
        }
    if (! V.UIKBase) { V.UIKBase = UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ); }
    if (! V.UIKBase) goto END_ERROR;

    if ((UIKBase->Library.lib_Revision < UIKLIB_MINREV) && (UIKBase->Library.lib_Version <= UIKLIB_MINVER))
        {
        UBYTE buf[100];
        struct IntuiText msg = { -1, -1, JAM1, 10, 10, NULL, NULL, NULL },
                         ok  = { -1, -1, JAM1, 6, 3, NULL, "OK", NULL };

        sprintf( buf, "%ls version %ld.%ld minimum\npour executer ce programme", UIKNAME, UIKLIB_MINVER, UIKLIB_MINREV );
        msg.IText = buf;
        AutoRequest( NULL, &msg, 0, &ok, 0, 0, (strlen( buf ) + 13) * 8, 50 );
        goto END_ERROR;
        }

    V.LocaleBase = LocaleBase = OpenLibrary( "locale.library", 0 );

    if (! (V.UIK = (struct UIKGlobal *) UIK_AddObjectTags( "UIK", 0,
             UIKTAG_GEN_LangEnglish,    (ULONG) LangEnglish,
             UIKTAG_GEN_LangOther,      (ULONG) LangOther,
             UIKTAG_GEN_Catalog,        (ULONG) "uik/uikprefs.catalog",
             TAG_END ))) goto END_ERROR;
    UIK_InvertBlackWhite( V.UIK, UIKBitMapTable, 1 );

    UIK_GetPrefs( &V.Prefs, sizeof(struct UIKPrefs) );
    UIK_GetPrefs( &V.OldPrefs, sizeof(struct UIKPrefs) );

    V.WLeft = 60; V.WTop = 13; V.WWidth = 522; V.WHeight = 161;
    V.HLeft = 13; V.HTop = 15; V.HWidth = 430; V.HHeight = 150;
    StrCpy( V.PreName, "SYS:Prefs/Presets/uik.pre" );

    if (! StartObjects()) goto END_ERROR;

    while (TRUE)
        {
        UIK_Do( V.UIK, SIGBREAKF_CTRL_C );
        UIK_Remove( V.WO );
        if (!V.Continue) break;

        UIK_SetPrefs( &V.Prefs, sizeof(struct UIKPrefs) );
        if (! StartObjects() && (V.Prefs.PrefsFlags & UIKDEFAULT_SCREEN))
            {
            V.Prefs.PrefsFlags &= ~UIKDEFAULT_SCREEN;
            UIK_SetPrefs( &V.Prefs, sizeof(struct UIKPrefs) );
            if (! StartObjects()) goto END_ERROR;
            }
        V.Continue = 0;
        }

    END_ERROR: Cleanup();
}

/*************************************************/

static void EndHelp();

static void WO_stopped( struct UIKObjWindow *wo )
{
  struct Window *w = wo->Obj.w;

    V.WLeft = w->LeftEdge;
    V.WTop = w->TopEdge;
    V.WWidth = w->Width - (wo->LeftBorderWidth + wo->RightBorderWidth);
    V.WHeight = w->Height - (wo->TopBorderHeight + wo->BottomBorderHeight);
}
static void WO_removed() { V.WO = 0; }

static void wh_stopped()
{
  struct UIKObjWindow *wo = V.wh->WO;

    V.HLeft = wo->NW.LeftEdge - wo->LeftBorderWidth;
    V.HTop = wo->NW.TopEdge - wo->TopBorderHeight;
    V.HWidth = wo->NW.Width - (wo->LeftBorderWidth + wo->RightBorderWidth);
    V.HHeight = wo->NW.Height - (2 + wo->TopBorderHeight + wo->BottomBorderHeight);
    UIK_Remove( V.wh );
}
static void wh_removed() { V.wh = 0; }

static struct TTextAttr __far FrameTTA = { 0, 0, FSF_ITALIC, 0, 0 };
static struct TTextAttr __far NormTTA = { 0, 0, 0, 0, 0 };

static struct TagItem __far tl_FrColors[] = {
    { UIKTAG_OBJ_LeftTop,           SETL(18,8) },
    { UIKTAG_OBJ_WidthHeight,       SETL(150,124) },
    { UIKTAG_OBJ_Title,             MSG_COLORS_FRAME },
    { UIKTAG_OBJ_TTextAttr,         (ULONG) &FrameTTA },
    { UIKTAG_OBJ_BoxFl_Zoneable,    TRUE },
    { TAG_END } };

static struct TagItem __far tl_ColBox[] = {
    { UIKTAG_OBJ_LeftTop,           SETL(66,36) },
    { UIKTAG_OBJ_WidthHeight,       SETL(32,13) },
    { UIKTAG_OBJ_TitleFl_Left,      TRUE },
    { UIKTAG_OBJ_Title,             MSG_LIGHT_GAD },
    { UIKTAG_OBJ_TTextAttr,         (ULONG) &NormTTA },
    //{ UIKTAG_OBJ_ParentRelative,    TRUE },
    { TAG_END } };

static struct TagItem __far tl_ColGad[] = {
    { UIKTAG_OBJ_LeftTop,           SETL(70,38) },
    { UIKTAG_OBJ_WidthHeight,       SETL(24,9) },
    { UIKTAG_OBJ_BoxAutomate,       },
    { UIKTAG_OBJ_BoxFl_Filled,      TRUE },
    { UIKTAG_OBJ_UserValue1,        UIKCOL_WHITE },
    { UIKTAG_OBJ_ActInactPens,      UIKCOLS(UIKCOL_WHITE,UIKCOL_WHITE,UIKCOL_WHITE,UIKCOL_WHITE) },
    { UIKTAG_OBJ_BoxAltAutomate,    },
    { UIKTAG_OBJ_BoxFl_NoAct,       TRUE },
    { UIKTAG_OBJ_ShortCutKey,       0 },
    //{ UIKTAG_OBJ_ParentRelative,    TRUE },
    { TAG_END } };

static struct TagItem __far tl_langfr[] = {
    { UIKTAG_OBJ_LeftTop,           SETL(354,64) },
    { UIKTAG_OBJ_WidthHeight,       SETL(150,68) },
    { UIKTAG_OBJ_Title,             MSG_LANGUAGE_FRAME },
    { UIKTAG_OBJ_TTextAttr,         (ULONG) &FrameTTA },
    { UIKTAG_OBJ_BoxFl_Zoneable,    TRUE },
    { TAG_END } };

static struct TagItem __far tl_lang[] = {
    { UIKTAG_OBJ_LeftTop,           SETL(12,9) },
    { UIKTAG_OBJ_WidthHeight,       SETL(95,17) },
    { UIKTAG_OBJ_Title,             MSG_ENGLISH_GAD },
    { UIKTAG_OBJ_BoxFl_Over,        },
    { UIKTAG_OBJ_BoxFl_Filled,      TRUE },
    { UIKTAG_OBJ_ParentRelative,    TRUE },
    { UIKTAG_OBJ_TTextAttr,         (ULONG) &NormTTA },
    { UIKTAG_OBJ_ShortCutKey,       0 },
    { TAG_END } };

static struct TagItem __far tl_ecranfr[] = {
    { UIKTAG_OBJ_LeftTop,           SETL(186,8) },
    { UIKTAG_OBJ_WidthHeight,       SETL(150,49) },
    { UIKTAG_OBJ_Title,             MSG_DEFSCREEN_FRAME },
    { UIKTAG_OBJ_TTextAttr,         (ULONG) &FrameTTA },
    { UIKTAG_OBJ_BoxFl_Zoneable,    TRUE },
    { TAG_END } };

static struct TagItem __far tl_ecranUIKfr[] = {
    { UIKTAG_OBJ_LeftTop,           SETL(354,8) },
    { UIKTAG_OBJ_WidthHeight,       SETL(150,49) },
    { UIKTAG_OBJ_Title,             MSG_UIKSCREEN_FRAME },
    { UIKTAG_OBJ_TTextAttr,         (ULONG) &FrameTTA },
    { UIKTAG_OBJ_BoxFl_Zoneable,    TRUE },
    { TAG_END } };

static struct TagItem __far tl_ecran[] = {
    { UIKTAG_OBJ_LeftTop,           SETL(12,9) },
    { UIKTAG_OBJ_WidthHeight,       SETL(104,17) },
    { UIKTAG_OBJ_Title,             MSG_WORKBENCH_GAD },
    { UIKTAG_OBJ_BoxFl_Over,        TRUE },
    { UIKTAG_OBJ_BoxFl_Filled,      TRUE },
    { UIKTAG_OBJ_ParentRelative,    TRUE },
    { UIKTAG_OBJ_TTextAttr,         (ULONG) &NormTTA },
    { UIKTAG_OBJ_ShortCutKey,       0 },
    { TAG_END } };

static struct TagItem __far tl_ecranUIK[] = {
    { UIKTAG_OBJ_LeftTop,           SETL(12,9) },
    { UIKTAG_OBJ_WidthHeight,       SETL(106,17) },
    { UIKTAG_OBJ_Title,             0 },
    { UIKTAG_OBJ_TitleFl_Image,     TRUE },
    { UIKTAG_OBJ_BoxFl_Over,        TRUE },
    { UIKTAG_OBJ_BoxFl_Filled,      TRUE },
    { UIKTAG_OBJ_ParentRelative,    TRUE },
    { UIKTAG_OBJ_ShortCutKey,       0 },
    { TAG_END } };

static struct TagItem __far tl_fontfr[] = {
    { UIKTAG_OBJ_LeftTop,           SETL(186,64) },
    { UIKTAG_OBJ_WidthHeight,       SETL(150,31) },
    { UIKTAG_OBJ_Title,             MSG_FONT_FRAME },
    { UIKTAG_OBJ_TTextAttr,         (ULONG) &FrameTTA },
    { UIKTAG_OBJ_ShortCutKey,       B2L('F',0,0,0) },
    { TAG_END } };

static struct TagItem __far tl_font[] = {
    { UIKTAG_OBJ_LeftTop,           SETL(29,8) },
    { UIKTAG_OBJ_WidthHeight,       SETL(92,17) },
    { UIKTAG_OBJ_Title,             0 },
    { UIKTAG_OBJ_BoxFl_Filled,      TRUE },
    { UIKTAG_OBJ_TitleFl_Image,     TRUE },
    { UIKTAG_OBJ_ParentRelative,    TRUE },
    { UIKTAG_OBJ_ShortCutKey,       0 },
    { TAG_END } };

/*************************************************/

/*
static void EndIt( struct UIKObj *obj )
{
    Signal( V.UIK->Task, SIGBREAKF_CTRL_C );
}
*/

static void ChangeCol( struct UIKObj *obj )
{
  UBYTE newobjpen, maxpen = 1 << obj->w->WScreen->BitMap.Depth;
  UBYTE *cols = (UBYTE *) &V.Prefs.Colors.Grey;
  UWORD *wp = (UWORD *) &obj->Box;

    newobjpen = cols[obj->UserULong1] + 1;
    if (newobjpen >= maxpen) newobjpen = 0;
    cols[obj->UserULong1] = newobjpen;

    SetAPen( obj->rp, newobjpen );
    RectFill( obj->rp, wp[0], wp[1], wp[0]+wp[2]-1, wp[1]+wp[3]-1 );
}

static void SysLook( struct UIKObj *obj )
{
    if (obj->Flags & BOX_OVER) V.Prefs.PrefsFlags &= ~UIKF_SYSLOOK;
    else V.Prefs.PrefsFlags |= UIKF_SYSLOOK;
}

void TestPrefs( struct UIKObj *obj )
{
  struct UIKGlobal *uik = obj->UIK;

    UIK_SetPrefs( &V.Prefs, sizeof(struct UIKPrefs) );
    uik->Obj.ObjExt->Lang->Langage = (V.Prefs.PrefsFlags & UIKLANG_OTHER) ? uik->Obj.ObjExt->Lang->Other : uik->Obj.ObjExt->Lang->English;
    V.Continue = 1;
}

static void SetLangEnglish( obj )
struct UIKObj *obj;
{
    UIK_Remove( V.WO );
    V.Prefs.PrefsFlags &= ~UIKLANG_OTHER;
    TestPrefs( obj );
}

static void SetLangOther( struct UIKObj *obj )
{
    UIK_Remove( V.WO );
    V.Prefs.PrefsFlags |= UIKLANG_OTHER;
    TestPrefs( obj );
}

static void SetLangLocal( struct UIKObj *obj )
{
    UIK_Remove( V.WO );
    if (obj->Flags & BOX_OVER) V.Prefs.PrefsFlags &= ~UIKLANG_LOCALE;
    else{
        struct UIKLang *lang = obj->UIK->Obj.ObjExt->Lang;
        V.Prefs.PrefsFlags |= UIKLANG_LOCALE;
        if (LocaleBase && lang->CatName)
            {
            struct Catalog *catalog = OpenCatalogA( NULL, lang->CatName, NULL );
            if (lang->Catalog) CloseCatalog( lang->Catalog );
            lang->Catalog = catalog;
            }
        }
    TestPrefs( obj );
}

static void Test( struct UIKObj *obj )
{
    UIK_Remove( V.WO );
    TestPrefs( obj );
}

static void SetScreen( struct UIKObj *obj, int wbench )
{
    UIK_Remove( V.WO );
    if (wbench) V.Prefs.PrefsFlags &= ~UIKDEFAULT_SCREEN;
    else V.Prefs.PrefsFlags |= UIKDEFAULT_SCREEN;
    TestPrefs( obj );
}

//-------------------------------------------

static void color_ok( struct UIKObjPalette *op )
{
    UIK_Remove( (struct UIKObj *) op->Obj.UserULong1 );
    UIK_GetPrefs( &TmpPrefs, sizeof(struct UIKColors) );
    CopyMem( TmpPrefs.ColorTable, V.Prefs.ColorTable, MAX_UIKCOLORS*2 );
    UIK_SetPrefs( &TmpPrefs, sizeof(struct UIKColors) + MAX_UIKCOLORS*2 );
    if (UIKBase->DefaultScreen)
        LoadRGB4( &UIKBase->DefaultScreen->ViewPort, TmpPrefs.ColorTable, 1 << UIKBase->UIKPrefs->DefScreen_Depth );
}

static void color_cancel( struct UIKObjPalette *op )
{
    UIK_Remove( (struct UIKObj *) op->Obj.UserULong1 );
}

static void color_restore( struct UIKObjPalette *op )
{
    UIK_GetPrefs( &TmpPrefs, sizeof(struct UIKPrefs) );
}

static void color_modify( struct UIKObjPalette *op, struct UIKColorDef *ucd )
{
    TmpPrefs.ColorTable[ucd->ColorNum] = ucd->B12Color;
}

static void ScreenColors( struct UIKObj *obj )
{
  struct UIKObj *os, *op, *parent;
  ULONG nowheel=FALSE;
  struct IntuiMessage *imsg;

    UIK_GetPrefs( &TmpPrefs, sizeof(struct UIKPrefs) );

    if (imsg = obj->UIK->IntuiMsg)
        if (imsg->Qualifier & (IEQUALIFIER_LSHIFT|IEQUALIFIER_RSHIFT|IEQUALIFIER_LALT|IEQUALIFIER_RALT))
            nowheel = TRUE;
    if (GfxBase->LibNode.lib_Version < 39)
            nowheel = TRUE;

    parent = obj;
    if (nowheel == FALSE)
        {
        if (! (os = UIK_AddObjectTags( "UIKObj_Screen", obj,
                UIKTAG_OBJ_WidthHeight,     W2L(350,200),
                UIKTAG_OBJ_Title,           PWTitle,
                UIKTAG_OBJ_TitleFl_Addr,    TRUE,
                UIKTAG_OBJScreenFl_Public,  TRUE,
                UIKTAG_OBJScreenFl_MaxDepth,TRUE,
                UIKTAG_OBJScreenFl_AutoCenter, TRUE,
                UIKTAG_OBJScreen_IntuiTags, (ULONG)IntuitionScreenTags,
                TAG_END ))) goto END_ERROR;
        parent = os;
        }
    if (! (op = UIK_AddObjectTags( "UIKObj_Palette", parent,
            UIKTAG_OBJ_LeftTop,             W2L(150,15),
            UIKTAG_OBJ_TTextAttr,           (ULONG) &NormTTA,
            UIKTAG_OBJ_Title,               (ULONG) PWTitle,
            UIKTAG_OBJ_AltTitle,            (ULONG) PWTitle,
            UIKTAG_OBJ_TitleFl_Addr,        TRUE,
            UIKTAG_Palette_FirstTitleNum,   MSG_COLORS_PAL,
            UIKTAG_Palette_OKFunc,          (ULONG)color_ok,
            UIKTAG_Palette_CancelFunc,      (ULONG)color_cancel,
            UIKTAG_Palette_RestoreFunc,     (ULONG)color_restore,
            UIKTAG_Palette_ModifyFunc,      (ULONG)color_modify,
            UIKTAG_Palette_NoColorWheel,    nowheel,
            UIKTAG_Palette_EditColorNum,    MIN(32,1L << UIKBase->UIKPrefs->DefScreen_Depth),
            UIKTAG_PaletteFl_DefaultScreen, (nowheel == TRUE) ? TRUE : FALSE,
            TAG_END ))) goto END_ERROR;

    op->UserULong1 = (ULONG) ((nowheel == FALSE) ? os : op);

    if (UIK_Start( (nowheel == FALSE) ? os : op )) return;

  END_ERROR:
    UIK_Remove( (nowheel == FALSE) ? os : op );
}

//-------------------------------------------

static void okfunc( struct UIKObjDisplayMode *dm, struct ModeInfo *mi, ULONG UserData)
{
    UIK_Remove( V.WO );
    V.Prefs.DefScreen_Depth  = mi->Depth;
    V.Prefs.DefScreen_ModeId = mi->vpm.ModeId;
    V.Prefs.DefScreen_Height = mi->Height;
    if (UIK_IsSystemVersion2())
        {
        V.Prefs.DefScreen_Width  = mi->Width;
        V.Prefs.DefScreen_OScan  = mi->OScanType;
        }
    TestPrefs( dm );
}

static void cancelfunc( struct UIKObjDisplayMode *dm )
{
   UIK_Remove( (struct UIKObj *) dm->Obj.UserULong1 );
}

static void windowclosed( struct UIKObj *wo )
{
   UIK_Remove( wo );
}

static void ScreenMode( struct UIKObj *obj )
{
  struct UIKObj *wo=0, *dm=0;
  struct ModeInfo mi;

   mi.Depth         = V.Prefs.DefScreen_Depth;
   mi.vpm.ModeId    = V.Prefs.DefScreen_ModeId;
   mi.Width         = V.Prefs.DefScreen_Width;
   mi.Height        = V.Prefs.DefScreen_Height;
   mi.OScanType     = V.Prefs.DefScreen_OScan;
   mi.MI_ColorShades = 0;

    if (! (wo = (struct UIKObjWindow *) UIK_AddObjectTags( "UIKObj_Window", obj,
            UIKTAG_OBJ_LeftTop,       SETL(10,30),
            UIKTAG_OBJ_WidthHeight,   SETL(574,165),
            UIKTAG_OBJ_Title,         (ULONG) DMTitle,
            UIKTAG_OBJ_AltTitle,      (ULONG) STitle,
            UIKTAG_OBJ_TitleFl_Addr,  TRUE,
            UIKTAG_OBJWindow_MinimumWidthHeight, SETL(537,165),
            UIKTAG_OBJ_TTextAttr,         (ULONG) &NormTTA,
            TAG_END ))) goto END_ERROR;
    if (! UIK_AddVector( wo, (void *) windowclosed, CLOSEWINDOW, 0 )) goto END_ERROR;

    if (! (dm = UIK_AddObjectTags( "UIKObj_DisplayMode", wo,
            UIKTAG_OBJ_LeftTop,           SETL(10,5),
            UIKTAG_OBJ_BoxFl_TranslateLeftTop, TRUE,
            UIKTAG_OBJ_UserValue1,      (ULONG)wo,
            UIKTAG_ObjDM_OKFunc,        (ULONG)okfunc,
            UIKTAG_ObjDM_CancelFunc,    (ULONG)cancelfunc,
            UIKTAG_ObjDM_ModeInfo,      (ULONG)&mi,
            TAG_END ))) goto END_ERROR;

    UIK_Start( wo );
    return;

  END_ERROR:
    if (dm) UIK_Remove( dm );
    if (wo) UIK_Remove( wo );
}

//-------------------------------------------

static void rm_stopped( struct UIKObj *rm ) { UIK_Remove( rm ); }

static void ReqModes( struct UIKObj *obj )
{
  struct UIKObj *o;

    if (o = UIK_AddPrivateObjectTags( &ObjReqModeRegGerm, obj,
            UIKTAG_ObjReqMode_FirstTitleNum, MSG_INPUTREQ,
            UIKTAG_OBJ_TTextAttr,            (ULONG) &NormTTA,
            UIKTAG_OBJ_UserStopFunc,         (ULONG) rm_stopped,
            TAG_END ))
        UIK_Start( o );
}

static void CancelPrefs( struct UIKObj *obj )
{
  struct TextFont *font;

    UIK_Remove( V.WO );
    UIK_MemCopy( &V.OldPrefs, &V.Prefs, sizeof(struct UIKPrefs) );
    if (font = UIK_OpenFont( V.Prefs.DefFontName, V.Prefs.DefFontHeight ))
        {
        CloseFont( V.UIKBase->DefaultFont );
        V.UIKBase->DefaultFont = font;
        UIK_Font2TTA( font, &V.UIKBase->DefaultFontAttr );
        }
    TestPrefs( obj );
    V.Continue = 0;
}

static void SavePrefs( struct UIKObj *obj, ULONG arc )
{
    UIK_Remove( V.WO );
    UIK_SetPrefs( &V.Prefs, sizeof(struct UIKPrefs) );
    if (arc) UIK_SavePrefs();
    else UIK_SaveEnvPrefs();
    TestPrefs( obj );
    V.Continue = 0;
}

static void ShowHelp()
{
    if (! V.wh)
        {
        V.wh = (struct UIKObjWinHelp *) UIK_AddObjectTags( "UIKObj_WinHelp", V.WO,
                UIKTAG_OBJ_LeftTop,             SETL(V.HLeft,V.HTop),
                UIKTAG_OBJ_WidthHeight,         SETL(V.HWidth,V.HHeight),
                UIKTAG_WinHelp_WindowTitle,     (ULONG) UIK_LangString( V.UIK, MSG_TITLE_HELPWINDOW ),
                UIKTAG_WinHelp_TextPtr,         (ULONG) UIK_LangString( V.UIK, MSG_HELP ),
                UIKTAG_OBJ_UserStopFunc,        (ULONG) wh_stopped,
                UIKTAG_OBJ_UserRemoveFunc,      (ULONG) wh_removed,
                TAG_END );
        }
    if (V.wh) UIK_Start( V.wh );
}

static void WinKeys( struct UIKObj *obj )
{
  register struct IntuiMessage *imsg = obj->UIK->IntuiMsg;

    if (imsg->Code == 0x5f) ShowHelp();
}

static void SetLook( struct UIKObj *gad )
{
    if (gad->Flags & BOX_OVER) V.Prefs.PrefsFlags &= ~UIKF_SPLOOK;
    else V.Prefs.PrefsFlags |= UIKF_SPLOOK;

}

//---------------------------------------

/*
static void Font_Cancel( struct UIKObjFontSelect *ofs )
{
    UIK_Remove( ofs->Obj.Parent );
}

static void Font_OK( struct UIKObjFontSelect *ofs, struct TextFont *font, UWORD Style )
{
  register struct TTextAttr *tta = &V.UIKBase->DefaultFontAttr;

    CloseFont( V.UIKBase->DefaultFont );
    V.UIKBase->DefaultFont = font;
    UIK_Font2TTA( font, tta );
    tta->tta_Style = Style;

    StrCpy( V.Prefs.DefFontName, font->tf_Message.mn_Node.ln_Name );
    V.Prefs.DefFontHeight = font->tf_YSize;
    UIK_Remove( V.WO );
    TestPrefs( ofs );
}

static void ChooseFont( struct UIKObj *obj )
{
  struct UIKObj *wo, *fo;

    if (wo = (struct UIKObj *) UIK_AddObjectTags( "UIKObj_Window", obj,
                        UIKTAG_OBJ_LeftTop,     SETL(150,50),
                        UIKTAG_OBJ_WidthHeight, SETL(330,122),
                        UIKTAG_OBJ_Title,       MSG_FONTSEL_TITLE,
                        UIKTAG_OBJ_TTextAttr,   (ULONG) &NormTTA,
                        TAG_END ))
        if (fo = UIK_AddObjectTags( "UIKObj_FontSelector", wo,
                            UIKTAG_OBJ_LeftTop,     SETL(5,4),
                            UIKTAG_OBJ_WidthHeight, SETL(320,115),
                            UIKTAG_FOS_OKFUNC,      Font_OK,
                            UIKTAG_FOS_CANCELFUNC,  Font_Cancel,
                            UIKTAG_FOS_HighlightFont, &wo->ObjExt->TTA,
                            TAG_END ))
            UIK_Start( wo );
}
*/

static void Font_Cancel( struct UIKObjWinFontSel *wfos )
{
    UIK_Remove( wfos );
}

static void Font_OK( struct UIKObjWinFontSel *wfos, struct TextFont *font, UWORD Style )
{
  register struct TTextAttr *tta = &V.UIKBase->DefaultFontAttr;

    CloseFont( V.UIKBase->DefaultFont );
    V.UIKBase->DefaultFont = font;
    UIK_Font2TTA( font, tta );
    tta->tta_Style = Style;

    StrCpy( V.Prefs.DefFontName, font->tf_Message.mn_Node.ln_Name );
    V.Prefs.DefFontHeight = font->tf_YSize;
    UIK_Remove( V.WO );
    TestPrefs( wfos );
}

static void ChooseFont( struct UIKObj *obj )
{
  struct UIKObj *fo;

    if (fo = UIK_AddObjectTags( "UIKObj_WinFontSel", obj,
            UIKTAG_OBJ_Title,           MSG_FONTSEL_TITLE,
            UIKTAG_OBJ_TTextAttr,       (ULONG) &NormTTA,
            UIKTAG_WFOS_OKFUNC,         Font_OK,
            UIKTAG_WFOS_CANCELFUNC,     Font_Cancel,
            UIKTAG_WFOS_HighlightFont,  &V.UIKBase->DefaultFontAttr,
            TAG_END ))
        UIK_Start( fo );
}

//---------------------------------------

static int StartObjects()
{
  struct UIKObj *o, *obj;
  UBYTE *pm = &UIKBase->UIKPrefs->Mask, mask = *pm;
    /*------------fenetre------------*/
    if (! (V.WO = UIK_AddObjectTags( "UIKObj_Window", V.UIK,
            UIKTAG_OBJ_LeftTop,           SETL(V.WLeft,V.WTop),
            UIKTAG_OBJ_WidthHeight,       SETL(V.WWidth,V.WHeight),
            UIKTAG_OBJ_Title,             (ULONG) WTitle,
            UIKTAG_OBJ_AltTitle,          (ULONG) STitle,
            UIKTAG_OBJ_TitleFl_Addr,      TRUE,
            UIKTAG_OBJWindowFl_With_Icon,FALSE,
            UIKTAG_OBJWindowFl_With_Size, FALSE,
            UIKTAG_OBJWindowFl_CloseStop, FALSE,
            UIKTAG_OBJ_UserStopFunc,      (ULONG) WO_stopped,
            UIKTAG_OBJ_UserRemoveFunc,    (ULONG) WO_removed,
            TAG_END ))) goto END_ERROR;
    if (! UIK_AddVector( V.WO, (void *)CancelPrefs/*EndIt*/, CLOSEWINDOW, 0 )) goto END_ERROR;
    if (! UIK_AddVector( V.WO, (void *)WinKeys, RAWKEY, 0 )) goto END_ERROR;

    InstallMenu( V.WO );

    /*------------Frame colors------------*/
    if (! (obj = UIK_AddObject( "UIKObj_Frame", V.WO, tl_FrColors ))) goto END_ERROR;

    /*------------petites boxes colors------------*/
    tl_ColBox[0].ti_Data = SETL(105,18); tl_ColBox[3].ti_Data = 6;
    if (! (UIK_AddObject( "UIKObj_Box", obj, tl_ColBox ))) goto END_ERROR;   // light
    tl_ColBox[0].ti_Data = SETL(105,31); tl_ColBox[3].ti_Data = 7;
    if (! (UIK_AddObject( "UIKObj_Box", obj, tl_ColBox ))) goto END_ERROR;   // dark
    tl_ColBox[0].ti_Data = SETL(105,44); tl_ColBox[3].ti_Data = 8;
    if (! (UIK_AddObject( "UIKObj_Box", obj, tl_ColBox ))) goto END_ERROR;   // special
    tl_ColBox[0].ti_Data = SETL(105,57); tl_ColBox[3].ti_Data = 9;
    if (! (UIK_AddObject( "UIKObj_Box", obj, tl_ColBox ))) goto END_ERROR;   // medium
    tl_ColBox[0].ti_Data = SETL(105,70); tl_ColBox[3].ti_Data = 1;
    if (! (o = UIK_AddObject( "UIKObj_Box", obj, tl_ColBox ))) goto END_ERROR;   // Cadre
    tl_ColBox[0].ti_Data = SETL(105,83); tl_ColBox[3].ti_Data = MSG_LOOKGAD_TITLE;
    if (! (o = UIK_AddObject( "UIKObj_Box", obj, tl_ColBox ))) goto END_ERROR;   // look
    tl_ColBox[0].ti_Data = SETL(105,96); tl_ColBox[3].ti_Data = MSG_IMPGAD_TITLE;
    if (! (o = UIK_AddObject( "UIKObj_Box", obj, tl_ColBox ))) goto END_ERROR;   // imp

    /*------------petits gadgets colors------------*/
    {
    struct Rectangle *r = &o->ObjExt->InnerRect;
    WORD x = r->MinX + 2 - ((struct UIKObjWindow *)V.WO)->LeftBorderWidth;

    tl_ColGad[1].ti_Data = W2L(r->MaxX-r->MinX-3,r->MaxY-r->MinY-1);
    tl_ColGad[2].ti_Data = tl_ColGad[6].ti_Data = (ULONG) Automate;

    tl_ColGad[0].ti_Data = SETL(x,20); tl_ColGad[4].ti_Data = UIKCOL_WHITE; tl_ColGad[5].ti_Data = UIKCOLS(UIKCOL_WHITE,UIKCOL_WHITE,UIKCOL_WHITE,UIKCOL_WHITE);
    tl_ColGad[8].ti_Data = B2L('1',0,0,0);
    if (! (o = UIK_AddObject( "UIKObj_Boolean", obj, tl_ColGad ))) goto END_ERROR;
    if (! UIK_AddVector( o, (void *)ChangeCol, GADGETDOWN, 0 )) goto END_ERROR;

    tl_ColGad[0].ti_Data = SETL(x,33); tl_ColGad[4].ti_Data = UIKCOL_BLACK; tl_ColGad[5].ti_Data = UIKCOLS(UIKCOL_BLACK,UIKCOL_BLACK,UIKCOL_BLACK,UIKCOL_BLACK);
    tl_ColGad[8].ti_Data = B2L('2',0,0,0);
    if (! (o = UIK_AddObject( "UIKObj_Boolean", obj, tl_ColGad ))) goto END_ERROR;
    if (! UIK_AddVector( o, (void *)ChangeCol, GADGETDOWN, 0 )) goto END_ERROR;

    tl_ColGad[0].ti_Data = SETL(x,46); tl_ColGad[4].ti_Data = UIKCOL_LIGHT; tl_ColGad[5].ti_Data = UIKCOLS(UIKCOL_LIGHT,UIKCOL_LIGHT,UIKCOL_LIGHT,UIKCOL_LIGHT);
    tl_ColGad[8].ti_Data = B2L('3',0,0,0);
    if (! (o = UIK_AddObject( "UIKObj_Boolean", obj, tl_ColGad ))) goto END_ERROR;
    if (! UIK_AddVector( o, (void *)ChangeCol, GADGETDOWN, 0 )) goto END_ERROR;

    tl_ColGad[0].ti_Data = SETL(x,59); tl_ColGad[4].ti_Data = UIKCOL_GREY; tl_ColGad[5].ti_Data = UIKCOLS(UIKCOL_GREY,UIKCOL_GREY,UIKCOL_GREY,UIKCOL_GREY);
    tl_ColGad[8].ti_Data = B2L('4',0,0,0);
    if (! (o = UIK_AddObject( "UIKObj_Boolean", obj, tl_ColGad ))) goto END_ERROR;
    if (! UIK_AddVector( o, (void *)ChangeCol, GADGETDOWN, 0 )) goto END_ERROR;

    tl_ColGad[0].ti_Data = SETL(x,72); tl_ColGad[4].ti_Data = UIKCOL_ZONE; tl_ColGad[5].ti_Data = UIKCOLS(UIKCOL_ZONE,UIKCOL_ZONE,UIKCOL_ZONE,UIKCOL_ZONE);
    tl_ColGad[8].ti_Data = B2L('5',0,0,0);
    if (! (o = UIK_AddObject( "UIKObj_Boolean", obj, tl_ColGad ))) goto END_ERROR;
    if (! UIK_AddVector( o, (void *)ChangeCol, GADGETDOWN, 0 )) goto END_ERROR;

    tl_ColGad[0].ti_Data = SETL(x,85); tl_ColGad[4].ti_Data = UIKCOL_LOOK; tl_ColGad[5].ti_Data = UIKCOLS(UIKCOL_LOOK,UIKCOL_LOOK,UIKCOL_LOOK,UIKCOL_LOOK);
    tl_ColGad[8].ti_Data = B2L('6',0,0,0);
    if (! (o = UIK_AddObject( "UIKObj_Boolean", obj, tl_ColGad ))) goto END_ERROR;
    if (! UIK_AddVector( o, (void *)ChangeCol, GADGETDOWN, 0 )) goto END_ERROR;

    tl_ColGad[0].ti_Data = SETL(x,98); tl_ColGad[4].ti_Data = UIKCOL_IMP; tl_ColGad[5].ti_Data = UIKCOLS(UIKCOL_IMP,UIKCOL_IMP,UIKCOL_IMP,UIKCOL_IMP);
    tl_ColGad[8].ti_Data = B2L('7',0,0,0);
    if (! (o = UIK_AddObject( "UIKObj_Boolean", obj, tl_ColGad ))) goto END_ERROR;
    if (! UIK_AddVector( o, (void *)ChangeCol, GADGETDOWN, 0 )) goto END_ERROR;
    }

    /*------------LookSys------------*/
    if (! (o = UIK_AddObjectTags( "UIKObj_Toggle", obj,
            UIKTAG_OBJ_LeftTop,         SETL(25,109),
            UIKTAG_OBJ_WidthHeight,     SETL(79,17),
            UIKTAG_OBJ_Title,           MSG_LOOKUIK_TITLE,
            UIKTAG_OBJ_AltTitle,        MSG_LOOKSYS_TITLE,
            UIKTAG_OBJ_BoxFl_Filled,    TRUE,       // save
            UIKTAG_OBJ_TTextAttr,       (ULONG) &NormTTA,
            UIKTAG_OBJ_BoxFl_Over,      (V.Prefs.PrefsFlags & UIKF_SYSLOOK) ? FALSE : TRUE,
            UIKTAG_OBJ_ShortCutKey,     B2L('O',0,0,0),
            TAG_END ))) goto END_ERROR;
    if (! UIK_AddVector( o, (void *)SysLook, GADGETUP, 0 )) goto END_ERROR;

    /*------------test------------*/
    if (! (o = UIK_AddObjectTags( "UIKObj_Boolean", obj,
            UIKTAG_OBJ_LeftTop,         SETL(105,109),
            UIKTAG_OBJ_WidthHeight,     SETL(56,17),
            UIKTAG_OBJ_Title,           MSG_TEST_GAD,
            UIKTAG_OBJ_BoxFl_Filled,    TRUE,       // save
            UIKTAG_OBJ_TTextAttr,       (ULONG) &NormTTA,
            UIKTAG_OBJ_ShortCutKey,     B2L('T',0,0,0),
            TAG_END ))) goto END_ERROR;
    if (! UIK_AddVector( o, (void *)Test, GADGETUP, 0 )) goto END_ERROR;

    /*------------look------------*/
    if (! (o = UIK_AddObjectTags( "UIKObj_Toggle", obj,
            UIKTAG_OBJ_LeftTop,         SETL(142,83),
            UIKTAG_OBJ_WidthHeight,     SETL(19,13),
            UIKTAG_OBJ_BoxAutomate,     (ULONG) LookAuto,
            UIKTAG_OBJ_BoxAltAutomate,  (ULONG) AltLookAuto,
            UIKTAG_OBJ_BoxFl_Over,      (V.Prefs.PrefsFlags & UIKF_SPLOOK) ? FALSE : TRUE,
            UIKTAG_OBJ_ShortCutKey,     B2L('X',0,0,0),
            TAG_END ))) goto END_ERROR;
    if (! UIK_AddVector( o, (void *)SetLook, GADGETUP, 0 )) goto END_ERROR;

    /*------------gadgets------------*/
    { struct TTextAttr boldtta = { 0, 0, FSF_BOLD, 0, 0 };
    if (! (o = UIK_AddObjectTags( "UIKObj_Boolean", V.WO,
            UIKTAG_OBJ_LeftTop,         SETL(35,138),
            UIKTAG_OBJ_WidthHeight,     SETL(117,17),
            UIKTAG_OBJ_Title,           MSG_SAVE_GAD,
            UIKTAG_OBJ_BoxFl_Filled,    TRUE,       // save
            UIKTAG_OBJ_TTextAttr,       (ULONG) &boldtta,
            UIKTAG_OBJ_ShortCutKey,     B2L('S',0,0,0),
            TAG_END ))) goto END_ERROR;
    if (! UIK_AddVector( o, (void *)SavePrefs, GADGETUP, 1 )) goto END_ERROR;
    }
    { struct TTextAttr boldtta = { 0, 0, FSF_BOLD, 0, 0 };
    if (! (o = UIK_AddObjectTags( "UIKObj_Boolean", V.WO,
            UIKTAG_OBJ_LeftTop,         SETL(203,138),
            UIKTAG_OBJ_WidthHeight,     SETL(117,17),
            UIKTAG_OBJ_Title,           MSG_USE_GAD,
            UIKTAG_OBJ_BoxFl_Filled,    TRUE,       // use
            UIKTAG_OBJ_TTextAttr,       (ULONG) &boldtta,
            UIKTAG_OBJ_ShortCutKey,     B2L('U',0,0,0),
            TAG_END ))) goto END_ERROR;
    if (! UIK_AddVector( o, (void *)SavePrefs, GADGETUP, 0 )) goto END_ERROR;
    }
    { struct TTextAttr boldtta = { 0, 0, FSF_BOLD, 0, 0 };
    if (! (o = UIK_AddObjectTags( "UIKObj_Boolean", V.WO,
            UIKTAG_OBJ_LeftTop,         SETL(370,138),
            UIKTAG_OBJ_WidthHeight,     SETL(117,17),
            UIKTAG_OBJ_Title,           MSG_CANCEL_GAD,
            UIKTAG_OBJ_BoxFl_Filled,    TRUE,       // cancel
            UIKTAG_OBJ_TTextAttr,       (ULONG) &boldtta,
            UIKTAG_OBJ_ShortCutKey,     B2L('A',0,0,0),
            TAG_END ))) goto END_ERROR;
    if (! UIK_AddVector( o, (void *)CancelPrefs, GADGETUP, 0 )) goto END_ERROR;
    }

    /*------------language------------*/
    if (! (obj = UIK_AddObject( "UIKObj_Frame", V.WO, tl_langfr ))) goto END_ERROR;

    tl_lang[0].ti_Data = SETL(27,8);
    tl_lang[2].ti_Data = MSG_ENGLISH_GAD;
    tl_lang[3].ti_Data = (V.Prefs.PrefsFlags & UIKLANG_OTHER) ? TRUE : FALSE;
    if (! (o = UIK_AddObject( "UIKObj_MutualExclude", obj, tl_lang ))) goto END_ERROR;
    if (! UIK_AddVector( o, (void *)SetLangEnglish, GADGETDOWN, 0 )) goto END_ERROR;

    tl_lang[0].ti_Data = SETL(27,25);
    tl_lang[2].ti_Data = MSG_OTHER_GAD;
    tl_lang[3].ti_Data = (V.Prefs.PrefsFlags & UIKLANG_OTHER) ? FALSE : TRUE;
    if (! (o = UIK_AddObject( "UIKObj_MutualExclude", obj, tl_lang ))) goto END_ERROR;
    if (! UIK_AddVector( o, (void *)SetLangOther, GADGETDOWN, 0 )) goto END_ERROR;

    if (V.UIKBase->LocaleBase)
        {
        tl_lang[0].ti_Data = SETL(27,45);
        tl_lang[2].ti_Data = MSG_LOCALE_GAD;
        tl_lang[3].ti_Data = (V.Prefs.PrefsFlags & UIKLANG_LOCALE) ? FALSE : TRUE;
        tl_lang[7].ti_Data = B2L('L',0,0,0);
        if (! (o = UIK_AddObject( "UIKObj_Toggle", obj, tl_lang ))) goto END_ERROR;
        if (! UIK_AddVector( o, (void *)SetLangLocal, GADGETUP, 0 )) goto END_ERROR;
        tl_lang[7].ti_Data = 0;
        }

    /*------------ecran------------*/
    if (! (obj = UIK_AddObject( "UIKObj_Frame", V.WO, tl_ecranfr ))) goto END_ERROR;

    tl_ecran[0].ti_Data = SETL(22,8);
    tl_ecran[2].ti_Data = MSG_WORKBENCH_GAD;
    tl_ecran[3].ti_Data = (V.Prefs.PrefsFlags & UIKDEFAULT_SCREEN) ? TRUE : FALSE;
    tl_ecran[7].ti_Data = B2L('W',0,0,0);
    if (! (o = UIK_AddObject( "UIKObj_MutualExclude", obj, tl_ecran ))) goto END_ERROR;
    if (! UIK_AddVector( o, (void *)SetScreen, GADGETDOWN, 1 )) goto END_ERROR;

    tl_ecran[0].ti_Data = SETL(22,25);
    tl_ecran[2].ti_Data = MSG_UIK_GAD;
    tl_ecran[3].ti_Data = (V.Prefs.PrefsFlags & UIKDEFAULT_SCREEN) ? FALSE : TRUE;
    tl_ecran[7].ti_Data = B2L('K',0,0,0);
    if (! (o = UIK_AddObject( "UIKObj_MutualExclude", obj, tl_ecran ))) goto END_ERROR;
    if (! UIK_AddVector( o, (void *)SetScreen, GADGETDOWN, 0 )) goto END_ERROR;

    /*------------ecran UIK------------*/
    if (! (obj = UIK_AddObject( "UIKObj_Frame", V.WO, tl_ecranUIKfr ))) goto END_ERROR;

    tl_ecranUIK[0].ti_Data = SETL(22,8);
    tl_ecranUIK[2].ti_Data = (ULONG) &UBM_Palette;
    tl_ecranUIK[7].ti_Data = B2L('P',0,0,0);
    if (! (o = UIK_AddObject( "UIKObj_Boolean", obj, tl_ecranUIK ))) goto END_ERROR;
    if (! UIK_AddVector( o, (void*)ScreenColors, GADGETUP, 0 )) goto END_ERROR;

    tl_ecranUIK[0].ti_Data = SETL(22,26);
    tl_ecranUIK[1].ti_Data = SETL(106,16);
    tl_ecranUIK[2].ti_Data = (ULONG) &UBM_Dims;
    tl_ecranUIK[7].ti_Data = B2L('D',0,0,0);
    if (! (o = UIK_AddObject( "UIKObj_Boolean", obj, tl_ecranUIK ))) goto END_ERROR;
    if (! UIK_AddVector( o, (void*)ScreenMode, GADGETUP, 0 )) goto END_ERROR;

    /*------------Font------------*/
    if (! (obj = UIK_AddObject( "UIKObj_Frame", V.WO, tl_fontfr ))) goto END_ERROR;

    tl_font[2].ti_Data = (ULONG) &UBM_Font;
    tl_font[6].ti_Data = B2L('F',0,0,0);
    if (! (o = UIK_AddObject( "UIKObj_Boolean", obj, tl_font ))) goto END_ERROR;
    if (! UIK_AddVector( o, (void *)ChooseFont, GADGETUP, 0 )) goto END_ERROR;

    /*------------Mode------------*/
    if (! (obj = UIK_AddObjectTags( "UIKObj_Frame", V.WO,
            UIKTAG_OBJ_LeftTop,           SETL(186,101),
            UIKTAG_OBJ_WidthHeight,       SETL(150,31),
            UIKTAG_OBJ_Title,             MSG_MODE_FRAME,
            UIKTAG_OBJ_TTextAttr,         (ULONG) &FrameTTA,
            UIKTAG_OBJ_ShortCutKey,       B2L('M',0,0,0),
            TAG_END ))) goto END_ERROR;

    if (! (o = UIK_AddObjectTags( "UIKObj_Boolean", obj,
            UIKTAG_OBJ_LeftTop,           SETL(29,8),
            UIKTAG_OBJ_WidthHeight,       SETL(92,17),
            UIKTAG_OBJ_Title,             (ULONG) &UBM_Mode,
            UIKTAG_OBJ_BoxFl_Filled,      TRUE,
            UIKTAG_OBJ_TitleFl_Image,     TRUE,
            UIKTAG_OBJ_ParentRelative,    TRUE,
            UIKTAG_OBJ_ShortCutKey,       B2L('M',0,0,0),
            TAG_END ))) goto END_ERROR;

    if (! UIK_AddVector( o, (void *)ReqModes, GADGETUP, 0 )) goto END_ERROR;

    *pm = 0xff;
    if (V.UIK->Obj.Status != UIKS_STARTED)
        { if (! UIK_Start( V.UIK )) goto END_ERROR; }
    else
        { if (! UIK_Start( V.WO )) goto END_ERROR; }
    *pm = mask;


    return(1);

  END_ERROR:
    *pm = mask;
    return(0);
}
