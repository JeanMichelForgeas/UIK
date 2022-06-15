#ifndef UIKDEFS_OBJSMUS

#define UIKDEFS_OBJSMUS
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

#ifndef UIKDEFS_MACRO
#include "uiki:uikmacros.h"
#endif

#define INLIMIT 32      /* 16 Instrument limit (!) */

#define MAIN1 0
#define MAIN2 4
#define ATTA1 8
#define ATTA2 12
#define STOP  16

#ifndef IFF_H
typedef struct {
    UBYTE sID;          /* SEvent type code */
    UBYTE data;         /* sID-dependent data */
    } SEvent;
#define IFF_DONE  -2L   /* clientProc returns this when it has READ enough.
                         * It means return thru all levels. File is Okay.*/
#endif

struct VectArg {
    struct UIKObjSMUS *smus;
    ULONG             chn;
    };

struct UIKObjSMUS {
    struct UIKObj   Obj;

    void            (*UserCurrentFunc[4])();
    void            (*UserEndFunc[4])();
    void            (*UserFinishFunc)();
    ULONG           UserParm[4];
    ULONG           UserFinishParm;
    ULONG           UserIndex[4];

    struct VectArg  VectArgs[4];
    struct UIKHook  *EventHook[4];
    void            (*EventVect[4])();

    /*--------score loader part */
    UBYTE           *psdata;        /* Pointer to start of SMUS data   */
    SEvent          *ptrak[8];      /* 1st and last events for 4 traks */
    LONG            trakcount;      /* Number of traks */
    UBYTE           IndiRegs[INLIMIT];
    LONG            ScoreInstrumentNo;
    ULONG           AnnoLength;
    UBYTE           *Anno;

    /*--------instrument loader part */
    LONG            instrumentno, clock, scount;
    UBYTE           *sbase,*sbases[INLIMIT];/* Pointers to sample block */
    LONG            *ptabptr,               /* and calculated periods   */
                    *ptabptrs[INLIMIT],     /* for 12 tones.  Table of  */
                    length[16*INLIMIT];     /* Lengths and pointers to the */
    BYTE            *psample[16*INLIMIT];   /* attack and main parts of 8  */
                                            /* octaves for each instrument */
    UBYTE           InstNames[INLIMIT][60]; /* rempli par inst.c */

    /*--------player part */
    struct IOAudio      *AudioIOBptr[20];   /* 2 attack parts, 2 main parts and  */
    struct MsgPort      *port[4];           /* 4 ports for attack/main AIOBs     */
    struct MsgPort      *tport[4];          /* 4 ports for timeouts              */
    ULONG               device[4];          /* Treat as 4 seperate audio devices */
    struct timerequest  *treq[8];           /* 2 x 4 timer request IOBs          */
    UBYTE               tsent[8];           /* Treat as 4 seperate timers        */
    ULONG               timer[4];           /* Treat as 4 seperate timers        */
    UBYTE               inreg[4];           /* 4 intrument registers    */

    UBYTE               odev[4];
    ULONG               mbits[8];
    SEvent              *sev[8];

    /*---copy from loader part for playing */
    SEvent          *pltrak[8];     /* 1st and last events for 4 traks */
    ULONG           TTable[64];
    UWORD           Tempo;          /* tempo, 128ths quarter note/minute */
    UWORD           donecount;
    ULONG           MaxNotes2Play[4];

    LONG            maxtrakcount;

    struct DosLibrary   *DOSBase;
    struct UIKBase      *UIKBase;
    };


/*-------Fonctions internes publiques------*/

#define UIKFUNC_SMUS_StartPlay               0
#define UIKFUNC_SMUS_StopPlay                4
#define UIKFUNC_SMUS_IsPlaying               8
#define UIKFUNC_SMUS_LoadSMUS               12 /* retourne IFF_DONE, ou code iff si erreur */
#define UIKFUNC_SMUS_UnloadSMUS             16
#define UIKFUNC_SMUS_LoadInstrument         20 /* retourne le n° de l'instrument, ou code iff si erreur (<0) */
/*#define UIKFUNC_SMUS_UnloadInstrument     24*/
#define UIKFUNC_SMUS_UnloadAllInstr         28
#define UIKFUNC_SMUS_SetTempo               32
#define UIKFUNC_SMUS_NewSMUS                36
#define UIKFUNC_SMUS_ChangeChanelInst       40
#define UIKFUNC_SMUS_SaveSMUS               44
#define UIKFUNC_SMUS_SetAnnoPointer         48
#define UIKFUNC_SMUS_GetAnnoPointer         52
#define UIKFUNC_SMUS_LoadInstrumentShared   56

#define UIKFUNC_SMUS_LastCmd                56


/*-----------------------------------------*/

/*----SMUSFlags----*/
#define UIKF_SMUS_              0x0001

/*---------Obj SMUS Tags---------- */
#define UIKTAG_OBJSMUS_UserCurrentFunc0     (UIKTAG_USER|0x0001) /*  */
#define UIKTAG_OBJSMUS_UserEndFunc0         (UIKTAG_USER|0x0002) /*  */
#define UIKTAG_OBJSMUS_UserParm0            (UIKTAG_USER|0x0003) /*  */
#define UIKTAG_OBJSMUS_UserCurrentFunc1     (UIKTAG_USER|0x0004) /*  */
#define UIKTAG_OBJSMUS_UserEndFunc1         (UIKTAG_USER|0x0005) /*  */
#define UIKTAG_OBJSMUS_UserParm1            (UIKTAG_USER|0x0006) /*  */
#define UIKTAG_OBJSMUS_UserCurrentFunc2     (UIKTAG_USER|0x0007) /*  */
#define UIKTAG_OBJSMUS_UserEndFunc2         (UIKTAG_USER|0x0008) /*  */
#define UIKTAG_OBJSMUS_UserParm2            (UIKTAG_USER|0x0009) /*  */
#define UIKTAG_OBJSMUS_UserCurrentFunc3     (UIKTAG_USER|0x000a) /*  */
#define UIKTAG_OBJSMUS_UserEndFunc3         (UIKTAG_USER|0x000b) /*  */
#define UIKTAG_OBJSMUS_UserParm3            (UIKTAG_USER|0x000c) /*  */
#define UIKTAG_OBJSMUS_UserFinishFunc       (UIKTAG_USER|0x000d) /*  */
#define UIKTAG_OBJSMUS_UserFinishParm       (UIKTAG_USER|0x000e) /*  */
#define UIKTAG_OBJSMUS_MaxTrackCount        (UIKTAG_USER|0x000f) /*  */

/*---------Obj SMUS Flags---------- */
#define UIKTAG_OBJSMUS_Flags    (UIKTAG_USER|0x0020)
#define UIKTAG_OBJSMUSFl_       (UIKTAG_USER|0x0021)

/*---------UIKFUNC_SMUS_NewSMUS NewSMUS Tags---------- */

#define UIKTAG_NEWSMUS_Instrument       (UIKTAG_USER|0x0101)
#define UIKTAG_NEWSMUS_TrackStart       (UIKTAG_USER|0x0102)
#define UIKTAG_NEWSMUS_TrackEnd         (UIKTAG_USER|0x0104)
#define UIKTAG_NEWSMUS_Tempo            (UIKTAG_USER|0x0108)

/*-- */

#endif
