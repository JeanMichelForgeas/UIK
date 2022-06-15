#ifndef UIKDEFS_JOYSTICK

#define UIKDEFS_JOYSTICK
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

#ifndef DEVICES_GAMEPORT_H
#include <devices/gameport.h>
#endif

#ifndef DEVICES_INPUTEVENT_H
#include <devices/inputevent.h>
#endif

struct UIKObjJoystick
    {
    struct UIKObj           Obj;
    struct Library          *DOSBase;
    struct Library          *UIKBase;
    void                    (*LButFunc)();
    void                    (*RButFunc)();
    void                    (*MButFunc)();
    void                    (*ForwFunc)();
    void                    (*BackFunc)();
    void                    (*LeftFunc)();
    void                    (*RightFunc)();
    void                    (*ReleaseFunc)();
    void                    (*MoveFunc)();
    void                    (*TimeoutFunc)();
    UBYTE                   Unit;
    UBYTE                   Controller;
    UWORD                   padw;

    struct UIKHook          *EventHook;
    void                    (*EventVect)();
    struct MsgPort          *Port;
    struct IOStdReq         *Req;
    struct Device           *Device;
    struct GamePortTrigger  Trigger;
    struct InputEvent       IEV1;
    struct InputEvent       IEV2;
    struct InputEvent       *ActiveEv;
    struct InputEvent       *ReceivedEv;
    ULONG                   Started;
    };


/*--------contacts flags--------*/
#define JOYF_LBUTTON         0x01
#define JOYF_RBUTTON         0x02
#define JOYF_MBUTTON         0x04
#define JOYF_FORWARD         0x08
#define JOYF_BACKWARD        0x10
#define JOYF_LEFT            0x20
#define JOYF_RIGHT           0x40
#define JOYF_TIMEOUT         0x80

#define JOYB_LBUTTON            0
#define JOYB_RBUTTON            1
#define JOYB_MBUTTON            2
#define JOYB_FORWARD            3
#define JOYB_BACKWARD           4
#define JOYB_LEFT               6
#define JOYB_RIGHT              5
#define JOYB_TIMEOUT            7
/*=========================================*/

/*--------directions values-----*/
#define JOYDIR_NONE            -1
#define JOYDIR_FORWARD          0
#define JOYDIR_FORWARD_RIGHT    1
#define JOYDIR_RIGHT            2
#define JOYDIR_BACKWARD_RIGHT   3
#define JOYDIR_BACKWARD         4
#define JOYDIR_BACKWARD_LEFT    5
#define JOYDIR_LEFT             6
#define JOYDIR_FORWARD_LEFT     7

/* fonctions publiques
 */
#define OBJFUNC_Joystick_Flush      0
#define OBJFUNC_Joystick_LastCmd    0


/*---------Obj Joystick Tags---------- */
#define UIKTAG_Joystick_LButFunc    (UIKTAG_USER|0x0001) /* appelée quand FIRE */
#define UIKTAG_Joystick_RButFunc    (UIKTAG_USER|0x0002) /*  */
#define UIKTAG_Joystick_MButFunc    (UIKTAG_USER|0x0003) /*  */
#define UIKTAG_Joystick_ForwFunc    (UIKTAG_USER|0x0004) /* appelée quand pousse devant */
#define UIKTAG_Joystick_BackFunc    (UIKTAG_USER|0x0005) /* appelée quand tire vers soit */
#define UIKTAG_Joystick_LeftFunc    (UIKTAG_USER|0x0006) /*  */
#define UIKTAG_Joystick_RightFunc   (UIKTAG_USER|0x0007) /*  */
#define UIKTAG_Joystick_ReleaseFunc (UIKTAG_USER|0x0008) /* appelée quand relâche une direction */
#define UIKTAG_Joystick_MoveFunc    (UIKTAG_USER|0x0009) /* appelée à chaque évènement */
#define UIKTAG_Joystick_TimeoutFunc (UIKTAG_USER|0x000a) /* appelée quand timeout sans évènement (répète les directions) */
#define UIKTAG_Joystick_Controller  (UIKTAG_USER|0x000b) /* défaut GPCT_ABSJOYSTICK, voir <devices/gameport.h> */
#define UIKTAG_Joystick_Unit        (UIKTAG_USER|0x000c) /* 0 ou défaut 1 */
#define UIKTAG_Joystick_TimeoutVal  (UIKTAG_USER|0x000d) /* défaut 0, ou un nombre de "vertical blanks" (50 en PAL 60 en NTSC) */
#define UIKTAG_Joystick_DeltaXY     (UIKTAG_USER|0x000e) /* défaut SETL(1,1), ou le déplacement générant un évènement */
#define UIKTAG_Joystick_UpDown      (UIKTAG_USER|0x000f) /* défaut (GPTF_DOWNKEYS|GPTF_UPKEYS), voir <devices/gameport.h> */


/*---------Obj Joystick Flags---------- */

/*-- */

#endif
