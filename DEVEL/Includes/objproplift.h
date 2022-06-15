#ifndef UIKDEFS_OBJPROPLIFT

#define UIKDEFS_OBJPROPLIFT
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

struct UIKObjPropGad
    {
    struct UIKObj       Obj;
    struct Gadget       Gad;
    UWORD               MouseStart, KnobOffs, KnobWidth;
    UWORD               ContWidth;
    UWORD               VisPC;
    UWORD               Reserved; /* OffPC */
    UBYTE               Ver, Moving;
    UWORD               Tot, Vis, Pos;
    UWORD               SaveKnob[4];
    UBYTE               *KnobAutomate;
    UBYTE               *KnobNorAutomate;
    UBYTE               *KnobAltAutomate;
    struct UIKBitMap    *KnobImage1;         /* ratio 1 */
    struct UIKBitMap    *KnobSpecialImage1;  /* utilisée avec option "surlignage" */
    struct UIKBitMap    *KnobImage2;         /* ratio 2 */
    struct UIKBitMap    *KnobSpecialImage2;  /* utilisée avec option "surlignage" */
    ULONG               LTot, LVis, LPos;
    ULONG               OldLPos;
    };

#define HORIZKNOB_MINWIDTH  10
#define VERTKNOB_MINHEIGHT   8

/*----UIKObjProp values Tags---- */
#define UIKTAG_OBJProp_Total            (UIKTAG_USER|0x0001)
#define UIKTAG_OBJProp_Visible          (UIKTAG_USER|0x0002)
#define UIKTAG_OBJProp_Position         (UIKTAG_USER|0x0003)
#define UIKTAG_OBJProp_Vertical         (UIKTAG_USER|0x0004)
#define UIKTAG_OBJProp_KnobAutomate     (UIKTAG_USER|0x0005)
#define UIKTAG_OBJProp_KnobAltAutomate  (UIKTAG_USER|0x0006)
#define UIKTAG_OBJProp_KnobImage1       (UIKTAG_USER|0x0007)
#define UIKTAG_OBJProp_KnobSpImage1     (UIKTAG_USER|0x0008)
#define UIKTAG_OBJProp_KnobImage2       (UIKTAG_USER|0x0009)
#define UIKTAG_OBJProp_KnobSpImage2     (UIKTAG_USER|0x000a)

#endif /* UIKDEFS_OBJPROPLIFT */
