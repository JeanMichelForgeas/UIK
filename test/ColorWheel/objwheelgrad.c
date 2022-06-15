/****************************************************************
 *
 *      Project:   UIK
 *      Function:  objet objwheelgrad
 *
 *      Created:   04/05/93     Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include <stdio.h>

#include <exec/types.h>
#include <exec/memory.h>
#include <intuition/intuition.h>
#include <intuition/intuitionbase.h>
#include <intuition/screens.h>
#include <graphics/displayinfo.h>
#include <intuition/gadgetclass.h>
#include <gadgets/colorwheel.h>
#include <gadgets/gradientslider.h>

#include <clib/intuition_protos.h>
#include <clib/exec_protos.h>
#include <clib/dos_protos.h>
#include <clib/graphics_protos.h>
#include <clib/colorwheel_protos.h>

#include <pragmas/intuition_pragmas.h>
#include <pragmas/exec_pragmas.h>
#include <pragmas/dos_pragmas.h>
#include <pragmas/graphics_pragmas.h>
//#include <pragmas/colorwheel_pragmas.h>

#include "uiki:uikbase.h"
#include "uiki:uikglobal.h"
#include "uiki:uikobj.h"
#include "uiki:uikmacros.h"
#include "uiki:uikgfx.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objwindow.h"

#include "objwheelgrad.h"


/****** Macros **************************************************/


/****** Imported ************************************************/

extern struct Library *UIKBase;


/****** Exported ************************************************/

struct Library *GfxBase;
struct Library *IntuitionBase;
struct Library *ColorWheelBase;
struct Library *GradientSliderBase;


/****** Statics *************************************************/


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/


/*------- Object entries ----------------------------------------*/

static int GenericAdd( struct ObjWheelGrad *cobj, struct TagItem *taglist)
{
  struct ColorWheelHSB *hsb;
  struct ColorWheelRGB *rgb;

    /*
    SysBase = (struct ExecBase *) *((ULONG*)4);
    if (! (cobj->UIKBase = UIKBase = OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    */
    if (! (cobj->GfxBase = GfxBase = OpenLibrary( "graphics.library", 39 ))) goto END_ERROR;
    if (! (cobj->IntuitionBase = IntuitionBase = OpenLibrary( "intuition.library", 39 ))) goto END_ERROR;
    if (! (cobj->ColorWheelBase = ColorWheelBase = OpenLibrary( "gadgets/colorwheel.gadget", 39 ))) goto END_ERROR;
    if (! (cobj->GradientSliderBase = GradientSliderBase = OpenLibrary( "gadgets/gradientslider.gadget", 39 ))) goto END_ERROR;

    /*---------------------------*/
    cobj->MaxVal     = UIK_GetTagData( UIKTAG_OBJWheelG_SLMaxVal, 0xFFFF, taglist );
    cobj->CurVal     = UIK_GetTagData( UIKTAG_OBJWheelG_SLCurVal, 0xFFFF, taglist );
    cobj->SkipVal    = UIK_GetTagData( UIKTAG_OBJWheelG_SLSkipVal, 0x1111, taglist );
    cobj->KnobPixels = UIK_GetTagData( UIKTAG_OBJWheelG_SLKnobPixels, 5, taglist );
    cobj->NumPens    = UIK_GetTagData( UIKTAG_OBJWheelG_SLNumPens, 4, taglist );

    if (! (cobj->PenArray = (WORD*) UIK_MemSpAlloc( 2 * (cobj->NumPens+1), MEMF_ANY | MEMF_CLEAR ))) goto END_ERROR;
    if (! (cobj->LoadCols = (struct load32*) UIK_MemSpAlloc( sizeof(struct load32) * (cobj->NumPens+1), MEMF_ANY | MEMF_CLEAR ))) goto END_ERROR;

    /*---------------------------*/
    cobj->RGB.cw_Red   = UIK_GetTagData( UIKTAG_OBJWheelG_Red, 0xFFFFFFFF, taglist );
    cobj->RGB.cw_Green = UIK_GetTagData( UIKTAG_OBJWheelG_Green, 0, taglist );
    cobj->RGB.cw_Blue  = UIK_GetTagData( UIKTAG_OBJWheelG_Blue, 0, taglist );

    cobj->HSB.cw_Hue        = UIK_GetTagData( UIKTAG_OBJWheelG_Hue, 0, taglist );
    cobj->HSB.cw_Saturation = UIK_GetTagData( UIKTAG_OBJWheelG_Saturation, 0xFFFFFFFF, taglist );
    cobj->HSB.cw_Brightness = UIK_GetTagData( UIKTAG_OBJWheelG_Brightness, 0xFFFFFFFF, taglist );

    if (rgb = (struct ColorWheelRGB *) UIK_GetTagData( UIKTAG_OBJWheelG_RGB, 0, taglist ))
        {
        ConvertRGBToHSB( rgb, &cobj->HSB );
        }
    else ConvertRGBToHSB( &cobj->RGB, &cobj->HSB );

    if (hsb = (struct ColorWheelHSB *) UIK_GetTagData( UIKTAG_OBJWheelG_HSB, 0, taglist ))
        {
        cobj->HSB = *hsb;
        }

    /*---------------------------*/
    cobj->Screen     = UIK_GetTagData( UIKTAG_OBJWheelG_Screen, 0, taglist );
    cobj->Abbrv      = UIK_GetTagData( UIKTAG_OBJWheelG_Abbrv, (ULONG)"GCBMRY", taglist );
    cobj->Donation   = UIK_GetTagData( UIKTAG_OBJWheelG_Donation, NULL, taglist );
    cobj->BevelBox   = UIK_GetTagData( UIKTAG_OBJWheelG_BevelBox, FALSE, taglist );
    cobj->MaxPens    = UIK_GetTagData( UIKTAG_OBJWheelG_MaxPens, 256, taglist );
    cobj->InitColor  = UIK_GetTagData( UIKTAG_OBJWheelG_InitColor, -1, taglist );

   return(1);

 END_ERROR:
   return(0);
}

