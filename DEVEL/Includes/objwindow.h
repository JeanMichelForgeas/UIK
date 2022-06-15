#ifndef UIKDEFS_OBJWINDOW

#define UIKDEFS_OBJWINDOW
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

struct UIKWinScrInfo
    {
    ULONG           ModeId;
    WORD            Left, Top, Width, Height;
    UBYTE           Depth;
    UBYTE           Padb;
    UWORD           Padw;
    struct TextFont *Font;
    struct Screen   *Scr;
    ULONG           Reserved[10];
    };

struct UIKObjWindow
    {
    struct UIKObj       Obj;
    struct ExtNewWindow NW;
    struct UIKObjBox    *TitleBox;
    struct UIKObjWindow *UniconW;
    ULONG               WinFlags;
    ULONG               IconLeftTop;
    struct TextFont     *WFont;
    UWORD               TopBorderHeight;
    UWORD               BottomBorderHeight;
    UWORD               LeftBorderWidth;
    UWORD               RightBorderWidth;
    UWORD               OldLeft;
    UWORD               OldTop;
    UWORD               OldWidth;
    UWORD               OldHeight;
    struct BitMap       *CustomBitMap;
    struct BitMap       *CustomAltBitMap;
    struct UIKObjMenu   *WMenu;
    ULONG               IconColors;
    struct Gadget       *GadgetList;
    struct UIKList      ShortCutList;
    struct TagItem      WTagList[3];
    struct DrawInfo     *DrawInfo;
    struct Screen       *ScrAdd;
    WORD                ShrunkWidth;
    WORD                ShrunkHeight;
    };

/*------window functions------*/
#define UIKFUNC_Window_Zip              0
#define UIKFUNC_Window_ChangeTitle      4
#define UIKFUNC_Window_GetWinScrInfo    8

#define UIKFUNC_Window_LastCmd          8

/*------window flags------*/
#define UIKW_TITLEFLAGS        0x003d

#define UIKW_DEPTH              0x0001
#define UIKW_SIZE               0x0002
#define UIKW_CLOSE              0x0004
#define UIKW_ZOOM               0x0008
#define UIKW_ICON               0x0010
#define UIKW_DRAG               0x0020
#define UIKW_SCREENLOCKED       0x0040
#define UIKW_ICONWINDOW         0x0080
#define UIKW_AUTOADJUSTSIZE     0x0100
#define UIKW_BITMAPED           0x0200
#define UIKW_AUTOADJUSTLEFTTOP  0x0400
#define UIKW_NODEFSCREEN        0x0800
#define UIKW_NOPERIMETER        0x1000
#define UIKW_DEFSCREENONLY      0x2000
#define UIKW_BACKDROP           0x4000
#define UIKW_MENULOOK           0x8000
#define UIKW_SYSTEMLOOK         0x00010000
#define UIKW_CLOSESTOP          0x00020000
#define UIKW_CLOSEREMOVE        0x00040000
#define UIKW_NOFILL             0x00080000
#define UIKW_SIMPLEREFRESH      0x00100000
#define UIKW_STARTINACTIVE      0x00200000
#define UIKW_FULLWIDTH          0x00400000
#define UIKW_FULLHEIGHT         0x00800000
#define UIKW_NEARFULL           0x01000000
#define UIKW_FULLDISPL          0x02000000
#define UIKW_AUTOSHRINK         0x04000000

/*----UIKObjWindow values Tags---- */
#define UIKTAG_OBJWindow_MinimumWidthHeight (UIKTAG_USER|0x0001)
#define UIKTAG_OBJWindow_MaximumWidthHeight (UIKTAG_USER|0x0002)
#define UIKTAG_OBJWindow_CustomBitMap       (UIKTAG_USER|0x0003)
#define UIKTAG_OBJWindow_CustomAltBitMap    (UIKTAG_USER|0x0004)
#define UIKTAG_OBJWindow_Depth_ShortKey     (UIKTAG_USER|0x0005)
#define UIKTAG_OBJWindow_Close_ShortKey     (UIKTAG_USER|0x0006)
#define UIKTAG_OBJWindow_Zoom_ShortKey      (UIKTAG_USER|0x0007)
#define UIKTAG_OBJWindow_Icon_ShortKey      (UIKTAG_USER|0x0008)
#define UIKTAG_OBJWindow_ScreenAddr         (UIKTAG_USER|0x0009)
#define UIKTAG_OBJWindow_PubScreenName      (UIKTAG_USER|0x000a)

