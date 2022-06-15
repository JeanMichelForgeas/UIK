/******************************************************************************************************************
 *                                                                                                                *
 *  File : DisplayMode.c                                                                                          *
 *  Project : Display mode selector.                                                                              *
 *                                                                                                                *
 *  Created : 16/07/92 ©1992 Frédéric DENIS / "ß release" Software Design.                                        *
 *                                                                                                                *
 *  All rights reserved.                                                                                          *
 *                                                                                                                *
 ******************************************************************************************************************/

/* --- Includes ------------------------------------------------------------------------------------------------- */

#include <graphics/gfxbase.h>

#include "uiki:uikbase.h"
#include "uiki:uikglobal.h"
#include "uiki:uikobj.h"
#include "uiki:uikbox.h"
#include "uiki:uikmacros.h"
#include "uiki:uikcolors.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"
#include "uiki:uikdrawauto.h"
#include "uiki:uikgfx.h"

#include "uiki:uiklist.h"
#include "uiki:objlist.h"
#include "uiki:objstring.h"
#include "uiki:objproplift.h"
#include "objdisplaymode.h"

/* --- Defines -------------------------------------------------------------------------------------------------- */

#define EMPTY_BUFF         "                                "
#define MODE_TITLE         1
#define FRAME_TITLE        2
#define MIN_OSCANT         3
#define TEXT_OSCANT        4
#define GFX_OSCANT         5
#define MAX_OSCANT         6
#define CUSTOM_SIZE        7
#define SHADES             8
#define COLORS             9
#define S_HAM              10
#define S_EXTRA            11
#define COLOR_TITLE        12
#define WIDTH_TITLE        13
#define HEIGHT_TITLE       14
#define PALLOWRESLACED     15
#define PALLOWRES          16
#define PALHIGHRESLACED    17
#define PALHIGHRES         18
#define NTSCLOWRESLACED    19
#define NTSCLOWRES         20
#define NTSCHIGHRESLACED   21
#define NTSCHIGHRES        22
#define IS_LACED           23
#define IS_ECS             24
#define IS_GENLOCK         25
#define IS_DRAGGABLE       26

#define LOCALE             0x00000001
#define OLDKICK            0x00000002
#define KICK37             0x00000004
#define KICK38             0x0000000C

/* --- Structures definitions ----------------------------------------------------------------------------------- */

/* Display's characteristics */
struct DispInfos
{
   ULONG             DisplayID;
   ULONG             PropertyFlags;
   union {
         ULONG      ColorShades;      /* # of different colors in the palette usable by this mode */
         struct {
                UBYTE  RedBits;
                UBYTE  GreenBits;
                UBYTE  BlueBits;
                UBYTE  pad;
                } cbits;
         } colshade;
   UWORD             MaxDepth;
   UWORD             MaxRasterWidth;
   UWORD             MaxRasterHeight;
   Point             Nominal;
   Point             MaxOScan;
   Point             TxtOScan;
   Point             StdOScan;
};

struct Disp1_3
{
   ULONG             Name;
   struct DispInfos  DispInfo;
};

/* Color setting */
struct ColorInfos
{
   ULONG             Depth;      /* depth of the display for this color setting. */
   ULONG             DisplayID;  /* This mode's display ID. */
};

/* --- Macros --------------------------------------------------------------------------------------------------- */
/* --- Exported ------------------------------------------------------------------------------------------------- */

ULONG __far SysBase;
struct DosBase * __far DOSBase;
struct UIKBase * __far UIKBase;
struct GfxBase * __far GfxBase;
struct IntuitionBase * __far IntuitionBase;

/* --- Imported ------------------------------------------------------------------------------------------------- */

extern UBYTE * __far LangOther[];
extern UBYTE * __far LangEnglish[];

/* --- Statics -------------------------------------------------------------------------------------------------- */

static UBYTE __far Copyright[] = "UIKObj_DisplayMode Copyright © 1992, Frédéric DENIS, Tous droits réservés";
static UBYTE __far Copyright2[] = "UIKObj_DisplayMode Copyright © 1992, Frédéric DENIS, All rights reserved";

static struct Disp1_3 __far PalModes[] =
{
   { PALLOWRESLACED,
      {
         0x4,
         0xAE1,
         4096,
         5,
         16368,
         16384,
         { 320, 512 },
         { 362, 566 },
         { 329, 560 },
         { 362, 566 }
      }
   },
   { PALLOWRES,
      {
         0x0,
         0xAE0,
         4096,
         5,
         16368,
         16384,
         { 320, 256 },
         { 362, 283 },
         { 329, 280 },
         { 362, 283 }
      }
   },
   { PALHIGHRESLACED,
      {
         0x8004,
         0xBE1,
         4096,
         4,
         16368,
         16384,
         { 640, 512 },
         { 724, 566 },
         { 658, 560 },
         { 724, 566 }
      }
   },
   { PALHIGHRES,
      {
         0x8000,
         0xBE0,
         4096,
         4,
         16368,
         16384,
         { 640, 256 },
         { 724, 283 },
         { 658, 280 },
         { 724, 283 }
      }
   }
};

static struct Disp1_3 __far NtscModes[] =
{
   { NTSCLOWRESLACED,
      {
         0x11004,
         0xAC1,
         4096,
         5,
         16368,
         16384,
         { 320, 400 },
         { 362, 482 },
         { 359, 464 },
         { 361, 480 }
      }
   },
   { NTSCLOWRES,
      {
         0x11000,
         0xAC0,
         4096,
         5,
         16368,
         16384,
         { 320, 200 },
         { 362, 241 },
         { 359, 232 },
         { 361, 240 }
      }
   },
   { NTSCHIGHRESLACED,
      {
         0x19004,
         0xBC1,
         4096,
         4,
         16368,
         16384,
         { 640, 400 },
         { 724, 482 },
         { 718, 464 },
         { 722, 480 }
      }
   },
   { NTSCHIGHRES,
      {
         0x19000,
         0xBC0,
         4096,
         4,
         16368,
         16384,
         { 640, 200 },
         { 724, 241 },
         { 718, 232 },
         { 722, 240 }
      }
   }
};

static UWORD __far Pat1[] = { 0xAAAA, 0x5555 };
static UWORD __far PropAuto[] =
{
   x_ADD_1, X_SUB_1,
   y_ADD_1, Y_SUB_1,
   COL_BLACK, MOVE_Xy, DRAW_XY, Y_ADD_1,
   MOVE_xy, DRAW_Xy,
   END_AUTO
};

/* --- Code ----------------------------------------------------------------------------------------------------- */

ULONG Cross_Insert_List(UBYTE *buff, struct UIKObjDisplayMode *odm, ULONG index)
{
   if(StrNCCmp(buff,odm->insstr)==1)
   {
      odm->index = index;
      return(0);
   }
   return(1);
}

ULONG Cross_Find_List(UBYTE *buff, struct UIKObjDisplayMode *odm, ULONG index)
{
   if(StrNCCmp(buff,odm->insstr)==0)
   {
      odm->index = index;
      return(0);
   }
   return(1);
}

ULONG Cross_Find_List_By_ModeID(struct DispInfos *buff, struct UIKObjDisplayMode *odm, ULONG index)
{
   if(buff->DisplayID == (odm->DisplayID & (LACE|HIRES|SUPERHIRES)))
   {
      odm->index = index;
      return(0);
   }
   return(1);
}

