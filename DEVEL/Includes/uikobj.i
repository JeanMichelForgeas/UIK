        IFND    UIKDEFS_OBJ
UIKDEFS_OBJ     SET 1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**

    IFND    EXEC_TYPES_I
    INCLUDE "exec/types.i"
    ENDC

    IFND    INTUITION_INTUITION_I
    INCLUDE "intuition/intuition.i"
    ENDC

    IFND    GRAPHICS_REGIONS_I
    INCLUDE "graphics/regions.i"
    ENDC

    IFND    DEVICES_TIMER_I
    INCLUDE "devices/timer.i"
    ENDC

    IFND    UIKDEFS_LIST
    INCLUDE "uiki:uiklist.i"
    ENDC

    IFND    UIKDEFS_BOX
    INCLUDE "uiki:uikbox.i"
    ENDC

    IFND    UIKDEFS_TAGITEM
    INCLUDE "uiki:uiktag.i"
    ENDC

    IFND    UIKDEFS_BUFFER
    INCLUDE "uiki:uikbuf.i"
    ENDC

    IFND    UIKDEFS_LANGAGE
    INCLUDE "uiki:uiklang.i"
    ENDC


 STRUCTURE  UIKObjGerm,0
    STRUCT      uog_ObjName,32
    FPTR        uog_AddVector
    FPTR        uog_StartVector
    FPTR        uog_StopVector
    FPTR        uog_RemoveVector
    FPTR        uog_RefreshVector
    FPTR        uog_ResizeVector
    FPTR        uog_ExceptActVector
    FPTR        uog_ExceptInaVector
    FPTR        uog_ExceptMovVector
    FPTR        uog_ExceptKeyVector
    FPTR        uog_StartAfterVector
    FPTR        uog_SelectVector
    APTR        uog_ExtentedVectors
    APTR        uog_GermTagList
    UWORD       uog_MinWidth
    UWORD       uog_MinHeight
    ULONG       uog_DefaultWidthHeight
    UWORD       uog_ObjId
    UWORD       uog_ObjSize
    ULONG       uog_DefaultFlags
    LABEL   UIKOBJGERM_SIZE
    LABEL   UIKObjGerm_SIZE


 STRUCTURE  UIKObjRegGerm,0
    APTR        uorg_Germ
    ULONG       uorg_UsageCount
    ULONG       uorg_Segment
    ULONG       uorg_Flags
    ULONG       uorg_UnloadFunc
    LABEL   UIKOBJREGGERM_SIZE
    LABEL   UIKObjRegGerm_SIZE

UIKRGF_UNLOAD equ $0001

 STRUCTURE  UIKObjExtension,0
    FPTR        uoe_UserStopFunc
    FPTR        uoe_UserRemoveFunc
    APTR        uoe_Font
    APTR        uoe_UFrame
    FPTR        uoe_CustomDraw
    ULONG       uoe_CustomDrawData
    APTR        uoe_ZoneList
    STRUCT      uoe_TTA,tta_SIZEOF
    APTR        uoe_Lang
    ULONG       uoe_LastFunc
    UBYTE       uoe_SCKey
    UBYTE       uoe_SCCode
    UWORD       uoe_SCQual
    STRUCT      uoe_InnerRect,ra_SIZEOF
    UWORD       uoe_MultiDelay
    UWORD       uoe_MultiNum
    UWORD       uoe_MultiCount
    UBYTE       uoe_SCWidth
    UBYTE       uoe_SCPos
    APTR        uoe_SCList
    APTR        uoe_SCHook
    APTR        uoe_ActKeyTank
    APTR        uoe_InactKeyTank
    APTR        uoe_IPMother
    APTR        uoe_IPRemoveFunc
    LABEL   UIKOBJEXT_SIZE
    LABEL   UIKObjExtension_SIZE


 STRUCTURE  UIKObj,0
    STRUCT      uo_List,UIKLIST_SIZE
    STRUCT      uo_Link,UIKLIST_SIZE
    STRUCT      uo_Hook,UIKLIST_SIZE
    APTR        uo_UIK
    APTR        uo_sc
    APTR        uo_w
    APTR        uo_rq
    APTR        uo_rp
    STRUCT      uo_Box,UIKBOX_SIZE
    ULONG       uo_UserULong1
    ULONG       uo_UserULong2
    UBYTE       uo_Status
    UBYTE       uo_MouseIn
    WORD        uo_MouseX
    WORD        uo_MouseY
    WORD        uo_RegisterId
    APTR        uo_Icon
    APTR        uo_Parent
    ULONG       uo_Flags
    ULONG       uo_TitleFlags
    ULONG       uo_ResizeFlags

    UBYTE       uo_ActBoxPen
    UBYTE       uo_InactBoxPen
    UBYTE       uo_ActTitlePen
    UBYTE       uo_InactTitlePen
    UBYTE       uo_BoxPen
    UBYTE       uo_TitlePen
    UWORD       uo_Id

    ULONG       uo_Title
    ULONG       uo_AltTitle
    APTR        uo_Automate
    APTR        uo_AltAutomate

    FPTR        uo_StartVector
    FPTR        uo_StopVector
    FPTR        uo_RemoveVector
    FPTR        uo_RefreshVector
    FPTR        uo_ResizeVector
    FPTR        uo_ExceptActVector
    FPTR        uo_ExceptInaVector
    FPTR        uo_ExceptMovVector
    FPTR        uo_ExceptKeyVector
    FPTR        uo_StartAfterVector
    FPTR        uo_SelectVector
    APTR        uo_ExtentedVectors

    APTR        uo_GermTagList
    UWORD       uo_MinWidth
    UWORD       uo_MinHeight

    APTR        uo_SigSemaphore
    APTR        uo_irp
    APTR        uo_Region
    APTR        uo_SpareRegion
    UBYTE       uo_ClipCount
    UBYTE       uo_PatternPower
    UBYTE       uo_EraseColor
    UBYTE       uo_AutoRatio
    APTR        uo_RegGerm
    ULONG       uo_Pattern
    UWORD       uo_XAspect
    UWORD       uo_YAspect
    APTR        uo_ObjExt
    LABEL   UIKOBJ_SIZE
    LABEL   UIKObj_SIZE


 STRUCTURE  UIKObjGad,0
    STRUCT      uog_Obj,UIKOBJ_SIZE
    STRUCT      uog_Gad,gg_SIZEOF
    LABEL   UIKObjGad_SIZE


