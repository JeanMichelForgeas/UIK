/******************************************************************************************************************
 *                                                                                                                *
 *  File : DisplayMode.h                                                                                          *
 *  Project : Display mode selector.                                                                              *
 *                                                                                                                *
 *  Created : 16/07/92 ©1992 Frédéric DENIS "ß release" Software Design.                                          *
 *                                                                                                                *
 *  All rights reserved.                                                                                          *
 *                                                                                                                *
 ******************************************************************************************************************/

/*
 un tag si dbl clck --> OK.
 touches fleches bouger ligne et shift couleurs alt -> props.
 equiv clavier ovscans.
*/

#ifndef UIKDEFS_DISPLAYMODE

#define UIKDEFS_DISPLAYMODE

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

#ifndef UIKDEFS_OBJLIST
#include "uiki:objlist.h"
#endif

struct ModeInfo
{
    union {
          ULONG ModeId;
          struct {
                 UWORD pad;
                 UWORD Modes;
                 } m13;
          } vpm;                       /* Selected mode's ModeID/DisplayModes */
    UWORD            Depth;            /* Selected mode's Depth */
    UWORD            Width;            /* Selected mode's Width */
    UWORD            Height;           /*                 Height */
    UWORD            OScanType;        /* type of overscan, see defines. */
    union {
          ULONG      ColorShades;      /* # of different colors in the palette usable by this mode */
          struct {
                 UBYTE  RedBits;
                 UBYTE  GreenBits;
                 UBYTE  BlueBits;
                 UBYTE  pad;
                 } cbits;
          } colshade;
    UBYTE            *Name;             /* Name of the mode. */
    ULONG            Reserved;          /* For future use, set this to 0 */
};

#define MI_ColorShades colshade.ColorShades
#define MI_RedBits     colshade.cbits.RedBits
#define MI_GreenBits   colshade.cbits.GreenBits
#define MI_BlueBits    colshade.cbits.BlueBits
#define MI_ModeId      vpm.ModeId
#define MI_Modes       vpm.m13.Modes

/* Overscan types : */
#define CUSTOM_OSCAN 0
#define TEXT_OSCAN   1
#define GFX_OSCAN    2
#define MIN_OSCAN    3
#define MAX_OSCAN    4

struct UIKObjDisplayMode
{
   struct UIKObj           Obj;

   struct Library          *DOSBase;
   struct Library          *GfxBase;
   struct Library          *IntuitionBase;
   struct Library          *UIKBase;
   void                    (*OKFunc)();
   void                    (*CancelFunc)();
   ULONG                   SysState;

   struct UIKList          NodeHead;
   struct UIKList          NameHead;
   struct UIKList          ColNodeHead;
   struct UIKList          ColNameHead;
   struct UIKObjList       *ModesObjList;
   struct UIKObjList       *ColObjList;
   struct UIKObj           *ModesUp;
   struct UIKObj           *ModesDown;
   struct UIKObjPropGad    *ModesProp;
   struct UIKObj           *MPPropFrame;
   struct UIKObjList       *MPPropList;
   struct UIKObjPropGad    *MPPropProp;
   struct UIKObj           *MPPropUp;
   struct UIKObj           *MPPropDown;
   struct UIKObj           *ColUp;
   struct UIKObj           *ColDown;
   struct UIKObjPropGad    *ColProp;
   struct UIKObjGad        *ObjOK;
   struct UIKObjGad        *ObjCancel;
   struct UIKObjGad        *MinSize;
   struct UIKObjGad        *TextSize;
   struct UIKObjGad        *GfxSize;
   struct UIKObjGad        *MaxSize;
   struct UIKObjGad        *CustomSize;
   struct UIKObjStrGad     *CustomWidth;
   struct UIKObjStrGad     *CustomHeight;
   struct UIKHook          *ResizeNewSizeHook;

   struct ModeInfo         Current;
   struct ModeInfo         *MIUser;

   UBYTE                   *insstr;
   UBYTE                   *CurrentBuffer;
   UBYTE                   *WidthBuff;
   UBYTE                   *HeightBuff;
   ULONG                   DispWidth;
   ULONG                   DispHeight;
   ULONG                   index;
   ULONG                   DisplayID;
   ULONG                   UserData;
};

/* The ModeInfo structure passed as the second parameter of the OKFunc is valid only during
   the abovesaid OKFunc, and as long as the DisplayMode object isn't UIK_Remove()'ed */

#define UIKTAG_ObjDM_OKFunc         (UIKTAG_USER|0x0101)   /* Fonction apellee si OK */
#define UIKTAG_ObjDM_CancelFunc     (UIKTAG_USER|0x0102)   /* idem si Cancel */
#define UIKTAG_ObjDM_ModeInfo       (UIKTAG_USER|0x0103)   /* struct ModeInfo utilisee comme defaut */
#define UIKTAG_ObjDM_UserData       (UIKTAG_USER|0x0104)   /* au choix, passe en dernier argument de ok ou cancel */
#define UIKTAG_ObjDM_ModeCantBe     (UIKTAG_USER|0x0105)   /* reservé pour plus tard... */
#define UIKTAG_ObjDM_ModeMustBe     (UIKTAG_USER|0x0106)   /* reservé pour plus tard... */
#define UIKTAG_ObjDM_ModeMinSize    (UIKTAG_USER|0x0107)   /* reservé pour plus tard... */
#define UIKTAG_ObjDM_ModeMaxSize    (UIKTAG_USER|0x0108)   /* reservé pour plus tard... */

#endif