static int BuildModesList( struct UIKObjDisplayMode *odm )
{
   ULONG current_ID = INVALID_ID;
   DisplayInfoHandle DIHandle;
   struct DispInfos DispInfos;
   ULONG Cross;
   UBYTE *ptr;
   register int i;

   if(!(UIK_nl_AllocList(&odm->NameHead,10))) goto BML_ERROR;
   if(!(UIK_nl_AllocList(&odm->NodeHead,10))) goto BML_ERROR;

   if(odm->SysState & KICK37)
   {
      while((current_ID = NextDisplayInfo(current_ID)) != INVALID_ID)
      {
         DIHandle = FindDisplayInfo(current_ID);
         if(!ModeNotAvailable(current_ID))
         {
            struct NameInfo ni;
            if(GetDisplayInfoData(DIHandle,(UBYTE *)&ni,sizeof(struct NameInfo),DTAG_NAME,NULL))
            {
               odm->insstr = ni.Name;
               Cross = UIK_nl_CrossList(&odm->NameHead,Cross_Find_List,1,(ULONG)odm);
               if(Cross)       /* It is not already in the list */
               {
                  Cross = UIK_nl_CrossList(&odm->NameHead,Cross_Insert_List,1,(ULONG)odm);
                  if(Cross) odm->index = -1; /* *** this one has to be inserted at the end */
                  if(!(ptr = UIK_nl_AllocNode(&odm->NameHead,odm->index,strlen(odm->insstr),0))) goto BML_ERROR;
                  strcpy(ptr,odm->insstr);
                  {
                     struct DisplayInfo di;
                     GetDisplayInfoData(DIHandle,(UBYTE *)&di,sizeof(struct DisplayInfo),DTAG_DISP,NULL);
                     DispInfos.PropertyFlags = di.PropertyFlags;
                     if((odm->SysState & KICK38) == KICK38)
                     {
                        DispInfos.MI_RedBits = di.RedBits;
                        DispInfos.MI_GreenBits = di.GreenBits;
                        DispInfos.MI_BlueBits = di.BlueBits;
                     }
                     else
                     {
                        DispInfos.MI_ColorShades = (ULONG)di.PaletteRange;
                     }
                  }
                  {
                     struct DimensionInfo di;
                     GetDisplayInfoData(DIHandle,(UBYTE *)&di,sizeof(struct DimensionInfo),DTAG_DIMS,NULL);
                     DispInfos.MaxDepth = di.MaxDepth;
                     DispInfos.Nominal.x = di.Nominal.MaxX - di.Nominal.MinX + 1;
                     DispInfos.Nominal.y = di.Nominal.MaxY - di.Nominal.MinY + 1;
                     DispInfos.MaxOScan.x = di.MaxOScan.MaxX - di.MaxOScan.MinX + 1;
                     DispInfos.MaxOScan.y = di.MaxOScan.MaxY - di.MaxOScan.MinY + 1;
                     DispInfos.TxtOScan.x = di.TxtOScan.MaxX - di.TxtOScan.MinX + 1;
                     DispInfos.TxtOScan.y = di.TxtOScan.MaxY - di.TxtOScan.MinY + 1;
                     DispInfos.StdOScan.x = di.StdOScan.MaxX - di.StdOScan.MinX + 1;
                     DispInfos.StdOScan.y = di.StdOScan.MaxY - di.StdOScan.MinY + 1;
                     DispInfos.MaxRasterWidth = di.MaxRasterWidth;
                     DispInfos.MaxRasterHeight = di.MaxRasterHeight;
                  }
                  DispInfos.DisplayID = current_ID;
                  if(!(ptr = UIK_nl_AllocNode(&odm->NodeHead,odm->index,sizeof(struct DispInfos),0))) goto BML_ERROR;
                  CopyMem(&DispInfos,ptr,sizeof(struct DispInfos));
               }
            }
         }
      }
   }
   else /* Running 1.3 or less release */
   {
      struct Disp1_3 *Current;

      for(i = 0; i < 4; i ++)
      {
         if(((struct GfxBase *)odm->GfxBase)->DisplayFlags & PAL)
            Current = &PalModes[i];
         else
            Current = &NtscModes[i];
         odm->insstr = UIK_LangString(odm,Current->Name);
         Cross = UIK_nl_CrossList(&odm->NameHead,Cross_Find_List,1,(ULONG)odm);
         if(Cross)       /* It is not already in the list */
         {
            Cross = UIK_nl_CrossList(&odm->NameHead,Cross_Insert_List,1,(ULONG)odm);
            if(Cross) odm->index = -1; /* *** this one has to be inserted at the end */
            if(!(ptr = UIK_nl_AllocNode(&odm->NameHead,odm->index,strlen(odm->insstr),0))) goto BML_ERROR;
            strcpy(ptr,odm->insstr);
            if(!(ptr = UIK_nl_AllocNode(&odm->NodeHead,odm->index,sizeof(struct DispInfos),0))) goto BML_ERROR;
            CopyMem(&Current->DispInfo,ptr,sizeof(struct DispInfos));
         }
      }
   }

   return(1);
BML_ERROR:
   return(0);
}

static int lnk_collist2datas( struct UIKObjList *ol, struct UIKObjDisplayMode *odm )
{
   struct ColorInfos *ci;

   UIK_LockObjVars( ol );
   ci = (struct ColorInfos *)UIK_nl_NodeBuf( &odm->ColNodeHead, ol->Sel);
   UIK_UnlockObjVars( ol );

   odm->Current.Depth = ci->Depth;
   odm->Current.MI_ModeId = ci->DisplayID;

   return(1);
}

static void Actualise_Gad( struct UIKObjStrGad *ost, ULONG Value)
{
   UBYTE Buffer[30];
   ULONG Len;

   sprintf(Buffer,"%ld",Value);
   Len = strlen(Buffer);
   UIK_OSt_Set( ost, Buffer, Len );
   UIK_LockObjVars(ost);
   ost->CursorPos = Len;
   UIK_UnlockObjVars(ost);
}

static int MinSizeVector( struct UIKObj *useless, struct UIKObjDisplayMode *odm )
{
   struct DispInfos *di = (struct DisplayInfos *)odm->CurrentBuffer;

   odm->Current.Width = di->Nominal.x;
   Actualise_Gad( odm->CustomWidth, odm->Current.Width);
   odm->Current.Height = di->Nominal.y;
   Actualise_Gad( odm->CustomHeight, odm->Current.Height);
   odm->Current.OScanType = MIN_OSCAN;
   UIK_InactivateKeyTank(odm->CustomWidth);
   UIK_InactivateKeyTank(odm->CustomHeight);
   return(1);
}

static int TextSizeVector( struct UIKObj *useless, struct UIKObjDisplayMode *odm )
{
   struct DispInfos *di = (struct DisplayInfos *)odm->CurrentBuffer;

   odm->Current.Width = di->TxtOScan.x;
   Actualise_Gad( odm->CustomWidth, odm->Current.Width);
   odm->Current.Height = di->TxtOScan.y;
   Actualise_Gad( odm->CustomHeight, odm->Current.Height);
   odm->Current.OScanType = TEXT_OSCAN;
   UIK_InactivateKeyTank(odm->CustomWidth);
   UIK_InactivateKeyTank(odm->CustomHeight);
   return(1);
}

static int GfxSizeVector( struct UIKObj *useless, struct UIKObjDisplayMode *odm )
{
   struct DispInfos *di = (struct DisplayInfos *)odm->CurrentBuffer;

   odm->Current.Width = di->StdOScan.x;
   Actualise_Gad( odm->CustomWidth, odm->Current.Width);
   odm->Current.Height = di->StdOScan.y;
   Actualise_Gad( odm->CustomHeight, odm->Current.Height);
   odm->Current.OScanType = GFX_OSCAN;
   UIK_InactivateKeyTank(odm->CustomWidth);
   UIK_InactivateKeyTank(odm->CustomHeight);
   return(1);
}

