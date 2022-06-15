/****************************************************************
 *
 *      File:      clipndraw.c
 *      Project:   Dessin délimité à un objet
 *
 *      Created:   04-02-91     Jean-Michel Forgeas
 *
 *      Note:      Ce code est linké avec rstartup.o qui est un
 *                 startup fourni par Commodore et permet de
 *                 faire un executable réentrant (pure, bit p)
 *                 que l'on peut rendre résident.
 *                 Comme stdout n'existe pas, il faut utiliser
 *                 fprintf( Output(),... ) de amiga.lib ou alors
 *                 Write( Output(),... ) au lieu de printf(...).
 *
 ****************************************************************/


/****** Includes ************************************************/

#include <stdio.h>

#ifdef LATTICE
int CXBRK(void) { return(0); }      /* disable SASC CTRLC handling */
int chkabort(void) { return(0); }   /* really */
#endif
#ifdef SAS
int CXBRK    ( void ) { return(0); }
int _CXBRK   ( void ) { return(0); }
int chkabort ( void ) { return(0); }
#endif

#include <exec/memory.h>
#include <libraries/dos.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikmacros.h"
#include "uiki:uikglobal.h"
#include "uiki:uikcolors.h"
#include "uiki:uiktree.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objwindow.h"

#define TEXT_WindowTitle    1
#define TEXT_ScreenTitle    2

struct Vars {
    struct UIKBase *UIKBase;
    struct GfxBase *GfxBase;
    struct UIKGlobal *UIK;
    struct UIKObjWindow *wo;
    struct UIKObjGad    *box;
    };
typedef struct Vars VARS;


/****** Imported ************************************************/

extern ULONG __far DOSBase;


/****** Exported ***********************************************/

struct UIKBase * __far UIKBase;
struct GfxBase * __far GfxBase;

/* Un peu de magie: main() commence par mettre dans A4 l'adresse
 * de la structure des variables, car dans cet exemple on n'utilise
 * pas c.o mais rstartup.o lors du link.
 * Si on définit comme relative (near) une seule variable, le
 * compilateur comprend que cette variable est à l'offset 0,
 * donc on peut l'utiliser directement dans le code. Cette
 * variable ne sert à rien sinon à générer du code relatif à A4
 * (A4 contenant l'adresse de notre structure allouée).
 * Exemple: 'V.box = ...' génèrera directement: move.l ...,16(A4)
 * ATTENTION: V doit être la seule ou la première variable
 * définie comme relative.
 * ATTENTION: rstartup.o possède un hunk de datas, donc il ne faut
 * pas utiliser l'option SMALLDATA de blink sous peine d'avoir des
 * mauvais offsets(A4) générés par le compilateur!
 * Cette déclaration mange un peu de mémoire pour rien...
 * Une autre méthode qui ne mange que 4 octets peut être de déclarer
 *      - 'long __near V' dans un autre source C
 *      - SECTION _MERGED,DATA
 *        IDNT    _MERGED
 *                xdef.l  _G
 *        _V:     dc.l    0  dans un source asm
 * et de déclarer dans celui-ci: 'extern VARS __near V'.
 */
VARS __near V;


/****** Statics ************************************************/

/* Chaines de caractères utilisées dans le programme */

static UBYTE * __far LangEnglish[] =
    {
    "",
    "ClipNDraw",
    "ClipNDraw Copyright © 1991 Jean-Michel Forgeas",
    0
    };

static UBYTE * __far LangOther[] =
    {
    "",
    "ClipNDraw",
    "ClipNDraw Copyright © 1991 Jean-Michel Forgeas",
    0
    };

/* Paramètres des objets sous forme de TagLists */

static struct TagItem __far uiktl[] = {  /* UIK */
    { UIKTAG_GEN_LangEnglish, (ULONG) LangEnglish },
    { UIKTAG_GEN_LangOther,   (ULONG) LangOther },
    { TAG_END } };

static struct TagItem __far wtl[] = {  /* window */
    { UIKTAG_OBJ_LeftTop,       SETL(180,30) },
    { UIKTAG_OBJ_WidthHeight,   SETL(319,113) },
    { UIKTAG_OBJ_Title,         1 },
    { UIKTAG_OBJ_AltTitle,      2 },
    { UIKTAG_OBJ_ActInactPens,  UIKCOL_LIGHT<<24 | UIKCOL_GREY<<16 | UIKCOL_WHITE<<8 | UIKCOL_BLACK },
    { UIKTAG_OBJWindowFl_AutoCenter, TRUE },
    { TAG_END } };