;----UIKObj Declencheurs----

 STRUCTURE  UIKHook,0
    FPTR        uh_Vect
    APTR        uh_Obj
    ULONG       uh_Mask
    ULONG       uh_Parm
    LABEL   UIKHook_SIZE

 STRUCTURE  UIKLink,0
    FPTR        li_Vect
    APTR        li_Obj
    APTR        li_Dest
    ULONG       li_Mask
    ULONG       li_Parm
    LABEL   UIKLink_SIZE


UIKOBJBOX_SIZE  equ UIKOBJ_SIZE


;----- object status ------

;flag
UIKSF_OUT_OF_THE_LAW equ $80

UIKS_STARTED        equ 0
UIKS_STOPPING       equ 1
UIKS_REMOVING       equ 2
UIKS_STOPPED        equ 3
UIKS_ICONIFIED      equ 4
UIKS_OLDICONIFIED   equ 5
UIKS_REMOVED        equ 6

;------ objects ids -------
OBJECTID_USER  equ $8000

UIKID_NULL     equ   0
UIKID_BOX      equ   1
UIKID_SCREEN   equ   2
UIKID_WINDOW   equ   3
UIKID_REQUEST  equ   4
UIKID_UPGAD    equ   5
UIKID_DOWNGAD  equ   6

UIKID_SYSGAD   equ   7
UIKID_DEPTGAD  equ   7
UIKID_SIZEGAD  equ   8
UIKID_CLOSGAD  equ   9
UIKID_ZOOMGAD  equ  10
UIKID_ICONGAD  equ  11
UIKID_DRAGGAD  equ  12

UIKID_BOOLGAD  equ  13
UIKID_TOGLGAD  equ  14
UIKID_STRGAD   equ  15
UIKID_PROPGAD  equ  16
UIKID_LIST     equ  17
UIKID_MEXGAD   equ  18
UIKID_MEM      equ  19
UIKID_RIGHTGAD equ  20
UIKID_LEFTGAD  equ  21

UIKID_FILESELECT    equ 22

UIKID_OKGAD         equ 23
UIKID_CANCELGAD     equ 24

UIKID_HELP          equ 25
UIKID_WINHELP       equ 26
UIKID_REQSTRING     equ 27
UIKID_DIAL          equ 28
UIKID_AREXXSIM      equ 29

UIKID_PROGRESS      equ 30
UIKID_MENU          equ 31
UIKID_JOYSTICK      equ 32
UIKID_INPUTICK      equ 33
UIKID_PALETTE       equ 34
UIKID_FONTSELECT    equ 35

UIKID_DISPLAYMODE   equ 36
UIKID_DISPATCH      equ 37
UIKID_TEXTVIEW      equ 38
UIKID_CXBROKER      equ 39
UIKID_APPWBENCH     equ 40

