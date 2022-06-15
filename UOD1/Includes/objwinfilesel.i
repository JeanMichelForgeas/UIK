        IFND    UIKDEFS_WINFILESEL

UIKDEFS_WINFILESEL  SET     1
**
**      Copyright (C) 1993 Jean-Michel Forgeas
**                  Tous Droits Réservés
**

    IFND UIKDEFS_OBJWINDOW
    INCLUDE "UIKI:objwindow.i"
    ENDC

    IFND UIKDEFS_FILESELECT
    INCLUDE "UIKI:objfileselector.i"
    ENDC


 STRUCTURE  WinFileSelPrefs,0
    STRUCT      wfsp_Box,UIKBox_SIZE
    ULONG       wfsp_DevFileWidth
    STRUCT      wfsp_Reserved,4*4
    LABEL   WinFileSelPrefs_SIZE


 STRUCTURE  UIKObjWinFileSel,0
    STRUCT      uowfs_Obj,UIKOBJ_SIZE
    APTR        uowfs_WO
    APTR        uowfs_FS
    APTR        uowfs_UIKBase
    APTR        uowfs_DOSBase
    STRUCT      uowfs_FIB,fib_SIZEOF
    ULONG       uowfs_Flags
    FPTR        uowfs_okfunc
    FPTR        uowfs_cancelfunc
    APTR        uowfs_InitPath
    APTR        uowfs_Pattern
    STRUCT      uowfs_Prefs,WinFileSelPrefs_SIZE
    STRUCT      uowfs_OldPrefs,WinFileSelPrefs_SIZE
    LABEL   UIKObjWinFileSel_SIZE

WINFS_STDWIDTH  equ (6+FS_DEVLIST_MINWIDTH+FS_DIRLIST_MINWIDTH+FS_EXTRA_WIDTH+6)
WINFS_STDHEIGHT equ (3+FS_MIN_HEIGHT+3)


;---------WinFileSel Flags----------
UIK_WFS_ASSIGNLIST      equ $00000002
UIK_WFS_RETURNRETURN    equ $00000004
UIK_WFS_OK_IFNOTEXISTS  equ $00000008
UIK_WFS_DIRSONLY        equ $00000010
UIK_WFS_SHOWICONS       equ $00000020
UIK_WFS_LEAVEPATH       equ $00000040
UIK_WFS_STRINGSTOP      equ $00000080

;---------WinFileSel Tags----------
; Ces Tags doivent rester en correspondance avec ceux
; de l'objet FileSelector
;
UIKTAG_WFS_OKFunc       equ (UIKTAG_USER|$0001)
UIKTAG_WFS_CancelFunc   equ (UIKTAG_USER|$0002)
UIKTAG_WFS_InitPath     equ (UIKTAG_USER|$0003)
UIKTAG_WFS_DevFiles     equ (UIKTAG_USER|$0004)
UIKTAG_WFS_AssignLst    equ (UIKTAG_USER|$0005)
UIKTAG_WFS_AssignList   equ (UIKTAG_USER|$0005)
UIKTAG_WFS_Pattern      equ (UIKTAG_USER|$0006)


;----Flags Tags----
; Ces flags doivent rester en correspondance avec ceux
; de l'objet FileSelector
;
UIKTAG_WFS_Flags            equ (UIKTAG_USER|$0020)
 ; sinon un par un...
UIKTAG_WFSFl_ReturnReturn   equ (UIKTAG_USER|$0021)
UIKTAG_WFSFl_OK_IfNotExists equ (UIKTAG_USER|$0022)
UIKTAG_WFSFl_DirsOnly       equ (UIKTAG_USER|$0023)
UIKTAG_WFSFl_ShowIcons      equ (UIKTAG_USER|$0024)
UIKTAG_WFSFl_StringsTop     equ (UIKTAG_USER|$0025)
UIKTAG_WFSFl_LeavePath      equ (UIKTAG_USER|$0026)


;--

        ENDC
