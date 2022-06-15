#ifndef UIKDEFS_OBJTIMER

#define UIKDEFS_OBJTIMER
/*
 *      Copyright (C) 1993 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

#ifndef DEVICES_TIMER_H
#include <devices/timer.h>
#endif

struct UIKObjTimer
    {
    struct UIKObj       Obj;
    struct UIKBase      *UIKBase;
    struct Library      *TimerBase;

    struct UIKHook      *EventHook;
    void                (*EventVect)();

    void                (*ClientFunc)();
    struct timeval      AskedInterval;  /* interval demandé: pris en compte à chaque UIK_Start() */

    struct MsgPort      *Port;
    struct timerequest  *Req;
    struct timerequest  *Cmd;
    struct timeval      PreviousTime;
    struct timeval      StartTime;      /* temps depuis écoulé le Timer_Start */
    struct timeval      CorrInterval;   /* interval corrigé */
    struct timeval      TotalTime;      /* temps total théorique */
    ULONG               EventCounter;   /* nombre de réveils du timer */
    ULONG               Flags;
    ULONG               Unit;           /* UNIT_MICROHZ ou UNIT_VBLANK */
    };

#define CHECK_TIMEIO(io) if ((io)->tv_secs == 0 && (io)->tv_micro < 2) (io)->tv_micro = 2

#define ONESECOND           1000000
#define DEFAULT_GRANULARITY 10

/*--------- Flags ----------*/
#define UIKTF_STARTED       0x0001
#define UIKTF_CORRECTION    0x0002

/*---------Obj Timer Tags-----------*/
#define UIKTAG_Timer_TimeFunc       (UIKTAG_USER|0x0001) /* ...on execute cette fonction */
#define UIKTAG_Timer_Seconds        (UIKTAG_USER|0x0002) /* toutes les n secondes... */
#define UIKTAG_Timer_Micros         (UIKTAG_USER|0x0003) /* et n micro secondes */
#define UIKTAG_Timer_Unit           (UIKTAG_USER|0x0004) /* UNIT_MICROHZ ou UNIT_VBLANK */

/*---------Obj Timer Tag Flags----------*/
#define UIKTAG_Timer_Flags (UIKTAG_USER|0x0020) /* pour remplacer les défauts */
 /* sinon un par un... */
#define UIKTAG_TimerFl_Correction   (UIKTAG_USER|0x0021) /* FALSE */

/*---------Functions------*/
#define UIKFUNC_Timer_SetInterval   0   /* ( secondes, micros ) */
#define UIKFUNC_Timer_Start         4   /* () */
#define UIKFUNC_Timer_Stop          8   /* () */
#define UIKFUNC_Timer_IsStarted    12   /* () */
#define UIKFUNC_Timer_GetTime      16   /* () */
#define UIKFUNC_Timer_GetElapsed   20   /* () */

#define UIKFUNC_Timer_LastCmd      20

/*---------Divers---------*/

#endif
