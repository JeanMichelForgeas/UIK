/****************************************************************
 *
 *      File:      smusplayer.c
 *      Project:   Essai de l'objet UIKObj_SMUS
 *
 *      Created:   06-03-92     Jean-Michel Forgeas
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

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikglobal.h"
#include "uiki:uikcolors.h"
#include "uiki:uikmacros.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objsmus.h"

#include <libraries/dos.h>
#include <clib/dos_protos.h>


/****** Imported ************************************************/

extern struct DOSBase *DOSBase;
extern struct ExecBase *SysBase;


/****** Exported ***********************************************/

struct UIKBase *UIKBase;


/****** Statics ************************************************/

static struct UIKGlobal *UIK;

/* Les tracks ci-dessous sont des SEvents du format SMUS qui décrivent
 * des notes et leur longueur.
 * Voir la doc IFF sur le format SMUS.
 * (les tracks n'ont pas besoin d'avoir la même longueur)
 */
static UBYTE Track1[] = {
    0x87,0, 0x83,0, 0x40,2, 0x41,2, 0x42,2, 0x43,2, 0x44,2, 0x45,2, 0x46,2, 0x47,2
    };
static UBYTE Track2[] = {
    0x87,0, 0x83,0, 0x50,2, 0x51,2, 0x52,2, 0x53,2, 0x54,2, 0x55,2, 0x56,2, 0x57,2
    };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void currfunc( struct UIKObjSMUS *smus, ULONG parm, ULONG index )
{
    printf( "{%ld:%ld} ", parm, index );
}

static void endfunc( struct UIKObjSMUS *smus, ULONG parm )
{
    printf( "{%ld:-} ", parm );
}

static void finishfunc( struct UIKObjSMUS *smus, ULONG parm )
{
    printf( "\nFinished\n" );
    Signal( FindTask(0), SIGBREAKF_CTRL_F );
}

/*----------------------------------------------------------------*/

static void Cleanup()
{
    if (UIK)            UIK_Remove( UIK );
    if (UIKBase)        CloseLibrary( UIKBase );
    exit(0);
}

void main( int argc, UBYTE *argv[] )
{
  struct UIKObj *smus;
  UBYTE *filename=0;
  UBYTE *instruments_dir=0;
  ULONG events;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (UIK = (struct UIKGlobal *) UIK_AddObject( "UIK", 0, 0 ))) goto END_ERROR;

    printf( "Usage: SMUSPlayer <fichier SMUS> <répertoire des instruments>\n" );

    if (argc > 1) filename = argv[1];
    if (argc > 2) instruments_dir = argv[2];

    if (! (smus = (struct UIKObj *) UIK_AddObjectTags( "UIKObj_SMUS", UIK,
                UIKTAG_OBJSMUS_UserCurrentFunc0, currfunc,
                UIKTAG_OBJSMUS_UserEndFunc0,     endfunc,
                UIKTAG_OBJSMUS_UserParm0,        0,
                UIKTAG_OBJSMUS_UserCurrentFunc1, currfunc,
                UIKTAG_OBJSMUS_UserEndFunc1,     endfunc,
                UIKTAG_OBJSMUS_UserParm1,        1,
                UIKTAG_OBJSMUS_UserCurrentFunc2, currfunc,
                UIKTAG_OBJSMUS_UserEndFunc2,     endfunc,
                UIKTAG_OBJSMUS_UserParm2,        2,
                UIKTAG_OBJSMUS_UserCurrentFunc3, currfunc,
                UIKTAG_OBJSMUS_UserEndFunc3,     endfunc,
                UIKTAG_OBJSMUS_UserParm3,        3,
                UIKTAG_OBJSMUS_UserFinishFunc,   finishfunc,
                UIKTAG_OBJSMUS_UserFinishParm,   0,
                TAG_END ))) goto END_ERROR;

    if (! UIK_Start( UIK )) goto END_ERROR;

    /*---------Préchargement de quelques instruments (pas obligatoire) */
    if (0 > UIK_CallObjectFunc( smus, UIKFUNC_SMUS_LoadInstrument, (ULONG) "ACCORDION", (ULONG) instruments_dir ))
        { printf( "Cannot preload ACCORDION\n" ); }
    if (0 > UIK_CallObjectFunc( smus, UIKFUNC_SMUS_LoadInstrument, (ULONG) "jAZZgUIT", (ULONG) instruments_dir ))
        { printf( "Cannot preload jAZZgUIT\n" ); }
    if (0 > UIK_CallObjectFunc( smus, UIKFUNC_SMUS_LoadInstrument, (ULONG) "flute", (ULONG) instruments_dir ))
        { printf( "Cannot preload flute\n" ); }
    if (0 > UIK_CallObjectFunc( smus, UIKFUNC_SMUS_LoadInstrument, (ULONG) "aHHvOICE", (ULONG) instruments_dir ))
        { printf( "Cannot preload aHHvOICE\n" ); }
    if (0 > UIK_CallObjectFunc( smus, UIKFUNC_SMUS_LoadInstrument, (ULONG) "ElecBass", (ULONG) instruments_dir ))
        { printf( "Cannot preload ElecBass\n" ); }

    /*===============================================================
     * 1er exemple: on joue une partition chargée depuis un fichier
     */

    /*---------Chargement d'une partition SMUS */
    if (IFF_DONE != UIK_CallObjectFunc( smus, UIKFUNC_SMUS_LoadSMUS, (ULONG) filename, (ULONG) instruments_dir )) goto END_ERROR;

    /*---------et on la joue */
    if (! UIK_CallObjectFunc( smus, UIKFUNC_SMUS_StartPlay, 0, 0 )) goto END_ERROR;

    /*---------Si on sort avec le signal SIGBREAKF_CTRL_F c'est que le morceau est fini */
    events = UIK_Do2( UIK, SIGBREAKF_CTRL_C|SIGBREAKF_CTRL_F, 1 );
    if (events & SIGBREAKF_CTRL_C) goto END_ERROR;

    /*===============================================================
     * 2ème exemple: on joue une partition directement définie dans
     * ce programme.
     * Exceptionnellement l'ordre des tags a ici sont importance
     * dans l'attribution des instruments à chaque piste.
     */

    /*---------Maintenant on installe une partion préprogrammée */
    if (! UIK_CallObjectFuncTags( smus, UIKFUNC_SMUS_NewSMUS, (ULONG) instruments_dir,
                UIKTAG_NEWSMUS_Tempo,       12000,
                UIKTAG_NEWSMUS_Instrument,  "Trumpet",
                UIKTAG_NEWSMUS_TrackStart,  (ULONG)Track1,
                UIKTAG_NEWSMUS_TrackEnd,    (ULONG)Track1 + sizeof(Track1),
                UIKTAG_NEWSMUS_Instrument,  "PipeOrgan",
                UIKTAG_NEWSMUS_TrackStart,  (ULONG)Track2,
                UIKTAG_NEWSMUS_TrackEnd,    (ULONG)Track2 + sizeof(Track2),
                TAG_END )) goto END_ERROR;

    /*---------et on la joue */
    if (! UIK_CallObjectFunc( smus, UIKFUNC_SMUS_StartPlay, 0, 0 )) goto END_ERROR;

    UIK_Do2( UIK, SIGBREAKF_CTRL_C|SIGBREAKF_CTRL_F, 1 );

  END_ERROR:
    Cleanup();
}
