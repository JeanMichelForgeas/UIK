/****************************************************************************
 *                                                                          *
 *  File : UIKObj_Cycle.c                                                   *
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

/* --- Includes ----------------------------------------------------------- */

#include "UIKI:UIKBase.h"
#include "UIKI:UIKGlobal.h"
#include "UIKI:UIKObj.h"
#include "UIKI:UIKMacros.h"
#include "UIKI:UIKGfx.h"
#include "UIKI:UIKDrawAuto.h"
#include "UIKI:UIK_Protos.h"
#include "UIKI:UIK_Pragmas.h"

#include "UIKI:ObjCycle.h"

/* --- Defines ------------------------------------------------------------ */
/* --- Structures definitions --------------------------------------------- */
/* --- Macros ------------------------------------------------------------- */
/* --- Exported ----------------------------------------------------------- */

struct ExecBase      * __far SysBase;
struct UIKBase       * __far UIKBase;
struct Library       * __far LayersBase;
struct GfxBase       * __far GfxBase;
struct IntuitionBase * __far IntuitionBase;

/* --- Imported ----------------------------------------------------------- */
/* --- Statics ------------------------------------------------------------ */

/* --- Copyright Stuff --- */

static UBYTE Copyright[] = "UIKObj_Cycle Copyright © 1993, Frédéric DENIS, Tous droits réservés";
static UBYTE Copyright2[] = "UIKObj_Cycle Copyright © 1993, Frédéric DENIS, All rights reserved";

/* ----------------------- */

static UWORD LogoAuto[] =
{
   x_ADD_N, 2, y_ADD_N, 2,
   Y_SUB_N, 4,
   IF_RATIO2_SKIP_N, 6,
   IF_NOTLOOK_SKIP_N, 2,
   Y_SUB_1,
   COL_BLACK,
   y_ADD_1, Y_SUB_1,
   MOVE_xy, DRAW_xY,
   x_ADD_1,
   y_SUB_1, Y_ADD_1,
   IF_RATIO1_SKIP_N, 4,
   MOVE_xy, DRAW_xY,
   MOVE_xy, x_ADD_N, 5, DRAW_xy, x_SUB_N, 5,
   MOVE_xY, x_ADD_N, 5, DRAW_xY,
   y_ADD_1, Y_SUB_1,
   IF_RATIO1_SKIP_N, 4,
   MOVE_xy, DRAW_MID_yY_x,
   x_ADD_1,
   MOVE_xy, DRAW_MID_yY_x,
   y_MID, y_ADD_N, 2,
   MOVE_xY, DRAW_xy,
   IF_RATIO1_SKIP_N, 8,
   x_SUB_1,
   MOVE_xY, DRAW_xy,
   x_ADD_1,
   y_SUB_N, 3,
   IF_RATIO1_SKIP_N, 30,
   x_SUB_N, 2, MOVE_xy, x_ADD_N, 3,
   DRAW_xy, y_SUB_1, x_SUB_N, 4, MOVE_xy,
   x_ADD_N, 5, DRAW_xy, SKIP_N, 24,
   x_SUB_1, MOVE_xy, x_ADD_N, 2,
   DRAW_xy, y_SUB_1, x_SUB_N, 3, MOVE_xy,
   x_ADD_N, 4, DRAW_xy,
   END_AUTO
};

static UWORD BarAuto[] =
{
   y_ADD_1, Y_SUB_N, 3,
   IF_RATIO2_SKIP_N, 6,
   IF_NOTLOOK_SKIP_N, 2,
   Y_SUB_1,
   COL_BLACK, MOVE_xy, DRAW_xY,
   x_ADD_1,
   COL_WHITE, MOVE_xy, DRAW_xY,
   END_AUTO
};

static UWORD AltBarAuto[] =
{
   y_ADD_1, Y_SUB_N, 3,
   IF_RATIO2_SKIP_N, 6,
   IF_NOTLOOK_SKIP_N, 2,
   Y_SUB_1,
   COL_WHITE, MOVE_xy, DRAW_xY,
   x_ADD_1,
   COL_BLACK, MOVE_xy, DRAW_xY,
   END_AUTO
};

