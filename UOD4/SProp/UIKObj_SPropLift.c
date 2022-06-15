/****************************************************************************
 *                                                                          *
 *  File : UIKObj_SPropLift.c                                               *
 *  Project : Proportional gadget with arrows & stop buttons.               *
 *                                                                          *
 *  Created : 12/06/93 Frédéric Denis / "Nemesis Software"                  *
 *                                                                          *
 ****************************************************************************
 *                                                                          *
 *                   Copyright (C) 1993, Frédéric Denis                     *
 *              All rights reserved.    Tous droits réservés.               *
 *                                                                          *
 ****************************************************************************/

/* --- Includes ----------------------------------------------------------- */

#include "UIKI:UIKBase.h"
#include "UIKI:UIKGlobal.h"
#include "UIKI:UIKObj.h"
#include "UIKI:UIKMacros.h"
#include "UIKI:UIKGfx.h"
#include "UIKI:UIKDrawAuto.h"
#include "UIKI:UIK_Protos.h"
#include "UIKI:UIK_Pragmas.h"

#include "UIKI:ObjPropLift.h"
#include "UIKI:ObjSPropLift.h"

/* --- Defines ------------------------------------------------------------ */
/* --- Structures definitions --------------------------------------------- */
/* --- Macros ------------------------------------------------------------- */
/* --- Exported ----------------------------------------------------------- */

struct ExecBase      * __far SysBase;
struct UIKBase       * __far UIKBase;
struct GfxBase       * __far GfxBase;
struct IntuitionBase * __far IntuitionBase;

/* --- Imported ----------------------------------------------------------- */
/* --- Statics ------------------------------------------------------------ */

/* --- Copyright Stuff --- */

static UBYTE Copyright[] = "UIKObj_SProp Copyright © 1993, Frédéric DENIS, Tous droits réservés";
static UBYTE Copyright2[] = "UIKObj_SProp Copyright © 1993, Frédéric DENIS, All rights reserved";

static UWORD Pat1[] = { 0xAAAA, 0x5555 };
static UWORD __far PropAuto[] =
    {
    x_ADD_N, 2, X_SUB_N, 2,
    y_ADD_1, Y_SUB_1,
    END_AUTO
    };

static struct TagItem __far flags_boolmap[] =
{
   { UIKTAG_OBJSplFl_Vertical,   UIKSPL_VERTICAL },
   { UIKTAG_OBJSplFl_Up,         UIKSPL_UP },
   { UIKTAG_OBJSplFl_Down,       UIKSPL_DOWN },
   { UIKTAG_OBJSplFl_Both,       UIKSPL_BOTH },
   { UIKTAG_OBJSplFl_Stops,      UIKSPL_STOPS },
   { TAG_DONE }
};

/* --- Code --------------------------------------------------------------- */

static void ButoirTop( int nil, struct UIKObjPropGad *pl, ULONG class, struct UIKObjSPropLift *obj )
{
   UIK_OPr_ChangePropGadget( pl, obj->PropLift->Tot, obj->PropLift->Vis, 0 );
   UIK_CauseObjLink( pl, GADGETDOWN );
}

static void ButoirDown( int nil, struct UIKObjPropGad *pl, ULONG class, struct UIKObjSPropLift *obj )
{
   UIK_OPr_ChangePropGadget( pl, obj->PropLift->Tot, obj->PropLift->Vis, obj->PropLift->Tot-obj->PropLift->Vis );
   UIK_CauseObjLink( pl, GADGETDOWN );
}

