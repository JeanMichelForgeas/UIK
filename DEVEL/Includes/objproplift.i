        IFND    UIKDEFS_OBJPROPLIFT
UIKDEFS_OBJPROPLIFT SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**


    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC


 STRUCTURE  UIKObjPropGad,0
    STRUCT      uop_Obj,UIKOBJ_SIZE
    STRUCT      uop_Gad,gg_SIZEOF
    UWORD       uop_MouseStart
    UWORD       uop_KnobOffs
    UWORD       uop_KnobWidth
    UWORD       uop_ContWidth
    UWORD       uop_VisPC
    UWORD       uop_Reserved    ; OffPC
    UBYTE       uop_Ver
    UBYTE       uop_Moving
    UWORD       uop_Tot
    UWORD       uop_Vis
    UWORD       uop_Pos
    STRUCT      uop_SaveKnob,4*2
    APTR        uop_KnobAutomate
    APTR        uop_KnobNorAutomate
    APTR        uop_KnobAltAutomate
    APTR        uop_KnobImage1           ; ratio 1
    APTR        uop_KnobSpecialImage1    ; utilisée avec option "surlignage"
    APTR        uop_KnobImage2           ; ratio 2
    APTR        uop_KnobSpecialImage2    ; utilisée avec option "surlignage"
    ULONG       uop_LTot
    ULONG       uop_LVis
    ULONG       uop_LPos
    ULONG       uop_OldLPos
    LABEL   UIKObjPropGad_SIZE

HORIZKNOB_MINWIDTH  equ 10
VERTKNOB_MINHEIGHT  equ  8

;---------------------------------------------------UIKObjPropGad
;----UIKObjProp values Tags----
UIKTAG_OBJProp_Total            equ (UIKTAG_USER|$0001)
UIKTAG_OBJProp_Visible          equ (UIKTAG_USER|$0002)
UIKTAG_OBJProp_Position         equ (UIKTAG_USER|$0003)
UIKTAG_OBJProp_Vertical         equ (UIKTAG_USER|$0004)
UIKTAG_OBJProp_KnobAutomate     equ (UIKTAG_USER|$0005)
UIKTAG_OBJProp_KnobAltAutomate  equ (UIKTAG_USER|$0006)
UIKTAG_OBJProp_KnobImage1       equ (UIKTAG_USER|$0007)
UIKTAG_OBJProp_KnobSpImage1     equ (UIKTAG_USER|$0008)
UIKTAG_OBJProp_KnobImage2       equ (UIKTAG_USER|$0009)
UIKTAG_OBJProp_KnobSpImage2     equ (UIKTAG_USER|$000a)

;----UIKObjProp Flags Tags----
;UIKTAG_OBJProp_Flags        equ (UIKTAG_USER|$0020) ; les flags sont déjà prêts
 ; sinon un par un...




        ENDC