static void GenericStop( struct ObjWheelGrad *cobj )
{
  struct Screen *sc = cobj->Screen ? cobj->Screen : cobj->Obj.w->WScreen;

    if (cobj->Wheel)
        {
        GetAttr( WHEEL_HSB, cobj->Wheel, (ULONG*)&cobj->HSB );

        RemoveGList( cobj->Obj.w, cobj->Wheel, 1 );
        DisposeObject( cobj->Wheel );
        cobj->Wheel = 0;
        }

    if (cobj->Slider)
        {
        GetAttr( GRAD_MaxVal, cobj->Slider, &cobj->MaxVal );
        GetAttr( GRAD_CurVal, cobj->Slider, &cobj->CurVal );
        GetAttr( GRAD_SkipVal, cobj->Slider, &cobj->SkipVal );
        GetAttr( GRAD_KnobPixels, cobj->Slider, &cobj->KnobPixels );

        RemoveGList( cobj->Obj.w, cobj->Slider, 1 );
        DisposeObject( cobj->Slider );
        cobj->Slider = 0;
        }

    while (cobj->AllocPens > 0)
        {
        cobj->AllocPens--;
        ReleasePen( sc->ViewPort.ColorMap, cobj->PenArray[cobj->AllocPens] );
        }
}

static void GenericRemove( struct ObjWheelGrad *cobj )
{
    UIK_MemSpFree( cobj->LoadCols );
    UIK_MemSpFree( cobj->PenArray );

    /*
    if (cobj->UIKBase) CloseLibrary( cobj->UIKBase );
    */
    if (cobj->GfxBase) CloseLibrary( cobj->GfxBase );
    if (cobj->IntuitionBase) CloseLibrary( cobj->IntuitionBase );
    if (cobj->ColorWheelBase) CloseLibrary( cobj->ColorWheelBase );
    if (cobj->GradientSliderBase) CloseLibrary( cobj->GradientSliderBase );
}

