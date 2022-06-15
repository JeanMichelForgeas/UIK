/****************************************************************
 *
 *      File:      HitMask.c
 *      Project:   UIK example code
 *
 *      Created:   22-11-92     Jean-Michel Forgeas
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
#include "uiki:uikgfx.h"
#include "uiki:uikmacros.h"
#include "uiki:uikglobal.h"
#include "uiki:uikcolors.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"
#include "uiki:objlist.h"
#include "uiki:objproplift.h"
#include "uiki:objwindow.h"

#define TEXT_WindowTitle    1
#define TEXT_ScreenTitle    2
#define TEXT_GadgetHit      3


/****** Imported ************************************************/

extern ULONG DOSBase;

extern struct UIKBitMap __far UBM_Inact;
extern struct UIKBitMap __far UBM_Act;
extern struct UIKBitMap * __far UIKBitMapTable[];


/****** Exported ***********************************************/

struct UIKBase *UIKBase;
struct UIKGlobal *UIK;


/****** Statics ************************************************/

static UBYTE *LangEnglish[] =
    {
    "",
    "HitMask",
    "HitMask Copyright © 1992 Jean-Michel Forgeas",
    "area activated!",
    0
    };

static UBYTE *LangOther[] =
    {
    "",
    "HitMask",
    "HitMask Copyright © 1992 Jean-Michel Forgeas",
    "zone activée!",
    0
    };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void Hit( struct UIKObj *obj )
{
    printf( "%ls\n", UIK_LangString( UIK, TEXT_GadgetHit ) );
}

static void Cleanup()
{
    if (UIK)     UIK_Remove( UIK );
    if (UIKBase) CloseLibrary( UIKBase );
    exit(0);
}

void main()
{
  struct UIKObj *wo;
  struct UIKObj *obj;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (UIK = (struct UIKGlobal *) UIK_AddObjectTags( "UIK", 0,
             UIKTAG_GEN_LangEnglish,    (ULONG) LangEnglish,
             UIKTAG_GEN_LangOther,      (ULONG) LangOther,
             TAG_END ))) goto END_ERROR;

    UIK_InvertBlackWhite( UIK, UIKBitMapTable, 1 );

    if (! (wo = UIK_AddObjectTags( "UIKObj_Window", UIK,
            UIKTAG_OBJ_LeftTop,       SETL(180,30),
            UIKTAG_OBJ_WidthHeight,   SETL(180,102),
            UIKTAG_OBJ_Title,         TEXT_WindowTitle,
            UIKTAG_OBJ_AltTitle,      TEXT_ScreenTitle,
            TAG_END ))) goto END_ERROR;

    if (! (obj = UIK_AddObjectTags( "UIKObj_Boolean", wo,
            UIKTAG_OBJ_LeftTop,       SETL(30,14),
            UIKTAG_OBJ_WidthHeight,   SETL(45,32),
            UIKTAG_OBJ_Title,         &UBM_Inact,
            UIKTAG_OBJ_AltTitle,      &UBM_Act,
            UIKTAG_OBJ_BoxAutomate,   0,
            UIKTAG_OBJ_BoxAltAutomate,0,
            UIKTAG_OBJ_HitMask,       UBM_Act.Mask,
            UIKTAG_OBJ_TitleFl_Image, TRUE,
            TAG_END ))) goto END_ERROR;
    if (! UIK_AddVector( obj, (void *)Hit, GADGETDOWN, 0 )) goto END_ERROR;

    if (! (obj = UIK_AddObjectTags( "UIKObj_Toggle", wo,
            UIKTAG_OBJ_LeftTop,       SETL(105,14),
            UIKTAG_OBJ_WidthHeight,   SETL(45,32),
            UIKTAG_OBJ_Title,         &UBM_Inact,
            UIKTAG_OBJ_AltTitle,      &UBM_Act,
            UIKTAG_OBJ_BoxAutomate,   0,
            UIKTAG_OBJ_BoxAltAutomate,0,
            UIKTAG_OBJ_HitMask,       UBM_Act.Mask,
            UIKTAG_OBJ_TitleFl_Image, TRUE,
            TAG_END ))) goto END_ERROR;
    if (! UIK_AddVector( obj, (void *)Hit, GADGETDOWN, 0 )) goto END_ERROR;

    if (! (obj = UIK_AddObjectTags( "UIKObj_MutualExclude", wo,
            UIKTAG_OBJ_LeftTop,       SETL(45,57),
            UIKTAG_OBJ_WidthHeight,   SETL(45,32),
            UIKTAG_OBJ_Title,         &UBM_Inact,
            UIKTAG_OBJ_AltTitle,      &UBM_Act,
            UIKTAG_OBJ_BoxAutomate,   0,
            UIKTAG_OBJ_BoxAltAutomate,0,
            UIKTAG_OBJ_HitMask,       UBM_Act.Mask,
            UIKTAG_OBJ_TitleFl_Image, TRUE,
            TAG_END ))) goto END_ERROR;
    if (! UIK_AddVector( obj, (void *)Hit, GADGETDOWN, 0 )) goto END_ERROR;

    if (! (obj = UIK_AddObjectTags( "UIKObj_MutualExclude", wo,
            UIKTAG_OBJ_LeftTop,       SETL(90,57),
            UIKTAG_OBJ_WidthHeight,   SETL(45,32),
            UIKTAG_OBJ_Title,         &UBM_Inact,
            UIKTAG_OBJ_AltTitle,      &UBM_Act,
            UIKTAG_OBJ_BoxAutomate,   0,
            UIKTAG_OBJ_BoxAltAutomate,0,
            UIKTAG_OBJ_HitMask,       UBM_Act.Mask,
            UIKTAG_OBJ_TitleFl_Image, TRUE,
            TAG_END ))) goto END_ERROR;
    if (! UIK_AddVector( obj, (void *)Hit, GADGETDOWN, 0 )) goto END_ERROR;

    if (! UIK_Start( UIK )) goto END_ERROR;

    UIK_Do( UIK, SIGBREAKF_CTRL_C );

  END_ERROR:
    Cleanup();
}
