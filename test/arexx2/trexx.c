/****************************************************************
 *
 *      File:      trexx.c
 *      Project:   Test de l'objet UIKObj_ARexxSim
 *
 *      Created:   17-08-91     Jean-Michel Forgeas
 *
 *  Lancer l'exécutable : "Run TRexx",
 *  puis lancer le script ARexx : "Rx tx.rexx"
 *
 ****************************************************************/


/****** Includes ************************************************/

#include <stdio.h>

#include <libraries/dos.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikglobal.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objarexxsim.h"

/* ces fichiers include sont fournis avec ceux du 2.0 */
#include <rexx/storage.h>
#include <rexx/errors.h>

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


/****** Exported ***********************************************/

struct UIKBase *UIKBase;
struct UIKGlobal *UIK;


/****** Statics ************************************************/


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

#ifdef LATTICE
int CXBRK(void) { return(0); }      /* disable SASC CTRLC handling */
int chkabort(void) { return(0); }   /* really */
#endif

static void ReceiveCmd( struct UIKObjARexxSim *aobj, UBYTE *cmd )
{
    /*----il faut retourner soit une erreur soit un résultat:
     *
     * Les champs de résultat et d'erreur sont mis à 0 avant
     * l'appel de cette fonction, donc il ne reste qu'à utiliser
     * ceux dont on a besoin.
     *
     * Exemples :
     *
     * si on veut indiquer un résultat :
     *      aobj->ResultCode   = RC_OK;
     *      aobj->ResultString = "400 20 10";
     *
     * si on veut indiquer une erreur :
     *      aobj->ResultCode  = RC_WARN ou RC_ERROR ou RC_FATAL;
     *      aobj->ErrorCode   = TEXT_NOMEMORY;
     *      aobj->ErrorString = UIK_LangString( UIK, TEXT_NOMEMORY );
     *
     * Dans un script ARexx, on retrouvera ces informations dans
     * les variables suivantes (mettre OPTIONS RESULTS)
     *      aobj->ResultCode   --> RC
     *      aobj->ResultString --> RESULT
     *      aobj->ErrorCode    --> <AppName>.LASTERROR
     *      aobj->ErrorString  --> <AppName>.LASTERRORTEXT
     */

    /*---- on reçoit la commande du script ARexx "tr.rexx" et on l'affiche */
    printf( "\nReçu: '%ls'... ", cmd );

    /*---- traitement sommaire : si la commande commence par un A on répond RC_OK
     *     sinon RC_ERROR. Si c'est 'Q' on quitte, le programme s'arrête.
     */
    if (*cmd == 'A')
        {
        aobj->ResultCode = RC_OK;
        aobj->ResultString = "Merci.";
        printf( "OK\n" );
        }
    else if (*cmd == 'Q')
        {
        aobj->ResultCode = RC_OK;
        aobj->ResultString = "Merci pour tout, à la prochaine!";
        printf( "Quitte\n" );
        Signal( FindTask(0), SIGBREAKF_CTRL_C ); /* fait sortir de UIK_Do() */
        }
    else{
        aobj->ResultCode = RC_ERROR;
        aobj->ErrorCode = 12345;
        aobj->ErrorString = "*** Non merci! (et je reste poli)";
        printf( "Erreur\n" );
        }
}

static struct TagItem tl[] = {
    { UIKTAG_ARexxSim_AppName,        (ULONG) "TRexx" },
    { UIKTAG_ARexxSim_PortNameDot,    (ULONG) '.' },
    { UIKTAG_ARexxSim_CmdReceiveFunc, (ULONG) ReceiveCmd },
    { TAG_END } };

static void Cleanup()
{
    if (UIK)        UIK_Remove( UIK );
    if (UIKBase)    CloseLibrary( UIKBase );
    exit(0);
}

void main()
{
  struct UIKObj *obj;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (UIK = (struct UIKGlobal *) UIK_AddObject( "UIK", 0, 0 ))) goto END_ERROR;

    /*------------objet ARexx------------*/
    if (! (obj = UIK_AddObject( "UIKObj_ARexxSim", UIK, tl ))) goto END_ERROR;

    if (! UIK_Start( UIK )) goto END_ERROR;
    printf( "PortName: %ls\n", UIK_CallObjectFunc( obj, UIKFUNC_ARexxSim_ARexxName, 0, 0 ) );

    UIK_Do2( UIK, SIGBREAKF_CTRL_C, 1 );

  END_ERROR:
    Cleanup();
}
