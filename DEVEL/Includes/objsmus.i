        IFND    UIKDEFS_OBJSMUS
UIKDEFS_OBJSMUS SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**


    ;IFND SMUS_I
    ;INCLUDE "iff/smus.i"
    ;ENDC

    ;IFND EIGHTSVX_I
    ;INCLUDE "iff/8svx.i"
    ;ENDC

    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC

    IFND UIKDEFS_MACRO
    INCLUDE "UIKI:uikmacros.i"
    ENDC


INLIMIT equ 32      ; 16 Instrument limit (!)

MAIN1 equ 0
MAIN2 equ 4
ATTA1 equ 8
ATTA2 equ 12
STOP  equ 16


 STRUCTURE  VectArg,0
    APTR        uosv_smus
    ULONG       uosv_chn
    LABEL   VectArg_SIZE


 STRUCTURE  UIKObjSMUS,0
    STRUCT      uosm_Obj,UIKOBJ_SIZE

    STRUCT      uosm_UserCurrentFunc,4*4
    STRUCT      uosm_UserEndFunc,4*4
    FPTR        uosm_UserFinishFunc
    STRUCT      uosm_UserParm,4*4
    ULONG       uosm_UserFinishParm
    STRUCT      uosm_UserIndex,4*4

    STRUCT      uosm_VectArgs,VectArg_SIZE*4
    STRUCT      uosm_EventHook,4*4
    STRUCT      uosm_EventVect,4*4

    ;--------score loader part
    APTR        uosm_psdata
    STRUCT      uosm_ptrak,8*4
    LONG        uosm_trakcount
    STRUCT      uosm_IndiRegs,INLIMIT
    LONG        uosm_ScoreInstrumentNo
    ULONG       uosm_AnnoLength
    APTR        uosm_Anno

    ;--------instrument loader part
    LONG        uosm_instrumentno
    LONG        uosm_clock
    LONG        uosm_scount
    APTR        uosm_sbase
    STRUCT      uosm_sbases,INLIMIT*4
    APTR        uosm_ptabptr
    STRUCT      uosm_ptabptrs,INLIMIT*4
    STRUCT      uosm_length,INLIMIT*16*4
    STRUCT      uosm_psample,INLIMIT*16*4

    STRUCT      uosm_InstNames,INLIMIT*60

    ;--------player part
    STRUCT      uosm_AudioIOBptr,20*4
    STRUCT      uosm_port,4*4
    STRUCT      uosm_tport,4*4
    STRUCT      uosm_device,4*4
    STRUCT      uosm_treq,8*4
    STRUCT      uosm_tsent,8
    STRUCT      uosm_timer,4*4
    STRUCT      uosm_inreg,4

    STRUCT      uosm_odev,4
    STRUCT      uosm_mbits,8*4
    STRUCT      uosm_sev,8*4

    ;---copy from loader part for playing
    STRUCT      uosm_pltrak,8*4
    STRUCT      uosm_TTable,64*4
    UWORD       uosm_Tempo
    UWORD       uosm_donecount
    STRUCT      uosm_MaxNotes2Play,4*4

    LONG        uosm_maxtrakcount

    APTR        uosm_DOSBase
    APTR        uosm_UIKBase
    LABEL   UIKObjSMUS_SIZE


;-------Fonctions internes publiques------

UIKFUNC_SMUS_StartPlay              equ  0
UIKFUNC_SMUS_StopPlay               equ  4
UIKFUNC_SMUS_IsPlaying              equ  8
UIKFUNC_SMUS_LoadSMUS               equ 12 ; retourne IFF_DONE, ou code iff si erreur
UIKFUNC_SMUS_UnloadSMUS             equ 16
UIKFUNC_SMUS_LoadInstrument         equ 20 ; retourne le n° de l'instrument, ou code iff si erreur (<0)
;UIKFUNC_SMUS_UnloadInstrument       equ 24
UIKFUNC_SMUS_UnloadAllInstr         equ 28
UIKFUNC_SMUS_SetTempo               equ 32
UIKFUNC_SMUS_NewSMUS                equ 36
UIKFUNC_SMUS_ChangeChanelInst       equ 40
UIKFUNC_SMUS_SaveSMUS               equ 44
UIKFUNC_SMUS_SetAnnoPointer         equ 48
UIKFUNC_SMUS_GetAnnoPointer         equ 52
UIKFUNC_SMUS_LoadInstrumentShared   equ 56

UIKFUNC_SMUS_LastCmd                equ 56


;-----------------------------------------

;----SMUSFlags----
UIKF_SMUS_              equ $0001

;---------Obj SMUS Tags----------
UIKTAG_OBJSMUS_UserCurrentFunc0     equ (UIKTAG_USER|$0001) /*  */
UIKTAG_OBJSMUS_UserEndFunc0         equ (UIKTAG_USER|$0002) /*  */
UIKTAG_OBJSMUS_UserParm0            equ (UIKTAG_USER|$0003) /*  */
UIKTAG_OBJSMUS_UserCurrentFunc1     equ (UIKTAG_USER|$0004) /*  */
UIKTAG_OBJSMUS_UserEndFunc1         equ (UIKTAG_USER|$0005) /*  */
UIKTAG_OBJSMUS_UserParm1            equ (UIKTAG_USER|$0006) /*  */
UIKTAG_OBJSMUS_UserCurrentFunc2     equ (UIKTAG_USER|$0007) /*  */
UIKTAG_OBJSMUS_UserEndFunc2         equ (UIKTAG_USER|$0008) /*  */
UIKTAG_OBJSMUS_UserParm2            equ (UIKTAG_USER|$0009) /*  */
UIKTAG_OBJSMUS_UserCurrentFunc3     equ (UIKTAG_USER|$000a) /*  */
UIKTAG_OBJSMUS_UserEndFunc3         equ (UIKTAG_USER|$000b) /*  */
UIKTAG_OBJSMUS_UserParm3            equ (UIKTAG_USER|$000c) /*  */
UIKTAG_OBJSMUS_UserFinishFunc       equ (UIKTAG_USER|$000d) /*  */
UIKTAG_OBJSMUS_UserFinishParm       equ (UIKTAG_USER|$000e) /*  */
UIKTAG_OBJSMUS_MaxTrackCount        equ (UIKTAG_USER|$000f) /*  */

;---------Obj SMUS Flags----------
UIKTAG_OBJSMUS_Flags    equ (UIKTAG_USER|$0020)
UIKTAG_OBJSMUSFl_       equ (UIKTAG_USER|$0021)

;---------UIKFUNC_SMUS_NewSMUS NewSMUS Tags----------

UIKTAG_NEWSMUS_Instrument       equ (UIKTAG_USER|$0101)
UIKTAG_NEWSMUS_TrackStart       equ (UIKTAG_USER|$0102)
UIKTAG_NEWSMUS_TrackEnd         equ (UIKTAG_USER|$0104)
UIKTAG_NEWSMUS_Tempo            equ (UIKTAG_USER|$0108)

;--

        ENDC
