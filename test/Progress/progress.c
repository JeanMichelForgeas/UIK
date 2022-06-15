/*****************************************************************************
 *                                                                           *
 * Progress.c Progress Object Example                                        *
 *                                                                           *
 * Created 19/9/91 Frédéric DENIS, The Software Laboratory                   *
 * Modified by JM Forgeas                                                    *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 *****************************************************************************/


/********* Includes **************/

#include <stdio.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikglobal.h"
#include "uiki:uikbox.h"
#include "uiki:uikcolors.h"
#include "uiki:uikdrawauto.h"
#include "uiki:uikmacros.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objprogress.h"
#include "uiki:objdial.h"
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


/************ Imported *************************************/


/************ Exported *************************************/

struct UIKBase *UIKBase;


/************ Statics **************************************/

static struct UIKGlobal *UIK;

static void (*CleanupVector)();

static struct UIKObjWindow *wo;
static struct UIKObjProgress *po, *po2, *po3, *po4, *po5, *po6, *po7, *po8, *po9;
static struct UIKObjDial   *dio;
static struct UIKObj       *of;

static UBYTE *LangEnglish[] =
{
   "",
   "The great Progress objects",
   0
};

static UBYTE *LangOther[] =
{
   "",
   "Les objets Progress chouettes",
   0
};

static struct TagItem wintagl[] =
{
   { UIKTAG_OBJ_LeftTop,      SETL(120,50) },
   { UIKTAG_OBJ_WidthHeight,  SETL(400,130) },
   { UIKTAG_OBJ_Title,        1 },
   { UIKTAG_OBJ_AltTitle,     1 },
   { UIKTAG_OBJ_ActInactPens, UIKCOLS( UIKCOL_LIGHT, UIKCOL_GREY, UIKCOL_WHITE, UIKCOL_BLACK ) },
   { UIKTAG_OBJWindowFl_With_Size, FALSE },
   { TAG_END }
};

static struct TagItem progtl[] =
{
   { UIKTAG_OBJ_LeftTop,            SETL(10,10) },
   { UIKTAG_OBJ_WidthHeight,        SETL(50,50) },
   { UIKTAG_OBJProgress_Total,      100 },
   { UIKTAG_OBJProgress_Pos,        1 },
   { UIKTAG_OBJProgress_ToDoPen,    UIKCOL_GREY },
   { UIKTAG_OBJProgress_DonePen,    UIKCOL_LIGHT },
   { UIKTAG_OBJProgress_RenderMode, PROG_RenderFromLeft },
   { UIKTAG_OBJ_BoxFl_NotDrawn,     TRUE },
   { TAG_END }
};
static struct TagItem progtl2[] =
{
   { UIKTAG_OBJ_LeftTop,            SETL(10,70) },
   { UIKTAG_OBJ_WidthHeight,        SETL(50,50) },
   { UIKTAG_OBJProgress_Total,      100 },
   { UIKTAG_OBJProgress_Pos,        1 },
   { UIKTAG_OBJProgress_ToDoPen,    UIKCOL_GREY },
   { UIKTAG_OBJProgress_DonePen,    UIKCOL_WHITE },
   { UIKTAG_OBJProgress_RenderMode, PROG_RenderFromTop },
   { UIKTAG_OBJ_BoxFl_Over,         TRUE },
   { TAG_END }
};

static struct TagItem progtl3[] =
{
   { UIKTAG_OBJ_LeftTop,            SETL(70,10) },
   { UIKTAG_OBJ_WidthHeight,        SETL(50,50) },
   { UIKTAG_OBJProgress_Total,      100 },
   { UIKTAG_OBJProgress_Pos,        1 },
   { UIKTAG_OBJProgress_ToDoPen,    UIKCOL_GREY },
   { UIKTAG_OBJProgress_DonePen,    UIKCOL_LIGHT },
   { UIKTAG_OBJProgress_RenderMode, PROG_RenderFromBottom },
   { UIKTAG_OBJ_BoxFl_Over,         TRUE },
   { TAG_END }
};
static struct TagItem progtl4[] =
{
   { UIKTAG_OBJ_LeftTop,            SETL(70,70) },
   { UIKTAG_OBJ_WidthHeight,        SETL(50,50) },
   { UIKTAG_OBJProgress_Total,      100 },
   { UIKTAG_OBJProgress_Pos,        1 },
   { UIKTAG_OBJProgress_ToDoPen,    UIKCOL_GREY },
   { UIKTAG_OBJProgress_DonePen,    UIKCOL_BLACK },
   { UIKTAG_OBJProgress_RenderMode, PROG_RenderFromRight },
   { TAG_END }
};

static struct TagItem progtl5[] =
{
   { UIKTAG_OBJ_LeftTop,            SETL(130,10) },
   { UIKTAG_OBJ_WidthHeight,        SETL(50,50) },
   { UIKTAG_OBJProgress_Total,      100 },
   { UIKTAG_OBJProgress_Pos,        1 },
   { UIKTAG_OBJProgress_ToDoPen,    UIKCOL_GREY },
   { UIKTAG_OBJProgress_DonePen,    UIKCOL_WHITE },
   { UIKTAG_OBJProgress_RenderMode, PROG_RenderFromUpperLeft },
   { TAG_END }
};

