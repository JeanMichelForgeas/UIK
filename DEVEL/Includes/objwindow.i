        IFND    UIKDEFS_OBJWINDOW
UIKDEFS_OBJWINDOW SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**


    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC


 STRUCTURE  UIKWinScrInfo,0
    ULONG       uowsi_ModeId
    WORD        uowsi_Left
    WORD        uowsi_Top
    WORD        uowsi_Width
    WORD        uowsi_Height
    UBYTE       uowsi_Depth
    UBYTE       uowsi_Padb
    UWORD       uowsi_Padw
    APTR        uowsi_Font
    APTR        uowsi_Scr
    STRUCT      uowsi_Reserved,10*4
    LABEL   UIKWinScrInfo_SIZE

 STRUCTURE  UIKObjWindow,0
    STRUCT      uow_Obj,UIKOBJ_SIZE
    STRUCT      uow_NW,enw_SIZEOF
    APTR        uow_TitleBox
    APTR        uow_UniconW
    ULONG       uow_WinFlags
    ULONG       uow_IconLeftTop
    APTR        uow_WFont
    UWORD       uow_TopBorderHeight
    UWORD       uow_BottomBorderHeight
    UWORD       uow_LeftBorderWidth
    UWORD       uow_RightBorderWidth
    UWORD       uow_OldLeft
    UWORD       uow_OldTop
    UWORD       uow_OldWidth
    UWORD       uow_OldHeight
    APTR        uow_CustomBitMap
    APTR        uow_CustomAltBitMap
    APTR        uow_WMenu
    ULONG       uow_IconColors
    APTR        uow_GadgetList
    STRUCT      uow_ShortCutList,UIKLIST_SIZE
    STRUCT      uow_WTagList,3*8
    APTR        uow_DrawInfo
    APTR        uow_ScrAdd
    WORD        uow_ShrunkWidth
    WORD        uow_ShrunkHeight
    LABEL   UIKObjWindow_SIZE
    LABEL   UIKOBJWINDOW_SIZE


;------ window functions -------
UIKFUNC_Window_Zip              equ 0
UIKFUNC_Window_ChangeTitle      equ 4
UIKFUNC_Window_GetWinScrInfo    equ 8

UIKFUNC_Window_LastCmd          equ 8

;------ window flags -------
UIKW_TITLEFLAGS equ     $003d

UIKW_DEPTH      equ     $0001   ; 2 depth
UIKW_SIZE       equ     $0002
UIKW_CLOSE      equ     $0004
UIKW_ZOOM       equ     $0008
UIKW_ICON       equ     $0010
UIKW_DRAG       equ     $0020
UIKW_SCREENLOCKED       equ $0040
UIKW_ICONWINDOW         equ $0080
UIKW_AUTOADJUSTSIZE     equ $0100
UIKW_BITMAPED           equ $0200
UIKW_AUTOADJUSTLEFTTOP  equ $0400
UIKW_NODEFSCREEN        equ $0800
UIKW_NOPERIMETER        equ $1000
UIKW_DEFSCREENONLY      equ $2000
UIKW_BACKDROP           equ $4000
UIKW_MENULOOK           equ $8000
UIKW_SYSTEMLOOK         equ $00010000
UIKW_CLOSESTOP          equ $00020000
UIKW_CLOSEREMOVE        equ $00040000
UIKW_NOFILL             equ $00080000
UIKW_SIMPLEREFRESH      equ $00100000
UIKW_STARTINACTIVE      equ $00200000
UIKW_FULLWIDTH          equ $00400000
UIKW_FULLHEIGHT         equ $00800000
UIKW_NEARFULL           equ $01000000
UIKW_FULLDISPL          equ $02000000
UIKW_AUTOSHRINK         equ $04000000

;------ window bits -------
UIKWB_ICONWINDOW        equ     7
UIKWB_AUTOADJUSTSIZE    equ     8
UIKWB_BITMAPED          equ     9
UIKWB_AUTOADJUSTLEFTTOP equ    10
UIKWB_NODEFSCREEN       equ    11
UIKWB_NOPERIMETER       equ    12
UIKWB_DEFSCREENONLY     equ    13
UIKWB_BACKDROP          equ    14
UIKWB_MENULOOK          equ    15
UIKWB_SYSTEMLOOK        equ    16
UIKWB_CLOSESTOP         equ    17
UIKWB_CLOSEREMOVE       equ    18
UIKWB_NOFILL            equ    19
UIKWB_SIMPLEREFRESH     equ    20
UIKWB_STARTINACTIVE     equ    21
UIKWB_FULLWIDTH         equ    22
UIKWB_FULLHEIGHT        equ    23
UIKWB_NEARFULL          equ    24
UIKWB_FULLDISPL         equ    25
UIKWB_AUTOSHRINK        equ    26

