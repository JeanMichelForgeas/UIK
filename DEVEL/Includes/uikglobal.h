#ifndef UIKDEFS_GLOBAL

#define UIKDEFS_GLOBAL
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef INTUITION_INTUITION_H
#include <intuition/intuition.h>
#endif

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

#ifndef UIKDEFS_LIST
#include "uiki:uiklist.h"
#endif

#ifndef UIK_ZONE_H
#include "uiki:uikzone.h"
#endif

#ifndef UIKDEFS_KEYSUPP
#include "uiki:uikkey.h"
#endif

struct UIKArgRegs
    {
    ULONG D0,D1,D2,D3,D4,D5,D6,D7;
    ULONG A0,A1,A2,A3,A4,A5;
    };

struct UIKGlobal
    {
    /* UIKGlobal est en quelque sorte le super objet de UIK, l'objet */
    /*   primordial auquel tous les autres vont être rattachés en tant */
    /*   qu'objets fils, pour former une arborescence. */
    struct UIKObj    Obj;           /* objet primordial */
    struct MsgPort   UPort;         /* *** */
    struct Task      *Task;         /* la tâche à laquelle est rattachée ce global */
    UBYTE            Flags;         /* *** */
    UBYTE            RemObj;        /* *** */
    UBYTE            Exable;        /* *** */
    UBYTE            ActiveSigBit;  /* *** */
    ULONG            ActiveMask;    /* *** */
    LONG             SigBit;        /* *** */
    ULONG            SigMask;       /* *** le handler signale le process */
    ULONG            CmdMask;       /* *** pour CmdPort */
    ULONG            ExcMask;       /* *** quand uik disabled */
    ULONG            ExtMask;       /* *** pour vectors externes à UIK_Do() */
    ULONG            WindowNum;     /* *** */
    struct UIKObj    *ActiveObj;    /* *** */
    void             (*StopVector)();   /* *** */
    int              (*RemoveVector)(); /* *** */
    UBYTE            **OldLangEnglish; /* adresse du tableau des textes en anglais */
    UBYTE            **OldLangOther;   /* adresse du tableau des textes de l'autre langue */
    UBYTE            **OldLangage;     /* selon la préférence de l'utilisateur, est égal */
                                    /*   à l'un des deux précédents. */
    struct IntuiMessage *IntuiMsg;  /* valable seulement lors d'un évènement Intuition. */
                                    /*   contient l'adresse du message Intuition qui */
                                    /*   à provoqué l'évènement. Le sous-programme */
                                    /*   activé peut le lire et l'utiliser. */
    struct MinList   IMSList;       /* *** IntuiMessage System List */
    struct MsgPort   CmdPort;       /* *** */
    struct IORequest CmdReq;        /* *** */
    ULONG            (*FilterIE)(); /* *** */
    struct UIKObj    *StartSpec;    /* *** */
    struct UIKKeySupp KeySupp;      /* valable seulement lors d'un évènement de */
                                    /*   type RAWKEY venant de Intuition. Le sous */
                                    /*   programme activé peut alors chercher le */
                                    /*   caractère ou la séquence ANSI correspondant */
                                    /*   aux touches tapées. */
                                    /*   voir "uiki:uikkey.h" */
    struct UIKObj   *ActiveKeyTank; /* *** */
    ULONG           Tmp;            /* *** */

    struct UIKObj   *OILBM;         /* *** */
    struct UIKObj   *O8SVX;         /* *** */
    struct UIKObj   *OIPOL;         /* *** */

    UBYTE           InputMode;      /* Ces valeurs sont copiées sur les préférences (voir uikbase.h) */
    UBYTE           InputHeld;      /* 0 ou 1 */
    UBYTE           InputAuto;      /* 0 ou 1 */
    UBYTE           padb;           /*  */

    struct UIKZoneList  *ZoneListBase;  /* *** */
    struct UIKZoneList  *ActiveZoneList;/* *** */
    struct RastPort     *ZoneRPortH;     /* *** */
    UBYTE               *ZonePlanesH;    /* *** */

    UWORD           InputFlags;         /* *** */
    UWORD           TempoSpeed;         /* *** */
    UBYTE           JoyVal;             /* *** */
    UBYTE           OldJoyVal;          /* *** */
    UBYTE           JoyButs;            /* *** */
    UBYTE           OldJoyButs;         /* *** */
    UBYTE           DirButs;            /* *** */
    UBYTE           ProcButs;           /* *** */
    UBYTE           IptButs;            /* *** */
    UBYTE           IptChgs;            /* *** */

    UWORD           TempoValidRight;    /* *** */
    UWORD           TempoWaitRight;     /* *** */
    UWORD           TempoValidLeft;     /* *** */
    UWORD           TempoWaitLeft;      /* *** */
    UWORD           TempoValidBack;     /* *** */
    UWORD           TempoWaitBack;      /* *** */
    UWORD           TempoValidForw;     /* *** */
    UWORD           TempoWaitForw;      /* *** */
    UWORD           TempoValidFire;     /* *** */
    UWORD           TempoWaitFire;      /* *** */

    UBYTE           JoyEvents[40];      /* *** */
    UBYTE           *JTBeg;             /* *** */
    UBYTE           *JTEnd;             /* *** */
    void            (*InputVector)();   /* *** */
    void            (*InputMainVector)();/* ** */
    struct Locale   *Locale;            /* *** */
    struct RastPort     *ZoneRPortW;    /* *** */
    UBYTE               *ZonePlanesW;   /* *** */
    struct UIKKeySupp   SCutSupp;       /* *** */
    struct UIKArgRegs   ArgRegs;        /* *** */
    };

/*----- UIKGlobal Flags ----*/
#define UIKGF_DEADMENU  0x01
#define UIKGF_RAWCTRLC  0x02

/*----- JoyVal ----*/
#define VBJOYF_RIGHT 0x01
#define VBJOYF_LEFT  0x02
#define VBJOYF_BACK  0x04
#define VBJOYF_FORW  0x08
#define VBJOYF_FIRE  0x10

#define CLIC_BUTTON VBJOYF_FIRE
#define MOVE_BUTTON (VBJOYF_RIGHT|VBJOYF_LEFT|VBJOYF_BACK|VBJOYF_FORW)
#define DIR_BUTTONS (VBJOYF_RIGHT|VBJOYF_LEFT|VBJOYF_BACK|VBJOYF_FORW)

#endif