/*----UIKObjWindow WinFlags Tags---- */
#define UIKTAG_OBJWindow_Flags               (UIKTAG_USER|0x0020) /* TRUE */
#define UIKTAG_OBJWindowFl_IconWindow        (UIKTAG_USER|0x0021) /* FALSE */
#define UIKTAG_OBJWindowFl_With_Depth        (UIKTAG_USER|0x0022) /* TRUE */
#define UIKTAG_OBJWindowFl_With_Size         (UIKTAG_USER|0x0023) /* TRUE */
#define UIKTAG_OBJWindowFl_With_Close        (UIKTAG_USER|0x0024) /* TRUE */
#define UIKTAG_OBJWindowFl_With_Zoom         (UIKTAG_USER|0x0025) /* TRUE */
#define UIKTAG_OBJWindowFl_With_Icon         (UIKTAG_USER|0x0026) /* TRUE */
#define UIKTAG_OBJWindowFl_With_Drag         (UIKTAG_USER|0x0027) /* TRUE */
#define UIKTAG_OBJWindowFl_AutoAdjustSize    (UIKTAG_USER|0x0028) /* TRUE */
#define UIKTAG_OBJWindowFl_AutoAdjustLeftTop (UIKTAG_USER|0x0029) /* TRUE */
#define UIKTAG_OBJWindowFl_NoDefScreen       (UIKTAG_USER|0x002a) /* FALSE */
#define UIKTAG_OBJWindowFl_NoBlackPerimeter  (UIKTAG_USER|0x002b) /* FALSE */
#define UIKTAG_OBJWindowFl_DefScreenOnly     (UIKTAG_USER|0x002c) /* FALSE */
#define UIKTAG_OBJWindowFl_Backdrop          (UIKTAG_USER|0x002d) /* FALSE */
#define UIKTAG_OBJWindowFl_NewLookMenu3      (UIKTAG_USER|0x002e) /* FALSE */
#define UIKTAG_OBJWindowFl_SystemLook        (UIKTAG_USER|0x002f) /* FALSE */
#define UIKTAG_OBJWindowFl_CloseStop         (UIKTAG_USER|0x0030) /* TRUE */
#define UIKTAG_OBJWindowFl_CloseRemove       (UIKTAG_USER|0x0031) /* FALSE */
#define UIKTAG_OBJWindowFl_NoFill            (UIKTAG_USER|0x0032) /* FALSE */
#define UIKTAG_OBJWindowFl_SimpleRefresh     (UIKTAG_USER|0x0033) /* FALSE */
#define UIKTAG_OBJWindowFl_StartInactive     (UIKTAG_USER|0x0034) /* FALSE */

#define UIKTAG_OBJWindowFl_FullScreenWidth      (UIKTAG_USER|0x0035) /* FALSE */
#define UIKTAG_OBJWindowFl_FullScreenHeight     (UIKTAG_USER|0x0036) /* FALSE */
#define UIKTAG_OBJWindowFl_NearFullScHeight     (UIKTAG_USER|0x0037) /* FALSE */
#define UIKTAG_OBJWindowFl_FullDisplayWidth     (UIKTAG_USER|0x0038) /* FALSE */
#define UIKTAG_OBJWindowFl_FullDisplayHeight    (UIKTAG_USER|0x0039) /* FALSE */
#define UIKTAG_OBJWindowFl_NearFullDiHeight     (UIKTAG_USER|0x003a) /* FALSE */
#define UIKTAG_OBJWindowFl_FullScreen           (UIKTAG_USER|0x003b) /* FALSE */
#define UIKTAG_OBJWindowFl_NearFullScreen       (UIKTAG_USER|0x003c) /* FALSE */
#define UIKTAG_OBJWindowFl_FullDisplay          (UIKTAG_USER|0x003d) /* FALSE */
#define UIKTAG_OBJWindowFl_NearFullDisplay      (UIKTAG_USER|0x003e) /* FALSE */

#define UIKTAG_OBJWindowFl_FrontScreen      (UIKTAG_USER|0x003f) /* FALSE */
#define UIKTAG_OBJWindowFl_AutoCenter       (UIKTAG_USER|0x0040) /* FALSE */
#define UIKTAG_OBJWindowFl_AutoShrink       (UIKTAG_USER|0x0041) /* FALSE */

#endif /* UIKDEFS_OBJWINDOW */
