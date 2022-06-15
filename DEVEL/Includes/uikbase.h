#ifndef CLIB_UIK_BASE_H
#define CLIB_UIK_BASE_H
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef EXEC_SEMAPHORES_H
#include <exec/semaphores.h>
#endif

#ifndef EXEC_LIBRARIES_H
#include <exec/libraries.h>
#endif

#ifndef EXEC_HNTERRUPTS_H
#include <exec/interrupts.h>
#endif

#ifndef   GRAPHICS_TEXT_H
#include <graphics/text.h>
#endif

#ifndef   UIKDEFS_LIST
#include "uiki:uiklist.h"
#endif

#ifndef   UIKDEFS_COLORS
#include "uiki:uikcolors.h"
#endif

#ifndef   UIKDEFS_KEYSUPP
#include "uiki:uikkey.h"
#endif


#define UIKNAME             "uik.library"

struct UIKPrefs {
    struct UIKColors Colors;
    UWORD           ColorTable[MAX_UIKCOLORS];
    UWORD           PrefsFlags;
    UWORD           DefFontHeight;
    UBYTE           DefFontName[32];

    UWORD           DefScreen_Depth;
    union {
          ULONG defScreen_ModeId;
          struct {
                 UWORD pad;
                 UWORD defScreen_Modes;
                 } m13;
          } vpm;
    UWORD           DefScreen_Width;
    UWORD           DefScreen_Height;

    UBYTE           InputMode;      /* */
    UBYTE           InputHeld;      /* */
    UBYTE           InputAuto;      /* */
    UBYTE           ZoneThick;      /* épaisseur du cadre en pixels */
    UWORD           ZoneSpeed;      /* durée avant de passer à une autre zone */
    UWORD           ValidTime;      /* durée minimum pour validation */
    UWORD           WaitTime;       /* durée minimum entre 2 clics (laisser derrière ValidTime) */
    UWORD           InputFlags;     /* SLIDEZONE, FRAMEPOINTER... */
    UBYTE           DefScreen_OScan;
    UBYTE           Mask;
    UWORD           Padw;
    };

/* Overscan types : */
#define CUSTOM_OSCAN 0
#define TEXT_OSCAN   1
#define GFX_OSCAN    2
#define MIN_OSCAN    3
#define MAX_OSCAN    4

#define DefScreen_ModeId    vpm.defScreen_ModeId
#define DefScreen_Modes     vpm.m13.defScreen_Modes

/*----- uikp_PrefsFlags */

#define UIKLANGB                 0  /* -------bit 0 */
#define UIKLANG_ENGLISH     0x0000  /* valeur bit 0 */
#define UIKLANG_OTHER       0x0001  /* valeur bit 0 */

#define UIKDEFAULT_SCREEN   0x0002  /* si applications sur screen par defaut */
#define UIKLANG_LOCALE      0x0004  /* priorité à locale.library */
#define UIKF_SPLOOK         0x0008  /* special look (surlignage) */
#define UIKF_SYSLOOK        0x0010  /* system look */

/*----- Input mode */  /* pris en compte en argument à UIK_Input_set() */

#define INPUTMODE_MOUSE     0
#define INPUTMODE_BOURIS    1
#define INPUTMODE_JOYMOUSE  2
#define INPUTMODE_1CONTACT  3
#define INPUTMODE_2CONTACTS 4
#define INPUTMODE_5CONTACTS 5
#define INPUTMODE_NUMBER    6


/*------ InputFlags */
#define INPUTF_SLIDEZONE    0x0001  /* (init)         */
#define INPUTF_FRAMEPOINTER 0x0002  /* (init)         */
#define INPUTF_JAMED        0x0004  /* (init/travail) */
#define INPUTF_KZAKZ_ON     0x0008  /* (travail)      */


struct UIKBase
    {
    /*---- library requirements */
    struct Library          Library;
    UBYTE                   Flags;
    UBYTE                   UIKFlags;
    /* We are now longword aligned */
    ULONG                   SegList;

    /*---- registration */
    struct UIKList          RegisteredObjList;
    struct UIKList          RegisteredImaList;
    struct SignalSemaphore  RegObjSemaphore;
    struct SignalSemaphore  RegImaSemaphore;

    /*---- Preferences */
    struct UIKPrefs         *UIKPrefs;
    struct SignalSemaphore  PrefsSemaphore;

    /*---- */
    struct TextFont         *DefaultFont;
    struct UIKKeySupp       KeySupp;

    /*---- */
    struct Screen           *DefaultScreen;
    struct TTextAttr        DefaultFontAttr;

    /*---- */
    struct Interrupt        VBlankInt;
    ULONG                   Ticks;
    struct UIKGlobal        *ZonedUIK;

    /*---- */
    UWORD                   OpenCount;
    UWORD                   padw;

    struct LocaleBase       *LocaleBase;

    struct List             TaskList;

    struct Task             *ScrPubTask;
    };

/*------ UIKFlags */


#endif  /* CLIB_UIK_BASE_H */