/* --- Code --------------------------------------------------------------- */

static ULONG alloc_string( struct UIKObjCycle *obj, struct UIKCycleEntry *uce )
{
   if (uce->Flags & CYCLE_TEXT_ADDR)
   {
      UBYTE *ptr;
      if (! (ptr = UIK_nl_AllocNode( &obj->Strings, -1, StrLen( (UBYTE*) uce->Title ), 0 ))) return(0);
      StrCpy( ptr, (UBYTE*) uce->Title );
      uce->Title = (ULONG) ptr;
   }
   return(1);
}

static void free_string( struct UIKObjCycle *obj, struct UIKCycleEntry *uce )
{
   if (uce->Flags & CYCLE_TEXT_ADDR)
   {
      LONG line;
      if ((line = UIK_nl_FindNode( &obj->Strings, (UBYTE *) uce->Title )) != -1)
      {
         UIK_nl_FreeNodes( &obj->Strings, line, line );
         uce->Title = 0;
         uce->Flags &= ~CYCLE_TEXT_ADDR;
      }
   }
}

static int ParseCallBack( struct UIKObjCycle *obj )
{
   if(obj->CurrentEntry->Func)
   {
      (obj->CurrentEntry->Func)( obj, obj->CurrentEntry->UserData );
   }
   return(0);
}

/* --- Standard object code ----------------------------------------------- */

static int AddCycle( struct UIKObjCycle *obj, struct TagItem *taglist)
{
   struct UIKCycleEntry *Entries;
   struct UIKCycleEntry *uce;

   SysBase = (struct ExecBase *) *((ULONG*)4);
   if(!(obj->IntuitionBase = IntuitionBase =
        (struct IntuitionBase *)OpenLibrary("intuition.library",0)))
      goto ADD_ERROR;
   if(!(obj->GfxBase = GfxBase =
        (struct GfxBase *)OpenLibrary("graphics.library",0)))
      goto ADD_ERROR;
   if(!(obj->LayersBase = LayersBase =
         (struct Library *)OpenLibrary("layers.library",0)))
      goto ADD_ERROR;
   if(!(obj->UIKBase = UIKBase =
        (struct UIKBase *)OpenLibrary(UIKNAME,0)))
      goto ADD_ERROR;

   if(!(Entries = (struct UIKCycleEntry *)GetTagData( UIKTAG_Cycle_Entries, 0, taglist )))
      goto ADD_ERROR;

   for(; Entries->Title; Entries++)
   {
      if(!(uce=(struct UIKCycleEntry *)UIK_nl_AllocNode( &obj->Entries, -1,
                                                         sizeof(struct UIKCycleEntry),0)))
         goto ADD_ERROR;
      UIK_MemCopy( (APTR) Entries, uce, sizeof(struct UIKCycleEntry) );
      if(!alloc_string(obj,uce))
         goto ADD_ERROR;
      if(uce->Flags & CYCLE_DISPFIRST)
         obj->CurrentEntry = uce;
   }

   if(!(obj->ClipRegion = NewRegion())) goto ADD_ERROR;

   obj->LastMState = 0xff;

   if(!obj->CurrentEntry)
      obj->CurrentEntry = (struct UIKCycleEntry *)UIK_nl_NodeBuf( &obj->Entries, 0 );

   obj->Gad.LeftEdge    = obj->Obj.Box.Left;
   obj->Gad.TopEdge     = obj->Obj.Box.Top;
   obj->Gad.Width       = obj->Obj.Box.Width;
   obj->Gad.Height      = obj->Obj.Box.Height;
   obj->Gad.Flags       = GADGHNONE;
   obj->Gad.Activation  = FOLLOWMOUSE | RELVERIFY | GADGIMMEDIATE;
   obj->Gad.GadgetType  = BOOLGADGET;
   obj->Gad.UserData    = (APTR) obj;

   if(!(obj->Hook = UIK_AddVector( obj, ParseCallBack, GADGETUP, NULL ))) return(0);

   return(1);

 ADD_ERROR:
   return(0);
}

static int StartCycle( struct UIKObjCycle *obj )
{
   UIK_StdGadStart( obj );
   return(1);
}