static struct TagItem progtl6[] =
{
   { UIKTAG_OBJ_LeftTop,            SETL(130,70) },
   { UIKTAG_OBJ_WidthHeight,        SETL(50,50) },
   { UIKTAG_OBJProgress_Total,      100 },
   { UIKTAG_OBJProgress_Pos,        1 },
   { UIKTAG_OBJProgress_ToDoPen,    UIKCOL_GREY },
   { UIKTAG_OBJProgress_DonePen,    UIKCOL_LIGHT },
   { UIKTAG_OBJProgress_RenderMode, PROG_RenderFromUpperRight },
   { UIKTAG_OBJ_BoxFl_Over,         TRUE },
   { TAG_END }
};
static struct TagItem progtl7[] =
{
   { UIKTAG_OBJ_LeftTop,            SETL(190,10) },
   { UIKTAG_OBJ_WidthHeight,        SETL(50,50) },
   { UIKTAG_OBJProgress_Total,      100 },
   { UIKTAG_OBJProgress_Pos,        1 },
   { UIKTAG_OBJProgress_ToDoPen,    UIKCOL_GREY },
   { UIKTAG_OBJProgress_DonePen,    UIKCOL_BLACK },
   { UIKTAG_OBJProgress_RenderMode, PROG_RenderFromLowerLeft },
   { UIKTAG_OBJ_BoxFl_Over,         TRUE },
   { TAG_END }
};

static struct TagItem progtl8[] =
{
   { UIKTAG_OBJ_LeftTop,            SETL(190,70) },
   { UIKTAG_OBJ_WidthHeight,        SETL(50,50) },
   { UIKTAG_OBJProgress_Total,      100 },
   { UIKTAG_OBJProgress_Pos,        1 },
   { UIKTAG_OBJProgress_ToDoPen,    UIKCOL_GREY },
   { UIKTAG_OBJProgress_DonePen,    UIKCOL_LIGHT },
   { UIKTAG_OBJProgress_RenderMode, PROG_RenderFromLowerRight },
   { TAG_END }
};

static struct TagItem tl_dial[] = {  /* dial */
    { UIKTAG_OBJ_LeftTop,           SETL(250,10) },
    { UIKTAG_OBJ_WidthHeight,       SETL(110,97) },
    { UIKTAG_OBJ_Sz_Attached_Flags, UO_ATTACHED_TOP | UO_ATTACHED_RIGHT },
    { UIKTAG_OBJDial_Total,         100 },
    { UIKTAG_OBJDial_Position,      30 },
    { UIKTAG_OBJDialFl_SupHalf,     FALSE },
    { TAG_END } };

static struct TagItem tl_dialdispl[] = {
    { UIKTAG_OBJ_LeftTop,           SETL(0,97) },
    { UIKTAG_OBJ_WidthHeight,       SETL(110,13) },
    { UIKTAG_OBJ_Sz_Attached_Flags, UO_ATTACHED_RIGHT | UO_ATTACHED_TOP },
    { UIKTAG_OBJ_ParentRelative,    TRUE },
    { UIKTAG_OBJ_BoxFl_Over,        FALSE },
    { UIKTAG_OBJ_TitleFl_Addr,      TRUE },
    { UIKTAG_OBJ_Title,             (ULONG)"                    " },
    { UIKTAG_OBJ_ActInactPens,      UIKCOLS(UIKCOL_GREY,UIKCOL_GREY,UIKCOL_BLACK,UIKCOL_BLACK) },
    { TAG_END } };

static struct TagItem progtl9[] =
{
   { UIKTAG_OBJ_LeftTop,            SETL(370,10) },
   { UIKTAG_OBJ_WidthHeight,        SETL(18,110) },
   { UIKTAG_OBJProgress_Total,      100 },
   { UIKTAG_OBJProgress_Pos,        12 },
   { UIKTAG_OBJProgress_ToDoPen,    UIKCOL_GREY },
   { UIKTAG_OBJProgress_DonePen,    UIKCOL_LIGHT },
   { UIKTAG_OBJProgress_RenderMode, PROG_RenderFromBottom },
   { UIKTAG_OBJ_BoxFl_Over,         FALSE },
   { TAG_END }
};

/***********************************************************
 *
 *  Code principal
 *
 ***********************************************************/

static void Cleanup()
{
   if (UIK) UIK_Remove (UIK );
   if (UIKBase)   CloseLibrary (UIKBase);
   exit(0);
}

static int lnk_dial2pro( struct UIKObjDial *od, struct UIKObjProgress *po)
{
   UIK_CallObjectFunc( po, UIKFUNC_Progress_ChangeProgress, 100, od->Pos);
   return (1);
}

static int lnk_dial2box( struct UIKObjDial *od, struct UIKObj *box )
{
    sprintf( UIK_ObjTitle( box ), "  %ld%%  ", od->Pos );
    UIK_Refresh( box, 1 );
    return(1);
}

