/****************************************************************
 *
 *      File:      menu.c
 *      Project:   Essai de l'objet UIKObj_Menu
 *
 *      Created:   25-02-92     Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include <stdio.h>

#include <libraries/dos.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikglobal.h"
#include "uiki:uikcolors.h"
#include "uiki:uikmacros.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objmenu.h"
#include "uiki:objscreen.h"
#include "uiki:objwindow.h"

#define TEXT_WindowTitle    1
#define TEXT_ScreenTitle    2

#define MSG_MENU_Project    3
#define MSG_MENU_New        4
#define MSG_MENU_Open       5
#define MSG_MENU_Save       6
#define MSG_MENU_Quit       7
#define MSG_MENU_Edit       8
#define MSG_MENU_Cut        9
#define MSG_MENU_Copy       10
#define MSG_MENU_Paste      11
#define MSG_MENU_Insert     12
#define MSG_MENU_Send       13
#define MSG_MENU_CR         14
#define MSG_MENU_LF         15
#define MSG_MENU_CRLF       16
#define MSG_MENU_FindSearch 17
#define MSG_MENU_Search     18
#define MSG_MENU_FindNext   19
#define MSG_MENU_FindBack   20
#define MSG_MENU_Case       21
#define MSG_MENU_Sensitive  22
#define MSG_MENU_Truc       23

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


/****** Statics ************************************************/

static void std_func();
static void Project_Quit();

static struct UIKGlobal *UIK;

static UBYTE *LangEnglish[] =
    {
    "",
    "MenuTest",
    "MenuTest 1.1 Copyright © 1992 J-M Forgeas",
    "Project",
        "New",
        "Open",
        "Save",
        "Quit",
    "Edit",
        "Cut",
        "Copy",
        "Paste",
        "Insert",
        "Send",
            "CR",
            "LF",
            "CRLF",
    "Search",
        "Search...",
        "Find Next",
        "Find Back",
        "Case",
            "Sensitive",
            "Truc...",
    0
    };

static UBYTE *LangOther[] =
    {
    "",
    "MenuTest",
    "MenuTest 1.1 Copyright © 1992 J-M Forgeas",
    " Projet ",
        "Nouveau",
        "Ouvrir",
        "Sauvegarder",
        "Quitter",
    " Edition ",
        "Couper",
        "Copier",
        "Coller",
        "Insertion",
        "Envoyer",
            "CR",
            "LF",
            "CRLF",
    " Recherche ",
        "Chercher...",
        "En Avant",
        "En Arrière",
        "Majuscules",
            "Sensible",
            "Truc...",
    0
    };

static struct TagItem TL_Window[] = {
    { UIKTAG_OBJ_LeftTop,       SETL(130,50) },
    { UIKTAG_OBJ_WidthHeight,   SETL(170,0) },
    { UIKTAG_OBJ_Title,         TEXT_WindowTitle },
    { UIKTAG_OBJ_AltTitle,      TEXT_ScreenTitle },
    { UIKTAG_OBJWindowFl_With_Size, FALSE },
    { UIKTAG_OBJWindowFl_With_Icon, FALSE },
    { UIKTAG_OBJWindowFl_NewLookMenu3, TRUE },
    { TAG_END } };

