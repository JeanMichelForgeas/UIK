/****************************************************************
 *
 *      File:      objtimer.c
 *      Project:   UIK
 *
 *      Created:   30/03/93     Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikglobal.h"
#include "uiki:uikcolors.h"
#include "uiki:uikmacros.h"
#include "uiki:uiktag.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objtimer.h"

#include <devices/timer.h>
#include <clib/timer_protos.h>
#include <pragmas/timer_pragmas.h>


/****** Imported ************************************************/


/****** Exported ***********************************************/

ULONG __far SysBase;
struct UIKBase * __far UIKBase;
struct Device * __far TimerBase;


/****** Statics ************************************************/

static void FuncStop();
static void TimerStop();

static struct TagItem __far TagBoolMap[] =
    {
    { UIKTAG_TimerFl_Correction, UIKTF_CORRECTION },
    { TAG_DONE }
    };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void get_systime( struct UIKObjTimer *timer )
{
    timer->Cmd->tr_node.io_Command = TR_GETSYSTIME;
    DoIO( timer->Cmd );
}

static void send_delay( struct UIKObjTimer *timer )
{
    timer->Req->tr_node.io_Command = TR_ADDREQUEST;
    timer->Req->tr_time = timer->CorrInterval;
    SendIO( timer->Req );
}

//---event function

static void EventFunc( struct UIKObjTimer *timer, ULONG received, struct UIKHook *hook )
{
    get_systime( timer ); // set timer->Cmd->tr_time
    WaitIO( timer->Req );

    ++timer->EventCounter;

    if (timer->Flags & UIKTF_CORRECTION)
        {
        struct timeval elaps, tmp;

        elaps = timer->Cmd->tr_time;
        SubTime( &elaps, &timer->PreviousTime );    // elaps = time elapsed since PreviousTime
        switch (CmpTime( &elaps, &timer->AskedInterval ))
            {
            case -1: //------ elapsed greater than asked --> we are late!
                SubTime( &elaps, &timer->AskedInterval );         // elaps == howmuch_larger
                if (CmpTime( &elaps, &timer->CorrInterval ) == 1) // howmuch_larger < corr ?
                    {
                    SubTime( &timer->CorrInterval, &elaps );      // corr -= howmuch_larger
                    CHECK_TIMEIO( &timer->CorrInterval );         // 2 micros minimum
                    }
                else{
                    timer->CorrInterval.tv_secs = 0;
                    timer->CorrInterval.tv_micro = 2;             // minimum
                    }
                break;
            case 1:  //------ elapsed shorter than asked --> must wait more time
                tmp = timer->AskedInterval;
                SubTime( &tmp, &elaps );                          // tmp == howmuch_larger
                AddTime( &timer->CorrInterval, &tmp );            // corr += howmuch_larger
                break;
            case 0:  //------ equals
            default:                                              // corr is perfect!
                break;
            }
        timer->PreviousTime = timer->Cmd->tr_time;
        }

    if (timer->Flags & UIKTF_STARTED)
        {
        send_delay( timer );

        AddTime( &timer->TotalTime, &timer->AskedInterval );
        if (timer->ClientFunc) (*timer->ClientFunc)( timer );

        timer->EventVect = (void*)EventFunc;
        }
}

//--------------------generic functions

static void Generic_Remove( struct UIKObjTimer *timer )
{
    if (timer->UIKBase)
        {
        UIK_RemoveVector( timer->Obj.UIK, timer->EventHook );
        if (timer->Port)
            {
            if (timer->Req)
                {
                if (timer->Cmd)
                    {
                    TimerStop( timer );
                    if (timer->TimerBase) CloseDevice( timer->Req );
                    DeleteExtIO( timer->Cmd );
                    }
                DeleteExtIO( timer->Req );
                }
            DeletePort( timer->Port );
            }
        CloseLibrary( timer->UIKBase );
        }
}

