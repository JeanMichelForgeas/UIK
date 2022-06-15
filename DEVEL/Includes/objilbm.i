        IFND    UIKDEFS_OBJILBM
UIKDEFS_OBJILBM SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**


    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC

    IFND UIKDEFS_MACRO
    INCLUDE "UIKI:uikmacros.i"
    ENDC


 STRUCTURE  UIKObjILBM,0
    STRUCT      uoilbm_Obj,UIKOBJ_SIZE
    APTR        uoilbm_IntuitionBase
    APTR        uoilbm_GfxBase
    APTR        uoilbm_DOSBase
    APTR        uoilbm_UIKBase
    LABEL   UIKObjILBM_SIZE


 STRUCTURE  UIKIlbmFrame,0
    APTR        uifr_ColorTable256
    ULONG       uifr_Colors
    ULONG       uifr_ViewMode
    ULONG       uifr_reserved
    UBYTE       uifr_FoundBMHD
    UBYTE       uifr_FoundFlags
    UWORD       uifr_W
    UWORD       uifr_H
    WORD        uifr_X
    WORD        uifr_Y
    UBYTE       uifr_Depth
    UBYTE       uifr_Masking
    UBYTE       uifr_Compression
    UBYTE       uifr_Flags
    UWORD       uifr_TransparentColor
    UBYTE       uifr_XAspect
    UBYTE       uifr_YAspect
    WORD        uifr_PageWidth
    WORD        uifr_PageHeight
    STRUCT      uifr_ColorTable,32*2

    FPTR        uifr_BMAllocator
    APTR        uifr_BMap
    ULONG       uifr_UserData
    UWORD       uifr_xb
    UWORD       uifr_yb
    UWORD       uifr_wb
    UWORD       uifr_hb
    UWORD       uifr_xi
    UWORD       uifr_yi
    UWORD       uifr_wi
    UWORD       uifr_hi

    UWORD       uifr_PCHGCompression
    UWORD       uifr_PCHGFlags
    WORD        uifr_PCHGStartLine
    UWORD       uifr_PCHGLineCount
    UWORD       uifr_PCHGChangedLines
    UWORD       uifr_PCHGMinReg
    UWORD       uifr_PCHGMaxReg
    UWORD       uifr_PCHGMaxChanges
    ULONG       uifr_PCHGTotalChanges
    APTR        uifr_PCHGDatas
    LABEL   UIKIlbmFrame_SIZE


IFF_OKAY        equ   0
END_MARK        equ  -1
IFF_DONE        equ  -2
DOS_ERROR       equ  -3
NOT_IFF         equ  -4
NO_FILE         equ  -5
CLIENT_ERROR    equ  -6
BAD_FORM        equ  -7
SHORT_CHUNK     equ  -8
BAD_IFF         equ  -9

; fonctions publiques

UIKFUNC_ILBM_File2BitMap    equ 0
UIKFUNC_ILBM_Colors2Screen  equ 4
UIKFUNC_ILBM_StartPCHG      equ 8
UIKFUNC_ILBM_StopPCHG       equ 12
UIKFUNC_ILBM_CleanupFrame   equ 16
UIKFUNC_ILBM_LastCmd        equ 16


;----ILBMFlags----
;UIKF_ILBM_         equ  $0001

;---------Obj ILBM Tags----------
;UIKTAG_OBJILBM_    equ  (UIKTAG_USER|$0001)

;---------Obj ILBM Flags----------
UIKTAG_OBJILBM_Flags    equ  (UIKTAG_USER|$0020)
;UIKTAG_OBJILBMFl_       equ  (UIKTAG_USER|$0021)

;--

        ENDC