static struct UIKMenuEntry MenuEntry[] = {
    { 0,  0, MSG_MENU_Project,          NULL,     0,0,0,MENUENABLED },
    { 1,'Z',    MSG_MENU_New,           std_func, 0,0,0,ITEMENABLED },
    { 1,  0,    ITEM_SEPARATION },
    { 1,'O',    MSG_MENU_Open,          std_func, 0,0,0,ITEMENABLED },
    { 1,'S',    MSG_MENU_Save,          std_func, 0,0,0,ITEMENABLED },
    { 1,  0,    ITEM_SEPARATION },
    { 1,  0,    ITEM_SYSSEPARATION },
    { 1,'Q',    MSG_MENU_Quit,          Project_Quit, 0,0,0,ITEMENABLED },
    { 0,  0, MSG_MENU_Edit,             NULL,     0,0,0,MENUENABLED },
    { 1,'X',    MSG_MENU_Cut,           std_func, 0,0,0,ITEMENABLED },
    { 1,'C',    MSG_MENU_Copy,          std_func, 0,0,0,ITEMENABLED },
    { 1,'V',    MSG_MENU_Paste,         std_func, 0,0,0,ITEMENABLED },
    { 1,  0,    ITEM_SEPARATION },
    { 1,'I',    MSG_MENU_Insert,        std_func, 0,0,0,ITEMENABLED|MENUTOGGLE|CHECKIT|CHECKED },
    { 1,  0,    MSG_MENU_Send,          NULL,     0,0,0,ITEMENABLED },
    { 2,  0,        MSG_MENU_CR,        std_func, 0,0xfe,0,ITEMENABLED|CHECKIT|CHECKED },
    { 2,  0,        ITEM_SYSSEPARATION },
    { 2,  0,        MSG_MENU_LF,        std_func, 0,0xfb,0,ITEMENABLED|CHECKIT },
    { 2,  0,        ITEM_SEPARATION },
    { 2,  0,        MSG_MENU_CRLF,      std_func, 0,0xf7,0,ITEMENABLED|CHECKIT },
    { 0,  0, MSG_MENU_FindSearch,       NULL,     0,0,0,MENUENABLED },
    { 1,'F',    MSG_MENU_Search,        std_func, 0,0,0,ITEMENABLED },
    { 1,  0,    ITEM_SEPARATION },
    { 1,'N',    MSG_MENU_FindNext,      std_func, 0,0,0,ITEMENABLED },
    { 1,'B',    MSG_MENU_FindBack,      std_func, 0,0,0,ITEMENABLED },
    { 1,  0,    ITEM_SYSSEPARATION },
    { 1,  0,    MSG_MENU_Case,          NULL,     0,0,0,ITEMENABLED },
    { 2,  0,        MSG_MENU_Sensitive, std_func, 0,0,0,ITEMENABLED|MENUTOGGLE|CHECKIT },
    { 2,  0,        MSG_MENU_Truc,      std_func, 0,0,0,ITEMENABLED|MENUTOGGLE|CHECKIT|CHECKED },
    { 0,0,0,0,0,0 },  /* marqueur de fin: Title doit être à 0 */
    };

/****************************************************************
 *
 *      Fonctions des menus
 *
 ****************************************************************/

static void std_func( struct UIKObjMenu *mo, struct UIKMenuEntry *ume, struct MenuItem *item, UWORD menunum, UWORD itemnum, UWORD subnum )
{
    printf( "%ld %ld %ld %ls\n", menunum, itemnum, subnum, UIK_LangString( mo, ume->Title ) );
}

static void Project_Quit( struct UIKObjMenu *mo, struct UIKMenuEntry *ume, struct MenuItem *item, UWORD menunum, UWORD itemnum, UWORD subnum )
{
    std_func( mo, ume, item, menunum, itemnum, subnum );
    Signal( FindTask(0), SIGBREAKF_CTRL_C );
}

/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void Cleanup()
{
    if (UIK)            UIK_Remove( UIK );
    if (UIKBase)        CloseLibrary( UIKBase );
    exit(0);
}

static void MenuVerify()
{
    printf( "IDCMP_MENUVERIFY!!!\n" );
}

static void MenuPick()
{
    printf( "IDCMP_MENUPICK!!!\n" );
}

void main( int argc )
{
  struct UIKObj *wo, *parent=0, *menu;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (UIK = (struct UIKGlobal *) UIK_AddObjectTags( "UIK", 0,
             UIKTAG_GEN_LangEnglish,    (ULONG) LangEnglish,
             UIKTAG_GEN_LangOther,      (ULONG) LangOther,
             TAG_END ))) goto END_ERROR;

    /* pour tester sur un autre écran lancer le programme avec n'importe
     * quel argument.
     */
    if (argc > 1)
        {
        parent = (struct UIKObj *) UIK_AddObjectTags( "UIKObj_Screen", UIK,
                    UIKTAG_OBJ_WidthHeight, W2L(280,250),
                    UIKTAG_OBJ_FontName, "Times.font",
                    UIKTAG_OBJ_FontHeight, 24,
                    UIKTAG_ObjScreen_ViewModes, argc > 2 ? HIRES : 0,
                    /*UIKTAG_OBJ_ActInactPens, B2L(3,3,2,2),*/
                    TAG_END );
        }
    if (! parent) parent = UIK;

    if (! (wo = UIK_AddObject( "UIKObj_Window", parent, TL_Window ))) goto END_ERROR;
    if (! (menu = UIK_AddObjectTags( "UIKObj_Menu", wo,
                UIKTAG_Menu_Entry,      (ULONG) MenuEntry,
                UIKTAG_OBJ_MinVerRev,   W2L(1,5),
                /*UIKTAG_OBJ_ActInactPens, B2L(1,1,2,2),*/
                TAG_END ))) goto END_ERROR;

    UIK_AddVector( menu, (void*) MenuVerify, IDCMP_MENUVERIFY, 0 );
    UIK_AddVector( menu, (void*) MenuPick, IDCMP_MENUPICK, 0 );

    if (! UIK_Start( UIK )) goto END_ERROR;

    UIK_Do( UIK, SIGBREAKF_CTRL_C );

  END_ERROR:
    Cleanup();
}