static int GenericStart( struct ObjWheelGrad *cobj )
{
  struct Screen *sc = cobj->Screen ? cobj->Screen : cobj->Obj.w->WScreen;
  struct ColorWheelHSB hsb;
  struct ColorWheelRGB rgb;

    hsb = cobj->HSB;
    if (cobj->InitColor != -1) /* get the RGB components of color, convert to HSB */
        {
        GetRGB32( sc->ViewPort.ColorMap, cobj->InitColor, 1, (ULONG*)&rgb );
        ConvertRGBToHSB( &rgb, &hsb );
        cobj->HSB = hsb;
        cobj->InitColor = -1;
        }

    hsb.cw_Brightness = 0xffffffff; /* max out B component */
    cobj->AllocPens = 0;
    while (cobj->AllocPens < cobj->NumPens)
        {
        hsb.cw_Brightness = 0xffffffff - ((0xffffffff / cobj->NumPens) * cobj->AllocPens);
        ConvertHSBToRGB( &hsb, &rgb );
        cobj->PenArray[cobj->AllocPens] = ObtainPen( sc->ViewPort.ColorMap, -1, rgb.cw_Red, rgb.cw_Green, rgb.cw_Blue, PEN_EXCLUSIVE );
        if (cobj->PenArray[cobj->AllocPens] == -1) break;
        cobj->LoadCols[cobj->AllocPens].l32_len = 1;
        cobj->LoadCols[cobj->AllocPens].l32_pen = cobj->PenArray[cobj->AllocPens];
        cobj->AllocPens++;
        }
    cobj->PenArray[cobj->AllocPens] = ~0;

    if (! (cobj->Slider = (struct Gadget *) NewObject( NULL, "gradientslider.gadget",
            GA_Top,             cobj->Obj.Box.Top,
            GA_Left,            cobj->Obj.Box.Left + cobj->Obj.Box.Width - 21,
            GA_Width,           20,
            GA_Height,          cobj->Obj.Box.Height,

            GRAD_MaxVal,        cobj->MaxVal,
            GRAD_CurVal,        cobj->CurVal,
            GRAD_SkipVal,       cobj->SkipVal,
            GRAD_KnobPixels,    cobj->KnobPixels,
            GRAD_PenArray,      cobj->PenArray,

            PGA_Freedom,        LORIENT_VERT,
            GA_FollowMouse,     TRUE,
            GA_RelVerify,       TRUE,
            GA_Immediate,       TRUE,
            GA_UserData,        cobj,
            TAG_END ))) goto END_ERROR;

    if (! (cobj->Wheel = (struct Gadget *) NewObject( NULL, "colorwheel.gadget",
            GA_Top,                 cobj->Obj.Box.Top,
            GA_Left,                cobj->Obj.Box.Left,
            GA_Width,               cobj->Obj.Box.Width - 25,
            GA_Height,              cobj->Obj.Box.Height,

            WHEEL_HSB,              &cobj->HSB,
            WHEEL_Abbrv,            cobj->Abbrv,
            //WHEEL_Donation,         cobj->Donation,
            WHEEL_BevelBox,         cobj->BevelBox,
            WHEEL_MaxPens,          cobj->MaxPens,

            WHEEL_Screen,           sc,
            WHEEL_GradientSlider,   cobj->Slider,

            GA_FollowMouse,         TRUE,
            GA_RelVerify,           TRUE,
            GA_Immediate,           TRUE,
            GA_Previous,            cobj->Slider,
            GA_UserData,            cobj,
            TAG_END ))) goto END_ERROR;

    if (cobj->Obj.rq)
        {
        cobj->Wheel->GadgetType |= GTYP_REQGADGET;
        cobj->Slider->GadgetType |= GTYP_REQGADGET;
        }
    AddGList( cobj->Obj.w, cobj->Slider, (USHORT)~0, 2, cobj->Obj.rq );

    return(1);

 END_ERROR:
   return(0);
}

static void GenericRefresh( struct ObjWheelGrad *cobj )
{
    RefreshGList( cobj->Slider, cobj->Obj.w, cobj->Obj.rq, 2 );
}

/*------ Public Funcs ---------------------------------------------------*/

