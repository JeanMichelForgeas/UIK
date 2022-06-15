/****************************************************************
 *
 *      File:      timer.c
 *      Project:   Exemple utilisant UIKObj_Timer
 *
 *      Created:   30-03-93     Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include <exec/execbase.h>
#include <libraries/dos.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikglobal.h"
#include "uiki:uikcolors.h"
#include "uiki:uikmacros.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objtimer.h"

#ifdef LATTICE
int CXBRK(void) { return(0); }      /* disable SASC CTRLC handling */
int chkabort(void) { return(0); }   /* really */
#endif


/****** Imported ************************************************/

extern ULONG __far DOSBase;
extern struct ExecBase * __far SysBase;


/****** Exported ***********************************************/

struct UIKBase * __far UIKBase;


/****** Statics ************************************************/

static struct UIKGlobal * __far UIK;


/****************************************************************
 *
 *      Fonctions sur intervale
 *
 ****************************************************************/

static ULONG dodo( struct timeval *t )
{   /* tient dans un long car l'interval choisi est inférieur à 4 heures */
    return( t->tv_secs * ONESECOND + t->tv_micro );
}

static void timer_func( struct UIKObjTimer *timer )
{
  ULONG elapsed, average;
  ULONG asked, corrected;

    elapsed = dodo( (struct timeval *) UIK_CallObjectFunc( timer, UIKFUNC_Timer_GetElapsed, 0, 0 ) );
    average = timer->EventCounter ? elapsed / timer->EventCounter : 0;

    asked = dodo( &timer->AskedInterval );
    corrected = dodo( &timer->CorrInterval );

    printf( "Asked=%ld, corrected=%ld, count=%ld, average=%ld\n",
                            asked, corrected, timer->EventCounter, average );
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
  struct UIKObj *timer;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (UIK = (struct UIKGlobal *) UIK_AddObject( "UIK", 0,0 ))) goto END_ERROR;

    if (! (timer = (struct UIKObj *) UIK_AddObjectTags( "UIKObj_Timer", UIK,
                UIKTAG_Timer_TimeFunc,      timer_func,
                UIKTAG_Timer_Seconds,       0,
                UIKTAG_Timer_Micros,        100000,
                UIKTAG_TimerFl_Correction,  TRUE,
                UIKTAG_Timer_Unit,          UNIT_MICROHZ, /* UNIT_VBLANK, */
                TAG_END ))) goto END_ERROR;

    if (! UIK_Start( UIK )) goto END_ERROR;

    UIK_CallObjectFunc( timer, UIKFUNC_Timer_Start, 0, 0 );

    UIK_Do2( UIK, SIGBREAKF_CTRL_C, 1 );

  END_ERROR:
    Cleanup();
}