static int AddSubObjsVert ( struct UIKObjSPropLift *obj )
{
   struct Rectangle *rect;
   UWORD left, top, width, height, right, bot;

   if (! (obj->EtchedBox = UIK_AddObjectTags( "UIKObj_Box", obj,
                  UIKTAG_OBJ_LeftTop,           SETL( 0, 0 ),
                  UIKTAG_OBJ_WidthHeight,       SETL( obj->Obj.Box.Width, obj->Obj.Box.Height ),
                  UIKTAG_OBJ_BoxFl_Filled,      TRUE,
                  UIKTAG_OBJ_BoxFl_Over,        FALSE,
                  UIKTAG_OBJ_ParentRelative,    TRUE,
                  TAG_END ))) goto END_ERROR;

   rect   = &obj->Obj.ObjExt->InnerRect;
   left   = rect->MinX - obj->Obj.Box.Left;
   top    = rect->MinY - obj->Obj.Box.Top;
   width  = rect->MaxX - rect->MinX + 1;
   height = rect->MaxY - rect->MinY + 1;
   right  = left + width;
   bot    = top + height;

   if( obj->Flags & UIKSPL_STOPS )
   {
      if (! (obj->TopStop = UIK_AddObjectTags("UIKObj_Boolean", obj->EtchedBox,
                     UIKTAG_OBJ_LeftTop,           SETL( left, top ),
                     UIKTAG_OBJ_WidthHeight,       SETL( width, obj->VStopsSize ),
                     UIKTAG_OBJ_ParentRelative,    TRUE,
                     UIKTAG_OBJ_BoxFl_NoPush,      TRUE,
                     UIKTAG_OBJ_ActInactPens,      B2L(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_WHITE,UIKCOL_GREY),
                     UIKTAG_OBJ_BoxFl_Filled,      TRUE,
                     TAG_END))) goto END_ERROR;
      top += obj->VStopsSize;
   }

   if(( obj->Flags & UIKSPL_UP ) && (( obj->Flags & UIKSPL_DOWN ) | ( obj->Flags & UIKSPL_BOTH )))
   {
      if (! (obj->UpArrow = UIK_AddObjectTags("UIKObj_Up", obj->EtchedBox,
                     UIKTAG_OBJ_LeftTop,           SETL( left, top ),
                     UIKTAG_OBJ_WidthHeight,       SETL( width, obj->VArrowSize ),
                     UIKTAG_OBJ_BoxFl_Filled,      TRUE,
                     UIKTAG_OBJ_ParentRelative,    TRUE,
                     UIKTAG_OBJ_BoxFl_NoPush,      TRUE,
                     UIKTAG_OBJ_ActInactPens,      B2L(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_WHITE,UIKCOL_GREY),
                     TAG_END))) goto END_ERROR;
      top += obj->VArrowSize;
   }

   if(( obj->Flags & UIKSPL_UP ) && ( obj->Flags & UIKSPL_BOTH ))
   {
      if (! (obj->DownArrow = UIK_AddObjectTags("UIKObj_Down", obj->EtchedBox,
                     UIKTAG_OBJ_LeftTop,           SETL( left, top ),
                     UIKTAG_OBJ_WidthHeight,       SETL( width, obj->VArrowSize ),
                     UIKTAG_OBJ_BoxFl_Filled,      TRUE,
                     UIKTAG_OBJ_ParentRelative,    TRUE,
                     UIKTAG_OBJ_BoxFl_NoPush,      TRUE,
                     UIKTAG_OBJ_ActInactPens,      B2L(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_WHITE,UIKCOL_GREY),
                     TAG_END))) goto END_ERROR;
      top += obj->VArrowSize;
   }

   if( obj->Flags & UIKSPL_STOPS )
   {
      bot -= obj->VStopsSize;
      if (! (obj->BotStop = UIK_AddObjectTags("UIKObj_Boolean", obj->EtchedBox,
                     UIKTAG_OBJ_LeftTop,           SETL( left, bot ),
                     UIKTAG_OBJ_WidthHeight,       SETL( width, obj->VStopsSize ),
                     UIKTAG_OBJ_ParentRelative,    TRUE,
                     UIKTAG_OBJ_BoxFl_NoPush,      TRUE,
                     UIKTAG_OBJ_ActInactPens,      B2L(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_WHITE,UIKCOL_GREY),
                     UIKTAG_OBJ_BoxFl_Filled,      TRUE,
                     TAG_END))) goto END_ERROR;
   }

   if(( obj->Flags & UIKSPL_DOWN ) && (( obj->Flags & UIKSPL_UP ) | ( obj->Flags & UIKSPL_BOTH )))
   {
      bot -= obj->VArrowSize;
      if (! (obj->DownArrow2 = UIK_AddObjectTags("UIKObj_Down", obj->EtchedBox,
                     UIKTAG_OBJ_LeftTop,           SETL( left, bot ),
                     UIKTAG_OBJ_WidthHeight,       SETL( width, obj->VArrowSize ),
                     UIKTAG_OBJ_BoxFl_Filled,      TRUE,
                     UIKTAG_OBJ_ParentRelative,    TRUE,
                     UIKTAG_OBJ_BoxFl_NoPush,      TRUE,
                     UIKTAG_OBJ_ActInactPens,      B2L(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_WHITE,UIKCOL_GREY),
                     TAG_END))) goto END_ERROR;
   }

   if(( obj->Flags & UIKSPL_DOWN ) && ( obj->Flags & UIKSPL_BOTH ))
   {
      bot -= obj->VArrowSize;
      if (! (obj->UpArrow2 = UIK_AddObjectTags("UIKObj_Up", obj->EtchedBox,
                     UIKTAG_OBJ_LeftTop,           SETL( left, bot ),
                     UIKTAG_OBJ_WidthHeight,       SETL( width, obj->VArrowSize ),
                     UIKTAG_OBJ_BoxFl_Filled,      TRUE,
                     UIKTAG_OBJ_ParentRelative,    TRUE,
                     UIKTAG_OBJ_BoxFl_NoPush,      TRUE,
                     UIKTAG_OBJ_ActInactPens,      B2L(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_WHITE,UIKCOL_GREY),
                     TAG_END))) goto END_ERROR;
   }

   if (! (obj->PropLift = (struct UIKObjPropGad *)UIK_AddObjectTags("UIKObj_PropLift", obj->EtchedBox,
                  UIKTAG_OBJ_LeftTop,           SETL( left-2, top-1 ),
                  UIKTAG_OBJ_WidthHeight,       SETL( width+4, bot-top+2 ),
                  UIKTAG_OBJProp_Vertical,      TRUE,
                  UIKTAG_OBJProp_Total,         obj->Total,
                  UIKTAG_OBJProp_Visible,       obj->Visible,
                  UIKTAG_OBJProp_Position,      obj->Position,
                  UIKTAG_OBJ_Pattern,           (ULONG)Pat1,
                  UIKTAG_OBJ_PatternPower,      1,
                  UIKTAG_OBJ_BoxFl_Filled,      TRUE,
                  UIKTAG_OBJ_ParentRelative,    TRUE,
                  UIKTAG_OBJ_BoxAutomate,       (ULONG) PropAuto,
                  TAG_END))) goto END_ERROR;

   if (obj->Flags & UIKSPL_STOPS)
   {
      if(!(UIK_Link( obj->TopStop, obj->PropLift, (void*)ButoirTop, GADGETUP, (ULONG)obj )))
         goto END_ERROR;
      if(!(UIK_Link( obj->BotStop, obj->PropLift, (void*)ButoirDown, GADGETUP, (ULONG)obj )))
         goto END_ERROR;
   }
   if (( obj->Flags & UIKSPL_UP ) && (( obj->Flags & UIKSPL_DOWN ) | ( obj->Flags & UIKSPL_BOTH )))
   {
      if(!(UIK_Link( obj->UpArrow, obj->PropLift, 0, MOUSEMOVE|GADGETDOWN|INTUITICKS, 0 )))
         goto END_ERROR;
      if(obj->Flags & UIKSPL_BOTH)
      {
         if(!(UIK_Link( obj->DownArrow, obj->PropLift, 0, MOUSEMOVE|GADGETDOWN|INTUITICKS, 0 )))
            goto END_ERROR;
      }
   }
   if (( obj->Flags & UIKSPL_DOWN ) && (( obj->Flags & UIKSPL_UP ) | ( obj->Flags & UIKSPL_BOTH )))
   {
      if(!(UIK_Link( obj->DownArrow2, obj->PropLift, 0, MOUSEMOVE|GADGETDOWN|INTUITICKS, 0 )))
         goto END_ERROR;
      if(obj->Flags & UIKSPL_BOTH)
      {
         if(!(UIK_Link( obj->UpArrow2, obj->PropLift, 0, MOUSEMOVE|GADGETDOWN|INTUITICKS, 0 )))
            goto END_ERROR;
      }
   }

   return(1);

END_ERROR:
   return(0);
}