static int MaxSizeVector( struct UIKObj *useless, struct UIKObjDisplayMode *odm )
{
   struct DispInfos *di = (struct DisplayInfos *)odm->CurrentBuffer;

   odm->Current.Width = di->MaxOScan.x;
   Actualise_Gad( odm->CustomWidth, odm->Current.Width);
   odm->Current.Height = di->MaxOScan.y;
   Actualise_Gad( odm->CustomHeight, odm->Current.Height);
   odm->Current.OScanType = MAX_OSCAN;
   UIK_InactivateKeyTank(odm->CustomWidth);
   UIK_InactivateKeyTank(odm->CustomHeight);
   return(1);
}

static int CustomSizeVector( struct UIKObj *useless, struct UIKObjDisplayMode *odm )
{
   Actualise_Gad( odm->CustomWidth, odm->Current.Width);
   Actualise_Gad( odm->CustomHeight, odm->Current.Height);
   odm->Current.OScanType = CUSTOM_OSCAN;
   UIK_ActivateKeyTank(odm->CustomWidth);
   return(1);
}

static int lnk_list2boxes( struct UIKObjList *ol, struct UIKObjDisplayMode *odm )
{
   struct DispInfos *di;
   UBYTE *str;
   register UWORD i;
   UBYTE buff[60];
   UBYTE *ptr;

   if (ol->Sel == -1 || odm->NodeHead.NodeNum == 0 || odm->NameHead.NodeNum == 0) return(0);

   UIK_LockObjVars( ol );
   di = (struct DispInfos *)UIK_nl_NodeBuf( &odm->NodeHead, ol->Sel);
   strcpy(odm->Current.Name,UIK_nl_NodeBuf( &odm->NameHead, ol->Sel));
   UIK_UnlockObjVars( ol );

   odm->CurrentBuffer = (UBYTE *)di;

   UIK_OLi_NewTextPtr( odm->MPPropList, "" );

   if(odm->SysState & LOCALE)
   {
      str = UIK_LangString( (struct UIKGlobal *)odm, TEXT_OSCANT ); // %...lD remplacé par ld
      sprintf( buff, "%-17.17ls : %6.6ld x %6.6ld", str, di->TxtOScan.x, di->TxtOScan.y);
      UIK_OLi_AddLine( odm->MPPropList, buff, -1, 0 );
      str = UIK_LangString( (struct UIKGlobal *)odm, GFX_OSCANT );  // %...lD remplacé par ld
      sprintf( buff, "%-17.17ls : %6.6ld x %6.6ld", str, di->StdOScan.x, di->StdOScan.y);
      UIK_OLi_AddLine( odm->MPPropList, buff, -1, 0 );
      str = UIK_LangString( (struct UIKGlobal *)odm, MIN_OSCANT );  // %...lD remplacé par ld
      sprintf( buff, "%-17.17ls : %6.6ld x %6.6ld", str, di->Nominal.x, di->Nominal.y);
      UIK_OLi_AddLine( odm->MPPropList, buff, -1, 0 );
      str = UIK_LangString( (struct UIKGlobal *)odm, MAX_OSCANT );  // %...lD remplacé par ld
      sprintf( buff, "%-17.17ls : %6.6ld x %6.6ld", str, di->MaxOScan.x, di->MaxOScan.y);
      UIK_OLi_AddLine( odm->MPPropList, buff, -1, 0 );
      str = UIK_LangString( (struct UIKGlobal *)odm, SHADES );
      if((odm->SysState & KICK38) == KICK38)
      {
         sprintf( buff, "%-17.17ls : %-10.10ld", str, 1 << (di->MI_RedBits + di->MI_GreenBits + di->MI_BlueBits ));
      }
      else
      {
         sprintf( buff, "%-17.17ls : %-10.10ld", str, di->MI_ColorShades );
      }
      UIK_OLi_AddLine( odm->MPPropList, buff, -1, 0 );
      str = UIK_LangString( (struct UIKGlobal *)odm, COLORS );
      sprintf( buff, "%-17.17ls : %-10.10ld", str, 1 << di->MaxDepth);  // %...lD remplacé par ld
      UIK_OLi_AddLine( odm->MPPropList, buff, -1, 0 );
   }
   else
   {
      str = UIK_LangString( (struct UIKGlobal *)odm, TEXT_OSCANT );
      sprintf( buff, "%-17.17ls : %6.6ld x %6.6ld", str, di->TxtOScan.x, di->TxtOScan.y);
      UIK_OLi_AddLine( odm->MPPropList, buff, -1, 0 );
      str = UIK_LangString( (struct UIKGlobal *)odm, GFX_OSCANT );
      sprintf( buff, "%-17.17ls : %6.6ld x %6.6ld", str, di->StdOScan.x, di->StdOScan.y);
      UIK_OLi_AddLine( odm->MPPropList, buff, -1, 0 );
      str = UIK_LangString( (struct UIKGlobal *)odm, MIN_OSCANT );
      sprintf( buff, "%-17.17ls : %6.6ld x %6.6ld", str, di->Nominal.x, di->Nominal.y);
      UIK_OLi_AddLine( odm->MPPropList, buff, -1, 0 );
      str = UIK_LangString( (struct UIKGlobal *)odm, MAX_OSCANT );
      sprintf( buff, "%-17.17ls : %6.6ld x %6.6ld", str, di->MaxOScan.x, di->MaxOScan.y);
      UIK_OLi_AddLine( odm->MPPropList, buff, -1, 0 );
      str = UIK_LangString( (struct UIKGlobal *)odm, SHADES );
      sprintf( buff, "%-17.17ls : %-10.10ld", str, di->MI_ColorShades );
      UIK_OLi_AddLine( odm->MPPropList, buff, -1, 0 );
      str = UIK_LangString( (struct UIKGlobal *)odm, COLORS );
      sprintf( buff, "%-17.17ls : %-10.10ld", str, 1 << di->MaxDepth);
      UIK_OLi_AddLine( odm->MPPropList, buff, -1, 0 );
   }
   if(di->PropertyFlags & DIPF_IS_LACE)
      UIK_OLi_AddLine( odm->MPPropList, UIK_LangString( (struct UIKGlobal *)odm, IS_LACED ), -1, 0 );
   if(di->PropertyFlags & DIPF_IS_ECS)
      UIK_OLi_AddLine( odm->MPPropList, UIK_LangString( (struct UIKGlobal *)odm, IS_ECS ), -1, 0 );
   if(di->PropertyFlags & DIPF_IS_GENLOCK)
      UIK_OLi_AddLine( odm->MPPropList, UIK_LangString( (struct UIKGlobal *)odm, IS_GENLOCK ), -1, 0 );
   if(di->PropertyFlags & DIPF_IS_DRAGGABLE)
      UIK_OLi_AddLine( odm->MPPropList, UIK_LangString( (struct UIKGlobal *)odm, IS_DRAGGABLE ), -1, 0 );

   UIK_LockObjVars(odm->ColObjList);
   UIK_nl_DetachList(&odm->ColObjList->ListText,10);
   UIK_UnlockObjVars(odm->ColObjList);
   UIK_nl_FreeList(&odm->ColNameHead);
   UIK_nl_FreeList(&odm->ColNodeHead);
   if(UIK_nl_AllocList(&odm->ColNameHead,10) && UIK_nl_AllocList(&odm->ColNodeHead,10))
   {
      odm->insstr = buff;
      for( i = 1; i <= di->MaxDepth; i++)
      {
         struct ColorInfos ci;
         /* if(odm->SysState & LOCALE)
            sprintf(buff,"%lD",1 << i);
         else */
            sprintf(buff,"%ld",1 << i);
         if(! (ptr = UIK_nl_AllocNode(&odm->ColNameHead,-1,strlen(buff),0)))
            goto BLIST_ERROR;
         strcpy(ptr,buff);
         ci.Depth = i;
         ci.DisplayID = di->DisplayID;
         if(! (ptr = UIK_nl_AllocNode(&odm->ColNodeHead,-1,sizeof(struct ColorInfos),0)))
            goto BLIST_ERROR;
         CopyMem(&ci,ptr,sizeof(struct ColorInfos));
      }
      /*************************************************************/
      if(odm->SysState & KICK37)
      {
         if(!ModeNotAvailable(di->DisplayID | EXTRAHALFBRITE_KEY)) /* Check if it can be in HAM */
         {
            struct ColorInfos ci;
            sprintf(buff,"EHB");
            if(! (ptr = UIK_nl_AllocNode(&odm->ColNameHead,-1,strlen(buff),0)))
               goto BLIST_ERROR;
            strcpy(ptr,buff);
            ci.Depth = 5;
            ci.DisplayID = di->DisplayID | EXTRAHALFBRITE_KEY;
            if(! (ptr = UIK_nl_AllocNode(&odm->ColNodeHead,-1,sizeof(struct ColorInfos),0)))
               goto BLIST_ERROR;
            CopyMem(&ci,ptr,sizeof(struct ColorInfos));
         }
         if(!ModeNotAvailable(di->DisplayID | HAM_KEY)) /* Check if it can be in HAM */
         {
            struct ColorInfos ci;
            sprintf(buff,"HAM");
            if(! (ptr = UIK_nl_AllocNode(&odm->ColNameHead,-1,strlen(buff),0)))
               goto BLIST_ERROR;
            strcpy(ptr,buff);
            ci.Depth = 4;
            ci.DisplayID = di->DisplayID | HAM_KEY;
            if(! (ptr = UIK_nl_AllocNode(&odm->ColNodeHead,-1,sizeof(struct ColorInfos),0)))
               goto BLIST_ERROR;
            CopyMem(&ci,ptr,sizeof(struct ColorInfos));
         }
      }
      else
      {
         if(!(di->DisplayID & HIRES_KEY))
         {
            struct ColorInfos ci;
            sprintf(buff,"EHB");
            if(! (ptr = UIK_nl_AllocNode(&odm->ColNameHead,-1,strlen(buff),0)))
               goto BLIST_ERROR;
            strcpy(ptr,buff);
            ci.Depth = 5;
            ci.DisplayID = di->DisplayID | EXTRAHALFBRITE_KEY;
            if(! (ptr = UIK_nl_AllocNode(&odm->ColNodeHead,-1,sizeof(struct ColorInfos),0)))
               goto BLIST_ERROR;
            CopyMem(&ci,ptr,sizeof(struct ColorInfos));
            sprintf(buff,"HAM");
            if(! (ptr = UIK_nl_AllocNode(&odm->ColNameHead,-1,strlen(buff),0)))
               goto BLIST_ERROR;
            strcpy(ptr,buff);
            ci.Depth = 4;
            ci.DisplayID = di->DisplayID | HAM_KEY;
            if(! (ptr = UIK_nl_AllocNode(&odm->ColNodeHead,-1,sizeof(struct ColorInfos),0)))
               goto BLIST_ERROR;
            CopyMem(&ci,ptr,sizeof(struct ColorInfos));
         }
      }
      /*******************************************************************/
      UIK_LockObjVars(odm->ColObjList);
      UIK_nl_AttachList(&odm->ColObjList->ListText,&odm->ColNameHead);
      UIK_UnlockObjVars(odm->ColObjList);

      odm->DisplayID = odm->Current.MI_ModeId;
      if(odm->DisplayID != INVALID_ID)
      {
         if(odm->DisplayID & HAM_KEY)
         {
            odm->insstr = "HAM";
         }
         else if(odm->DisplayID & EXTRAHALFBRITE_KEY)
         {
            odm->insstr = "EHB";
         }
         else
         {
            /* if(odm->SysState & LOCALE)
               sprintf(buff,"%lD",1 << odm->Current.Depth);
            else */
               sprintf(buff,"%ld",1 << odm->Current.Depth);
            odm->insstr = buff;
         }
         if(UIK_nl_CrossList(&odm->ColNameHead,Cross_Find_List,1,(ULONG)odm)) odm->index = 0;
      }
      else
      {
         odm->index = 0;
      }
      UIK_OLi_SetListTop(odm->ColObjList,odm->index );
      UIK_OLi_SelectList(odm->ColObjList,odm->index );
      UIK_CauseObjHook(odm->ColObjList,GADGETDOWN);
   }
BLIST_ERROR:
   UIK_OLi_ActualizeList(odm->ColObjList);

   /*******************************************************************/
   if(odm->SysState & KICK37)
   {
      if(!ModeNotAvailable(di->DisplayID | HAM_KEY)) /* Check if it can be in HAM */
      {
         str = UIK_LangString( (struct UIKGlobal *)odm, S_HAM );
         sprintf( buff, "%-30.30ls", str);
         UIK_OLi_AddLine( odm->MPPropList, buff, -1, 0 );
      }
      if(!ModeNotAvailable(di->DisplayID | EXTRAHALFBRITE_KEY)) /* Check also for EHB */
      {
         str = UIK_LangString( (struct UIKGlobal *)odm, S_EXTRA);
         sprintf( buff, "%-30.30ls", str);
         UIK_OLi_AddLine( odm->MPPropList, buff, -1, 0 );
      }
   }
   else
   {
      if(!(di->DisplayID & HIRES_KEY))
      {
         str = UIK_LangString( (struct UIKGlobal *)odm, S_HAM );
         sprintf( buff, "%-30.30ls", str);
         UIK_OLi_AddLine( odm->MPPropList, buff, -1, 0 );
         str = UIK_LangString( (struct UIKGlobal *)odm, S_EXTRA);
         sprintf( buff, "%-30.30ls", str);
         UIK_OLi_AddLine( odm->MPPropList, buff, -1, 0 );
      }
   }
   /*******************************************************************/
   odm->Current.MI_ColorShades = di->MI_ColorShades;

   switch(odm->Current.OScanType)
   {
      case GFX_OSCAN:
         UIK_Select(odm->GfxSize,GADGETDOWN);
         break;
      case MIN_OSCAN:
         UIK_Select(odm->MinSize,GADGETDOWN);
         break;
      case MAX_OSCAN:
         UIK_Select(odm->MaxSize,GADGETDOWN);
         break;
      case TEXT_OSCAN:
         UIK_Select(odm->TextSize,GADGETDOWN);
         break;
      case CUSTOM_OSCAN:
      default:
         UIK_Select(odm->CustomSize,GADGETDOWN);
         break;
   }

   return(1);
}

