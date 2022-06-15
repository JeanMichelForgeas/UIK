/****************************************************************
 *
 *      Project:   UIK
 *      Function:  Test d'objets externes à la librairie
 *
 *      Created:   02/08/91    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include <stdio.h>

#include <libraries/dos.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikmacros.h"
#include "uiki:uikglobal.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objscreen.h"
#include "uiki:objfileselector.h"
#include "uiki:objwindow.h"

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


/****** Exported ************************************************/

struct UIKBase *UIKBase;
struct IntuitionBase *IntuitionBase;


/****** Statics *************************************************/

static void okfunc();
static void cancelfunc();

static struct UIKGlobal *UIK;

static UBYTE *LangEnglish[] =
    {
    "",
    "Waouh!",
    "12",
    "Screen title",
    0
    };

static UBYTE *LangOther[] =
    {
    "",
    "Waouh!",
    "12",
    "Titre écran",
    0
    };

static struct TTextAttr WinTTA = { "Times.font", 24, FSF_ITALIC|FSF_UNDERLINED, 0, 0 };

static struct TagItem wtl[] = {  /* window */
    { UIKTAG_OBJ_LeftTop,       SETL(800,150) },
    { UIKTAG_OBJ_WidthHeight,   SETL(290,170) },
    { UIKTAG_OBJ_Title,         1 },
    { UIKTAG_OBJ_AltTitle,      1 },
    { UIKTAG_OBJ_ActInactPens,  UIKCOLS(UIKCOL_LIGHT,UIKCOL_GREY,UIKCOL_WHITE,UIKCOL_BLACK) },
    { UIKTAG_OBJWindow_MinimumWidthHeight, SETL(200,150) },
    { UIKTAG_OBJ_TTextAttr,         (ULONG) &WinTTA },
    { TAG_END } };

static struct TagItem fstl[] = {  /* file selector */
    { UIKTAG_OBJ_LeftTop,           SETL(5,3) },
    { UIKTAG_OBJ_WidthHeight,       SETL(280,165) },
    { UIKTAG_FS_OKFUNC,             (ULONG) okfunc },
    { UIKTAG_FS_CANCELFUNC,         (ULONG) cancelfunc },
    { UIKTAG_OBJ_FontName,          (ULONG) "topaz.font" },
    { UIKTAG_OBJ_FontHeight,        (ULONG) 8 },
    { UIKTAG_FSFl_ReturnReturn,     TRUE },
    { UIKTAG_FSFl_OK_IfNotExists,   TRUE },
    { UIKTAG_OBJ_Sz_Attached_Flags, UO_ATTACHED_TOP|UO_ATTACHED_BOT|UO_ATTACHED_LEFT|UO_ATTACHED_RIGHT },
    { TAG_END }
    };

static struct TagItem scrtl[] = {  /* screen */
    { UIKTAG_OBJ_Title,             3 },
    { UIKTAG_OBJ_FontName,          (ULONG) "Times.font" },
    { UIKTAG_OBJ_FontHeight,        (ULONG) 24 },
    { UIKTAG_OBJ_WidthHeight,       SETL(320,256) },
    { UIKTAG_ObjScreen_Depth,       3 },
    { UIKTAG_ObjScreen_ViewModes,   0 },
    { TAG_END }
    };

/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void okfunc( struct UIKObjFileSelector *fso, UBYTE *filename )
{
    printf("OK: '%ls'\n",filename);
}
static void cancelfunc( struct UIKObj *gad, struct UIKObjFileSelector *fso )
{
    printf("Cancel!\n");
}

static void Cleanup()
{
    if (UIK)            UIK_Remove( UIK );
    if (UIKBase)        CloseLibrary( UIKBase );
    if (IntuitionBase)  CloseLibrary( IntuitionBase );
    exit(0);
}

void main( int argc )
{
  struct UIKObj *parent=0;
  struct UIKObjWindow *wo;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) exit(20);
    if (! (IntuitionBase = (struct UIKBase *) OpenLibrary( "intuition.library", 0 ))) exit(20);

    if (! (UIK = (struct UIKGlobal *) UIK_AddObjectTags( "UIK", 0,
             UIKTAG_GEN_LangEnglish,    (ULONG) LangEnglish,
             UIKTAG_GEN_LangOther,      (ULONG) LangOther,
             TAG_END ))) goto END_ERROR;

    /* pour tester sur un autre écran lancer le programme avec n'importe
     * quel argument.
     */
    if (argc > 1)
        {
        parent = UIK_AddObject( "UIKObj_Screen", UIK, scrtl );
        }
    if (! parent) parent = UIK;

    if (! (wo = (struct UIKObjWindow *) UIK_AddObject( "UIKObj_Window", parent, wtl ))) goto END_ERROR;
    if (! UIK_AddObject( "UIKObj_FileSelector", wo, fstl )) goto END_ERROR;

    if (! UIK_Start( UIK )) goto END_ERROR;

    UIK_Do( UIK, SIGBREAKF_CTRL_C );

    END_ERROR: Cleanup();
}

