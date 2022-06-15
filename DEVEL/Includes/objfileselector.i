        IFND    UIKDEFS_FILESELECT
UIKDEFS_FILESELECT  SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**


    IFND UIKDEFS_OBJ
    INCLUDE "UIKI:uikobj.i"
    ENDC

    IFND LIBRARIES_DOS_I
    INCLUDE "libraries/dos.i"
    ENDC


 STRUCTURE  UIKObjFileSelector,0
    STRUCT      uofs_Obj,UIKOBJ_SIZE

    FPTR        uofs_OKFunc
    FPTR        uofs_CancelFunc
    APTR        uofs_UserData

    STRUCT      uofs_PathHead,UIKLIST_SIZE
    APTR        uofs_DevObjList
    APTR        uofs_VolList
    APTR        uofs_AssList
    APTR        uofs_DevList
    APTR        uofs_StrGad
    APTR        uofs_StrPatGad
    APTR        uofs_DevUp
    APTR        uofs_DevDown
    APTR        uofs_DirObjList
    APTR        uofs_DirLeft
    APTR        uofs_DirRight
    APTR        uofs_DirUp
    APTR        uofs_DirDown
    APTR        uofs_WHelp

    WORD        uofs_DevWidth
    WORD        uofs_DirWidth
    ULONG       uofs_DirSelClick
    LONG        uofs_DirSelSecs
    LONG        uofs_DirSelMicros

    APTR        uofs_DirObjProp
    APTR        uofs_DownObjBox
    APTR        uofs_DateObjBox
    APTR        uofs_SizeObjBox

    STRUCT      uofs_FIB,fib_SIZEOF
    ULONG       uofs_Lock
    ULONG       uofs_SigBit
    ULONG       uofs_SigMask
    FPTR        uofs_Vector
    APTR        uofs_UIKHook
    ULONG       uofs_Tmp
    ULONG       uofs_From
    ULONG       uofs_ListNode

    APTR        uofs_SelectorKeysHook
    APTR        uofs_DiskRemovedHook
    APTR        uofs_DiskInsertedHook
    APTR        uofs_MouseButtonsHook
    APTR        uofs_ResizeVerifyHook
    APTR        uofs_ResizeNewSizeHook

    ULONG       uofs_Flags
    UWORD       uofs_Counter
    UWORD       uofs_Counter2
    APTR        uofs_GivenPath
    STRUCT      uofs_PatAux,256
    STRUCT      uofs_TotName,256

    APTR        uofs_IntuitionBase
    APTR        uofs_DOSBase
    APTR        uofs_UIKBase

    APTR        uofs_OKGad
    APTR        uofs_CancelGad
    APTR        uofs_FileBut1
    APTR        uofs_FileBut2
    APTR        uofs_FileProp
    APTR        uofs_DevBut1
    APTR        uofs_DevBut2
    APTR        uofs_DevProp
    APTR        uofs_PathString
    STRUCT      uofs_CmplString,40
    APTR        uofs_ScanGad
    APTR        uofs_StopGad
    STRUCT      uofs_LastDate,4*3
    UWORD       uofs_RefCount
    UWORD       uofs_padw
    LABEL   UIKOBJFILESELECTOR_SIZE
    LABEL   UIKObjFileSelector_SIZE


FS_DEVLIST_MINWIDTH     equ 40
FS_DIRLIST_MINWIDTH     equ 80
FS_EXTRA_WIDTH          equ 60
FS_MIN_HEIGHT           equ 74

*---------File Selector Public functions----------*
UIKFUNC_FS_UpdateFile   equ 0
UIKFUNC_FS_ChangePath   equ 4
UIKFUNC_FS_ChangePattern equ 8

UIKFUNC_FS_LastCmd      equ 8


*---------File Selector Flags----------*
UIK_FS_ASSIGNLIST       equ $00000002
UIK_FS_RETURNRETURN     equ $00000004
UIK_FS_OK_IFNOTEXISTS   equ $00000008
UIK_FS_DIRSONLY         equ $00000010
UIK_FS_SHOWICONS        equ $00000020
UIK_FS_LEAVEPATH        equ $00000040
UIK_FS_STRINGSTOP       equ $00000080
*---------File Selector Bit Number-----*
UIK_FSB_ASSIGNLIST      equ 1
UIK_FSB_RETURNRETURN    equ 2
UIK_FSB_OK_IFNOTEXISTS  equ 3
UIK_FSB_DIRSONLY        equ 4
UIK_FSB_SHOWICONS       equ 5
UIK_FSB_LEAVEPATH       equ 6
UIK_FSB_STRINGSTOP      equ 7

*---------File Selector Tags-----------*
UIKTAG_FS_OKFUNC        equ (UIKTAG_USER|$0001)
UIKTAG_FS_CANCELFUNC    equ (UIKTAG_USER|$0002)
UIKTAG_FS_INITPATH      equ (UIKTAG_USER|$0003)
UIKTAG_FS_DEVFILES      equ (UIKTAG_USER|$0004)
UIKTAG_FS_ASSIGNLST     equ (UIKTAG_USER|$0005)
UIKTAG_FS_PATTERN       equ (UIKTAG_USER|$0006)

*----UIKObj Flags Tags----*
UIKTAG_FS_Flags             equ (UIKTAG_USER|$0020)
* UIKTAG_FS_ASSIGNLST devrait être ici, mais n'y sera pas...
UIKTAG_FSFl_ReturnReturn    equ (UIKTAG_USER|$0021)
UIKTAG_FSFl_OK_IfNotExists  equ (UIKTAG_USER|$0022)
UIKTAG_FSFl_DirsOnly        equ (UIKTAG_USER|$0023)
UIKTAG_FSFl_ShowIcons       equ (UIKTAG_USER|$0024)
UIKTAG_FSFl_StringsTop      equ (UIKTAG_USER|$0025)
UIKTAG_FSFl_LeavePath       equ (UIKTAG_USER|$0026)

*--*

        ENDC
