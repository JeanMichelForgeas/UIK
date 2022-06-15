        IFND    UIKDEFS_JOYSTICK
UIKDEFS_JOYSTICK SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**

    IFND UIKDEFS_OBJ
    INCLUDE "uiki:uikobj.i"
    ENDC

    IFND DEVICES_GAMEPORT_I
    INCLUDE "devices/gameport.i"
    ENDC

    IFND DEVICES_INPUTEVENT_I
    INCLUDE "devices/inputevent.i"
    ENDC

 STRUCTURE  UIKObjJoystick,0
    STRUCT      uoj_Obj,UIKOBJ_SIZE
    APTR        uoj_DOSBase
    APTR        uoj_UIKBase

    FPTR        uoj_LButFunc
    FPTR        uoj_RButFunc
    FPTR        uoj_MButFunc
    FPTR        uoj_ForwFunc
    FPTR        uoj_BackFunc
    FPTR        uoj_LeftFunc
    FPTR        uoj_RightFunc
    FPTR        uoj_ReleaseFunc
    FPTR        uoj_MoveFunc
    FPTR        uoj_TimeoutFunc
    UBYTE       uoj_Unit
    UBYTE       uoj_Controller
    UWORD       uoj_padw

    APTR        uoj_EventHook
    FPTR        uoj_EventVect
    APTR        uoj_Port
    APTR        uoj_Req
    APTR        uoj_Device
    STRUCT      uoj_Trigger,gpt_SIZEOF
    STRUCT      uoj_IEV1,ie_SIZEOF
    STRUCT      uoj_IEV2,ie_SIZEOF
    APTR        uoj_ActiveEv
    APTR        uoj_ReceivedEv
    ULONG       uoj_Started
    LABEL   uojoy_SIZE
    LABEL   UIKObjJoystick_SIZE



*================================pas implémentés
*--------contacts flags--------*
JOYF_LBUTTON         equ $01
JOYF_RBUTTON         equ $02
JOYF_MBUTTON         equ $04
JOYF_FORWARD         equ $08
JOYF_BACKWARD        equ $10
JOYF_LEFT            equ $20
JOYF_RIGHT           equ $40
JOYF_TIMEOUT         equ $80

JOYB_LBUTTON         equ    0
JOYB_RBUTTON         equ    1
JOYB_MBUTTON         equ    2
JOYB_FORWARD         equ    3
JOYB_BACKWARD        equ    4
JOYB_LEFT            equ    6
JOYB_RIGHT           equ    5
JOYB_TIMEOUT         equ    7
*=========================================*

*--------directions values-----*
JOYDIR_NONE            equ -1
JOYDIR_FORWARD         equ  0
JOYDIR_FORWARD_RIGHT   equ  1
JOYDIR_RIGHT           equ  2
JOYDIR_BACKWARD_RIGHT  equ  3
JOYDIR_BACKWARD        equ  4
JOYDIR_BACKWARD_LEFT   equ  5
JOYDIR_LEFT            equ  6
JOYDIR_FORWARD_LEFT    equ  7

* fonctions publiques

OBJFUNC_Joystick_Flush      equ 0
OBJFUNC_Joystick_LastCmd    equ 0


*---------Obj Joystick Tags---------- *
UIKTAG_Joystick_LButFunc    equ (UIKTAG_USER|$0001) * appelée quand FIRE *
UIKTAG_Joystick_RButFunc    equ (UIKTAG_USER|$0002) *  *
UIKTAG_Joystick_MButFunc    equ (UIKTAG_USER|$0003) *  *
UIKTAG_Joystick_ForwFunc    equ (UIKTAG_USER|$0004) * appelée quand pousse devant *
UIKTAG_Joystick_BackFunc    equ (UIKTAG_USER|$0005) * appelée quand tire vers soit *
UIKTAG_Joystick_LeftFunc    equ (UIKTAG_USER|$0006) *  *
UIKTAG_Joystick_RightFunc   equ (UIKTAG_USER|$0007) *  *
UIKTAG_Joystick_ReleaseFunc equ (UIKTAG_USER|$0008) * appelée quand relâche une direction *
UIKTAG_Joystick_MoveFunc    equ (UIKTAG_USER|$0009) * appelée à chaque évènement *
UIKTAG_Joystick_TimeoutFunc equ (UIKTAG_USER|$000a) * appelée quand timeout sans évènement (répète les directions) *
UIKTAG_Joystick_Controller  equ (UIKTAG_USER|$000b) * défaut GPCT_ABSJOYSTICK, voir <devices/gameport.h> *
UIKTAG_Joystick_Unit        equ (UIKTAG_USER|$000c) * 0 ou défaut 1 *
UIKTAG_Joystick_TimeoutVal  equ (UIKTAG_USER|$000d) * défaut 0, ou un nombre de "vertical blanks" (50 en PAL 60 en NTSC) *
UIKTAG_Joystick_DeltaXY     equ (UIKTAG_USER|$000e) * défaut SETL(1,1), ou le déplacement générant un évènement *
UIKTAG_Joystick_UpDown      equ (UIKTAG_USER|$000f) * défaut (GPTF_DOWNKEYS|GPTF_UPKEYS), voir <devices/gameport.h> *


*---------Obj Joystick Flags---------- *

*-- *

        ENDC
