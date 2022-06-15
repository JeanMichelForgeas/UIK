/****************************************************************************
 *                                                                          *
 *  File : ObjCycle.h                                                       *
 *  Project : Cycling object.                                               *
 *                                                                          *
 *  Created : 31/03/93 Frédéric Denis / "The Software Laboratory"           *
 *                                                                          *
 ****************************************************************************
 *                                                                          *
 *                   Copyright (C) 1993, Frédéric Denis                     *
 *              All rights reserved.    Tous droits réservés.               *
 *                                                                          *
 ****************************************************************************/

#ifndef UIKDEFS_CYCLE
#define UIKDEFS_CYCLE

#ifndef UIKDEFS_OBJ
#include "UIKI:UIKObj.h"
#endif

#ifndef UIKDEFS_TAGITEM_H
#include "UIKI:UIKTag.h"
#endif

/* --- Structures definition ---------------------------------------------- */

struct UIKCycleEntry
{
   ULONG                Title;      /* Offset of title in string array */
   void                 (*Func)();  /* Function called when selected */
   ULONG                UserData;   /* This field is left as is */
   UBYTE                Flags;      /* Entry Flags, see below */
};

struct UIKObjCycle
{
   struct UIKObj        Obj;
   struct Gadget        Gad;

   struct UIKBase       *UIKBase;
   struct GfxBase       *GfxBase;
   struct IntuitionBase *IntuitionBase;
   struct Library       *LayersBase;

   struct UIKList       Entries;
   struct UIKList       Strings;
   struct UIKCycleEntry *CurrentEntry;
   struct UIKHook       *Hook;
   struct Region        *ClipRegion;
   struct Region        *ObjRegion;
   UBYTE                LastMState;
};

/* --- Public internal functions ------------------------------------------ */

#define UIKFUNC_Cycle_LastCmd    0

/* ------------------------------------------------------------------------ */

#define OBJCYCLE_BOX_STDWIDTH    50
#define OBJCYCLE_BOX_STDHEIGHT   11

/* --- Cycle Flags -------------------------------------------------------- */

#define CYCLE_DISPFIRST       0x0001 /* This entry will be displayed first */
#define CYCLE_TEXT_ADDR       0x0002 /* If true, title is a string pointer */
#define CYCLE_TEXT_IMAGE      0x0004 /* If true, title is a UIKBitMap */

/* --- Cycle Object Tags -------------------------------------------------- */

#define UIKTAG_Cycle_Entries        (UIKTAG_USER|0x0001)

/* --- Cycle Object Flags ------------------------------------------------- */

#define UIKTAG_OBJCycle_Flags       (UIKTAG_USER|0x0020)

#endif /* UIKDEFS_CYCLE */
