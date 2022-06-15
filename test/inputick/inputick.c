/****************************************************************
 *
 *      File:      inputick.c
 *      Project:   Essai de l'objet UIKObj_Inputick
 *
 *      Created:   28-04-92     Jean-Michel Forgeas
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
#include "uiki:uikglobal.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objinputick.h"


/****** Imported ************************************************/

extern ULONG DOSBase;
extern struct ExecBase *SysBase;


/****** Exported ***********************************************/

struct UIKBase *UIKBase;


/****** Statics ************************************************/

static struct UIKGlobal *UIK;


/****************************************************************
 *
 *      Fonctions sur les boutons
 *
 ****************************************************************/

static void EventFunc( struct UIKObjInputick *ipt, UBYTE buttons, UBYTE changes )
{
    printf( "%ls%ld%ls%ld%ls%ld%ls%ld%ls%ld   \n",
        buttons & VBJOYF_RIGHT ? "  RIGHT-" : "       -", changes & VBJOYF_RIGHT ? 1 : 0,
        buttons & VBJOYF_LEFT  ? "  LEFT-"  : "      -",  changes & VBJOYF_LEFT  ? 1 : 0,
        buttons & VBJOYF_BACK  ? "  DOWN-"  : "      -",  changes & VBJOYF_BACK  ? 1 : 0,
        buttons & VBJOYF_FORW  ? "  UP-"    : "    -",    changes & VBJOYF_FORW  ? 1 : 0,
        buttons & VBJOYF_FIRE  ? "  FIRE-"  : "      -",  changes & VBJOYF_FIRE  ? 1 : 0
        );
}

void CtrlCFunc()
{
    printf( "  \n***Break\n" );
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

void main()
{
  void (*CtrlCVector)();

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (UIK = (struct UIKGlobal *) UIK_AddObject( "UIK", 0,0 ))) goto END_ERROR;

    UIK_Input_Set( UIK, INPUTMODE_5CONTACTS, 0, 0 );

    if (! UIK_AddObjectTags( "UIKObj_Inputick", UIK,
                UIKTAG_Inputick_EventFunc,   EventFunc,
                TAG_END )) goto END_ERROR;

    CtrlCVector = CtrlCFunc;
    if (! UIK_AddVector( UIK, (void*)&CtrlCVector, SIGBREAKF_CTRL_C, 0 )) goto END_ERROR;

    if (! UIK_Start( UIK )) goto END_ERROR;

    printf( "Bougez le joystick, (1=changement d'état)\n" );
    printf( "Ctrl+C (Break) pour arrêter\n" );
    UIK_Do2( UIK, SIGBREAKF_CTRL_C, 1 );

  END_ERROR:
    Cleanup();
}
