        IFND    UIKDEFS_DISPLAYMODE
UIKDEFS_DISPLAYMODE SET     1
******************************************************************************************************************
*                                                                                                                *
*  File : DisplayMode.h                                                                                          *
*  Project : Display mode selector.                                                                              *
*                                                                                                                *
*  Created : 16/07/92 ©1992 Frédéric DENIS "ß release" Software Design.                                          *
*                                                                                                                *
*  All rights reserved.                                                                                          *
*                                                                                                                *
******************************************************************************************************************


    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC

    IFND UIKDEFS_OBJLIST
    INCLUDE "UIKI:objlist.i"
    ENDC


 STRUCTURE  ModeInfo,0
    UWORD       odmi_ModeId     ; en fait 4 octets en incluant les 2 suivants
    UWORD       odmi_Modes
    UWORD       odmi_Depth
    UWORD       odmi_Width
    UWORD       odmi_Height
    UWORD       odmi_OScanType
    LABEL       odmi_ColorShades; en fait 4 octets en incluant les 3 suivants
    UBYTE       odmi_RedBits
    UBYTE       odmi_GreenBits
    UBYTE       odmi_BlueBits
    UBYTE       odmi_Pad
    ULONG       odmi_Name
    STRUCT      odmi_reserved,4
    LABEL   ModeInfo_SIZE


; Overscan types :

CUSTOM_OSCAN equ 0
TEXT_OSCAN   equ 1
GFX_OSCAN    equ 2
MIN_OSCAN    equ 3
MAX_OSCAN    equ 4


 STRUCTURE  UIKObjDisplayMode,0
    STRUCT      uodm_Obj,UIKOBJ_SIZE

    APTR        uodm_DOSBase
    APTR        uodm_GfxBase
    APTR        uodm_IntuitionBase
    APTR        uodm_UIKBase
    FPTR        uodm_OKFunc
    FPTR        uodm_CancelFunc
    ULONG       uodm_SysState

    STRUCT      uodm_NodeHead,UIKLIST_SIZE
    STRUCT      uodm_NameHead,UIKLIST_SIZE
    STRUCT      uodm_ColNodeHead,UIKLIST_SIZE
    STRUCT      uodm_ColNameHead,UIKLIST_SIZE

    APTR        uodm_ModesObjList
    APTR        uodm_ColObjList
    APTR        uodm_ModesUp
    APTR        uodm_ModesDown
    APTR        uodm_ModesProp
    APTR        uodm_MPPropFrame
    APTR        uodm_MPPropList
    APTR        uodm_MPPropProp
    APTR        uodm_MPPropUp
    APTR        uodm_MPPropDown
    APTR        uodm_ColUp
    APTR        uodm_ColDown
    APTR        uodm_ColProp
    APTR        uodm_ObjOK
    APTR        uodm_ObjCancel
    APTR        uodm_MinSize
    APTR        uodm_TextSize
    APTR        uodm_GfxSize
    APTR        uodm_MaxSize
    APTR        uodm_CustomSize
    APTR        uodm_CustomWidth
    APTR        uodm_CustomHeight
    APTR        uodm_ResizeNewSizeHook

    STRUCT      uodm_Current,ModeInfo_SIZE
    APTR        uodm_MIUser

    APTR        uodm_insstr
    APTR        uodm_CurrentBuffer
    APTR        uodm_WidthBuff
    APTR        uodm_HeightBuff
    ULONG       uodm_DispWidth
    ULONG       uodm_DispHeight
    ULONG       uodm_index
    ULONG       uodm_DisplayID
    ULONG       uodm_UserData

    LABEL   UIKObjDisplayMode_SIZE


; fonctions publiques

OBJFUNC_ObjDM_  equ 0


;---------Obj Tags----------
UIKTAG_ObjDM_OKFunc         equ (UIKTAG_USER|$0101)   ; Fonction apellee si OK
UIKTAG_ObjDM_CancelFunc     equ (UIKTAG_USER|$0102)   ; idem si Cancel
UIKTAG_ObjDM_ModeInfo       equ (UIKTAG_USER|$0103)   ; struct ModeInfo utilisee comme defaut
UIKTAG_ObjDM_UserData       equ (UIKTAG_USER|$0104)   ; au choix, passe en dernier argument de ok ou cancel
UIKTAG_ObjDM_ModeCantBe     equ (UIKTAG_USER|$0105)   ; reservé pour plus tard...
UIKTAG_ObjDM_ModeMustBe     equ (UIKTAG_USER|$0106)   ; reservé pour plus tard...
UIKTAG_ObjDM_ModeMinSize    equ (UIKTAG_USER|$0107)   ; reservé pour plus tard...
UIKTAG_ObjDM_ModeMaxSize    equ (UIKTAG_USER|$0108)   ; reservé pour plus tard...

;---------Obj Flags---------

;--

        ENDC
