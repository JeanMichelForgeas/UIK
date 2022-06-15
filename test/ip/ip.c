/****************************************************************
 *
 *      File:      SmallTest.c
 *      Project:   UIK example code
 *
 *      Created:   23-08-92     Jean-Michel Forgeas
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

#include <exec/lists.h>
#include <exec/execbase.h>
#include <libraries/dos.h>
#include <libraries/dosextens.h>

#include <clib/exec_protos.h>
#include <pragmas/exec_pragmas.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikmacros.h"
#include "uiki:uikglobal.h"
#include "uiki:uikcolors.h"
#include "uiki:uikip_protos.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objwindow.h"

#define TEXT_Usage          1
#define TEXT_Found          2
#define TEXT_NotFound       3
#define TEXT_UIKNotFound    4
#define TEXT_ExplTasks      5
#define TEXT_ExplTask       6
#define TEXT_Removing       7


/****** Imported ************************************************/

extern struct ExecBase * __far SysBase;


/****** Exported ***********************************************/

struct UIKBase * __far UIKBase;
struct UIKGlobal * __far UIK;


/****** Statics ************************************************/

static ULONG __far total_added;
static ULONG __far adding;

static UBYTE * __far LangEnglish[] =
    {
    "",
    "Usage : ip <nom tâche> <nom tâche> <...>\n"
        "Run several programs using UIK and opening a window, then run\n"
        "'ip' with these programs task names.\n"
        "A gadget will be added on every opened  window of every task.\n"
        "A click on a gadget will add a child window.\n"
        ,
    "»»» Task found:",
    "*** Task not found:",
    "*** UIK not found on task :",
    "Objets have been added on other tasks, and a vector has been added\n"
        "on them, which code is located in this program.\n"
        "So only after the other tasks will be terminated you will may\n"
        "terminate this one with Ctrl+C.\n"
        "To improve this example one could keep track of all the added\n"
        "gadgets, and at the end do IPUIK_Remove() on every one.\n"
        "So one could do Ctrl+C at any moment.\n"
        ,
    "An objet has been added on an other task, and a vector has been\n"
        "added on it, which code is located in this program.\n"
        "So only after the other task will be terminated you will may\n"
        "terminate this one with Ctrl+C.\n"
        "To improve this example one could keep track of all the added\n"
        "gadgets, and at the end do IPUIK_Remove() on every one.\n"
        "So one could do Ctrl+C at any moment.\n"
        ,
    "is removing object",
    0
    };

static UBYTE * __far LangOther[] =
    {
    "",
    "Usage : ip <nom tâche> <nom tâche> <...>\n"
        "Lancez quelques programmes utilisant UIK et ouvrant une fenêtre,\n"
        "puis lancez 'ip' avec le nom des tâches de ces programmes.\n"
        "Un gadget sera greffé sur chaque fenêtre ouverte de chacune des\n"
        "tâches. Un clic sur un gadget greffera une fenêtre fille.\n"
        ,
    "»»» Tâche trouvée :",
    "*** Tâche introuvable :",
    "*** UIK introuvable sur la tâche :",
    "Des objets ont été greffés sur d'autres tâches, et un vecteur\n"
        "leur a été ajouté dont le code se trouve dans ce programme.\n"
        "Donc seulement une fois que les autres tâches seront terminées\n"
        "vous pourrez terminer celle-ci par Ctrl+C.\n"
        "Pour améliorer cet exemple, on pourrait garder la trace de tous\n"
        "les gadgets ajoutés, et à la fin faire IPUIK_Remove() sur chacun.\n"
        "Ainsi on pourrait faire Ctrl+C n'importe quand.\n"
        ,
    "Un objet a été greffé sur une autre tâche, et un vecteur lui a\n"
        "été ajouté dont le code se trouve dans ce programme.\n"
        "Donc seulement une fois que l'autre tâche sera terminée vous\n"
        "pourrez terminer celle-ci par Ctrl+C.\n"
        "Pour améliorer cet exemple, on pourrait garder la trace de tous\n"
        "les gadgets ajoutés, et à la fin faire IPUIK_Remove() sur chacun.\n"
        "Ainsi on pourrait faire Ctrl+C n'importe quand.\n"
        ,
    "supprime l'objet",
    0
    };