static int AddSubObjsHoriz ( struct UIKObjSPropLift *obj )
{
   struct Rectangle *rect;
   UWORD left, top, width, height, right, bot;

   if (! (obj->EtchedBox = UIK_AddObjectTags( "UIKObj_Box", obj,
                  UIKTAG_OBJ_LeftTop,           SETL( 0, 0 ),
                  UIKTAG_OBJ_WidthHeight,       SETL( obj->Obj.Box.Width, obj->Obj.Box.Height ),
                  UIKTAG_OBJ_BoxFl_Filled,      TRUE,
                  UIKTAG_OBJ_BoxFl_Over,        FALSE,
                  UIKTAG_OBJ_ParentRelative,    TRUE,
                  TAG_END ))) goto END_ERROR;

   rect   = &obj->Obj.ObjExt->InnerRect;
   left   = rect->MinX - obj->Obj.Box.Left;
   top    = rect->MinY - obj->Obj.Box.Top;
   width  = rect->MaxX - rect->MinX + 1;
   height = rect->MaxY - rect->MinY + 1;
   right  = left + width;
   bot    = top + height;

   if( obj->Flags & UIKSPL_STOPS )
   {
      if (! (obj->TopStop = UIK_AddObjectTags("UIKObj_Boolean", obj->EtchedBox,
                     UIKTAG_OBJ_LeftTop,           SETL( left, top ),
                     UIKTAG_OBJ_WidthHeight,       SETL( obj->HStopsSize, height ),
                     UIKTAG_OBJ_ParentRelative,    TRUE,
                     UIKTAG_OBJ_BoxFl_NoPush,      TRUE,
                     UIKTAG_OBJ_ActInactPens,      B2L(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_WHITE,UIKCOL_GREY),
                     UIKTAG_OBJ_BoxFl_Filled,      TRUE,
                     TAG_END))) goto END_ERROR;
      left += obj->HStopsSize;
   }

   if(( obj->Flags & UIKSPL_UP ) && (( obj->Flags & UIKSPL_DOWN ) | ( obj->Flags & UIKSPL_BOTH )))
   {
      if (! (obj->UpArrow = UIK_AddObjectTags("UIKObj_Left", obj->EtchedBox,
                     UIKTAG_OBJ_LeftTop,           SETL( left, top ),
                     UIKTAG_OBJ_WidthHeight,       SETL( obj->HArrowSize, height ),
                     UIKTAG_OBJ_BoxFl_Filled,      TRUE,
                     UIKTAG_OBJ_ParentRelative,    TRUE,
                     UIKTAG_OBJ_BoxFl_NoPush,      TRUE,
                     UIKTAG_OBJ_ActInactPens,      B2L(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_WHITE,UIKCOL_GREY),
                     TAG_END))) goto END_ERROR;
      left += obj->HArrowSize;
   }

   if(( obj->Flags & UIKSPL_UP ) && ( obj->Flags & UIKSPL_BOTH ))
   {
      if (! (obj->DownArrow = UIK_AddObjectTags("UIKObj_Right", obj->EtchedBox,
                     UIKTAG_OBJ_LeftTop,           SETL( left, top ),
                     UIKTAG_OBJ_WidthHeight,       SETL( obj->HArrowSize, height ),
                     UIKTAG_OBJ_BoxFl_Filled,      TRUE,
                     UIKTAG_OBJ_ParentRelative,    TRUE,
                     UIKTAG_OBJ_BoxFl_NoPush,      TRUE,
                     UIKTAG_OBJ_ActInactPens,      B2L(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_WHITE,UIKCOL_GREY),
                     TAG_END))) goto END_ERROR;
      left += obj->HArrowSize;
   }

   if( obj->Flags & UIKSPL_STOPS )
   {
      right -= obj->HStopsSize;
      if (! (obj->BotStop = UIK_AddObjectTags("UIKObj_Boolean", obj->EtchedBox,
                     UIKTAG_OBJ_LeftTop,           SETL( right, top ),
                     UIKTAG_OBJ_WidthHeight,       SETL( obj->HStopsSize, height ),
                     UIKTAG_OBJ_ParentRelative,    TRUE,
                     UIKTAG_OBJ_BoxFl_NoPush,      TRUE,
                     UIKTAG_OBJ_ActInactPens,      B2L(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_WHITE,UIKCOL_GREY),
                     UIKTAG_OBJ_BoxFl_Filled,      TRUE,
                     TAG_END))) goto END_ERROR;
   }

   if(( obj->Flags & UIKSPL_DOWN ) && (( obj->Flags & UIKSPL_UP ) | ( obj->Flags & UIKSPL_BOTH )))
   {
      right -= obj->HArrowSize;
      if (! (obj->DownArrow2 = UIK_AddObjectTags("UIKObj_Right", obj->EtchedBox,
                     UIKTAG_OBJ_LeftTop,           SETL( right, top ),
                     UIKTAG_OBJ_WidthHeight,       SETL( obj->HArrowSize, height ),
                     UIKTAG_OBJ_BoxFl_Filled,      TRUE,
                     UIKTAG_OBJ_ParentRelative,    TRUE,
                     UIKTAG_OBJ_BoxFl_NoPush,      TRUE,
                     UIKTAG_OBJ_ActInactPens,      B2L(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_WHITE,UIKCOL_GREY),
                     TAG_END))) goto END_ERROR;
   }

   if(( obj->Flags & UIKSPL_DOWN ) && ( obj->Flags & UIKSPL_BOTH ))
   {
      right -= obj->HArrowSize;
      if (! (obj->UpArrow2 = UIK_AddObjectTags("UIKObj_Left", obj->EtchedBox,
                     UIKTAG_OBJ_LeftTop,           SETL( right, top ),
                     UIKTAG_OBJ_WidthHeight,       SETL( obj->HArrowSize, height ),
                     UIKTAG_OBJ_BoxFl_Filled,      TRUE,
                     UIKTAG_OBJ_ParentRelative,    TRUE,
                     UIKTAG_OBJ_BoxFl_NoPush,      TRUE,
                     UIKTAG_OBJ_ActInactPens,      B2L(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_WHITE,UIKCOL_GREY),
                     TAG_END))) goto END_ERROR;
   }

   if (! (obj->PropLift = (struct UIKObjPropGad *)UIK_AddObjectTags("UIKObj_PropLift", obj->EtchedBox,
                  UIKTAG_OBJ_LeftTop,           SETL( left-2, top-1 ),
                  UIKTAG_OBJ_WidthHeight,       SETL( right-left+4, height+2 ),
                  UIKTAG_OBJProp_Vertical,      FALSE,
                  UIKTAG_OBJProp_Total,         obj->Total,
                  UIKTAG_OBJProp_Visible,       obj->Visible,
                  UIKTAG_OBJProp_Position,      obj->Position,
                  UIKTAG_OBJ_Pattern,           (ULONG)Pat1,
                  UIKTAG_OBJ_PatternPower,      1,
                  UIKTAG_OBJ_BoxFl_Filled,      TRUE,
                  UIKTAG_OBJ_ParentRelative,    TRUE,
                  UIKTAG_OBJ_BoxAutomate,       (ULONG) PropAuto,
                  TAG_END))) goto END_ERROR;

   if (obj->Flags & UIKSPL_STOPS)
   {
      if(!(UIK_Link( obj->TopStop, obj->PropLift, (void*)ButoirTop, GADGETUP, (ULONG)obj )))
         goto END_ERROR;
      if(!(UIK_Link( obj->BotStop, obj->PropLift, (void*)ButoirDown, GADGETUP, (ULONG)obj )))
         goto END_ERROR;
   }
   if (( obj->Flags & UIKSPL_UP ) && (( obj->Flags & UIKSPL_DOWN ) | ( obj->Flags & UIKSPL_BOTH )))
   {
      if(!(UIK_Link( obj->UpArrow, obj->PropLift, 0, MOUSEMOVE|GADGETDOWN|INTUITICKS, 0 )))
         goto END_ERROR;
      if(obj->Flags & UIKSPL_BOTH)
      {
         if(!(UIK_Link( obj->DownArrow, obj->PropLift, 0, MOUSEMOVE|GADGETDOWN|INTUITICKS, 0 )))
            goto END_ERROR;
      }
   }
   if (( obj->Flags & UIKSPL_DOWN ) && (( obj->Flags & UIKSPL_UP ) | ( obj->Flags & UIKSPL_BOTH )))
   {
      if(!(UIK_Link( obj->DownArrow2, obj->PropLift, 0, MOUSEMOVE|GADGETDOWN|INTUITICKS, 0 )))
         goto END_ERROR;
      if(obj->Flags & UIKSPL_BOTH)
      {
         if(!(UIK_Link( obj->UpArrow2, obj->PropLift, 0, MOUSEMOVE|GADGETDOWN|INTUITICKS, 0 )))
            goto END_ERROR;
      }
   }

   return(1);

END_ERROR:
   return(0);
}