static struct TagItem __far boxtl[] = {  /* boolean */
    { UIKTAG_OBJ_LeftTop,       SETL(55,3) },
    { UIKTAG_OBJ_WidthHeight,   SETL(211,107) },
    { UIKTAG_OBJ_ActInactPens,  UIKCOL_WHITE<<24 | UIKCOL_WHITE<<16 | UIKCOL_BLACK<<8 | UIKCOL_BLACK },
    { UIKTAG_OBJ_BoxFl_Over,    FALSE },
    { UIKTAG_OBJ_BoxFl_NoAct,   TRUE },
    { UIKTAG_OBJ_BoxFl_Filled,  TRUE },
    { UIKTAG_OBJ_BoxFl_Clipped, TRUE },
    { UIKTAG_OBJ_BoxFl_ExtGadgetUp, TRUE }, /* reçoit GADGETUP même si relevé en dehors du gadget */
    { UIKTAG_OBJ_Sz_Attached_Flags, UO_ATTACHED_LEFT|UO_ATTACHED_TOP|UO_ATTACHED_RIGHT|UO_ATTACHED_BOT },
    { TAG_END } };

/****************************************************************
 *
 *      Fonctions appelées lors de la réception d'évènements
 *
 ****************************************************************/

/* Relève du bouton : on 'déclip' l'objet de dessin.
 */
static void MouseUpFunc( struct UIKObj *obj )
{
    UIK_UnclipObject( obj );
}

/* Appui sur le bouton : on clip l'objet de dessin et on trace
 * un point.
 */
static void MouseDownFunc( struct UIKObj *obj )
{
  register struct RastPort *rp = obj->rp;

    UIK_ClipObject( obj );
    SetAPen( rp, V.UIKBase->UIKPrefs->Colors.Black );
    WritePixel( rp, obj->MouseX, obj->MouseY );
    Move( rp, obj->MouseX, obj->MouseY );
}

/* Déplacement avec bouton appuyé : on trace une droite vers
 * le point où est le pointeur de la souris.
 */
static void MouseMoveFunc( struct UIKObj *obj )
{
    Draw( obj->rp, obj->MouseX, obj->MouseY );
}

/****************************************************************
 *
 *      Arborescence des objets+fonctions pour UIK_MakeUIKTree()
 *
 ****************************************************************/

static struct UIKHookItem __far BoxHooks[] =
    {
    { MouseDownFunc, GADGETDOWN },
    { MouseUpFunc, GADGETUP },
    { MouseMoveFunc, MOUSEMOVE },
    { 0 }
    };

static struct UIKTreeItem __far Arbo[] =
    {
    OBJECT( 0, offsetof(VARS,UIK), "UIK", uiktl, 0, 0, 0 ),
        OBJECT( 1, offsetof(VARS,wo), "UIKObj_Window", wtl, 0, 0, 0 ),
            OBJECT( 2, offsetof(VARS,box), "UIKObj_Boolean", boxtl, 0, 0, BoxHooks ),
    OBJEND
    };

/****************************************************************
 *
 *      Code principal
 *
 ****************************************************************/

/* La fonction __builtin_getreg( 12 ) est propre au SAS C et sert
 * à mettre le contenu du registre A4 dans une variable.
 * Ici on le met dans 'vars' puis on libère cette mémoire allouée
 * dans main().
 */
static void Cleanup()
{
  VARS *vars;

    if (vars = (VARS *) __builtin_getreg( 12 )) /* 12 = A4, qui point sur la structure des variables */
        {
        if (V.UIK)     UIK_Remove( V.UIK );
        if (V.UIKBase) CloseLibrary( V.UIKBase );
        if (V.GfxBase) CloseLibrary( V.GfxBase );
        FreeMem( vars, sizeof(VARS) );
        }
    exit(0);
}

/* Ce programme sert aussi à montrer comment faire du code réentrant.
 * main() alloue de la mémoire à l'image d'une structure destinée à
 * recevoir les variables pour cette exécution.
 * Dans le makefile les utilisateurs du SAS C voient que comme l'option
 * -b0 n'est pas mise, le code généré par le compilateur accède à ses
 * variables par un offset relatif au registre A4.
 * Donc on sait que A4 ne sera pas modifié, et on s'en sert pour stocker
 * la base de nos variables, c'est à dire l'adresse de la mémoire allouée
 * qui est 'vars'.
 * La fonction __builtin_putreg( 12, vars ) est propre au SAS C et sert
 * à mettre l'adresse 'vars' dans le registre A4.
 * Les autres compilateurs devraient avoir des fonctions comme geta4()
 * et seta4(), mais si ce n'est pas le cas, refaites cet exemple plus
 * simplement en enlevant cette mixture propre au SAS C.
 */
void main()
{
  VARS *vars = (VARS *) AllocMem( sizeof(VARS), MEMF_PUBLIC | MEMF_CLEAR );

    __builtin_putreg( 12, vars ); /* A4 pointe maintenant sur la structure des variables */
    if (! vars) goto END_ERROR;

    if (! (V.GfxBase = GfxBase = (struct GfxBase *) OpenLibrary( "graphics.library", 0 ))) goto END_ERROR;
    if (! (V.UIKBase = UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;

    if (! UIK_MakeUIKTree( Arbo, V.UIK, &V )) goto END_ERROR; /* &V est remplacé par 'LEA 0(A4),An' */

    if (! UIK_Start( V.UIK )) goto END_ERROR;

    UIK_Do( V.UIK, SIGBREAKF_CTRL_C );

  END_ERROR:
    Cleanup();
}
