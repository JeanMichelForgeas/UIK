        IFND    UIKDEFS_OBJDIAL
UIKDEFS_OBJDIAL SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**


    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC


DGAVECS         equ  6          * area info vectors for needle

 STRUCTURE  UIKObjDial,0
    STRUCT      uod_Obj,UIKOBJ_SIZE         * struct UIKObj
    STRUCT      uod_Gad,gg_SIZEOF           * struct Gadget
    STRUCT      uod_Image,ig_SIZEOF         * struct Image

    UWORD       uod_Total
    UWORD       uod_Pos
    UWORD       uod_Angle
    UWORD       uod_PiVal
    ULONG       uod_DialFlags

    WORD        uod_CoordX                 * ray vector of needle
    WORD        uod_CoordY
    UWORD       uod_Ratio
    WORD        uod_CorrectedY
    WORD        uod_BaseX                  * vector for base of needle
    WORD        uod_BaseY
    WORD        uod_CenterX
    WORD        uod_CenterY
    LONG        uod_Norm
    STRUCT      uod_AInfo,ai_SIZEOF         * struct AreaInfo
    STRUCT      uod_TmpRas,tr_SIZEOF        * struct TmpRas
    STRUCT      uod_ABuffer,DGAVECS*5

    UBYTE       uod_CirclePen
    UBYTE       uod_DiscPen
    UBYTE       uod_NeedlePen
    UBYTE       uod_HThick
    UBYTE       uod_VThick
    UBYTE       uod_Padb
    APTR        uod_IntuitionBase
    APTR        uod_GfxBase
    APTR        uod_UIKBase
    LABEL   uod_SIZE
    LABEL   UIKObjDial_SIZE

UIKFUNC_Dial_ChangeDialGadget   equ 0
UIKFUNC_Dial_ChangeDialNoDraw   equ 4
UIKFUNC_Dial_LastCmd            equ 4

OBJDIAL_BOX_STDWIDTH    equ 40
OBJDIAL_BOX_STDHEIGHT   equ 20

*----DialFlags----*
UIKF_DIAL_SUPHALF       equ $0001

*---------Obj Dial Tags---------- *
UIKTAG_OBJDial_Total        equ (UIKTAG_USER!$0001)
UIKTAG_OBJDial_Position     equ (UIKTAG_USER!$0002)
UIKTAG_OBJDial_CirclePen    equ (UIKTAG_USER!$0003)
UIKTAG_OBJDial_DiscPen      equ (UIKTAG_USER!$0004)
UIKTAG_OBJDial_NeedlePen    equ (UIKTAG_USER!$0005)
UIKTAG_OBJDial_HorizThick   equ (UIKTAG_USER!$0006)
UIKTAG_OBJDial_VertThick    equ (UIKTAG_USER!$0007)

*---------Obj Dial Flags---------- *
UIKTAG_OBJDial_Flags        equ (UIKTAG_USER!$0020)
UIKTAG_OBJDialFl_SupHalf    equ (UIKTAG_USER!$0021)


        ENDC
