/****************************************************************
 *
 *      File:      ColorWheel.c
 *      Project:   UIK/BOOPSI example code
 *
 *      Created:   04/05/93     Jean-Michel Forgeas
 *
 *      Dans cet exemple, le "colorwheel/gradient slider" est
 *      un objet privé linké avec cet exemple.
 *      Il pourrait tout aussi bien être résident sur disque.
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

#include <exec/types.h>
#include <intuition/intuition.h>
#include <intuition/intuitionbase.h>
#include <intuition/screens.h>
#include <graphics/displayinfo.h>
#include <intuition/gadgetclass.h>
#include <gadgets/colorwheel.h>
#include <gadgets/gradientslider.h>
#include <libraries/dos.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikmacros.h"
#include "uiki:uikglobal.h"
#include "uiki:uikcolors.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objscreen.h"
#include "uiki:objwindow.h"

#include "objwheelgrad.h"

#define TEXT_WindowTitle    1
#define TEXT_ScreenTitle    2

#define MAXGRADPENS         8


/****** Imported ************************************************/

extern struct UIKObjRegGerm WheelGradRegGerm;


/****** Exported ***********************************************/

struct UIKBase *UIKBase;
struct UIKGlobal *UIK;


/****** Statics ************************************************/

static UBYTE *LangEnglish[] =
    {
    "",
    "ColorWheel",
    "ColorWheel example, Copyright © 1993 Jean-Michel Forgeas",
    0
    };

static UBYTE *LangOther[] =
    {
    "",
    "ColorWheel",
    "Exemple ColorWheel, Copyright © 1993 Jean-Michel Forgeas",
    0
    };

static struct TagItem IntuitionScreenTags[] = {
    SA_SharePens,   TRUE,  /* Intuition V39 */
    SA_Interleaved, TRUE,  /* Intuition V39 */
    TAG_END
    };

struct UIKObj *ActiveGad;


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void box_values( struct ObjWheelGrad *wheelgrad, struct UIKObj *box )
{
  struct ColorWheelHSB hsb;

    GetAttr( WHEEL_HSB, wheelgrad->Wheel, (ULONG *)&hsb );
    sprintf( UIK_ObjTitle( box ), "   %8.8lx %8.8lx %8.8lx   ", hsb.cw_Hue, hsb.cw_Saturation, hsb.cw_Brightness );
    UIK_Refresh( box, 1 );
}

static void wheel_to_grad( struct ObjWheelGrad *wheelgrad, struct UIKObj *box )
{
    UIK_CallObjectFunc( wheelgrad, UIKFUNC_WheelGrad_Wheel2Grad, 0, 0 );
    box_values( wheelgrad, box );
}

static void color_change( struct ObjWheelGrad *wheelgrad, struct UIKObj *box )
{
  struct ColorWheelRGB rgb;
  UBYTE colornum = UIK_GetColor( ActiveGad, ActiveGad->ActBoxPen );

    GetAttr( WHEEL_RGB, wheelgrad->Wheel, (ULONG *)&rgb );
    SetRGB32( &box->w->WScreen->ViewPort, colornum, rgb.cw_Red, rgb.cw_Green, rgb.cw_Blue );
    wheel_to_grad( wheelgrad, box );
}

static void color_to_wheel( struct UIKObj *gad, struct ObjWheelGrad *wheelgrad )
{
    ActiveGad = gad;
    UIK_CallObjectFunc( wheelgrad, UIKFUNC_WheelGrad_Color2Wheel, UIK_GetColor( gad, gad->ActBoxPen ), 0 );
    wheel_to_grad( wheelgrad, (struct UIKObj *)gad->UserULong1 /*box*/ );
}

static void Cleanup()
{
    if (UIK)     UIK_Remove( UIK );
    if (UIKBase) CloseLibrary( UIKBase );
    exit(0);
}

