        IFND    UIKDEFS_AREXXSIM
UIKDEFS_AREXXSIM    SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**


    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC


 STRUCTURE  UIKObjARexxSim,0
    STRUCT      uoas_Obj,UIKOBJ_SIZE
    APTR        uoas_Hook
    APTR        uoas_EventVect
    APTR        uoas_NewMsgFunc
    APTR        uoas_CmdBackFunc
    APTR        uoas_RexxStuff
    APTR        uoas_UIKBase
    APTR        uoas_CmdReceiveFunc

    LONG        uoas_ResultCode
    LONG        uoas_ErrorCode
    APTR        uoas_ResultString
    APTR        uoas_ErrorString
    ULONG       uoas_Flags
    APTR        uoas_RMsg
    LABEL   uoas_SIZE
    LABEL   UIKObjARexxSim_SIZE

; Flags
UASF_ENABLEKEEP     equ $0001

 STRUCTURE  UIKRexxCustom,0
    LONG        urc_Action
    LONG        urc_Stdin
    LONG        urc_Stdout
    LABEL   UIKRexxCustom_SIZE


UIKFUNC_ARexxSim_ARexxName          equ     0
UIKFUNC_ARexxSim_SendARexxCmd       equ     4
UIKFUNC_ARexxSim_SendARexxCmdCustom equ     8
UIKFUNC_ARexxSim_SendSyncCmd        equ    12
UIKFUNC_ARexxSim_SendSyncCmdCustom  equ    16
UIKFUNC_ARexxSim_ReplyARexxMsg      equ    20

UIKFUNC_ARexxSim_LastCmd            equ    20


*---------Obj ARexxSim Tags----------*
UIKTAG_ARexxSim_AppName         equ (UIKTAG_USER|$0001)
UIKTAG_ARexxSim_Extension       equ (UIKTAG_USER|$0002)
UIKTAG_ARexxSim_NewMsgFunc      equ (UIKTAG_USER|$0003) ; obsolete
UIKTAG_ARexxSim_CmdBackFunc     equ (UIKTAG_USER|$0004)
UIKTAG_ARexxSim_CmdReceiveFunc  equ (UIKTAG_USER|$0005)
UIKTAG_ARexxSim_PortNameDot     equ (UIKTAG_USER|$0006)
UIKTAG_ARexxSim_PortNameNoNum   equ (UIKTAG_USER|$0007)
UIKTAG_ARexxSim_VarName         equ (UIKTAG_USER|$0008)

*---------Obj ARexxSim Flags----------*
UIKTAG_ARexxSimFl_EnableKeep    equ (UIKTAG_USER|$0100)

*--*


        ENDC