static int OKFunc( struct UIKObj *useless, struct UIKObjDisplayMode *odm )
{
   (odm->OKFunc)(odm, &odm->Current, odm->UserData);
   return(1);
}
static int CancelFunc( struct UIKObj *useless, struct UIKObjDisplayMode *odm )
{
   (odm->CancelFunc)(odm, odm->UserData);
   return(1);
}

static int __asm StrGadgetHandler( register __a1 UBYTE *buf )
{
   if (*buf < '0' || *buf > '9') return(0);
   return( (int)buf );
}

static void CustomWidthkeys( struct UIKObjStrGad *obj, struct UIKObjDisplayMode *odm )
{
   struct IntuiMessage *imsg = obj->Obj.UIK->IntuiMsg;
   register UWORD code = imsg->Code;

   if (code & 0x80) return;
   switch (code)
   {
      case 0x43:
      case 0x44:
         if ((odm->DispWidth <= ((struct DispInfos *)odm->CurrentBuffer)->MaxRasterWidth) &&
             (odm->DispWidth))
         {
            if(odm->Current.Width != odm->DispWidth)
            {
               UIK_Select(odm->CustomSize,0);
               odm->Current.Width = odm->DispWidth;
            }
            UIK_ActivateKeyTank(odm->CustomHeight);
         }
         else
         {
            DisplayBeep(odm->Obj.w->WScreen);
            UIK_ActivateKeyTank(odm->CustomWidth);
         }
         break;
   }
}

