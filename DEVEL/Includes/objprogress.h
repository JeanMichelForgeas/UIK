/*****************************************************************************
 *                                                                           *
 * ObjProgress.h                                                             *
 *                                                                           *
 * Created 19/9/91 Copyright © 1992 Frédéric DENIS, The Software Laboratory  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 *****************************************************************************/

#ifndef UIKDEFS_OBJPROGRESS

#define UIKDEFS_OBJPROGRESS

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif


struct UIKObjProgress
{
   struct UIKObj     Obj;
   struct Gadget     Gadget;

   struct Image      Image;
   struct RastPort   RastPort;
   struct BitMap     BitMap;

   ULONG             Total;
   ULONG             Pos;
   ULONG             RenderMode;

   UBYTE             DonePen;
   UBYTE             ToDoPen;

    struct IntuitionBase *IntuitionBase;
    struct GfxBase   *GfxBase;
    struct UIKBase   *UIKBase;
};

/*--------- public internal functions --------------*/
#define UIKFUNC_Progress_ChangeProgress 0
/* correspond à la fonction :
 * ChangeProgress( struct UIKObjProgress *obj, int Total, int Position)
 * Appel: UIK_CallObjectFunc( obj, UIKFUNC_Progress_ChangeProgress, Total, Position);
 */

#define UIKFUNC_Progress_LastCmd    0


#define OBJPROGRESS_BOX_STDWIDTH  10
#define OBJPROGRESS_BOX_STDHEIGHT 10

/*--------- Progress Object Render Modes -------------*/

#define PROG_RenderFromLeft            1
#define PROG_RenderFromTop             2
#define PROG_RenderFromRight           3
#define PROG_RenderFromBottom          4
#define PROG_RenderFromUpperLeft       5
#define PROG_RenderFromUpperRight      6
#define PROG_RenderFromLowerLeft       7
#define PROG_RenderFromLowerRight      8

/*--------- Progress Object Tags---------- */
#define UIKTAG_OBJProgress_Total        (UIKTAG_USER|0x0001)
#define UIKTAG_OBJProgress_Pos          (UIKTAG_USER|0x0002)
#define UIKTAG_OBJProgress_ToDoPen      (UIKTAG_USER|0x0003)
#define UIKTAG_OBJProgress_DonePen      (UIKTAG_USER|0x0004)
#define UIKTAG_OBJProgress_RenderMode   (UIKTAG_USER|0x0005)

/*-- */

#endif