UIKID_WINFILESEL    equ 41
UIKID_WFILEBUTTON   equ 42
UIKID_WINFONTSEL    equ 43
UIKID_WFONTBUTTON   equ 44
UIKID_CHECKTOGGLE   equ 45
UIKID_RADIOEXCLUDE  equ 46
UIKID_IMAGEVIEW     equ 47
UIKID_TIMER         equ 48
UIKID_CYCLE         equ 49
UIKID_IPOL          equ 50
UIKID_NOTIFY        equ 51
UIKID_SPROPLIFT     equ 52
UIKID_COLORSLIDE    equ 53

UIKID_SERIAL        equ 54
UIKID_BOXNBOX       equ 55
UIKID_OBJNULL       equ 56
UIKID_PROCESS       equ 57
UIKID_ASYNCFILE     equ 58
UIKID_NARRATOR      equ 59
UIKID_JOYMOUSE      equ 60

UIKID_FBHELP        equ 61
UIKID_FBWINHELP     equ 62
UIKID_FBSCROLLER    equ 63
UIKID_FB2C          equ 64
UIKID_FB3C          equ 65
UIKID_FBFILEPP      equ 66
UIKID_FBWINTV       equ 67
UIKID_FBMED         equ 68
UIKID_FBPAUSE       equ 69
UIKID_FBPLAY        equ 70
UIKID_FBSTOP        equ 71
UIKID_FBMEDPLAYER   equ 72
UIKID_FBVOLUME      equ 73
UIKID_FBCHOICE      equ 74
UIKID_FBLISTER      equ 75
UIKID_TEXTEDITOR    equ 76
UIKID_CALENDAR      equ 77
UIKID_FBBUTTONS     equ 78
UIKID_FBBUTTONLIST  equ 79

;------ objects flags -----
BOX_NDRAWN      equ   $0001
BOX_INNERZONE   equ   $0002
BOX_FRAME       equ   $0004
BOX_NOACT       equ   $0008
BOX_OVER        equ   $0010
BOX_FILLED      equ   $0020
BOX_reserved    equ   $0040
BOX_CLIPPED     equ   $0080
BOX_SEMAPHORE   equ   $0100
BOX_NOPUSH      equ   $0200
BOX_TRANSLATELEFTTOP equ $0400
BOX_VISUALERASE equ   $0800
BOX_OPENEDFONT  equ   $1000
BOX_CUSTOMDRAW  equ   $2000
BOX_USEBITMAP   equ   $4000
BOX_TRANSLATED  equ   $8000
BOX_ZONEABLE    equ   $00010000
BOX_DISPLAYABLE equ   $00020000
BOX_STARTSPEC   equ   $00040000
BOX_EXTGADUP    equ   $00080000
BOX_DISABLED    equ   $00100000
BOX_CANTDISABLE equ   $00200000
BOX_GROUPABLE   equ   $00400000
BOX_VGROUP      equ   $00800000
BOX_HGROUP      equ   $01000000

;--------------------------
BOXB_NDRAWN     equ     0
BOXB_INNERZONE  equ     1
BOXB_FRAME      equ     2
BOXB_NOACT      equ     3
BOXB_OVER       equ     4
BOXB_FILLED     equ     5
BOXB_reserved   equ     6
BOXB_CLIPPED    equ     7
BOXB_SEMAPHORE  equ     8
BOXB_NOPUSH     equ     9
BOXB_TRANSLATELEFTTOP   equ 10
BOXB_VISUALERASE equ    11
BOXB_OPENEDFONT equ     12
BOXB_CUSTOMDRAW equ     13
BOXB_USEBITMAP  equ     14
BOXB_TRANSLATED equ     15
BOXB_ZONEABLE   equ     16
BOXB_DISPLAYABLE equ    17
BOXB_STARTSPEC  equ     18
BOXB_EXTGADUP   equ     19
BOXB_DISABLED   equ     20
BOXB_CANTDISABLE equ    21
BOXB_GROUPABLE   equ    22
BOXB_VGROUP      equ    23
BOXB_HGROUP      equ    24