static void CustomHeightkeys( struct UIKObjStrGad *obj, struct UIKObjDisplayMode *odm )
{
   struct IntuiMessage *imsg = obj->Obj.UIK->IntuiMsg;
   register UWORD code = imsg->Code;

   if (code & 0x80) return;
   switch (code)
   {
      case 0x43:
      case 0x44:
         if ((odm->DispHeight <= ((struct DispInfos *)odm->CurrentBuffer)->MaxRasterHeight) &&
             (odm->DispHeight))
         {
            if(odm->Current.Height != odm->DispHeight)
            {
               UIK_Select(odm->CustomSize,0);
               odm->Current.Height = odm->DispHeight;
            }
            UIK_ActivateKeyTank(odm->CustomWidth);
         }
         else
         {
            DisplayBeep(odm->Obj.w->WScreen);
            UIK_ActivateKeyTank(odm->CustomHeight);
         }
         break;
   }
}

static int AddSubObjects( struct UIKObjDisplayMode *odm, struct TagItem *taglist )
{
   WORD  owidth = odm->Obj.Box.Width;
   WORD  oheight = odm->Obj.Box.Height;
   ULONG Size,Pos;

   Size = SETL( owidth - 278, oheight - 97 );
   if(!(odm->ModesObjList = (struct UIKObjList *)UIK_AddObjectTags("UIKObj_List",odm,
                                    UIKTAG_OBJ_LeftTop,              SETL(0,10),
                                    UIKTAG_OBJ_WidthHeight,          Size,
                                    UIKTAG_OBJList_Flags,            UIKL_CLICKSEL|UIKL_SELDISPL,
                                    UIKTAG_OBJ_Title,                MODE_TITLE,
                                    UIKTAG_OBJ_TitleFl_Above,        TRUE,
                                    UIKTAG_OBJ_ParentRelative,       TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Left,   TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Right,  TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Top,    TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Bot,    TRUE,
                                    UIKTAG_OBJ_LangMaster,           (ULONG)odm,
                                    TAG_END))) goto ADDS_ERROR;
   Pos = SETL( owidth - 276, 0);
   Size = SETL( 17, oheight - 116);
   if(!(odm->ModesProp = (struct UIKObjPropGad *)UIK_AddObjectTags("UIKObj_PropLift",odm->ModesObjList,
                                    UIKTAG_OBJ_LeftTop,              Pos,
                                    UIKTAG_OBJ_WidthHeight,          Size,
                                    UIKTAG_OBJProp_Vertical,         TRUE,
                                    UIKTAG_OBJ_BoxFl_Filled,         TRUE,
                                    UIKTAG_OBJ_BoxFl_Over,           FALSE,
                                    UIKTAG_OBJ_Pattern,              (ULONG)Pat1,
                                    UIKTAG_OBJ_PatternPower,         1,
                                    UIKTAG_OBJ_ParentRelative,       TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Right,  TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Top,    TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Bot,    TRUE,
                                    TAG_END))) goto ADDS_ERROR;
   Pos = SETL( 0, oheight - 115);
   if(!(odm->ModesUp = (struct UIKObj *)UIK_AddObjectTags("UIKObj_Up",odm->ModesProp,
                                    UIKTAG_OBJ_LeftTop,              Pos,
                                    UIKTAG_OBJ_WidthHeight,          SETL(17,9),
                                    UIKTAG_OBJ_ParentRelative,       TRUE,
                                    UIKTAG_OBJ_BoxFl_Filled,         TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Right,  TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Bot,    TRUE,
                                    TAG_END))) goto ADDS_ERROR;
   Pos = SETL( 0, oheight - 106);
   if(!(odm->ModesDown = (struct UIKObj *)UIK_AddObjectTags("UIKObj_Down",odm->ModesProp,
                                    UIKTAG_OBJ_LeftTop,              Pos,
                                    UIKTAG_OBJ_WidthHeight,          SETL(17,9),
                                    UIKTAG_OBJ_ParentRelative,       TRUE,
                                    UIKTAG_OBJ_BoxFl_Filled,         TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Right,  TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Bot,    TRUE,
                                    TAG_END))) goto ADDS_ERROR;
   if(!(UIK_Link(odm->ModesProp,odm->ModesObjList,0,MOUSEMOVE|GADGETDOWN,0))) goto ADDS_ERROR;
   if(!(UIK_Link(odm->ModesUp,odm->ModesProp,0,MOUSEMOVE|GADGETDOWN|INTUITICKS,0))) goto ADDS_ERROR;
   if(!(UIK_Link(odm->ModesDown,odm->ModesProp,0,MOUSEMOVE|GADGETDOWN|INTUITICKS,0))) goto ADDS_ERROR;
   Pos = SETL( 0, oheight - 78);
   Size = SETL( owidth - 128, 78);
   if(!(odm->MPPropFrame = UIK_AddObjectTags("UIKObj_Frame",odm,
                                    UIKTAG_OBJ_LeftTop,              Pos,
                                    UIKTAG_OBJ_WidthHeight,          Size,
                                    UIKTAG_OBJ_Title,                FRAME_TITLE,
                                    UIKTAG_OBJ_ParentRelative,       TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Left,   TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Right,  TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Bot,    TRUE,
                                    UIKTAG_OBJ_LangMaster,           (ULONG)odm,
                                    TAG_END))) goto ADDS_ERROR;
   Size = SETL( owidth - 165, 66);
   if(!(odm->MPPropList = (struct UIKObjList *)UIK_AddObjectTags("UIKObj_List",odm->MPPropFrame,
                                    UIKTAG_OBJ_LeftTop,              SETL(10,7),
                                    UIKTAG_OBJ_WidthHeight,          Size,
                                    UIKTAG_OBJ_ParentRelative,       TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Left,   TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Right,  TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Bot,    TRUE,
                                    UIKTAG_OBJ_ActInactPens,         UIKCOL_WHITE<<24 | UIKCOL_WHITE<<16 | UIKCOL_WHITE<<8 | UIKCOL_WHITE ,
                                    TAG_END))) goto ADDS_ERROR;
   Pos = SETL( owidth - 162, 0);
   if(!(odm->MPPropProp = (struct UIKObjPropGad *)UIK_AddObjectTags("UIKObj_PropLift",odm->MPPropList,
                                    UIKTAG_OBJ_LeftTop,              Pos,
                                    UIKTAG_OBJ_WidthHeight,          SETL(17,47),
                                    UIKTAG_OBJProp_Vertical,         TRUE,
                                    UIKTAG_OBJ_BoxFl_Filled,         TRUE,
                                    UIKTAG_OBJ_BoxFl_Over,           FALSE,
                                    UIKTAG_OBJ_Pattern,              (ULONG)Pat1,
                                    UIKTAG_OBJ_PatternPower,         1,
                                    UIKTAG_OBJ_Sz_AttachedFl_Right,  TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Bot,    TRUE,
                                    UIKTAG_OBJ_ParentRelative,       TRUE,
                                    TAG_END))) goto ADDS_ERROR;
   if(!(odm->MPPropUp = (struct UIKObj *)UIK_AddObjectTags("UIKObj_Up",odm->MPPropProp,
                                    UIKTAG_OBJ_LeftTop,              SETL(0,48),
                                    UIKTAG_OBJ_WidthHeight,          SETL(17,9),
                                    UIKTAG_OBJ_ParentRelative,       TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Right,  TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Bot,    TRUE,
                                    UIKTAG_OBJ_BoxFl_Filled,         TRUE,
                                    TAG_END))) goto ADDS_ERROR;
   if(!(odm->MPPropDown = (struct UIKObj *)UIK_AddObjectTags("UIKObj_Down",odm->MPPropProp,
                                    UIKTAG_OBJ_LeftTop,              SETL(0,57),
                                    UIKTAG_OBJ_WidthHeight,          SETL(17,9),
                                    UIKTAG_OBJ_ParentRelative,       TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Right,  TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Bot,    TRUE,
                                    UIKTAG_OBJ_BoxFl_Filled,         TRUE,
                                    TAG_END))) goto ADDS_ERROR;
   if(!(UIK_Link(odm->MPPropProp,odm->MPPropList,0,MOUSEMOVE|GADGETDOWN,0))) goto ADDS_ERROR;
   if(!(UIK_Link(odm->MPPropUp,odm->MPPropProp,0,MOUSEMOVE|GADGETDOWN|INTUITICKS,0))) goto ADDS_ERROR;
   if(!(UIK_Link(odm->MPPropDown,odm->MPPropProp,0,MOUSEMOVE|GADGETDOWN|INTUITICKS,0))) goto ADDS_ERROR;
   Pos = SETL( owidth - 118, oheight - 12);
   if(!(odm->ObjOK = (struct UIKObjGad *)UIK_AddObjectTags("UIKObj_OK",odm,
                                    UIKTAG_OBJ_LeftTop,              Pos,
                                    UIKTAG_OBJ_ParentRelative,       TRUE,
                                    UIKTAG_OBJ_BoxFl_Filled,         TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Right,  TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Bot,    TRUE,
                                    TAG_END))) goto ADDS_ERROR;
   Pos = SETL( owidth - 40, oheight - 12);
   if(!(odm->ObjCancel = (struct UIKObjGad *)UIK_AddObjectTags("UIKObj_Cancel",odm,
                                    UIKTAG_OBJ_LeftTop,              Pos,
                                    UIKTAG_OBJ_ParentRelative,       TRUE,
                                    UIKTAG_OBJ_BoxFl_Filled,         TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Right,  TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Bot,    TRUE,
                                    TAG_END))) goto ADDS_ERROR;
   Pos = SETL( owidth - 247, 10);
   Size = SETL( 100, oheight - 97 );
   if(!(odm->ColObjList = (struct UIKObjList *)UIK_AddObjectTags("UIKObj_List",odm,
                                    UIKTAG_OBJ_LeftTop,              Pos,
                                    UIKTAG_OBJ_WidthHeight,          Size,
                                    UIKTAG_OBJList_Flags,            UIKL_CLICKSEL|UIKL_SELDISPL,
                                    UIKTAG_OBJ_Title,                COLOR_TITLE,
                                    UIKTAG_OBJ_TitleFl_Above,        TRUE,
                                    UIKTAG_OBJ_ParentRelative,       TRUE,
                                    UIKTAG_OBJ_LangMaster,           (ULONG)odm,
                                    UIKTAG_OBJ_Sz_AttachedFl_Right,  TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Top,    TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Bot,    TRUE,
                                    TAG_END))) goto ADDS_ERROR;
   Size = SETL( 17, oheight - 116);
   if(!(odm->ColProp = (struct UIKObjPropGad *)UIK_AddObjectTags("UIKObj_PropLift",odm->ColObjList,
                                    UIKTAG_OBJ_LeftTop,              SETL(102,0),
                                    UIKTAG_OBJ_WidthHeight,          Size,
                                    UIKTAG_OBJProp_Vertical,         TRUE,
                                    UIKTAG_OBJ_BoxFl_Filled,         TRUE,
                                    UIKTAG_OBJ_BoxFl_Over,           FALSE,
                                    UIKTAG_OBJ_Pattern,              (ULONG)Pat1,
                                    UIKTAG_OBJ_PatternPower,         1,
                                    UIKTAG_OBJ_ParentRelative,       TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Right,  TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Top,    TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Bot,    TRUE,
                                    TAG_END))) goto ADDS_ERROR;
   Pos = SETL( 0, oheight - 115);
   if(!(odm->ColUp = (struct UIKObj *)UIK_AddObjectTags("UIKObj_Up",odm->ColProp,
                                    UIKTAG_OBJ_LeftTop,              Pos,
                                    UIKTAG_OBJ_WidthHeight,          SETL(17,9),
                                    UIKTAG_OBJ_BoxFl_Filled,         TRUE,
                                    UIKTAG_OBJ_ParentRelative,       TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Right,  TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Bot,    TRUE,
                                    TAG_END))) goto ADDS_ERROR;
   Pos = SETL( 0, oheight - 106);
   if(!(odm->ColDown = (struct UIKObj *)UIK_AddObjectTags("UIKObj_Down",odm->ColProp,
                                    UIKTAG_OBJ_LeftTop,              Pos,
                                    UIKTAG_OBJ_WidthHeight,          SETL(17,9),
                                    UIKTAG_OBJ_BoxFl_Filled,         TRUE,
                                    UIKTAG_OBJ_ParentRelative,       TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Right,  TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Bot,    TRUE,
                                    TAG_END))) goto ADDS_ERROR;
   if(!(UIK_Link(odm->ColProp,odm->ColObjList,0,MOUSEMOVE|GADGETDOWN,0))) goto ADDS_ERROR;
   if(!(UIK_Link(odm->ColUp,odm->ColProp,0,MOUSEMOVE|GADGETDOWN|INTUITICKS,0))) goto ADDS_ERROR;
   if(!(UIK_Link(odm->ColDown,odm->ColProp,0,MOUSEMOVE|GADGETDOWN|INTUITICKS,0))) goto ADDS_ERROR;
   Pos = SETL( owidth - 118, 53);
   if(!(odm->MinSize = (struct UIKObjGad *)UIK_AddObjectTags("UIKObj_MutualExclude",odm,
                                    UIKTAG_OBJ_LeftTop,              Pos,
                                    UIKTAG_OBJ_WidthHeight,          SETL(118,15),
                                    UIKTAG_OBJ_ParentRelative,       TRUE,
                                    UIKTAG_OBJ_ActInactPens,         UIKCOLS(UIKCOL_GREY,UIKCOL_GREY,
                                                                             UIKCOL_WHITE, UIKCOL_BLACK ),
                                    UIKTAG_OBJ_Title,                MIN_OSCANT,
                                    UIKTAG_OBJ_LangMaster,           (ULONG)odm,
                                    UIKTAG_OBJ_BoxFl_Filled,         TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Right,  TRUE,
                                    TAG_END))) goto ADDS_ERROR;
   Pos = SETL( owidth - 118, 71);
   if(!(odm->TextSize = (struct UIKObjGad *)UIK_AddObjectTags("UIKObj_MutualExclude",odm,
                                    UIKTAG_OBJ_LeftTop,              Pos,
                                    UIKTAG_OBJ_WidthHeight,          SETL(118,15),
                                    UIKTAG_OBJ_ParentRelative,       TRUE,
                                    UIKTAG_OBJ_ActInactPens,         UIKCOLS(UIKCOL_GREY,UIKCOL_GREY,
                                                                             UIKCOL_WHITE, UIKCOL_BLACK ),
                                    UIKTAG_OBJ_Title,                TEXT_OSCANT,
                                    UIKTAG_OBJ_LangMaster,           (ULONG)odm,
                                    UIKTAG_OBJ_BoxFl_Filled,         TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Right,  TRUE,
                                    TAG_END))) goto ADDS_ERROR;
   Pos = SETL( owidth - 118, 89);
   if(!(odm->GfxSize = (struct UIKObjGad *)UIK_AddObjectTags("UIKObj_MutualExclude",odm,
                                    UIKTAG_OBJ_LeftTop,              Pos,
                                    UIKTAG_OBJ_WidthHeight,          SETL(118,15),
                                    UIKTAG_OBJ_ParentRelative,       TRUE,
                                    UIKTAG_OBJ_ActInactPens,         UIKCOLS(UIKCOL_GREY,UIKCOL_GREY,
                                                                             UIKCOL_WHITE, UIKCOL_BLACK ),
                                    UIKTAG_OBJ_Title,                GFX_OSCANT,
                                    UIKTAG_OBJ_LangMaster,           (ULONG)odm,
                                    UIKTAG_OBJ_BoxFl_Filled,         TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Right,  TRUE,
                                    TAG_END))) goto ADDS_ERROR;
   Pos = SETL( owidth - 118, 107);
   if(!(odm->MaxSize = (struct UIKObjGad *)UIK_AddObjectTags("UIKObj_MutualExclude",odm,
                                    UIKTAG_OBJ_LeftTop,              Pos,
                                    UIKTAG_OBJ_WidthHeight,          SETL(118,15),
                                    UIKTAG_OBJ_ParentRelative,       TRUE,
                                    UIKTAG_OBJ_ActInactPens,         UIKCOLS(UIKCOL_GREY,UIKCOL_GREY,
                                                                             UIKCOL_WHITE, UIKCOL_BLACK ),
                                    UIKTAG_OBJ_Title,                MAX_OSCANT,
                                    UIKTAG_OBJ_LangMaster,           (ULONG)odm,
                                    UIKTAG_OBJ_BoxFl_Filled,         TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Right,  TRUE,
                                    TAG_END))) goto ADDS_ERROR;
   Pos = SETL( owidth - 118, 125);
   if(!(odm->CustomSize = (struct UIKObjGad *)UIK_AddObjectTags("UIKObj_MutualExclude",odm,
                                    UIKTAG_OBJ_LeftTop,              Pos,
                                    UIKTAG_OBJ_WidthHeight,          SETL(118,15),
                                    UIKTAG_OBJ_ParentRelative,       TRUE,
                                    UIKTAG_OBJ_ActInactPens,         UIKCOLS(UIKCOL_GREY,UIKCOL_GREY,
                                                                             UIKCOL_WHITE, UIKCOL_BLACK ),
                                    UIKTAG_OBJ_Title,                CUSTOM_SIZE,
                                    UIKTAG_OBJ_LangMaster,           (ULONG)odm,
                                    UIKTAG_OBJ_BoxFl_Filled,         TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Right,  TRUE,
                                    TAG_END))) goto ADDS_ERROR;
   Pos = SETL( owidth - 49, 10);
   if(!(odm->CustomWidth = (struct UIKObjStrGad *)UIK_AddObjectTags("UIKObj_String",odm,
                                    UIKTAG_OBJ_LeftTop,              Pos,
                                    UIKTAG_OBJ_WidthHeight,          SETL(49,15),
                                    UIKTAG_OBJ_ParentRelative,       TRUE,
                                    UIKTAG_OBJ_TitleFl_Left,         TRUE,
                                    UIKTAG_OBJStr_MaxChars,          10,
                                    UIKTAG_OBJ_Title,                WIDTH_TITLE,
                                    UIKTAG_OBJStr_UserHandler,       (ULONG)StrGadgetHandler,
                                    UIKTAG_OBJStr_ResultPointer,     (ULONG)&odm->DispWidth,
                                    UIKTAG_OBJ_LangMaster,           (ULONG)odm,
                                    UIKTAG_OBJ_Sz_AttachedFl_Right,  TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Top,    TRUE,
                                    TAG_END))) goto ADDS_ERROR;
   Pos = SETL( owidth - 49, 31);
   if(!(odm->CustomHeight = (struct UIKObjStrGad *)UIK_AddObjectTags("UIKObj_String",odm,
                                    UIKTAG_OBJ_LeftTop,              Pos,
                                    UIKTAG_OBJ_WidthHeight,          SETL(49,15),
                                    UIKTAG_OBJ_ParentRelative,       TRUE,
                                    UIKTAG_OBJ_TitleFl_Left,         TRUE,
                                    UIKTAG_OBJStr_MaxChars,          29,
                                    UIKTAG_OBJ_Title,                HEIGHT_TITLE,
                                    UIKTAG_OBJStr_UserHandler,       (ULONG)StrGadgetHandler,
                                    UIKTAG_OBJStr_ResultPointer,     (ULONG)&odm->DispHeight,
                                    UIKTAG_OBJ_LangMaster,           (ULONG)odm,
                                    UIKTAG_OBJ_Sz_AttachedFl_Right,  TRUE,
                                    UIKTAG_OBJ_Sz_AttachedFl_Top,    TRUE,
                                    TAG_END))) goto ADDS_ERROR;

   if (!UIK_AddVector(odm->CustomWidth,(void *)CustomWidthkeys,RAWKEY,(ULONG)odm)) goto ADDS_ERROR;
   if (!UIK_AddVector(odm->CustomHeight,(void *)CustomHeightkeys,RAWKEY,(ULONG)odm)) goto ADDS_ERROR;
   if (! (UIK_AddVector( odm->ObjOK, (void *)OKFunc, GADGETUP, (ULONG)odm ))) goto ADDS_ERROR;
   if (! (UIK_AddVector( odm->ObjCancel, (void *)CancelFunc, GADGETUP, (ULONG)odm ))) goto ADDS_ERROR;
   if (! (UIK_AddVector( odm->ModesObjList, (void *)lnk_list2boxes, GADGETDOWN, (ULONG)odm ))) goto ADDS_ERROR;
   if (! (UIK_AddVector( odm->ColObjList, (void *)lnk_collist2datas, GADGETDOWN, (ULONG)odm ))) goto ADDS_ERROR;
   if (! (UIK_AddVector( odm->MinSize, (void *)MinSizeVector, GADGETDOWN, (ULONG)odm ))) goto ADDS_ERROR;
   if (! (UIK_AddVector( odm->TextSize, (void *)TextSizeVector, GADGETDOWN, (ULONG)odm ))) goto ADDS_ERROR;
   if (! (UIK_AddVector( odm->GfxSize, (void *)GfxSizeVector, GADGETDOWN, (ULONG)odm ))) goto ADDS_ERROR;
   if (! (UIK_AddVector( odm->MaxSize, (void *)MaxSizeVector, GADGETDOWN, (ULONG)odm ))) goto ADDS_ERROR;
   if (! (UIK_AddVector( odm->CustomSize, (void *)CustomSizeVector, GADGETDOWN, (ULONG)odm ))) goto ADDS_ERROR;

   return(1);
ADDS_ERROR:
   return(0);
}

