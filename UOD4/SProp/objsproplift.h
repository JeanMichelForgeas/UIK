/****************************************************************************
 *                                                                          *
 *  File : ObjSPropLift.h                                                   *
 *  Project : PropLift with arrows & stop buttons.                          *
 *                                                                          *
 *  Created : 12/06/93 Frédéric Denis / "Nemesis Software"                  *
 *                                                                          *
 ****************************************************************************
 *                                                                          *
 *                   Copyright (C) 1993, Frédéric Denis                     *
 *              All rights reserved.    Tous droits réservés.               *
 *                                                                          *
 ****************************************************************************/

#ifndef UIKDEFS_SPROPLIFT
#define UIKDEFS_SPROPLIFT

#ifndef UIKDEFS_OBJ
#include "UIKI:UIKObj.h"
#endif

#ifndef UIKDEFS_TAGITEM_H
#include "UIKI:UIKTag.h"
#endif

/* --- Structures definition ---------------------------------------------- */

struct UIKObjSPropLift
{
   struct UIKObj        Obj;

   struct UIKBase       *UIKBase;
   struct GfxBase       *GfxBase;
   struct IntuitionBase *IntuitionBase;
   struct Library       *LayersBase;

   struct UIKObj        *EtchedBox;
   struct UIKObj        *TopStop;
   struct UIKObj        *BotStop;
   struct UIKObj        *UpArrow;
   struct UIKObj        *DownArrow;
   struct UIKObj        *UpArrow2;
   struct UIKObj        *DownArrow2;
   struct UIKObjPropGad *PropLift;

   UWORD                VArrowSize;
   UWORD                VStopsSize;
   UWORD                HArrowSize;
   UWORD                HStopsSize;
   ULONG                Flags;
   ULONG                Total;
   ULONG                Visible;
   ULONG                Position;
};

struct SplParms
{
   int Total;
   int Visible;
   int Position;
};

/* --- Public internal functions ------------------------------------------ */

#define UIKFUNC_SPL_ChangePropGadget   0 /* takes a SplParms struct a first argument */
#define UIKFUNC_SPL_ChangePropNoDraw   4 /*   ~        ~        ~       ~       ~    */
#define UIKFUNC_SPL_Link2List          8 /* takes a list prt as 1st argument & Events as second arg. */
#define UIKFUNC_SPL_LastFunc           8

/* --- Flags -------------------------------------------------------------- */

#define UIKSPL_VERTICAL 0x0001
#define UIKSPL_UP       0x0002
#define UIKSPL_DOWN     0x0004
#define UIKSPL_BOTH     0x0008
#define UIKSPL_STOPS    0x0010

/* --- Object Tags -------------------------------------------------------- */

#define UIKTAG_OBJSpl_Total      (UIKTAG_USER|0x0001)
#define UIKTAG_OBJSpl_Visible    (UIKTAG_USER|0x0002)
#define UIKTAG_OBJSpl_Position   (UIKTAG_USER|0x0003)
#define UIKTAG_OBJSpl_VArrowSize (UIKTAG_USER|0x0004)
#define UIKTAG_OBJSpl_VStopsSize (UIKTAG_USER|0x0005)
#define UIKTAG_OBJSpl_HArrowSize (UIKTAG_USER|0x0006)
#define UIKTAG_OBJSpl_HStopsSize (UIKTAG_USER|0x0007)

/* --- Object Flags ------------------------------------------------------- */

#define UIKTAG_OBJSpl_Flags      (UIKTAG_USER|0x0020)
#define UIKTAG_OBJSplFl_Vertical (UIKTAG_USER|0x0021)
#define UIKTAG_OBJSplFl_Up       (UIKTAG_USER|0x0022)
#define UIKTAG_OBJSplFl_Down     (UIKTAG_USER|0x0023)
#define UIKTAG_OBJSplFl_Both     (UIKTAG_USER|0x0024)
#define UIKTAG_OBJSplFl_Stops    (UIKTAG_USER|0x0025)

#endif /* VMSDEFS_SPROPLIFT */
