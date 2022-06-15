
#ifndef OBJPALETTE_H
#define OBJPALETTE_H
/*
 *   Copyright (c) 1993 David Scrève, Frédéric Denis, Jean-Michel Forgeas
 *       All Rights Reserved
 *       Tous droits réservés
 */

#include "uiki:uikbase.h"
#include "uiki:uikglobal.h"
#include "uiki:uikobj.h"
#include "uiki:uikbox.h"
#include "uiki:uikmacros.h"
#include "uiki:uikcolors.h"
#include "uiki:objwindow.h"
#include "uiki:objproplift.h"


struct UIKColorDef
    {
    UWORD   ColorNum;
    UWORD   B12Color;
    ULONG   B32Red;
    ULONG   B32Green;
    ULONG   B32Blue;
    };

struct UIKObjPaletteDispl
    {
    struct UIKObj     Obj;
    ULONG             Color;
    struct UIKObj     *ColGads[32];
    struct UIKHook    *GadsHooks[32];
    union
       {
        struct
           {
            UWORD             ColorTable[32];
            UWORD             UnDoCTable[32];
           } ECSGfx;
        struct
           {
            ULONG             *ColorTable;
            ULONG             *UnDoCTable;
           } AAGfx;
       } GfxStruct;
    };

struct UIKObjPalette
    {
    struct UIKObj           Obj;

    ULONG                   (*OKFunc)( struct UIKObjPalette * );
    ULONG                   (*CancelFunc)( struct UIKObjPalette * );
    ULONG                   (*RestoreFunc)( struct UIKObjPalette * );
    ULONG                   (*ModifyFunc)( struct UIKObjPalette *, struct UIKColorDef * );
    ULONG                   (*SelectFunc)( struct UIKObjPalette *, ULONG );

    /* objet de visualisation de la palette */
    /* il recoit les evenements quand on choisi une couleur */
    struct UIKObjPaletteDispl *paldsp;

    struct UIKObjWindow     *Wino;
    struct UIKObj           *ColFram;
    struct UIKObj           *RVBFram;
    struct UIKObj           *colname;
    struct UIKObj           *ok;

    /* bouton restore ou défaire : restore automatiquement la palette */
    /* del'écran courant */
    struct UIKObj           *restore;

    /* les 3 gadgets proportionnels ****/
    struct UIKObjProp       *rprop;
    struct UIKObjProp       *gprop;
    struct UIKObjProp       *bprop;

    struct UIKObj           *rdisp;
    struct UIKObj           *gdisp;
    struct UIKObj           *bdisp;

    struct UIKObj           *rdisp2;
    struct UIKObj           *gdisp2;
    struct UIKObj           *bdisp2;

    ULONG                   RGBTotPos;
    struct UIKBase          *UIKBase;
    struct DOSBase          *DOSBase;
    struct GfxBase          *GfxBase;
    struct Library          *IntuitionBase;

    struct ObjWheelGrad     *ColorWheel; /* si OS>=39 et requis */
    ULONG                   PenNum;
    ULONG                   ChangeSystem;
    ULONG                   Offset;
    ULONG                   Color32Num;
    ULONG                   EditColNum;
    struct UIKObjPropGad    *PaletteSlider;
    struct UIKColorDef      UCD;

    ULONG                   UserULongData;
    LONG                    UserLongData;
    UWORD                   UserUWordData;
    WORD                    UserWordData;
    };


/*------------ Valeurs pour le champ ChangeSystem */
#define PAL_CHGSYS_RGBSLIDERS  1L
#define PAL_CHGSYS_COLORWHEEL  2L

/*------------ Palette Tags */
#define UIKTAG_Palette_OKFunc           (UIKTAG_USER|0x0001)
#define UIKTAG_Palette_CancelFunc       (UIKTAG_USER|0x0002)
#define UIKTAG_Palette_RestoreFunc      (UIKTAG_USER|0x0003)
#define UIKTAG_Palette_ModifyFunc       (UIKTAG_USER|0x0004)
#define UIKTAG_Palette_SelectFunc       (UIKTAG_USER|0x0005)
#define UIKTAG_Palette_FirstTitleNum    (UIKTAG_USER|0x0006)
#define UIKTAG_Palette_NoColorWheel     (UIKTAG_USER|0x0007)
#define UIKTAG_Palette_WheelColorNum    (UIKTAG_USER|0x0008) /* nombre de couleurs pour la Colorwheel */
#define UIKTAG_Palette_GradientColorNum (UIKTAG_USER|0x0009) /* nombre de couleurs pour le Gradient Slider */
#define UIKTAG_Palette_EditColorNum     (UIKTAG_USER|0x000a) /* nombre de couleurs à éditer */
#define UIKTAG_Palette_ExtraWidth       (UIKTAG_USER|0x000b)

/*------------ Palette Flags */
#define UIKTAG_PaletteFl_DefaultScreen  (UIKTAG_USER|0x0100) /* (FALSE) */

#endif