/* --- Standard object code ------------------------------------------------------------------------------------- */

static int AddDisplayMode( struct UIKObjDisplayMode *odm, struct TagItem *taglist )
{
   struct Library *Dummy;

   SysBase = (*(ULONG *)4);
   if (! (odm->IntuitionBase = IntuitionBase = (struct IntuitionBase *) OpenLibrary( "intuition.library",0))) goto ADD_ERROR;
   if (! (odm->GfxBase = GfxBase = (struct GfxBase *) OpenLibrary( "graphics.library",0))) goto ADD_ERROR;
   if (! (odm->DOSBase = DOSBase = (struct DOSBase *) OpenLibrary( DOSNAME, 0 ))) goto ADD_ERROR;
   if (! (odm->UIKBase = UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0))) goto ADD_ERROR;

   if(Dummy = OpenLibrary("locale.library",0L))
   {
      odm->SysState |= LOCALE;
      CloseLibrary(Dummy);
   }
   if(((struct ExecBase *)SysBase)->LibNode.lib_Version < 37) odm->SysState |= OLDKICK;
   else if(((struct ExecBase *)SysBase)->LibNode.lib_Version < 38) odm->SysState |= KICK37;
   else odm->SysState |= KICK38;


   odm->OKFunc = (void *)GetTagData( UIKTAG_ObjDM_OKFunc, NULL, taglist);
   if(!(odm->OKFunc)) goto ADD_ERROR;
   odm->CancelFunc = (void *)GetTagData (UIKTAG_ObjDM_CancelFunc, NULL, taglist);
   if(!(odm->CancelFunc)) goto ADD_ERROR;
   odm->UserData = GetTagData (UIKTAG_ObjDM_UserData, 0, taglist);
   odm->MIUser = (struct ModeInfo *)GetTagData (UIKTAG_ObjDM_ModeInfo, NULL, taglist);
   if(odm->MIUser) CopyMem(odm->MIUser,&odm->Current,sizeof(struct ModeInfo));
   else
   {
      if(UIK_IsSystemVersion2())
      {
         odm->Current.MI_Modes = odm->Obj.w->WScreen->ViewPort.Modes;
      }
      else
      {
         odm->Current.MI_Modes = odm->Obj.w->WScreen->ViewPort.Modes;
      }
      odm->Current.OScanType = MIN_OSCAN;
   }
   if(!(odm->Current.Name = UIK_MemSpAlloc(DISPLAYNAMELEN,MEMF_ANY|MEMF_CLEAR))) goto ADD_ERROR;
   if(! (AddSubObjects( odm, taglist ))) goto ADD_ERROR;
   return(1);