;------ Title Flags -------
TITLE_TOP         equ $0001
TITLE_LEFT        equ $0002
TITLE_RIGHT       equ $0004
TITLE_CENTER      equ $0008
TITLE_reserved    equ $0010
TITLE_ADDR        equ $0020
TITLE_CLIPPED     equ $0040
TITLE_IMAGE       equ $0080
TITLE_ABOVE       equ $0100
TITLE_INSRIGHT    equ $0200
TITLE_INSLEFT     equ $0400
;--------------------------
TITLEB_TOP        equ   0   +3  +0
TITLEB_LEFT       equ   1   +3
TITLEB_RIGHT      equ   2   +3
TITLEB_CENTER     equ   3   +3
TITLEB_reserved   equ   4   +3
TITLEB_ADDR       equ   5   +3
TITLEB_CLIPPED    equ   6   +3
TITLEB_IMAGE      equ   7   +3
TITLEB_ABOVE      equ   0   +2  +8
TITLEB_INSRIGHT   equ   1   +2  +9
TITLEB_INSLEFT    equ   2   +2  +10

;------ Sp Flags --------
UO_ATTACHED_LEFT  equ $01
UO_ATTACHED_TOP   equ $02
UO_ATTACHED_RIGHT equ $04
UO_ATTACHED_BOT   equ $08
;------------------------
UOB_ATTACHED_LEFT  equ  0
UOB_ATTACHED_TOP   equ  1
UOB_ATTACHED_RIGHT equ  2
UOB_ATTACHED_BOT   equ  3


;-------------------------------------------------------------
;---------------------------- T A G S ------------------------
;-------------------------------------------------------------

;-------------------------------------------------GENERIQUES
UIKTAG_GEN_LangEnglish          equ (TAG_USER|$0001)
UIKTAG_GEN_LangOther            equ (TAG_USER|$0002)
UIKTAG_GEN_LastFuncId           equ (TAG_USER|$0003)
UIKTAG_GEN_Catalog              equ (TAG_USER|$0004)
UIKTAG_GEN_ActInactPens         equ (TAG_USER|$0005)
UIKTAG_GEN_BoxAutomate          equ (TAG_USER|$0006)
UIKTAG_GEN_BoxAltAutomate       equ (TAG_USER|$0007)
UIKTAG_GEN_ShortCutKey          equ (TAG_USER|$0008)
UIKTAG_GEN_ActKeyTank           equ (TAG_USER|$0009)
UIKTAG_GEN_InactKeyTank         equ (TAG_USER|$000a)
UIKTAG_GEN_VersionRevision      equ (TAG_USER|$000b)
UIKTAG_GEN_LangDuplicate        equ (TAG_USER|$000c)
UIKTAG_GEN_LoadFunc             equ (TAG_USER|$000d)
UIKTAG_GEN_UnloadFunc           equ (TAG_USER|$000e)
UIKTAG_GEN_FilterIEventsFunc    equ (TAG_USER|$000f)
UIKTAG_GEN_SysLookGenTagList    equ (TAG_USER|$0010)

;---------------------------------------------------UIKObj
;----UIKObj values Tags----
UIKTAG_OBJ_ParentRelative       equ (TAG_USER|$0000)
UIKTAG_OBJ_LeftTop              equ (TAG_USER|$0001)
UIKTAG_OBJ_WidthHeight          equ (TAG_USER|$0002)
UIKTAG_OBJ_HitMask              equ (TAG_USER|$0003)
UIKTAG_OBJ_TitleUnderscore      equ (TAG_USER|$0004)
UIKTAG_OBJ_ActInactPens         equ (TAG_USER|$0005)
UIKTAG_OBJ_Title                equ (TAG_USER|$0006)
UIKTAG_OBJ_AltTitle             equ (TAG_USER|$0007)
UIKTAG_OBJ_BoxAutomate          equ (TAG_USER|$0008)
UIKTAG_reserved1                equ (TAG_USER|$0009)
UIKTAG_OBJ_UserValue1           equ (TAG_USER|$000a)
UIKTAG_OBJ_UserValue2           equ (TAG_USER|$000b)
UIKTAG_OBJ_BoxAltAutomate       equ (TAG_USER|$000c)
UIKTAG_OBJ_Pattern              equ (TAG_USER|$000d)
UIKTAG_OBJ_PatternPower         equ (TAG_USER|$000e)
UIKTAG_OBJ_VisualEraseColor     equ (TAG_USER|$000f)
UIKTAG_OBJ_UserStopFunc         equ (TAG_USER|$0010)
UIKTAG_OBJ_UserRemoveFunc       equ (TAG_USER|$0011)
UIKTAG_OBJ_FontName             equ (TAG_USER|$0012)
UIKTAG_OBJ_FontHeight           equ (TAG_USER|$0013)
UIKTAG_OBJ_CustomDraw           equ (TAG_USER|$0014)
UIKTAG_OBJ_CustomDrawData       equ (TAG_USER|$0015)
UIKTAG_OBJ_IPRemoveFunc         equ (TAG_USER|$0016)
UIKTAG_OBJ_ZoneFunc             equ (TAG_USER|$0017)
UIKTAG_OBJ_TTextAttr            equ (TAG_USER|$0018)
UIKTAG_OBJ_LangMaster           equ (TAG_USER|$0019)
UIKTAG_OBJ_ShortCutKey          equ (TAG_USER|$001a)
UIKTAG_OBJ_reserved2            equ (TAG_USER|$001b)
UIKTAG_OBJ_MinVerRev            equ (TAG_USER|$001c)
UIKTAG_OBJ_MultiTitleDelay      equ (TAG_USER|$001d)
UIKTAG_OBJ_MultiTitleNum        equ (TAG_USER|$001e)