static void RGB2HSB( struct ColorWheelRGB *rgb, struct ColorWheelHSB *hsb )
{
    ConvertRGBToHSB( rgb, hsb );
}

static void HSB2RGB( struct ColorWheelHSB *hsb, struct ColorWheelRGB *rgb )
{
    ConvertHSBToRGB( hsb, rgb );
}

static void Wheel2Grad( struct ObjWheelGrad *cobj )
{
  struct Screen *sc = cobj->Screen ? cobj->Screen : cobj->Obj.w->WScreen;
  struct ColorWheelHSB hsb;
  struct ColorWheelRGB rgb;
  ULONG i;

    GetAttr( WHEEL_HSB, cobj->Wheel, (ULONG *)&hsb );

    if (hsb.cw_Hue != cobj->HSB.cw_Hue || hsb.cw_Saturation != cobj->HSB.cw_Saturation)
        {
        cobj->HSB = hsb;
        i = 0;
        while (i < cobj->AllocPens)
            {
            hsb.cw_Brightness = 0xffffffff - ((0xffffffff / cobj->AllocPens) * i);
            ConvertHSBToRGB( &hsb, &rgb );
            cobj->LoadCols[i].l32_red = rgb.cw_Red;
            cobj->LoadCols[i].l32_grn = rgb.cw_Green;
            cobj->LoadCols[i].l32_blu = rgb.cw_Blue;
            i++;
            }
        LoadRGB32( &sc->ViewPort, (ULONG*)cobj->LoadCols );
        }
}

static void Color2Wheel( struct ObjWheelGrad *cobj, ULONG colornum )
{
  struct Screen *sc = cobj->Screen ? cobj->Screen : cobj->Obj.w->WScreen;
  struct ColorWheelRGB rgb;

    if (cobj->Wheel)
        {
        GetRGB32( sc->ViewPort.ColorMap, colornum, 1, (ULONG*)&rgb );
        SetGadgetAttrs( cobj->Wheel, cobj->Obj.w, cobj->Obj.rq, WHEEL_RGB, (ULONG*)&rgb, TAG_END );
        }
}

static ULONG FuncTable[] =
    {
    (ULONG) RGB2HSB,
    (ULONG) HSB2RGB,
    (ULONG) Wheel2Grad,
    (ULONG) Color2Wheel,
    };

/*------ TagList for defaults ---------------------------------------------------*/

static struct TagItem GermTagList[] =
{
   { UIKTAG_GEN_LastFuncId, UIKFUNC_WheelGrad_LastCmd },
   { TAG_END }
};

/*------ Germ ---------------------------------------------------*/

static struct UIKObjGerm Germ = {
    "",
    GenericAdd,         /*int     (*AddVector)(); */
    GenericStart,       /*int     (*StartVector)(); */
    GenericStop,        /*void    (*StopVector)(); */
    GenericRemove,      /*void    (*RemoveVector)(); */
    GenericRefresh,     /*void    (*RefreshVector)(); */
    NULL,               /*void    (*ResizeVector)(); */
    NULL,               /*void    (*ExceptActVector)(); */
    NULL,               /*void    (*ExceptInaVector)(); */
    NULL,               /*void    (*ExceptMovVector)(); */
    NULL,               /*void    (*ExceptKeyVector)(); */
    NULL,               /*int     (*StartAfterVector)(); */
    NULL,               /*int     (*SelectVector)(); */
    FuncTable,          /*ULONG   *ExtentedVectors; */
    GermTagList,        /*struct TagItem *GermTagList; */
    10,                 /*UWORD   MinWidth; */
    10,                 /*UWORD   MinHeight; */
    SETL(WHEELGRAD_BOX_STDWIDTH,WHEELGRAD_BOX_STDHEIGHT), /*ULONG DefaultWidthHeight; */
    0,                  /*UWORD   ObjId; */
    sizeof(struct ObjWheelGrad),   /*UWORD   ObjSize; */
    BOX_EXTGADUP | BOX_DISPLAYABLE | BOX_OVER | BOX_TRANSLATELEFTTOP,
    };

struct UIKObjRegGerm WheelGradRegGerm = { &Germ, 0, 0 ,0 };
