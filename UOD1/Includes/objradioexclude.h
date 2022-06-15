#ifndef UIKDEFS_OBJRADIO

#define UIKDEFS_OBJRADIO
/*
 *      Copyright (C) 1993 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

#ifndef UIKDEFS_TAGITEM_H
#include "uiki:uiktag.h"
#endif


struct UIKObjRadioExclude
    {
    struct UIKObj           Obj;
    struct Gadget           Gad;
    struct IntuitionBase    *IntuitionBase;
    struct GfxBase          *GfxBase;
    struct UIKBase          *UIKBase;

    ULONG                   Flags;

    struct UIKBitMap        *Image1;
    struct UIKBitMap        *AltImage1;
    struct UIKBitMap        *Image2;
    struct UIKBitMap        *AltImage2;
    };

/*-------Fonctions internes publiques------*/

#define UIKFUNC_Radio_Select    0

#define UIKFUNC_Radio_LastCmd   0


/*-----------------------------------------*/

#define OBJRADIO_BOX_STDWIDTH  26
#define OBJRADIO_BOX_STDHEIGHT 11

/*----Radio Flags----*/
#define UIKF_RADIO_CHECK        0x0001

/*---------Obj Radio Tags---------- */
#define UIKTAG_OBJRadio_Image1      (UIKTAG_USER|0x0001)
#define UIKTAG_OBJRadio_AltImage1   (UIKTAG_USER|0x0002)
#define UIKTAG_OBJRadio_Image2      (UIKTAG_USER|0x0003)
#define UIKTAG_OBJRadio_AltImage2   (UIKTAG_USER|0x0004)

/*---------Obj Radio Flags---------- */
#define UIKTAG_OBJRadio_Flags       (UIKTAG_USER|0x0020)
#define UIKTAG_OBJRadioFl_Check     (UIKTAG_USER|0x0021)

/*-- */

#endif
