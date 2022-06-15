#ifndef UIKDEFS_AREXXSIM

#define UIKDEFS_AREXXSIM
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

#ifndef UIKDEFS_TAGITEM_H
#include "uiki:uiktag.h"
#endif

#ifndef AREXXCONTEXT
typedef void *AREXXCONTEXT;
#endif /* AREXXCONTEXT */

struct UIKObjARexxSim
    {
    struct UIKObj       Obj;
    struct UIKHook      *Hook;
    VOID                (*EventVect)();
    LONG                (*NewMsgFunc)();
    LONG                (*CmdBackFunc)();
    AREXXCONTEXT        RexxStuff;
    struct UIKBase      *UIKBase;
    LONG                (*CmdReceiveFunc)();

    LONG    ResultCode;
    LONG    ErrorCode;
    char    *ResultString;
    char    *ErrorString;
    ULONG   Flags;
    APTR    RMsg;
    };

/* Flags */
#define UASF_ENABLEKEEP     0x0001
#define UASF_LEAVECASE      0x0002

struct UIKRexxCustom
    {
    LONG    Action;      /* command (action) code */
    LONG    Stdin;       /* input stream (filehandle) */
    LONG    Stdout;      /* output stream (filehandle) */
    };

#define UIKFUNC_ARexxSim_ARexxName          0
#define UIKFUNC_ARexxSim_SendARexxCmd       4
#define UIKFUNC_ARexxSim_SendARexxCmdCustom 8
#define UIKFUNC_ARexxSim_SendSyncCmd       12
#define UIKFUNC_ARexxSim_SendSyncCmdCustom 16
#define UIKFUNC_ARexxSim_ReplyARexxMsg     20

#define UIKFUNC_ARexxSim_LastCmd           20


/*---------Obj ARexxSim Tags---------- */
#define UIKTAG_ARexxSim_AppName         (UIKTAG_USER|0x0001)
#define UIKTAG_ARexxSim_Extension       (UIKTAG_USER|0x0002)
#define UIKTAG_ARexxSim_NewMsgFunc      (UIKTAG_USER|0x0003) /* obsolete */
#define UIKTAG_ARexxSim_CmdBackFunc     (UIKTAG_USER|0x0004)
#define UIKTAG_ARexxSim_CmdReceiveFunc  (UIKTAG_USER|0x0005)
#define UIKTAG_ARexxSim_PortNameDot     (UIKTAG_USER|0x0006)
#define UIKTAG_ARexxSim_PortNameNoNum   (UIKTAG_USER|0x0007)
#define UIKTAG_ARexxSim_VarName         (UIKTAG_USER|0x0008)

/*---------Obj ARexxSim Flags---------- */
#define UIKTAG_ARexxSimFl_EnableKeep    (UIKTAG_USER|0x0100)
#define UIKTAG_ARexxSimFl_LeaveCase     (UIKTAG_USER|0x0200)

/*-- */

#endif