/****************************************************************
 *
 *      Fonction exécutée sous le contexte des autres tâches
 *
 *      Du fait qu'elle est un vecteur ajouté aux objets
 *      greffés sur les autres tâches, elle est appelée par
 *      les UIK_Do() des UIK des autres tâches.
 *      Comme on veut que ce bout de code ajoute une fenêtre
 *      aux UIK des autres tâches, il utilise simplement les
 *      fonctions commençant par UIK_
 *
 *      Attention, pas de variables globales ici si le programme
 *      est compilé en data-relative.
 *      Si il est compilé en adressage absolu (-b0 avec SAS C)
 *      alors pas de problème.
 *      Attention, printf() nécessite des variables globales
 *      i(stdout, etc...).
 *
 *      De plus il faut que UIKBase et toutes les bases des
 *      librairies utilisées dans les fonctions exécutées par
 *      l'autre tâche soient déclarées en "far" (pour qu'elles
 *      soient déclarées en absolu).
 *
 ****************************************************************/

static void task_addwindow( struct UIKObj *obj )
{
  struct UIKObj *wo;

    if (wo = UIK_AddObjectTags( "UIKObj_Window", obj,
                UIKTAG_OBJ_LeftTop,         SETL(180,30),
                UIKTAG_OBJ_WidthHeight,     SETL(200,50),
                UIKTAG_OBJ_Title,           "Greffe",
                UIKTAG_OBJ_TitleFl_Addr,    TRUE,
                UIKTAG_OBJWindowFl_CloseRemove, TRUE,
                TAG_END ))
            UIK_Start( wo );
}

/****************************************************************
 *
 *      Code exécuté sous le contexte du programme ip
 *
 ****************************************************************/

//------ utilitaires pour obtenir et chercher le nom d'une tâche

static UBYTE *get_taskname( struct Node *node, UBYTE *dest )
{
  UBYTE *name=0;

    if (node->ln_Type == NT_PROCESS)
        {
        struct Process *process = (struct Process *)node;
        struct CommandLineInterface *cli;
        if (process->pr_TaskNum)
            if (cli = (struct CommandLineInterface *) BADDR(process->pr_CLI))
                {
                StrB2C( (ULONG)BADDR(cli->cli_CommandName), dest );
                name = dest;
                }
        }
    if (name == 0)
        {
        if (node->ln_Name) name = node->ln_Name;
        else name = "";
        }
    return( name );
}

static struct Task *find_in_list( struct List *list, UBYTE *findname )
{
  struct Node *node;
  UBYTE buf[200];

    for (node=list->lh_Head; node->ln_Succ; node=node->ln_Succ)
        {
        if (StrNCCmp( UIK_BaseName( get_taskname( node, buf ) ), findname ) == 0)
            return( (struct Task *) node );
        }
    return(0);
}

static struct Task *FindTaskName( UBYTE *name ) // doit être appelée sous Forbid()
{
  struct Task *task;

    if (! (task = find_in_list( &SysBase->TaskWait, name )))
        task = find_in_list( &SysBase->TaskReady, name );
    return( task );
}

//------ Callback appelé par UIK_Do() prévenant qu'un objet extérieur
//       (en visite) va être supprimé

static void VisitorObjectRemoved( struct Task *othertask, struct UIKObj *obj )
{
  UBYTE buf[200];

    printf( "\n!!! Task '%ls' %ls %lx '%ls' '%ls'\n",
                    get_taskname( (struct Node *)othertask, buf ),
                    UIK_LangString( UIK, TEXT_Removing ),
                    obj,
                    obj->RegGerm->Germ->ObjName,
                    UIK_ObjTitle( obj )
                    );

    // On a mis l'adresse de la variable où est rangée le pointeur sur
    // l'objet. On la met à zéro, ainsi le programme pourra la tester.

    *((ULONG*)obj->UserULong1) = 0;

    // Si cette fonction est appelée en dehors de la phase d'ajout on
    // décrémente le nombre des objets restant sur les tâches visitées
    // car alors la suppression est faite par la tâche visitée (valable
    // pour cet exemple car ce programme ne fait pas d'IPUIK_Remove()
    // en dehors de la phase d'ajout).

    if (adding == 0)
        {
        --total_added;

        // on termine le programme si tous les objets visiteurs ont été
        // supprimés par les tâches visitées.

        if (total_added == 0) Signal( FindTask(0), SIGBREAKF_CTRL_C );
        }
}