void main()
{
  struct UIKObj *so, *wo, *gad, *wheelgrad, *box;
  ULONG i;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (UIK = (struct UIKGlobal *) UIK_AddObjectTags( "UIK", 0,
             UIKTAG_GEN_LangEnglish,    (ULONG) LangEnglish,
             UIKTAG_GEN_LangOther,      (ULONG) LangOther,
             UIKTAG_GEN_Catalog,        (ULONG) "uik/colorwheel.catalog",
             TAG_END ))) goto END_ERROR;

    /*------------------ écran -------------------*/

    if (! (so = UIK_AddObjectTags( "UIKObj_Screen", UIK,
            UIKTAG_OBJ_Title,           TEXT_ScreenTitle,
            UIKTAG_OBJScreenFl_Public,  TRUE,
            UIKTAG_ObjScreen_Depth,     8,
            UIKTAG_OBJScreen_IntuiTags, IntuitionScreenTags,
            TAG_END ))) goto END_ERROR;

    /*------------------ fenêtre -------------------*/

    if (! (wo = UIK_AddObjectTags( "UIKObj_Window", so,
            UIKTAG_OBJ_LeftTop,         SETL(180,30),
            UIKTAG_OBJ_WidthHeight,     SETL(240,135),
            UIKTAG_OBJ_Title,           TEXT_WindowTitle,
            UIKTAG_OBJ_AltTitle,        TEXT_ScreenTitle,
            TAG_END ))) goto END_ERROR;

    /*------------------ gadgets -------------------*/

    if (! (box = UIK_AddObjectTags( "UIKObj_Box", wo,
            UIKTAG_OBJ_LeftTop,         SETL(10,110),
            UIKTAG_OBJ_WidthHeight,     SETL(220,20),
            UIKTAG_OBJ_BoxFl_Over,      FALSE,
            UIKTAG_OBJ_Title,           "                                  ",
            UIKTAG_OBJ_TitleFl_Addr,    TRUE,
            TAG_END ))) goto END_ERROR;

    if (! (wheelgrad = UIK_AddPrivateObjectTags( &WheelGradRegGerm, wo,
            UIKTAG_OBJ_LeftTop,         SETL(50,5),
            UIKTAG_OBJ_WidthHeight,     SETL(180,100),
            UIKTAG_OBJWheelG_InitColor, UIK_GetColor( so, 0 ),
            UIKTAG_OBJWheelG_SLNumPens, 16,
            TAG_END ))) goto END_ERROR;
    if (! UIK_AddVector( wheelgrad, (void*)color_change, IDCMP_GADGETDOWN | IDCMP_MOUSEMOVE, (ULONG)box )) goto END_ERROR;

    for (i=0; i<5; i++)
        {
        if (! (gad = UIK_AddObjectTags( "UIKObj_MutualExclude", wo,
                UIKTAG_OBJ_LeftTop,         SETL(10,5+i*20),
                UIKTAG_OBJ_WidthHeight,     SETL(30,15),
                UIKTAG_OBJ_BoxFl_Over,      i ? TRUE : FALSE,
                UIKTAG_OBJ_BoxFl_Filled,    TRUE,
                UIKTAG_OBJ_ActInactPens,    UIKCOLS(i,i,0,0), // i est pris comme indice dans la table de redirection de UIK
                UIKTAG_OBJ_UserValue1,      box,
                TAG_END ))) goto END_ERROR;
        if (! UIK_AddVector( gad, (void*)color_to_wheel, IDCMP_GADGETDOWN, (ULONG)wheelgrad )) goto END_ERROR;
        if (i == 0) ActiveGad = gad;
        }

    /*------------------ exécution ------------------*/

    if (! UIK_Start( UIK )) goto END_ERROR;
    box_values( (struct ObjWheelGrad *)wheelgrad, box );

    UIK_Do( UIK, SIGBREAKF_CTRL_C );
    Cleanup();
    return;

  END_ERROR:
    Cleanup();
}
