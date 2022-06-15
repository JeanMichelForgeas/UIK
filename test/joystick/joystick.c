/****************************************************************
 *
 *      File:      joystick.c
 *      Project:   Essai de l'objet UIKObj_Joystick
 *
 *      Created:   03-03-92     Jean-Michel Forgeas
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

#include <exec/execbase.h>
#include <libraries/dos.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikglobal.h"
#include "uiki:uikcolors.h"
#include "uiki:uikmacros.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objjoystick.h"


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

static void func_lbut( struct UIKObjJoystick *joy, BOOL down )
{ printf( "LEFT BUTTON %ls, ", down ? "DOWN" : "UP" ); }

static void func_rbut( struct UIKObjJoystick *joy, BOOL down )
{ printf( "RIGHT BUTTON %ls, ", down ? "DOWN" : "UP" ); }

static void func_mbut( struct UIKObjJoystick *joy, BOOL down )
{ printf( "MIDDLE BUTTON %ls, ", down ? "DOWN" : "UP" ); }

static void func_forward( struct UIKObjJoystick *joy, WORD direction )
{ printf( "FORWARD, " ); }

static void func_backward( struct UIKObjJoystick *joy, WORD direction )
{ printf( "BACKWARD, " ); }

static void func_left( struct UIKObjJoystick *joy, WORD direction )
{ printf( "LEFT, " ); }

static void func_right( struct UIKObjJoystick *joy, WORD direction )
{ printf( "RIGHT, " ); }

static void func_timeout( struct UIKObjJoystick *joy, WORD direction )
{ printf( "TIMEOUT, " ); }

static void func_release( struct UIKObjJoystick *joy, WORD direction )
{ printf( "RELEASE, " ); }

static void func_move( struct UIKObjJoystick *joy, WORD xmove, WORD ymove, WORD direction )
{ printf( "MOVE dx=%ld dy=%ld direction=%ld\n", xmove, ymove, direction ); }


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
  struct UIKObj *joy;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (UIK = (struct UIKGlobal *) UIK_AddObject( "UIK", 0,0 ))) goto END_ERROR;

    if (! (joy = (struct UIKObj *) UIK_AddObjectTags( "UIKObj_Joystick", UIK,
                UIKTAG_Joystick_LButFunc,   func_lbut,
                UIKTAG_Joystick_RButFunc,   func_rbut,
                UIKTAG_Joystick_MButFunc,   func_mbut,
                UIKTAG_Joystick_ForwFunc,   func_forward,
                UIKTAG_Joystick_BackFunc,   func_backward,
                UIKTAG_Joystick_LeftFunc,   func_left,
                UIKTAG_Joystick_RightFunc,  func_right,
                UIKTAG_Joystick_ReleaseFunc,func_release,
                UIKTAG_Joystick_MoveFunc,   func_move,
                UIKTAG_Joystick_TimeoutFunc,func_timeout,
                UIKTAG_Joystick_TimeoutVal, SysBase->VBlankFrequency, /* 1 seconde, PAL ET NTSC */
                UIKTAG_Joystick_Controller, GPCT_ABSJOYSTICK,  /* defaut */
                UIKTAG_Joystick_Unit,       1,                 /* defaut */
                UIKTAG_Joystick_DeltaXY,    SETL(1,1),         /* defaut */
                UIKTAG_Joystick_UpDown,     (GPTF_DOWNKEYS|GPTF_UPKEYS), /* defaut */
                TAG_END ))) goto END_ERROR;

    if (! UIK_Start( UIK )) goto END_ERROR;

    UIK_Do2( UIK, SIGBREAKF_CTRL_C, 1 );

  END_ERROR:
    Cleanup();
}