static void ChangePropGadget( struct UIKObjSPropLift *obj, struct SplParms *splp, ULONG dummy )
{
   UIK_OPr_ChangePropGadget( obj->PropLift, splp->Total, splp->Visible, splp->Position );
}

static void ChangePropNoDraw( struct UIKObjSPropLift *obj, struct SplParms *splp, ULONG dummy )
{
   UIK_OPr_ChangePropNoDraw( obj->PropLift, splp->Total, splp->Visible, splp->Position );
}

static int Link2List( struct UIKObjSPropLift *obj, struct UIKObjList *ol, ULONG Events )
{
   if(!UIK_Link(obj->PropLift, ol, 0, Events, 0))
      goto LL_ERROR;

   return(1);
LL_ERROR:
   return(0);
}

/* --- Standard object code ----------------------------------------------- */

static void ResizeObject( struct UIKObjSPropLift *obj )
{
/*   if(obj->Flags & UIKSPL_VERTICAL)
   {
      struct Rectangle *rect;
      UWORD left, top, width, height, right, bot;

      left = obj->Obj.Box.Left;
      top = obj->Obj.Box.Top;
      width = obj->Obj.Box.Width;
      height = obj->Obj.Box.Height;

      obj->EtchedBox->Box.Left = left;
      obj->EtchedBox->Box.Top = top;
      obj->EtchedBox->Box.Width = width;
      obj->EtchedBox->Box.Height = height;

/*      rect   = &obj->Obj.ObjExt->InnerRect;
      left   = rect->MinX - obj->Obj.Box.Left;
      top    = rect->MinY - obj->Obj.Box.Top;
      width  = rect->MaxX - rect->MinX + 1;
      height = rect->MaxY - rect->MinY + 1;
      right  = left + width;
      bot    = top + height;
*/
      if( obj->Flags & UIKSPL_STOPS )
      {
         obj->TopStop->Box.Left = left;
         obj->TopStop->Box.Top = top;
         obj->TopStop->Box.Width = width;
         obj->TopStop->Box.Height = obj->VStopsSize;
      }
/*
   if( obj->Flags & UIKSPL_STOPS )
   {
      if (! (obj->TopStop = UIK_AddObjectTags("UIKObj_Boolean", obj->EtchedBox,
                     UIKTAG_OBJ_LeftTop,           SETL( left, top ),
                     UIKTAG_OBJ_WidthHeight,       SETL( width, obj->VStopsSize ),
      top += obj->VStopsSize;
   }

   if(( obj->Flags & UIKSPL_UP ) && (( obj->Flags & UIKSPL_DOWN ) | ( obj->Flags & UIKSPL_BOTH )))
   {
      if (! (obj->UpArrow = UIK_AddObjectTags("UIKObj_Up", obj->EtchedBox,
                     UIKTAG_OBJ_LeftTop,           SETL( left, top ),
                     UIKTAG_OBJ_WidthHeight,       SETL( width, obj->VArrowSize ),
                     UIKTAG_OBJ_BoxFl_Filled,      TRUE,
                     UIKTAG_OBJ_ParentRelative,    TRUE,
                     UIKTAG_OBJ_BoxFl_NoPush,      TRUE,
                     UIKTAG_OBJ_ActInactPens,      B2L(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_WHITE,UIKCOL_GREY),
                     TAG_END))) goto END_ERROR;
      top += obj->VArrowSize;
   }

   if(( obj->Flags & UIKSPL_UP ) && ( obj->Flags & UIKSPL_BOTH ))
   {
      if (! (obj->DownArrow = UIK_AddObjectTags("UIKObj_Down", obj->EtchedBox,
                     UIKTAG_OBJ_LeftTop,           SETL( left, top ),
                     UIKTAG_OBJ_WidthHeight,       SETL( width, obj->VArrowSize ),
                     UIKTAG_OBJ_BoxFl_Filled,      TRUE,
                     UIKTAG_OBJ_ParentRelative,    TRUE,
                     UIKTAG_OBJ_BoxFl_NoPush,      TRUE,
                     UIKTAG_OBJ_ActInactPens,      B2L(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_WHITE,UIKCOL_GREY),
                     TAG_END))) goto END_ERROR;
      top += obj->VArrowSize;
   }

   if( obj->Flags & UIKSPL_STOPS )
   {
      bot -= obj->VStopsSize;
      if (! (obj->BotStop = UIK_AddObjectTags("UIKObj_Boolean", obj->EtchedBox,
                     UIKTAG_OBJ_LeftTop,           SETL( left, bot ),
                     UIKTAG_OBJ_WidthHeight,       SETL( width, obj->VStopsSize ),
                     UIKTAG_OBJ_ParentRelative,    TRUE,
                     UIKTAG_OBJ_BoxFl_NoPush,      TRUE,
                     UIKTAG_OBJ_ActInactPens,      B2L(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_WHITE,UIKCOL_GREY),
                     UIKTAG_OBJ_BoxFl_Filled,      TRUE,
                     TAG_END))) goto END_ERROR;
   }

   if(( obj->Flags & UIKSPL_DOWN ) && (( obj->Flags & UIKSPL_UP ) | ( obj->Flags & UIKSPL_BOTH )))
   {
      bot -= obj->VArrowSize;
      if (! (obj->DownArrow2 = UIK_AddObjectTags("UIKObj_Down", obj->EtchedBox,
                     UIKTAG_OBJ_LeftTop,           SETL( left, bot ),
                     UIKTAG_OBJ_WidthHeight,       SETL( width, obj->VArrowSize ),
                     UIKTAG_OBJ_BoxFl_Filled,      TRUE,
                     UIKTAG_OBJ_ParentRelative,    TRUE,
                     UIKTAG_OBJ_BoxFl_NoPush,      TRUE,
                     UIKTAG_OBJ_ActInactPens,      B2L(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_WHITE,UIKCOL_GREY),
                     TAG_END))) goto END_ERROR;
   }

   if(( obj->Flags & UIKSPL_DOWN ) && ( obj->Flags & UIKSPL_BOTH ))
   {
      bot -= obj->VArrowSize;
      if (! (obj->UpArrow2 = UIK_AddObjectTags("UIKObj_Up", obj->EtchedBox,
                     UIKTAG_OBJ_LeftTop,           SETL( left, bot ),
                     UIKTAG_OBJ_WidthHeight,       SETL( width, obj->VArrowSize ),
                     UIKTAG_OBJ_BoxFl_Filled,      TRUE,
                     UIKTAG_OBJ_ParentRelative,    TRUE,
                     UIKTAG_OBJ_BoxFl_NoPush,      TRUE,
                     UIKTAG_OBJ_ActInactPens,      B2L(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_WHITE,UIKCOL_GREY),
                     TAG_END))) goto END_ERROR;
   }

   if (! (obj->PropLift = (struct UIKObjPropGad *)UIK_AddObjectTags("UIKObj_PropLift", obj->EtchedBox,
                  UIKTAG_OBJ_LeftTop,           SETL( left-2, top-1 ),
                  UIKTAG_OBJ_WidthHeight,       SETL( width+4, bot-top+2 ),
                  UIKTAG_OBJProp_Vertical,      TRUE,
                  UIKTAG_OBJProp_Total,         obj->Total,
                  UIKTAG_OBJProp_Visible,       obj->Visible,
                  UIKTAG_OBJProp_Position,      obj->Position,
                  UIKTAG_OBJ_Pattern,           (ULONG)Pat1,
                  UIKTAG_OBJ_PatternPower,      1,
                  UIKTAG_OBJ_BoxFl_Filled,      TRUE,
                  UIKTAG_OBJ_ParentRelative,    TRUE,
                  UIKTAG_OBJ_BoxAutomate,       (ULONG) PropAuto,
                  TAG_END))) goto END_ERROR;

*/
   }
   else
   {
   }