ADD_ERROR:
   return(0);
}

static int StartDisplayMode( struct UIKObjDisplayMode *odm )
{
   if(!(odm->WidthBuff = (UBYTE *)AllocMem(30,MEMF_CLEAR|MEMF_PUBLIC))) goto START_ERROR;
   if(!(odm->HeightBuff = (UBYTE *)AllocMem(30,MEMF_CLEAR|MEMF_PUBLIC))) goto START_ERROR;

   return(1);
START_ERROR:
   return(0);
}

static int StartAfterDisplayMode( struct UIKObjDisplayMode *odm )
{
   struct NameInfo ni;

   if (! BuildModesList( odm )) goto SA_ERROR;
   odm->DisplayID = odm->Current.MI_ModeId;
   if(odm->DisplayID != INVALID_ID)
   {
      if(odm->DisplayID & HAM_KEY) odm->DisplayID &= ~HAM_KEY;
      if(odm->DisplayID & EXTRAHALFBRITE_KEY) odm->DisplayID &= ~EXTRAHALFBRITE_KEY;
      if(odm->SysState & KICK37)
      {
         GetDisplayInfoData(NULL,(UBYTE *)&ni,sizeof(struct NameInfo),DTAG_NAME,odm->DisplayID);
         odm->insstr = ni.Name;
         UIK_nl_CrossList(&odm->NameHead,Cross_Find_List,1,(ULONG)odm);
      }
      else
      {
         if(UIK_nl_CrossList(&odm->NodeHead,Cross_Find_List_By_ModeID,1,(ULONG)odm))
            odm->index = 0;
      }
   }
   else
   {
      odm->index = 0;
   }
   UIK_LockObjVars(odm->ModesObjList);
   UIK_nl_AttachList(&odm->ModesObjList->ListText,&odm->NameHead);
   UIK_UnlockObjVars(odm->ModesObjList);
   UIK_OLi_ActualizeList(odm->ModesObjList);
   UIK_OLi_SetListTop(odm->ModesObjList,odm->index );
   UIK_OLi_SelectList(odm->ModesObjList,odm->index );
   UIK_CauseObjHook(odm->ModesObjList,GADGETDOWN);

   return(1);
SA_ERROR:
   return(0);
}

