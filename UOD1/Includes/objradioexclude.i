        IFND    UIKDEFS_OBJRADIO

UIKDEFS_OBJRADIO  SET     1
**
**      Copyright (C) 1993 Jean-Michel Forgeas
**                  Tous Droits Réservés
**

    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC


 STRUCTURE  UIKObjRadioExclude,0
    STRUCT      uorad_Obj,UIKOBJ_SIZE
    STRUCT      uorad_Gad,gg_SIZEOF
    APTR        uorad_IntuitionBase
    APTR        uorad_GfxBase
    APTR        uorad_UIKBase

    ULONG       uorad_Flags

    APTR        uorad_Image1
    APTR        uorad_AltImage1
    APTR        uorad_Image2
    APTR        uorad_AltImage2
    LABEL   UIKObjRadioExclude_SIZE

;-------Fonctions internes publiques------

UIKFUNC_Radio_Select    equ 0

UIKFUNC_Radio_LastCmd   equ 0


;-----------------------------------------

OBJRADIO_BOX_STDWIDTH   equ 26
OBJRADIO_BOX_STDHEIGHT  equ 11

;----Radio Flags----
UIKF_RADIO_CHECK        equ $0001

;---------Obj Radio Tags----------
UIKTAG_OBJRadio_Image1      equ (UIKTAG_USER|$0001)
UIKTAG_OBJRadio_AltImage1   equ (UIKTAG_USER|$0002)
UIKTAG_OBJRadio_Image2      equ (UIKTAG_USER|$0003)
UIKTAG_OBJRadio_AltImage2   equ (UIKTAG_USER|$0004)

;---------Obj Radio Flags----------
UIKTAG_OBJRadio_Flags       equ (UIKTAG_USER|$0020)
UIKTAG_OBJRadioFl_Check     equ (UIKTAG_USER|$0021)

;--

        ENDC