static int driveprogress( struct UIKObj *po )
{
  ULONG pos, height;
  struct Rectangle *r = &po->ObjExt->InnerRect;

    height = r->MaxY - r->MinY + 1;

    if (po->MouseY > r->MaxY) pos = 0;
    else if (po->MouseY < r->MinY) pos = 100;
    else pos = ((height - (po->MouseY - r->MinY)) * 100) / height;

    UIK_CallObjectFunc( po, UIKFUNC_Progress_ChangeProgress, 100, pos);
    return(1);
}

void main ()
{
   if (! (UIKBase = (struct UIKBase *) OpenLibrary (UIKNAME, 0))) exit(20);

   if (! (UIK = (struct UIKGlobal *) UIK_AddObjectTags( "UIK", 0,
             UIKTAG_GEN_LangEnglish,    (ULONG) LangEnglish,
             UIKTAG_GEN_LangOther,      (ULONG) LangOther,
             TAG_END ))) goto END_ERROR;

   if (! (wo = (struct UIKObjWindow *) UIK_AddObject ("UIKObj_Window", UIK, wintagl ))) goto END_ERROR;

   if (! (dio = (struct UIKObjDial *) UIK_AddObject ("UIKObj_Dial", wo, tl_dial ))) goto END_ERROR;

   if (! (po = (struct UIKObjProgress *) UIK_AddObject ("UIKObj_Progress", wo, progtl ))) goto END_ERROR;

   if (! (po2 = (struct UIKObjProgress *) UIK_AddObject ("UIKObj_Progress", wo, progtl2 ))) goto END_ERROR;

   if (! (of = (struct UIKObj *) UIK_AddObject ("UIKObj_Box", dio, tl_dialdispl ))) goto END_ERROR;

   if (! (po3 = (struct UIKObjProgress *) UIK_AddObject ("UIKObj_Progress", wo, progtl3 ))) goto END_ERROR;

   if (! (po4 = (struct UIKObjProgress *) UIK_AddObject ("UIKObj_Progress", wo, progtl4 ))) goto END_ERROR;

   if (! (po5 = (struct UIKObjProgress *) UIK_AddObject ("UIKObj_Progress", wo, progtl5 ))) goto END_ERROR;

   if (! (po6 = (struct UIKObjProgress *) UIK_AddObject ("UIKObj_Progress", wo, progtl6 ))) goto END_ERROR;

   if (! (po7 = (struct UIKObjProgress *) UIK_AddObject ("UIKObj_Progress", wo, progtl7 ))) goto END_ERROR;

   if (! (po8 = (struct UIKObjProgress *) UIK_AddObject ("UIKObj_Progress", wo, progtl8 ))) goto END_ERROR;

   if (! (po9 = (struct UIKObjProgress *) UIK_AddObject ("UIKObj_Progress", wo, progtl9 ))) goto END_ERROR;
   if (! (UIK_AddVector( po9, driveprogress, GADGETDOWN|MOUSEMOVE, 0 ))) goto END_ERROR;

   if (! (UIK_AddVector( dio, lnk_dial2pro, GADGETDOWN|MOUSEMOVE|GADGETUP, (ULONG)po ))) goto END_ERROR;

   if (! (UIK_AddVector( dio, lnk_dial2pro, GADGETDOWN|MOUSEMOVE|GADGETUP, (ULONG)po2 ))) goto END_ERROR;

   if (! (UIK_AddVector( dio, lnk_dial2pro, GADGETDOWN|MOUSEMOVE|GADGETUP, (ULONG)po3 ))) goto END_ERROR;

   if (! (UIK_AddVector( dio, lnk_dial2pro, GADGETDOWN|MOUSEMOVE|GADGETUP, (ULONG)po4 ))) goto END_ERROR;

   if (! (UIK_AddVector( dio, lnk_dial2pro, GADGETDOWN|MOUSEMOVE|GADGETUP, (ULONG)po5 ))) goto END_ERROR;

   if (! (UIK_AddVector( dio, lnk_dial2pro, GADGETDOWN|MOUSEMOVE|GADGETUP, (ULONG)po6 ))) goto END_ERROR;

   if (! (UIK_AddVector( dio, lnk_dial2pro, GADGETDOWN|MOUSEMOVE|GADGETUP, (ULONG)po7 ))) goto END_ERROR;

   if (! (UIK_AddVector( dio, lnk_dial2pro, GADGETDOWN|MOUSEMOVE|GADGETUP, (ULONG)po8 ))) goto END_ERROR;

   if (! (UIK_AddVector( dio, lnk_dial2box, GADGETDOWN|MOUSEMOVE|GADGETUP, (ULONG)of ))) goto END_ERROR;

   if (! UIK_Start(UIK)) goto END_ERROR;

   UIK_CauseObjHook (dio, GADGETDOWN );

   CleanupVector = Cleanup;
   if (UIK_AddVector ( UIK, (void *)&CleanupVector, SIGBREAKF_CTRL_E, 0))
      UIK_Do(UIK,SIGBREAKF_CTRL_C);

 END_ERROR:
   Cleanup();
}