static void StopDisplayMode( struct UIKObjDisplayMode *odm )
{
   if(odm->WidthBuff) FreeMem((void *)odm->WidthBuff,30);
   if(odm->HeightBuff) FreeMem((void *)odm->HeightBuff,30);
   UIK_LockObjVars(odm->ColObjList);
   UIK_nl_DetachList(&odm->ColObjList->ListText,10);
   UIK_UnlockObjVars(odm->ColObjList);
   UIK_nl_FreeList(&odm->ColNameHead);
   UIK_nl_FreeList(&odm->ColNodeHead);
   UIK_LockObjVars(odm->ModesObjList);
   UIK_nl_DetachList(&odm->ModesObjList->ListText,10);
   UIK_UnlockObjVars(odm->ModesObjList);
   UIK_nl_FreeList(&odm->NameHead);
   UIK_nl_FreeList(&odm->NodeHead);
}

static void RemoveDisplayMode( struct UIKObjDisplayMode *odm )
{
   if (odm->Current.Name) UIK_MemSpFree(odm->Current.Name);

   if (odm->UIKBase)   CloseLibrary(odm->UIKBase);
   if (odm->DOSBase)   CloseLibrary(odm->DOSBase);
   if (odm->GfxBase)   CloseLibrary(odm->GfxBase);
   if (odm->IntuitionBase) CloseLibrary(odm->IntuitionBase);
}

/* --- FuncTable ------------------------------------------------------------------------------------------------ */
/* --- Germ ----------------------------------------------------------------------------------------------------- */

static struct TagItem __far GermTagList[] =
{
   { UIKTAG_GEN_VersionRevision, W2L(1,4) },
   { UIKTAG_GEN_LangEnglish,     (ULONG)LangEnglish },
   { UIKTAG_GEN_LangOther,       (ULONG)LangOther },
   { UIKTAG_GEN_Catalog,         (ULONG)"UIK/Objects/UIKObj_DisplayMode.catalog" },
   { UIKTAG_GEN_LastFuncId,      0 },
   { TAG_END }
};

struct UIKObjGerm __far Germ =
{
   "UIKObj_DisplayMode",
   AddDisplayMode,     /*int     (*AddVector)(); */
   StartDisplayMode,   /*int     (*StartVector)(); */
   StopDisplayMode,    /*void    (*StopVector)(); */
   RemoveDisplayMode,  /*void    (*RemoveVector)(); */
   NULL,               /*void    (*RefreshVector)(); */
   NULL,               /*void    (*ResizeVector)(); */
   NULL,               /*void    (*ExceptActVector)(); */
   NULL,               /*void    (*ExceptInaVector)(); */
   NULL,               /*void    (*ExceptMovVector)(); */
   NULL,               /*void    (*ExceptKeyVector)(); */
   StartAfterDisplayMode,/*int     (*StartAfterVector)(); */
   NULL,               /*int     (*SetGetValues)(); */
   NULL,               /*ULONG   *ExtentedVectors; */
   GermTagList,        /*ULONG   ExReserved[1]; */
   480,                /*UWORD   MinWidth; */
   155,                /*UWORD   MinHeight; */
   SETL(554,155),      /*ULONG DefaultWidthHeight; */
   UIKID_DISPLAYMODE,         /*UWORD   ObjId; */
   sizeof(struct UIKObjDisplayMode),   /*UWORD   ObjSize; */
   BOX_SEMAPHORE,
};
