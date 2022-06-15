        IFND    UIKDEFS_OBJ8SVX
UIKDEFS_OBJ8SVX SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**


    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC


DIGITBLOCKSIZE  equ (4*1024)
TMPBLOCKSIZE    equ (256)

OBJ8SVX_STOP_AND_PLAY    equ 0
OBJ8SVX_WAIT_AND_PLAY    equ 1
OBJ8SVX_QUEUE_AND_PLAY   equ 2


 STRUCTURE  FileVars,0
    ULONG       fv8_File
    ULONG       fv8_DataLen
    UWORD       fv8_Period
    LABEL   fv8_SIZE

 STRUCTURE  PlayVars,0
    ULONG       pv8_Used
    APTR        pv8_Data
    APTR        pv8_FV
    APTR        pv8_IOAudio
    LABEL   pv8_SIZE


 STRUCTURE  UIKObj8SVX,0
    STRUCT      uo8_Obj,UIKOBJ_SIZE

    APTR        uo8_DOSBase
    APTR        uo8_GfxBase
    APTR        uo8_UIKBase

    APTR        uo8_SoundList
    APTR        uo8_SoundEventHook
    FPTR        uo8_SoundEventVector

    ULONG       uo8_Clock
    APTR        uo8_AudioPort
    UBYTE       uo8_ASig
    APTR        uo8_TmpBuf
    ULONG       uo8_Device
    UBYTE       uo8_Playing

    STRUCT      uo8_Play1,pv8_SIZE
    STRUCT      uo8_Play2,pv8_SIZE
    APTR        uo8_IOFinish
    STRUCT      uo8_Channels,4

    STRUCT      uo8_DigitName,256

    LABEL   uo8_SIZE
    LABEL   UIKObj8SVX_SIZE


*---------Fonctions internes publiques---------- *
UIKFUNC_8SVX_StartDigit         equ 0
UIKFUNC_8SVX_WaitAudioEnd       equ 4
UIKFUNC_8SVX_StopDigit          equ 8
UIKFUNC_8SVX_IsAudioPlaying     equ 12

UIKFUNC_8SVX_LastCmd            equ 12


*----DialFlags----*

*---------Obj Dial Tags---------- *

*---------Obj Dial Flags---------- *


        ENDC