static void StopCycle( struct UIKObjCycle *obj )
{
   UIK_StdGadStop( obj );
}

static void RemoveCycle( struct UIKObjCycle *obj )
{
   if( obj->Hook ) UIK_RemoveVector( obj, obj->Hook);
   if( obj->ClipRegion ) DisposeRegion( obj->ClipRegion );

   UIK_nl_FreeList( &obj->Strings );
   UIK_nl_FreeList( &obj->Entries );

   if(obj->UIKBase)        CloseLibrary(obj->UIKBase);
   if(obj->LayersBase)     CloseLibrary(obj->LayersBase);
   if(obj->GfxBase)        CloseLibrary(obj->GfxBase);
   if(obj->IntuitionBase)  CloseLibrary(obj->IntuitionBase);
}

static void RefreshImage( struct UIKObjCycle *obj )
{
   UBYTE *ObjText;
   UBYTE ClipOK = TRUE;
   struct Rectangle Rect;
   struct UIKBitMap *ubm;
   struct RastPort *rp = (SysBase->ThisTask == obj->Obj.UIK->Task) ?
                         obj->Obj.rp : obj->Obj.irp;

   UIK_MemCopy( &obj->Obj.ObjExt->InnerRect, &Rect, sizeof(struct Rectangle));
   UIK_DrawAuto( obj, LogoAuto, Rect.MinX - obj->Obj.Box.Left,
                                Rect.MinY - obj->Obj.Box.Top );
   if(obj->Obj.Flags & BOX_OVER)
   {
      UIK_DrawAuto( obj, BarAuto, Rect.MinX - obj->Obj.Box.Left + 13,
                                  Rect.MinY - obj->Obj.Box.Top );
   }
   else
   {
      UIK_DrawAuto( obj, AltBarAuto, Rect.MinX - obj->Obj.Box.Left + 13,
                                     Rect.MinY - obj->Obj.Box.Top );
   }
   Rect.MinX += 16;
   ClearRegion( obj->ClipRegion );
   if (OrRectRegion( obj->ClipRegion, &Rect ))
   {
      obj->ObjRegion = InstallClipRegion( rp->Layer, obj->ClipRegion );
      ClipOK = TRUE;
   }
   SetAPen( rp, UIK_GetColor(obj, obj->Obj.BoxPen) );
   RectFill( rp, Rect.MinX, Rect.MinY, Rect.MaxX, Rect.MaxY );
   if (obj->CurrentEntry->Flags & CYCLE_TEXT_IMAGE)
   {
      ubm = (struct UIKBitMap *)obj->CurrentEntry->Title;
      BltMaskBitMapRastPort( UIK2BM(ubm), 0, 0, rp,
                             Rect.MinX + ((Rect.MaxX - Rect.MinX) - ubm->Width) / 2,
                             Rect.MinY + ((Rect.MaxY - Rect.MinY) - ubm->Rows) / 2,
                             ubm->Width, ubm->Rows, 0xe0, ubm->Mask );
   }
   else
   {
      ObjText = (obj->CurrentEntry->Flags & CYCLE_TEXT_ADDR) ?
                obj->CurrentEntry->Title :
                UIK_LangString( obj, obj->CurrentEntry->Title );
      SetAPen( rp, UIK_GetColor(obj, obj->Obj.TitlePen) );
      Move( rp, Rect.MinX + (Rect.MaxX - Rect.MinX + 1) / 2 -
                ( UIK_StrWidth( obj->Obj.ObjExt->Font, ObjText, StrLen(ObjText) ) / 2 ),
                Rect.MinY + (Rect.MaxY - Rect.MinY + 1) / 2 +
                (obj->Obj.ObjExt->Font->tf_YSize - obj->Obj.ObjExt->Font->tf_Baseline) );
      Text( rp, ObjText, StrLen(ObjText) );
   }
   if (ClipOK)
   {
      obj->ClipRegion = InstallClipRegion( rp->Layer, obj->ObjRegion );
   }
}

