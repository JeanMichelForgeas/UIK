#ifndef UIKDEFS_OBJ8SVX

#define UIKDEFS_OBJ8SVX
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */


#include <devices/audio.h>

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

#define DIGITBLOCKSIZE  (4*1024)
#define TMPBLOCKSIZE    (256)

#define OBJ8SVX_STOP_AND_PLAY    0
#define OBJ8SVX_WAIT_AND_PLAY    1
#define OBJ8SVX_QUEUE_AND_PLAY   2


struct FileVars {
    ULONG File;
    ULONG DataLen;
    UWORD Period;
    };

struct PlayVars {
    ULONG Used;
    UBYTE *Data;
    struct FileVars *FV;
    struct IOAudio *IOAudio;
    };

struct UIKObj8SVX
    {
    struct UIKObj   Obj;

    struct Library  *DOSBase;
    struct GfxBase  *GfxBase;
    struct UIKBase  *UIKBase;

    struct UIKList  *SoundList;
    struct UIKHook  *SoundEventHook;
    void            (*SoundEventVector)();

    ULONG           Clock;
    struct MsgPort  *AudioPort;
    UBYTE           ASig;
    UBYTE           *TmpBuf;
    ULONG           Device;
    UBYTE           Playing;

    struct PlayVars Play1, Play2;
    struct IOAudio  *IOFinish;
    UBYTE           Channels[4];

    UBYTE           DigitName[256];
    };

/*-------Fonctions internes publiques------*/

#define UIKFUNC_8SVX_StartDigit         0
#define UIKFUNC_8SVX_WaitAudioEnd       4
#define UIKFUNC_8SVX_StopDigit          8
#define UIKFUNC_8SVX_IsAudioPlaying     12

#define UIKFUNC_8SVX_LastCmd            12


/*-----------------------------------------*/

/*----DialFlags----*/

/*---------Obj Dial Tags---------- */

/*---------Obj Dial Flags---------- */

/*-- */

#endif