;----UIKObjWindow values Tags----
UIKTAG_OBJWindow_MinimumWidthHeight equ (UIKTAG_USER|$0001)
UIKTAG_OBJWindow_MaximumWidthHeight equ (UIKTAG_USER|$0002)
UIKTAG_OBJWindow_CustomBitMap       equ (UIKTAG_USER|$0003)
UIKTAG_OBJWindow_CustomAltBitMap    equ (UIKTAG_USER|$0004)
UIKTAG_OBJWindow_Depth_ShortKey     equ (UIKTAG_USER|$0005)
UIKTAG_OBJWindow_Close_ShortKey     equ (UIKTAG_USER|$0006)
UIKTAG_OBJWindow_Zoom_ShortKey      equ (UIKTAG_USER|$0007)
UIKTAG_OBJWindow_Icon_ShortKey      equ (UIKTAG_USER|$0008)
UIKTAG_OBJWindow_ScreenAddr         equ (UIKTAG_USER|$0009)
UIKTAG_OBJWindow_PubScreenName      equ (UIKTAG_USER|$000a)

;----UIKObjWindow Flags Tags----
UIKTAG_OBJWindow_Flags          equ (UIKTAG_USER|$0020) ; les flags sont déjà prêts
 ; sinon un par un...
UIKTAG_OBJWindowFl_IconWindow   equ (UIKTAG_USER|$0021) ; FALSE
UIKTAG_OBJWindowFl_With_Depth   equ (UIKTAG_USER|$0022) ; TRUE
UIKTAG_OBJWindowFl_With_Size    equ (UIKTAG_USER|$0023) ; TRUE
UIKTAG_OBJWindowFl_With_Close   equ (UIKTAG_USER|$0024) ; TRUE
UIKTAG_OBJWindowFl_With_Zoom    equ (UIKTAG_USER|$0025) ; TRUE
UIKTAG_OBJWindowFl_With_Icon    equ (UIKTAG_USER|$0026) ; TRUE
UIKTAG_OBJWindowFl_With_Drag    equ (UIKTAG_USER|$0027) ; TRUE
UIKTAG_OBJWindowFl_AutoAdjustSize    equ (UIKTAG_USER|$0028) ; TRUE
UIKTAG_OBJWindowFl_AutoAdjustLeftTop equ (UIKTAG_USER|$0029) ; TRUE
UIKTAG_OBJWindowFl_NoDefScreen       equ (UIKTAG_USER|$002a) ; FALSE
UIKTAG_OBJWindowFl_NoBlackPerimeter  equ (UIKTAG_USER|$002b) ; FALSE
UIKTAG_OBJWindowFl_DefScreenOnly     equ (UIKTAG_USER|$002c) ; FALSE
UIKTAG_OBJWindowFl_Backdrop          equ (UIKTAG_USER|$002d) ; FALSE
UIKTAG_OBJWindowFl_NewLookMenu3      equ (UIKTAG_USER|$002e) ; FALSE
UIKTAG_OBJWindowFl_SystemLook        equ (UIKTAG_USER|$002f) ; FALSE
UIKTAG_OBJWindowFl_CloseStop         equ (UIKTAG_USER|$0030) ; TRUE
UIKTAG_OBJWindowFl_CloseRemove       equ (UIKTAG_USER|$0031) ; FALSE
UIKTAG_OBJWindowFl_NoFill            equ (UIKTAG_USER|$0032) ; FALSE
UIKTAG_OBJWindowFl_SimpleRefresh     equ (UIKTAG_USER|$0033) ; FALSE
UIKTAG_OBJWindowFl_StartInactive     equ (UIKTAG_USER|$0034) ; FALSE

UIKTAG_OBJWindowFl_FullScreenWidth      equ (UIKTAG_USER|$0035) ; FALSE
UIKTAG_OBJWindowFl_FullScreenHeight     equ (UIKTAG_USER|$0036) ; FALSE
UIKTAG_OBJWindowFl_NearFullScHeight     equ (UIKTAG_USER|$0037) ; FALSE
UIKTAG_OBJWindowFl_FullDisplayWidth     equ (UIKTAG_USER|$0038) ; FALSE
UIKTAG_OBJWindowFl_FullDisplayHeight    equ (UIKTAG_USER|$0039) ; FALSE
UIKTAG_OBJWindowFl_NearFullDiHeight     equ (UIKTAG_USER|$003a) ; FALSE
UIKTAG_OBJWindowFl_FullScreen           equ (UIKTAG_USER|$003b) ; FALSE
UIKTAG_OBJWindowFl_NearFullScreen       equ (UIKTAG_USER|$003c) ; FALSE
UIKTAG_OBJWindowFl_FullDisplay          equ (UIKTAG_USER|$003d) ; FALSE
UIKTAG_OBJWindowFl_NearFullDisplay      equ (UIKTAG_USER|$003e) ; FALSE

UIKTAG_OBJWindowFl_FrontScreen          equ (UIKTAG_USER|$003f) ; FALSE
UIKTAG_OBJWindowFl_AutoCenter           equ (UIKTAG_USER|$0040) ; FALSE
UIKTAG_OBJWindowFl_AutoShrink           equ (UIKTAG_USER|$0041) ; FALSE

        ENDC