//-----------------------------------------------------------

static void Cleanup()
{
    if (UIK)     UIK_Remove( UIK );
    if (UIKBase) CloseLibrary( UIKBase );
    exit(0);
}

void main( int argc, char *argv[] )
{
  struct Task *task;
  struct UIKGlobal *tuik;
  struct UIKObj *gad, *after;
  ULONG i, ok;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (UIK = (struct UIKGlobal *) UIK_AddObjectTags( "UIK", 0,
             UIKTAG_GEN_LangEnglish,    LangEnglish,
             UIKTAG_GEN_LangOther,      LangOther,
             UIKTAG_GEN_Catalog,        "uik/ip.catalog", /* prend seulement si présent */
             TAG_END ))) goto END_ERROR;
    if (! UIK_Start( UIK )) goto END_ERROR;

    if (argc == 1)
        {
        printf( "%ls", UIK_LangString( UIK, TEXT_Usage ) );
        goto END_ERROR;
        }

    /* chaque argument est un nom de tâche */
    for (i=1; i < argc; i++)
        {
        Forbid(); /* Forbid() pour être sûr qu'une tâche trouvée ne se termine
                   * pas entre-temps.
                   * Les fonctions qui commencent par IPUIK_ cassent Forbid()
                   * car elles appellent Wait().
                   */
        if (task = FindTaskName( argv[i] ))
            {
            printf( "%ls '%ls'\n", UIK_LangString( UIK, TEXT_Found ), argv[i] );
            if (tuik = UIK_GetTaskUIK( task ))
                {
                after = 0;
                while (after = UIK_GetObject( tuik, after, UIKID_WINDOW ))
                    {
                    if (after->Status == UIKS_STARTED)
                        {
                        // les fonctions AddVector, Start, Stop, Remove, etc...
                        // testent le pointeur sur l'objet, donc on peut leur
                        // passer 0 (voir uik.doc).
                        // la fonction VisitorObjectRemoved() peut être appelée
                        // pendant n'importe quelle fonction IP, c'est à dire
                        // que l'application visitée pourra supprimer notre
                        // objet pendant chaque appel d'une fonction IP.
                        // La variable 'gad' peut donc être mise à 0 lors de
                        // chaque appel de ces fonctions.
                        ok = 0;
                        adding = 1;
                        if (gad = IPUIK_AddObjectTags( task, "UIKObj_Boolean", after,
                            UIKTAG_OBJ_LeftTop,         SETL(0,15),
                            UIKTAG_OBJ_WidthHeight,     SETL(77,23),
                            UIKTAG_OBJ_Title,           "_Greffe",
                            UIKTAG_OBJ_TitleFl_Addr,    TRUE,
                            UIKTAG_OBJ_TitleUnderscore, '_',
                            UIKTAG_OBJ_BoxFl_Filled,    TRUE,
                            UIKTAG_OBJ_IPRemoveFunc,    VisitorObjectRemoved,
                            UIKTAG_OBJ_UserValue1,      &gad,
                            TAG_END ))
                            {
                            if (IPUIK_AddVector( task, gad, (void*)task_addwindow, GADGETUP, 0 ))
                                if (IPUIK_Start( task, gad ))
                                    {
                                    total_added++;
                                    ok = 1;
                                    }
                            }
                        if (gad && !ok) IPUIK_Remove( task, gad );
                        adding = 0;
                        }
                    }
                }
            }
        Permit();

        if (! task) printf( "%ls '%ls'\n", UIK_LangString( UIK, TEXT_NotFound ), argv[i] );
        else if (! tuik) printf( "%ls '%ls'\n", UIK_LangString( UIK, TEXT_UIKNotFound ), argv[i] );
        }

    if (total_added)
        {
        printf( "%ls", UIK_LangString( UIK, (total_added > 1) ? TEXT_ExplTasks : TEXT_ExplTask ) );

        UIK_Do2( UIK, SIGBREAKF_CTRL_C, 1 );
        }

  END_ERROR:
    Cleanup();
}