*/
}

static int AddObject( struct UIKObjSPropLift *obj, struct TagItem *taglist)
{

   SysBase = (struct ExecBase *) *((ULONG*)4);
   if(!(obj->IntuitionBase = IntuitionBase =
        (struct IntuitionBase *)OpenLibrary("intuition.library",0)))
      goto ADD_ERROR;
   if(!(obj->GfxBase = GfxBase =
        (struct GfxBase *)OpenLibrary("graphics.library",0)))
      goto ADD_ERROR;
   if(!(obj->UIKBase = UIKBase =
        (struct UIKBase *)OpenLibrary(UIKNAME,0)))
      goto ADD_ERROR;

   obj->Total = GetTagData( UIKTAG_OBJSpl_Total , 100, taglist );
   obj->Visible = GetTagData( UIKTAG_OBJSpl_Visible , 20, taglist );
   obj->Position = GetTagData( UIKTAG_OBJSpl_Position , 50, taglist );
   obj->VArrowSize = (UWORD)GetTagData( UIKTAG_OBJSpl_VArrowSize, 8, taglist );
   obj->VStopsSize = (UWORD)GetTagData( UIKTAG_OBJSpl_VStopsSize, 8, taglist );
   obj->HArrowSize = (UWORD)GetTagData( UIKTAG_OBJSpl_HArrowSize, 10, taglist );
   obj->HStopsSize = (UWORD)GetTagData( UIKTAG_OBJSpl_HStopsSize, 10, taglist );

   obj->Flags = GetTagData( UIKTAG_OBJSpl_Flags, (UIKSPL_VERTICAL|UIKSPL_STOPS), taglist );
   obj->Flags = PackBoolTags( obj->Flags, taglist, flags_boolmap );

   if(obj->Flags & UIKSPL_VERTICAL)
   {
      if(!AddSubObjsVert(obj))
         goto ADD_ERROR;
   }
   else
   {
      if(!AddSubObjsHoriz(obj))
         goto ADD_ERROR;
   }

   return(1);

 ADD_ERROR:
   return(0);
}

