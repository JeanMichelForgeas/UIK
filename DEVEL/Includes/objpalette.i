
IFND  UIKDEFS_OBJPALETTE
UIKDEFS_OBJPALETTE  SET     1
**
**   Copyright (c) 1993 David Screve, Frédéric Denis, Jean-Michel Forgeas
**       All Rights Reserved
**       Tous droits réservés
**


    INCLUDE "uiki:uikbase.i"
    INCLUDE "uiki:uikglobal.i"
    INCLUDE "uiki:uikobj.i"
    INCLUDE "uiki:uikbox.i"
    INCLUDE "uiki:uikmacros.i"
    INCLUDE "uiki:uikcolors.i"
    INCLUDE "uiki:objwindow.i"
    INCLUDE "uiki:objproplift.i"

STRUCTURE UIKColorDef,0
    UWORD   ucd_ColorNum
    UWORD   ucd_B12Color
    ULONG   ucd_B32Red
    ULONG   ucd_B32Green
    ULONG   ucd_B32Blue
    LABEL UIKColorDef_SIZE

STRUCTURE UIKObjPaletteDispl,0
    STRUCT  uopd_Obj,UIKOBJ_SIZE
    ULONG   uopd_Color
    STRUCT  uopd_ColGads,32*4
    STRUCT  uopd_GadsHooks,32*4
    STRUCT  uopd_GfxStruct,64*2
  uopd_ECS_ColorTable equ GfxStruct
  uopd_ECS_UnDoCTable equ (GfxStruct+32*2)
  uopd_AA_ColorTable  equ ECSGfx
  uopd_AA_UnDoCTable  equ (GfxStruct+4)
    LABEL PaletteDispl_SIZE

STRUCTURE UIKObjPalette,0
    STRUCT   uopa_Obj,UIKOBJ_SIZE
    FPTR     uopa_OkFunc
    FPTR     uopa_CancelFunc
    FPTR     uopa_RestoreFunc
    FPTR     uopa_ModifyFunc
    FPTR     uopa_SelectFunc
    APTR     uopa_paldsp
    APTR     uopa_Wino
    APTR     uopa_ColFram
    APTR     uopa_RVBFram
    APTR     uopa_colname
    APTR     uopa_ok
    APTR     uopa_restore
    APTR     uopa_rprop
    APTR     uopa_gprop
    APTR     uopa_bprop
    APTR     uopa_rdisp
    APTR     uopa_gdisp
    APTR     uopa_bdisp
    APTR     uopa_rdisp2
    APTR     uopa_gdisp2
    APTR     uopa_bdisp2
    ULONG    uopa_RGBTotPos
    APTR     uopa_UIKBase
    APTR     uopa_DOSBase
    APTR     uopa_GfxBase
    APTR     uopa_IntuitionBase
    APTR     uopa_ColorWheel
    ULONG    uopa_PenNum
    ULONG    uopa_ChangeSystem
    ULONG    uopa_Offset
    ULONG    uopa_Color32Num
    ULONG    uopa_EditColNum
    APTR     uopa_PaletteSlider
    STRUCT   uopa_UCD,UIKColorDef_SIZE
    ULONG    uopa_UserULongData
    LONG     uopa_UserLongData
    UWORD    uopa_UserUWordData
    WORD     uopa_UserWordData
    LABEL  UIKObjPalette_SIZE


;------------ Valeurs pour le champ ChangeSystem
PAL_CHGSYS_RGBSLIDERS equ  1
PAL_CHGSYS_COLORWHEEL equ  2

;------------ Palette Tags
UIKTAG_Palette_OKFunc           equ (UIKTAG_USER|$0001)
UIKTAG_Palette_CancelFunc       equ (UIKTAG_USER|$0002)
UIKTAG_Palette_RestoreFunc      equ (UIKTAG_USER|$0003)
UIKTAG_Palette_ModifyFunc       equ (UIKTAG_USER|$0004)
UIKTAG_Palette_SelectFunc       equ (UIKTAG_USER|$0005)
UIKTAG_Palette_FirstTitleNum    equ (UIKTAG_USER|$0006)
UIKTAG_Palette_NoColorWheel     equ (UIKTAG_USER|$0007)
UIKTAG_Palette_WheelColorNum    equ (UIKTAG_USER|$0008)
UIKTAG_Palette_GradientColorNum equ (UIKTAG_USER|$0009)
UIKTAG_Palette_EditColorNum     equ (UIKTAG_USER|$000a)

;------------ Palette Flags
UIKTAG_PaletteFl_DefaultScreen  equ (UIKTAG_USER|$0100) ; (FALSE)


    ENDC