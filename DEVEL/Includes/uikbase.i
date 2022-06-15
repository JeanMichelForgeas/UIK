    IFND  UIK_BASE_I
UIK_BASE_I SET 1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**

    IFND  EXEC_TYPES_I
    INCLUDE  'exec/types.i'
    ENDC   ; EXEC_TYPES_I

    IFND  EXEC_SEMAPHORES_I
    INCLUDE  'exec/semaphores.i'
    ENDC   ; EXEC_SEMAPHORES_I

    IFND  EXEC_ABLES_I
    INCLUDE "exec/ables.i"
    ENDC   ; EXEC_ABLES_I

    IFND  EXEC_LIBRARIES_I
    INCLUDE  'exec/libraries.i'
    ENDC   ; EXEC_LIBRARIES_I

    IFND  EXEC_INTERRUPTS_I
    INCLUDE  'exec/interrupts.i'
    ENDC   ; EXEC_INTERRUPTS_I

    IFND    GRAPHICS_TEXT_I
    INCLUDE  'graphics/text.i'
    ENDC

    IFND    UIKDEFS_LIST
    INCLUDE  'uiki:uiklist.i'
    ENDC

    IFND    UIKDEFS_COLORS
    INCLUDE  'uiki:uikcolors.i'
    ENDC

    IFND    UIKDEFS_KEYSUPP
    INCLUDE  "uiki:uikkey.i"
    ENDC

UIKNAME MACRO
        dc.b    'uik.library',0
        ENDM


 STRUCTURE  UIKPrefs,0
    STRUCT      uikp_Colors,MAX_UIKCOLORS
    STRUCT      uikp_ColorTable,MAX_UIKCOLORS*2
    UWORD       uikp_PrefsFlags
    UWORD       uikp_DefFontHeight
    STRUCT      uikp_DefFontName,32
    UWORD       uikp_DefScreen_Depth
    UWORD       uikp_DefScreen_ModeId   ; !!! en fait c'est un ULONG
    UWORD       uikp_DefScreen_Modes
    UWORD       uikp_DefScreen_Width
    UWORD       uikp_DefScreen_Height
    UBYTE       uikp_InputMode
    UBYTE       uikp_InputHeld
    UBYTE       uikp_InputAuto
    UBYTE       uikp_ZoneThick
    UWORD       uikp_ZoneSpeed
    UWORD       uikp_ValidTime
    UWORD       uikp_WaitTime
    UWORD       uikp_InputFlags
    UBYTE       uikp_DefScreen_OScan
    UBYTE       uikp_Mask
    UWORD       uikp_Padw
    LABEL   UIKPrefs_SIZE

; Overscan types :
UIKF_CUSTOM_OSCAN equ 0
UIKF_TEXT_OSCAN   equ 1
UIKF_GFX_OSCAN    equ 2
UIKF_MIN_OSCAN    equ 3
UIKF_MAX_OSCAN    equ 4

;------ uikp_PrefsFlags
UIKLANGB            equ     0   ---bit 0
UIKLANG_ENGLISH     equ $0000   valeur bit 0
UIKLANG_OTHER       equ $0001   valeur bit 0

UIKDEFAULT_SCREEN   equ $0002   default screen requested
UIKLANG_LOCALE      equ $0004
UIKF_SPLOOK         equ $0008
UIKF_SYSLOOK        equ $0010


UIKDEFAULTB_SCREEN  equ 1
UIKLANGB_LOCALE     equ 2
UIKB_SPLOOK         equ 3
UIKB_SYSLOOK        equ 4


;------ uikp_InputMode
INPUTMODE_MOUSE     equ 0
INPUTMODE_BOURIS    equ 1
INPUTMODE_JOYMOUSE  equ 2
INPUTMODE_1CONTACT  equ 3
INPUTMODE_2CONTACTS equ 4
INPUTMODE_5CONTACTS equ 5
INPUTMODE_NUMBER    equ 6

;------ uikp_InputFlags
INPUTF_SLIDEZONE    equ $0001
INPUTF_FRAMEPOINTER equ $0002
INPUTF_JAMED        equ $0004
INPUTF_KZAKZ_ON     equ $0008

INPUTB_SLIDEZONE    equ 0
INPUTB_FRAMEPOINTER equ 1
INPUTB_JAMED        equ 2
INPUTB_KZAKZ_ON     equ 3


 STRUCTURE  UIKBase,0
    STRUCT      lb_Library,LIB_SIZE
    UBYTE       lb_Flags
    UBYTE       lb_UIKFlags
    ;We are now longword aligned
    ULONG       lb_SegList

    STRUCT      lb_RegisteredObjList,UIKLIST_SIZE
    STRUCT      lb_RegisteredImaList,UIKLIST_SIZE
    STRUCT      lb_RegObjSemaphore,SS_SIZE
    STRUCT      lb_RegImaSemaphore,SS_SIZE

    APTR        lb_UIKPrefs
    STRUCT      lb_PrefsSemaphore,SS_SIZE

    APTR        lb_DefaultFont
    STRUCT      lb_KeySupp,UIKKEYSUPP_SIZE

    APTR        lb_DefaultScreen
    STRUCT      lb_DefaultFontAttr,tta_SIZEOF

    STRUCT      lb_VBlankInt,IS_SIZE
    ULONG       lb_Ticks
    APTR        lb_ZonedUIK

    UWORD       lb_OpenCount
    UWORD       lb_padw
    APTR        lb_LocaleBase
    STRUCT      lb_TaskList,LH_SIZE
    APTR        lb_ScrPubTask
    LABEL   UIKBase_SIZE
    LABEL   LibBase_SIZEOF

;------ lb_UIKFlags


    ENDC
