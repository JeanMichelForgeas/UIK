#ifndef UIKDEFS_OBJDIAL

#define UIKDEFS_OBJDIAL
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

#ifndef   GRAPHICS_RASTPORT_H
#include <graphics/rastport.h>
#endif

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

#ifndef UIKDEFS_TAGITEM_H
#include "uiki:uiktag.h"
#endif

#define DGAVECS (6)     /* area info vectors for needle */

struct UIKObjDial
    {
    struct UIKObj       Obj;
    struct Gadget       Gad;
    struct Image        Image;

    UWORD               Total;
    UWORD               Pos;        /* de 0 à Total-1 */
    UWORD               Angle;      /* en degrés */
    UWORD               PiVal;      /* en degrés */
    ULONG               DialFlags;

    WORD                CoordX;     /* ray vector of needle */
    WORD                CoordY;
    UWORD               Ratio;      /* multiplié par 1000 */
    WORD                CorrectedY;
    WORD                BaseX;      /* vector for base of needle */
    WORD                BaseY;
    WORD                CenterX;
    WORD                CenterY;
    LONG                Norm;
    struct AreaInfo     AInfo;
    struct TmpRas       TmpRas;
    UBYTE               ABuffer[ DGAVECS * 5 ];

    UBYTE               CirclePen;
    UBYTE               DiscPen;
    UBYTE               NeedlePen;
    UBYTE               HThick;
    UBYTE               VThick;
    UBYTE               Padb;
    struct IntuitionBase *IntuitionBase;
    struct GfxBase      *GfxBase;
    struct UIKBase      *UIKBase;
    };

/*-------Fonctions internes publiques------*/

#define UIKFUNC_Dial_ChangeDialGadget   0
#define UIKFUNC_Dial_ChangeDialNoDraw   4

#define UIKFUNC_Dial_LastCmd            4


/*-----------------------------------------*/

#define OBJDIAL_BOX_STDWIDTH  40
#define OBJDIAL_BOX_STDHEIGHT 20

/*----DialFlags----*/
#define UIKF_DIAL_SUPHALF   0x0001

/*---------Obj Dial Tags---------- */
#define UIKTAG_OBJDial_Total        (UIKTAG_USER|0x0001)
#define UIKTAG_OBJDial_Position     (UIKTAG_USER|0x0002)
#define UIKTAG_OBJDial_CirclePen    (UIKTAG_USER|0x0003)
#define UIKTAG_OBJDial_DiscPen      (UIKTAG_USER|0x0004)
#define UIKTAG_OBJDial_NeedlePen    (UIKTAG_USER|0x0005)
#define UIKTAG_OBJDial_HorizThick   (UIKTAG_USER|0x0006)
#define UIKTAG_OBJDial_VertThick    (UIKTAG_USER|0x0007)

/*---------Obj Dial Flags---------- */
#define UIKTAG_OBJDial_Flags        (UIKTAG_USER|0x0020)
#define UIKTAG_OBJDialFl_SupHalf    (UIKTAG_USER|0x0021)

/*-- */

#endif