static void RemoveObject( struct UIKObjSPropLift *obj )
{
   if(obj->UIKBase)        CloseLibrary((struct Library *)obj->UIKBase);
   if(obj->GfxBase)        CloseLibrary((struct Library *)obj->GfxBase);
   if(obj->IntuitionBase)  CloseLibrary((struct Library *)obj->IntuitionBase);

}

/* --- Public functions table --------------------------------------------- */

static ULONG FuncTable[] =
{
   ChangePropGadget,
   ChangePropNoDraw,
   Link2List,
   NULL
};

/* --- Object germ taglist ------------------------------------------------ */

static struct TagItem GermTagList[] =
{
   { UIKTAG_GEN_LastFuncId,      UIKFUNC_SPL_LastFunc },
   { TAG_END }
};

/* --- Object germ -------------------------------------------------------- */

struct UIKObjGerm Germ =
{
   "UIKObj_SPropLift",            /* UBYTE           ObjName[32];           */
   AddObject,                     /* int             (*AddVector)();        */
   NULL,                          /* int             (*StartVector)();      */
   NULL,                          /* void            (*StopVector)();       */
   RemoveObject,                  /* void            (*RemoveVector)();     */
   NULL,                          /* void            (*RefreshVector)();    */
   ResizeObject,                  /* void            (*ResizeVector)();     */
   NULL,                          /* void            (*ExceptActVector)();  */
   NULL,                          /* void            (*ExceptInaVector)();  */
   NULL,                          /* void            (*ExceptMovVector)();  */
   NULL,                          /* void            (*ExceptKeyVector)();  */
   NULL,                          /* int             (*StartAfterVector)(); */
   NULL,                          /* int             (*SelectVector)();     */
   FuncTable,                     /* ULONG           *ExtentedVectors;      */
   GermTagList,                   /* struct TagItem  *GermTagList;          */
   15,                            /* UWORD           MinWidth;              */
   15,                            /* UWORD           MinHeight;             */
   SETL(15,75),                   /* ULONG DefaultWidthHeight;    */
   UIKID_SPROPLIFT,               /* UWORD           ObjId;                 */
   sizeof(struct UIKObjSPropLift),/* UWORD           ObjSize;               */
   BOX_DISPLAYABLE|BOX_SEMAPHORE|BOX_TRANSLATELEFTTOP
};
