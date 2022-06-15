        IFND    UIKDEFS_OBJTIMER

UIKDEFS_OBJTIMER  SET     1
**
**      Copyright (C) 1993 Jean-Michel Forgeas
**                  Tous Droits Réservés
**

    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC

    IFND DEVICES_TIMER_I
    INCLUDE "devices/timer.i"
    ENDC

 STRUCTURE  UIKObjTimer,0
    STRUCT      uoti_Obj,UIKOBJ_SIZE
    APTR        uoti_UIKBase
    APTR        uoti_TimerBase

    APTR        uoti_EventHook
    FPTR        uoti_EventVect

    FPTR        uoti_ClientFunc
    STRUCT      uoti_AskedInterval,TV_SIZE

    APTR        uoti_Port
    APTR        uoti_Req
    APTR        uoti_Cmd
    STRUCT      uoti_PreviousTime,TV_SIZE
    STRUCT      uoti_StartTime,TV_SIZE
    STRUCT      uoti_CorrInterval,TV_SIZE
    STRUCT      uoti_TotalTime,TV_SIZE
    ULONG       uoti_EventCounter
    ULONG       uoti_Flags
    ULONG       uoti_Unit
    LABEL   UIKObjTimer_SIZE


ONESECOND           equ 1000000
DEFAULT_GRANULARITY equ 10

;--------- Flags ----------
UIKTF_STARTED       equ $0001
UIKTF_CORRECTION    equ $0002

;---------Obj Timer Tags-----------
UIKTAG_Timer_TimeFunc       equ (UIKTAG_USER|$0001)
UIKTAG_Timer_Seconds        equ (UIKTAG_USER|$0002)
UIKTAG_Timer_Micros         equ (UIKTAG_USER|$0003)
UIKTAG_Timer_Unit           equ (UIKTAG_USER|$0004)

;---------Obj Timer Tag Flags----------
UIKTAG_Timer_Flags          equ (UIKTAG_USER|$0020)
 ; sinon un par un...
UIKTAG_TimerFl_Correction   equ (UIKTAG_USER|$0021)

;---------Functions------
UIKFUNC_Timer_SetInterval   equ  0
UIKFUNC_Timer_Start         equ  4
UIKFUNC_Timer_Stop          equ  8
UIKFUNC_Timer_IsStarted     equ 12
UIKFUNC_Timer_GetTime       equ 16
UIKFUNC_Timer_GetElapsed    equ 20

UIKFUNC_Timer_LastCmd       equ 20

;---------Divers---------

        ENDC
