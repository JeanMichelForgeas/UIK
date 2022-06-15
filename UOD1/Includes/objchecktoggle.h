#ifndef UIKDEFS_OBJCHECK

#define UIKDEFS_OBJCHECK
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


struct UIKObjCheckToggle
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

#define UIKFUNC_Check_Select    0
#define UIKFUNC_Check_Unselect  4

#define UIKFUNC_Check_LastCmd   4


/*-----------------------------------------*/

#define OBJCHECK_BOX_STDWIDTH  26
#define OBJCHECK_BOX_STDHEIGHT 11

/*----CheckFlags----*/
#define UIKF_CHECK_RADIO    0x0001

/*---------Obj Check Tags---------- */
#define UIKTAG_OBJCheck_Image1      (UIKTAG_USER|0x0001)
#define UIKTAG_OBJCheck_AltImage1   (UIKTAG_USER|0x0002)
#define UIKTAG_OBJCheck_Image2      (UIKTAG_USER|0x0003)
#define UIKTAG_OBJCheck_AltImage2   (UIKTAG_USER|0x0004)

/*---------Obj Check Flags---------- */
#define UIKTAG_OBJCheck_Flags       (UIKTAG_USER|0x0020)
#define UIKTAG_OBJCheckFl_Radio     (UIKTAG_USER|0x0021)

/*-- */

#endif
