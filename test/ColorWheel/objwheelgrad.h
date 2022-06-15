#ifndef UIKDEFS_OBJWHEELGRAD

#define UIKDEFS_OBJWHEELGRAD

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef INTUITION_INTUITION_H
#include <intuition/intuition.h>
#endif

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

#ifndef UIKDEFS_TAGITEM_H
#include "uiki:uiktag.h"
#endif

struct load32
    {
    UWORD   l32_len;
    UWORD   l32_pen;
    ULONG   l32_red;
    ULONG   l32_grn;
    ULONG   l32_blu;
    };

struct ObjWheelGrad
    {
    struct UIKObj       Obj;

    struct Library      *UIKBase;
    struct Library      *GfxBase;
    struct Library      *IntuitionBase;
    struct Library      *ColorWheelBase;
    struct Library      *GradientSliderBase;

    struct Gadget       *Slider;
    struct Gadget       *Wheel;

    struct ColorWheelHSB HSB;
    struct ColorWheelRGB RGB;
    ULONG               Screen;
    ULONG               Abbrv;
    ULONG               Donation;
    ULONG               BevelBox;
    ULONG               MaxPens;
    ULONG               InitColor;

    ULONG               MaxVal;
    ULONG               CurVal;
    ULONG               SkipVal;
    ULONG               KnobPixels;
    ULONG               NumPens;
    ULONG               AllocPens;
    WORD                *PenArray;
    struct load32       *LoadCols;
    };

/*-------Fonctions internes publiques------*/

#define UIKFUNC_WheelGrad_RGB2HSV       0
#define UIKFUNC_WheelGrad_HSV2RGB       4
#define UIKFUNC_WheelGrad_Wheel2Grad    8
#define UIKFUNC_WheelGrad_Color2Wheel  12

#define UIKFUNC_WheelGrad_LastCmd      12


/*-----------------------------------------*/

#define WHEELGRAD_BOX_STDWIDTH  50
#define WHEELGRAD_BOX_STDHEIGHT 50

/*----WheelGradFlags----*/
#define UIKF_WheelG_   0x0001

/*---------Obj WheelGrad Tags---------- */

/* pour la documentation des tags, voir la doc Système:
 * "colorwheel_gc.doc"
 */

/* BOOPSI ColorWheel object : WHEEL_... <gadgets/colorwheel.h> */
#define UIKTAG_OBJWheelG_Hue        (UIKTAG_USER|0x0002)
#define UIKTAG_OBJWheelG_Saturation (UIKTAG_USER|0x0003)
#define UIKTAG_OBJWheelG_Brightness (UIKTAG_USER|0x0004)
#define UIKTAG_OBJWheelG_HSB        (UIKTAG_USER|0x0005)
#define UIKTAG_OBJWheelG_Red        (UIKTAG_USER|0x0006)
#define UIKTAG_OBJWheelG_Green      (UIKTAG_USER|0x0007)
#define UIKTAG_OBJWheelG_Blue       (UIKTAG_USER|0x0008)
#define UIKTAG_OBJWheelG_RGB        (UIKTAG_USER|0x0009)
#define UIKTAG_OBJWheelG_Screen     (UIKTAG_USER|0x000a)
#define UIKTAG_OBJWheelG_Abbrv      (UIKTAG_USER|0x000b)
#define UIKTAG_OBJWheelG_Donation   (UIKTAG_USER|0x000c)
#define UIKTAG_OBJWheelG_BevelBox   (UIKTAG_USER|0x000d)
#define UIKTAG_OBJWheelG_MaxPens    (UIKTAG_USER|0x000e)
#define UIKTAG_OBJWheelG_InitColor  (UIKTAG_USER|0x000f)

/* BOOPSI Gradient object : GRAD_... <gadgets/gradientslider.h> */
#define UIKTAG_OBJWheelG_SLMaxVal       (UIKTAG_USER|0x0022)
#define UIKTAG_OBJWheelG_SLCurVal       (UIKTAG_USER|0x0023)
#define UIKTAG_OBJWheelG_SLSkipVal      (UIKTAG_USER|0x0024)
#define UIKTAG_OBJWheelG_SLKnobPixels   (UIKTAG_USER|0x0025)
#define UIKTAG_OBJWheelG_SLNumPens      (UIKTAG_USER|0x0026)

/*---------Obj WheelGrad Flags---------- */
#define UIKTAG_OBJWheelGFl_    (UIKTAG_USER|0x0051)

/*-- */

#endif
