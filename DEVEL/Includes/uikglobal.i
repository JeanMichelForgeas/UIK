    IFND    UIKDEFS_GLOBAL
UIKDEFS_GLOBAL  SET 1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**


    IFND    UIKDEFS_OBJ
    INCLUDE "uiki:uikobj.i"
    ENDC

    IFND    UIKDEFS_LIST
    INCLUDE  'uiki:uiklist.i'
    ENDC

    IFND    UIKDEFS_KEYSUPP
    INCLUDE  "uiki:uikkey.i"
    ENDC


 STRUCTURE  UIKArgRegs,0
    ULONG       uar_D0
    ULONG       uar_D1
    ULONG       uar_D2
    ULONG       uar_D3
    ULONG       uar_D4
    ULONG       uar_D5
    ULONG       uar_D6
    ULONG       uar_D7
    ULONG       uar_A0
    ULONG       uar_A1
    ULONG       uar_A2
    ULONG       uar_A3
    ULONG       uar_A4
    ULONG       uar_A5
    LABEL   UIKArgRegs_SIZE


 STRUCTURE  UIKGlobal,0
    STRUCT      uike_Obj,UIKOBJ_SIZE
    STRUCT      uike_UPort,MP_SIZE
    APTR        uike_Task
    UBYTE       uike_Flags
    UBYTE       uike_RemObj
    UBYTE       uike_Exable
    UBYTE       uike_ActiveSigBit
    ULONG       uike_ActiveMask
    LONG        uike_SigBit
    ULONG       uike_SigMask
    ULONG       uike_CmdMask
    ULONG       uike_ExcMask
    ULONG       uike_ExtMask
    ULONG       uike_WindowNum
    APTR        uike_ActiveObj
    FPTR        uike_StopVector
    FPTR        uike_RemoveVector
    APTR        uike_OldLangEnglish
    APTR        uike_OldLangOther
    APTR        uike_OldLangage
    APTR        uike_IntuiMsg
    STRUCT      uike_IMSList,MLH_SIZE
    STRUCT      uike_CmdPort,MP_SIZE
    STRUCT      uike_CmdReq,IO_SIZE
    APTR        uike_FilterIE
    APTR        uike_StartSpec
    STRUCT      uike_KeySupp,UIKKEYSUPP_SIZE
    APTR        uike_ActiveKeyTank
    ULONG       uike_Tmp

    APTR        uike_OILBM
    APTR        uike_O8SVX
    APTR        uike_OIPOL

    UBYTE       uike_InputMode
    UBYTE       uike_InputHeld
    UBYTE       uike_InputAuto
    UBYTE       uike_padb

    APTR        uike_ZoneListBase
    APTR        uike_ActiveZoneList
    APTR        uike_ZoneRPortH
    APTR        uike_ZonePlanesH

    UWORD       uike_InputFlags
    UWORD       uike_TempoSpeed
    UBYTE       uike_JoyVal
    UBYTE       uike_OldJoyVal
    UBYTE       uike_JoyButs
    UBYTE       uike_OldJoyButs
    UBYTE       uike_DirButs
    UBYTE       uike_ProcButs
    UBYTE       uike_IptButs
    UBYTE       uike_IptChgs

    UWORD       uike_TempoValidRight
    UWORD       uike_TempoWaitRight
    UWORD       uike_TempoValidLeft
    UWORD       uike_TempoWaitLeft
    UWORD       uike_TempoValidBack
    UWORD       uike_TempoWaitBack
    UWORD       uike_TempoValidForw
    UWORD       uike_TempoWaitForw
    UWORD       uike_TempoValidFire
    UWORD       uike_TempoWaitFire

    STRUCT      uike_JoyEvents,40
    APTR        uike_JTBeg
    APTR        uike_JTEnd
    FPTR        uike_InputVector
    FPTR        uike_InputMainVector

    APTR        uike_Locale

    APTR        uike_ZoneRPortW
    APTR        uike_ZonePlanesW

    STRUCT      uike_SCutSupp,UIKKEYSUPP_SIZE
    STRUCT      uike_ArgRegs,UIKArgRegs_SIZE

    LABEL   UIKGLOBAL_SIZE
    LABEL   UIKGlobal_SIZE

;----- UIKGlobal Flags ----
UIKGF_DEADMENU  equ $01
UIKGF_RAWCTRLC  equ $02

UIKGB_DEADMENU  equ 0
UIKGB_RAWCTRLC  equ 1

;----- JoyVal ----
VBJOYF_RIGHT equ $01
VBJOYF_LEFT  equ $02
VBJOYF_BACK  equ $04
VBJOYF_FORW  equ $08
VBJOYF_FIRE  equ $10

VBJOYB_RIGHT equ 0
VBJOYB_LEFT  equ 1
VBJOYB_BACK  equ 2
VBJOYB_FORW  equ 3
VBJOYB_FIRE  equ 4

CLIC_BUTTON equ (VBJOYF_FIRE)
MOVE_BUTTON equ (VBJOYF_RIGHT|VBJOYF_LEFT|VBJOYF_BACK|VBJOYF_FORW)
DIR_BUTTONS equ (VBJOYF_RIGHT|VBJOYF_LEFT|VBJOYF_BACK|VBJOYF_FORW)


        ENDC