static void NextEntry( struct UIKObjCycle *obj )
{
   int Number;
   Number = (UIK_nl_FindNode( &obj->Entries, (UBYTE *)obj->CurrentEntry ) + 1) %
            UIK_nl_ListNodeNum( &obj->Entries );
   obj->CurrentEntry = (struct UIKCycleEntry *)UIK_nl_NodeBuf( &obj->Entries, (LONG)Number );
}

static void PrevEntry( struct UIKObjCycle *obj )
{
   int Number, NEntries;
   NEntries  = UIK_nl_ListNodeNum( &obj->Entries );
   Number = (UIK_nl_FindNode( &obj->Entries, (UBYTE *)obj->CurrentEntry )
            + (NEntries - 1)) % NEntries;
   obj->CurrentEntry = (struct UIKCycleEntry *)UIK_nl_NodeBuf( &obj->Entries, (LONG)Number );
}

static void RefreshCycle( struct UIKObjCycle *obj )
{
   UIK_StdGadRefresh( obj );
   RefreshImage( obj );
}

static void ResizeCycle( struct UIKObjCycle *obj )
{
   UIK_StdGadResize( obj );

}

static void __asm ActCycle( register __a1 struct UIKObjCycle *obj )
{
   UIK_StdGadAct( obj );
   RefreshImage( obj );
}

static void __asm InactCycle( register __a1 struct UIKObjCycle *obj,
                              register __a3 struct IntuiMessage *imsg)
{
   UIK_StdGadInact( obj );
   if (obj->Obj.MouseIn)
   {
      if(imsg->Qualifier & (IEQUALIFIER_LSHIFT|IEQUALIFIER_RSHIFT))
         PrevEntry( obj );
      else
         NextEntry( obj );
   }
   RefreshImage( obj );
}

static void __asm MoveCycle( register __a1 struct UIKObjCycle *obj,
                             register __a3 struct IntuiMessage *imsg )
{
   if(obj->LastMState != obj->Obj.MouseIn)
   {
      obj->LastMState = obj->Obj.MouseIn;
      UIK_StdGadMove( obj, imsg );
      RefreshImage( obj );
   }
}

static int SelectCycle( struct UIKObjCycle *obj, ULONG Class )
{
   return(1);
}

/* --- Public functions table --------------------------------------------- */

static ULONG FuncTable[] =
{
   NULL
};

/* --- Object germ taglist ------------------------------------------------ */

static struct TagItem GermTagList[] =
{
   { TAG_END }
};

/* --- Object germ -------------------------------------------------------- */

struct UIKObjGerm Germ =
{
   "UIKObj_Cycle",                /* UBYTE           ObjName[32];           */
   AddCycle,                      /* int             (*AddVector)();        */
   StartCycle,                    /* int             (*StartVector)();      */
   StopCycle,                     /* void            (*StopVector)();       */
   RemoveCycle,                   /* void            (*RemoveVector)();     */
   RefreshCycle,                  /* void            (*RefreshVector)();    */
   ResizeCycle,                   /* void            (*ResizeVector)();     */
   ActCycle,                      /* void            (*ExceptActVector)();  */
   InactCycle,                    /* void            (*ExceptInaVector)();  */
   MoveCycle,                     /* void            (*ExceptMovVector)();  */
   NULL,                          /* void            (*ExceptKeyVector)();  */
   NULL,                          /* int             (*StartAfterVector)(); */
   NULL,                          /* int             (*SelectVector)();     */
   FuncTable,                     /* ULONG           *ExtentedVectors;      */
   GermTagList,                   /* struct TagItem  *GermTagList;          */
   8,                             /* UWORD           MinWidth;              */
   4,                             /* UWORD           MinHeight;             */
   SETL(OBJCYCLE_BOX_STDWIDTH,OBJCYCLE_BOX_STDHEIGHT),/* ULONG DefaultWidthHeight;    */
   0,                             /* UWORD           ObjId;                 */
   sizeof(struct UIKObjCycle),    /* UWORD           ObjSize;               */
   BOX_INNERZONE | BOX_OVER | BOX_FILLED | BOX_SEMAPHORE |
   BOX_TRANSLATELEFTTOP | BOX_ZONEABLE | BOX_DISPLAYABLE
};