;----UIKObj Flags Tags----
UIKTAG_OBJ_Box_Flags            equ (TAG_USER|$0020) ; les flags sont déjà prêts
UIKTAG_OBJ_BoxFl_NotDrawn       equ (TAG_USER|$0021) ; FALSE
UIKTAG_OBJ_BoxFl_ExtGadgetUp    equ (TAG_USER|$0022) ; FALSE
UIKTAG_OBJ_BoxFl_Frame          equ (TAG_USER|$0023) ; FALSE
UIKTAG_OBJ_BoxFl_NoAct          equ (TAG_USER|$0024) ; FALSE
UIKTAG_OBJ_BoxFl_Over           equ (TAG_USER|$0025) ; TRUE
UIKTAG_OBJ_BoxFl_Filled         equ (TAG_USER|$0026) ; FALSE
UIKTAG_OBJ_BoxFl_Clipped        equ (TAG_USER|$0027) ; FALSE
UIKTAG_OBJ_BoxFl_Semaphore      equ (TAG_USER|$0028) ; TRUE
UIKTAG_OBJ_BoxFl_NoPush         equ (TAG_USER|$0029) ; FALSE
UIKTAG_OBJ_BoxFl_TranslateLeftTop equ (TAG_USER|$002a) ; TRUE
UIKTAG_OBJ_BoxFl_UseBitMap      equ (TAG_USER|$002b) ; FALSE
UIKTAG_OBJ_BoxFl_Zoneable       equ (TAG_USER|$002c) ; depend
UIKTAG_OBJ_BoxFl_Displayable    equ (TAG_USER|$002d) ; depend
UIKTAG_OBJ_BoxFl_StartSpecify   equ (TAG_USER|$002e) ; FALSE
UIKTAG_OBJ_BoxFl_Disabled       equ (TAG_USER|$002f) ; FALSE
UIKTAG_OBJ_BoxFl_CannotDisable  equ (TAG_USER|$0030) ; FALSE

UIKTAG_OBJ_Title_Flags          equ (TAG_USER|$0040) ; les flags sont déjà prêts
 ; default = TITLE_CENTER | TITLE_CLIPPED
 ; sinon un par un...
UIKTAG_OBJ_TitleFl_Top          equ (TAG_USER|$0042) ; FALSE
UIKTAG_OBJ_TitleFl_Left         equ (TAG_USER|$0043) ; FALSE
UIKTAG_OBJ_TitleFl_Right        equ (TAG_USER|$0044) ; FALSE
UIKTAG_OBJ_TitleFl_Center       equ (TAG_USER|$0045) ; TRUE
UIKTAG_reserved4                equ (TAG_USER|$0046) ;
UIKTAG_OBJ_TitleFl_Addr         equ (TAG_USER|$0047) ; FALSE
UIKTAG_OBJ_TitleFl_Clipped      equ (TAG_USER|$0048) ; TRUE
UIKTAG_OBJ_TitleFl_Image        equ (TAG_USER|$0049) ; FALSE
UIKTAG_OBJ_TitleFl_Above        equ (TAG_USER|$004a) ; FALSE
UIKTAG_OBJ_TitleFl_InsRight     equ (TAG_USER|$004b) ; FALSE
UIKTAG_OBJ_TitleFl_InsLeft      equ (TAG_USER|$004c) ; FALSE

UIKTAG_OBJ_Sz_Attached_Flags    equ (TAG_USER|$0060) ; les flags sont déjà prêts
 ; default = 0
 ; sinon un par un...
UIKTAG_OBJ_Sz_AttachedFl_Left   equ (TAG_USER|$0061) ; FALSE
UIKTAG_OBJ_Sz_AttachedFl_Top    equ (TAG_USER|$0062) ; FALSE
UIKTAG_OBJ_Sz_AttachedFl_Right  equ (TAG_USER|$0063) ; FALSE
UIKTAG_OBJ_Sz_AttachedFl_Bot    equ (TAG_USER|$0064) ; FALSE



        ENDC
