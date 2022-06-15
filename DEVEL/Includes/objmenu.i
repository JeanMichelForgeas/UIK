        IFND    UIKDEFS_MENU
UIKDEFS_MENU SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**


    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC


 STRUCTURE  UIKMenuEntry,0
    UBYTE       umen_Depth
    UBYTE       umen_ComKey
    ULONG       umen_Title
    FPTR        umen_Func
    ULONG       umen_UserData
    ULONG       umen_MutualExclude
    APTR        umen_IntuiData
    UWORD       umen_Flags
    LABEL   UIKMenuEntry_SIZE

MENUTEXTADDR    equ $8000


 STRUCTURE  UIKObjMenu,0
    STRUCT      uome_Obj,UIKOBJ_SIZE
    ULONG       uome_Flags
    APTR        uome_EnglishMenu
    APTR        uome_OtherMenu
    APTR        uome_MenuEntry
    APTR        uome_Menu
    FPTR        uome_MenuFunc
    STRUCT      uome_TTAttr,tta_SIZEOF
    APTR        uome_UIKBase
    APTR        uome_DOSBase
    APTR        uome_GfxBase
    APTR        uome_IntuitionBase
    UWORD       uome_TitleSep
    UBYTE       uome_TitleStart
    UBYTE       uome_TitleExtra
    STRUCT      uome_Strings,UIKList_SIZE
    UWORD       uome_LifeId
    UWORD       uome_Padw
    LABEL   UIKObjMenu_SIZE


;---------Obj Flags---------
MENUSTRIP_SET   equ $0001
ONEENTRY        equ $0002
MENUCHANGED     equ $0004
SSEP_FONTS      equ $0008


;---------fonctions publiques
UIKFUNC_Menu_AddEntry       equ 0
UIKFUNC_Menu_RemoveEntry    equ 4
UIKFUNC_Menu_EnableEntry    equ 8
UIKFUNC_Menu_DisableEntry   equ 12
UIKFUNC_Menu_GetEntryPos    equ 16
UIKFUNC_Menu_GetPosEntry    equ 20

UIKFUNC_Menu_LastCmd        equ 20

;---------Obj Tags----------
UIKTAG_Menu_Entry           equ (UIKTAG_USER|$0003)
UIKTAG_Menu_TitleStart      equ (UIKTAG_USER|$0004)
UIKTAG_Menu_TitleSeparation equ (UIKTAG_USER|$0005)
UIKTAG_Menu_TitleExtra      equ (UIKTAG_USER|$0006)

;---------Obj Flags Tags----------
UIKTAG_MenuFl_SysSepFont    equ (UIKTAG_USER|$0040)

;--

        ENDC