static int Generic_Add( struct UIKObjTimer *timer, struct TagList *taglist )
{
    SysBase = (*(ULONG*)4);
    if (! (timer->UIKBase = UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;

    timer->Flags = UIK_GetTagData( UIKTAG_Timer_Flags, 0, taglist );
    timer->Flags = (ULONG) UIK_PackBoolTags( (ULONG)timer->Flags, taglist, TagBoolMap );

    timer->AskedInterval.tv_secs = UIK_GetTagData( UIKTAG_Timer_Seconds, 0, taglist );
    timer->AskedInterval.tv_micro = UIK_GetTagData( UIKTAG_Timer_Micros, 20000, taglist ); // 1/50 sec
    CHECK_TIMEIO( &timer->AskedInterval );
    timer->ClientFunc = (void*) UIK_GetTagData( UIKTAG_Timer_TimeFunc, 0, taglist );
    timer->Unit = UIK_GetTagData( UIKTAG_Timer_Unit, UNIT_MICROHZ, taglist );
    if (timer->Unit != UNIT_MICROHZ && timer->Unit != UNIT_VBLANK) goto END_ERROR;

    //---timer.device stuff
    if (! (timer->Port = (struct MsgPort *) CreatePort( 0, 0 ))) goto END_ERROR;
    if (! (timer->Req = (struct timerequest *) CreateExtIO( timer->Port, sizeof(struct timerequest) ))) goto END_ERROR;
    if (! (timer->Cmd = (struct timerequest *) CreateExtIO( timer->Port, sizeof(struct timerequest) ))) goto END_ERROR;
    if (OpenDevice( TIMERNAME, timer->Unit, (struct IORequest *) timer->Req, 0L )) goto END_ERROR;
    timer->TimerBase = TimerBase = (struct Library *) timer->Req->tr_node.io_Device;

    //---to avoid CheckIO() problem with timer->Req
    timer->Req->tr_node.io_Command = TR_GETSYSTIME;
    DoIO( timer->Req );
    CopyMem( timer->Req, timer->Cmd, sizeof(struct timerequest) );

    //---Hook to execute every time the io comes back
    timer->EventVect = (void*)EventFunc;
    if (! (timer->EventHook = UIK_AddVector( timer->Obj.UIK, (void*)&timer->EventVect, 1L<<timer->Port->mp_SigBit, (ULONG)timer ))) goto END_ERROR;
    return(1);

  END_ERROR:
    return(0);
}

static void Generic_Stop( struct UIKObjTimer *timer )
{
    TimerStop( timer );
}

//-------------------public functions

static int TimerStart( struct UIKObjTimer *timer )
{
    //-----------get the current starting system time
    get_systime( timer );
    timer->StartTime = timer->PreviousTime = timer->Cmd->tr_time;

    //-----------begin the thing
    timer->CorrInterval = timer->AskedInterval;
    send_delay( timer );

    timer->EventVect = (void*)EventFunc;
    timer->Flags |= UIKTF_STARTED;
    return(1);

  END_ERROR:
    return(0);
}

static void TimerStop( struct UIKObjTimer *timer )
{
    timer->EventVect = 0;
    if (! CheckIO( timer->Req )) AbortIO( timer->Req );
    WaitIO( timer->Req );
    SetSignal( 0, 1L << timer->Port->mp_SigBit );
    timer->Flags &= ~UIKTF_STARTED;
}

static int FuncStart( struct UIKObjTimer *timer )
{
  int rc=0;

    if (! (timer->Flags & UIKTF_STARTED))
        {
        rc = TimerStart( timer );
        timer->EventCounter = 0;
        timer->TotalTime.tv_secs = 0;
        timer->TotalTime.tv_micro = 0;
        }
    return( rc );
}

static void FuncStop( struct UIKObjTimer *timer )
{
    if (timer->Flags & UIKTF_STARTED) TimerStop( timer );
}

static int SetInterval( struct UIKObjTimer *timer, ULONG secs, ULONG micro )
{
    timer->AskedInterval.tv_secs = secs;
    timer->AskedInterval.tv_micro = micro;
    CHECK_TIMEIO( &timer->AskedInterval );
    if (timer->Flags & UIKTF_STARTED)
        {
        TimerStop( timer );
        if (! TimerStart( timer )) return(0);
        }
    return(1);
}

static ULONG IsStarted( struct UIKObjTimer *timer )
{
    return( (timer->Flags & UIKTF_STARTED) );
}

static struct timeval *GetTime( struct UIKObjTimer *timer )
{
    get_systime( timer );
    return( &timer->Cmd->tr_time );
}

static struct timeval *GetElapsedTime( struct UIKObjTimer *timer )
{
    get_systime( timer );
    SubTime( &timer->Cmd->tr_time, &timer->StartTime );
    return( &timer->Cmd->tr_time );
}

static ULONG __far FuncTable[] = {
    (ULONG) SetInterval,
    (ULONG) FuncStart,
    (ULONG) FuncStop,
    (ULONG) IsStarted,
    (ULONG) GetTime,
    (ULONG) GetElapsedTime,
    };


/****************************************************************
 *
 *      Germe
 *
 ****************************************************************/

static struct TagItem __far GermTagList[] =
{
   { UIKTAG_GEN_VersionRevision, W2L(1,3) },
   { UIKTAG_GEN_LastFuncId, UIKFUNC_Timer_LastCmd },
   { TAG_END }
};

struct UIKObjGerm __far Germ = {
    "UIKObj_Timer",     /* */
    Generic_Add,        /* int     (*AddVector)(); */
    NULL,               /* int     (*StartVector)(); */
    Generic_Stop,       /* void    (*StopVector)(); */
    Generic_Remove,     /* void    (*RemoveVector)(); */
    NULL,               /* void    (*RefreshVector)(); */
    NULL,               /* void    (*ResizeVector)(); */
    NULL,               /* void    (*ExceptActVector)(); */
    NULL,               /* void    (*ExceptInaVector)(); */
    NULL,               /* void    (*ExceptMovVector)(); */
    NULL,               /* void    (*ExceptKeyVector)(); */
    NULL,               /* int     (*StartAfterVector)(); */
    NULL,               /* int     (*SetGetValues)(); */
    FuncTable,          /* ULONG   *ExtentedVectors; */
    GermTagList,        /*struct TagItem *GermTagList; */
    0,                  /* UWORD   MinWidth; */
    0,                  /* UWORD   MinHeight; */
    0,                  /* ULONG   DefaultWidthHeight; */
    UIKID_TIMER,        /* UWORD   ObjId; */
    sizeof(struct UIKObjTimer), /* UWORD   ObjSize; */
    0,                  /* ULONG   DefaultFlags */
    };
