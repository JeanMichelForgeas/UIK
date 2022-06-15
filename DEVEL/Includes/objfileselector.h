#ifndef UIKDEFS_FILESELECT

#define UIKDEFS_FILESELECT
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

#ifndef UIKDEFS_OBJLIST
#include "uiki:objlist.h"
#endif

#ifndef UIKDEFS_OBJSTRING
#include "uiki:objstring.h"
#endif

#ifndef UIKDEFS_OBJPROPGAD
#include "uiki:objproplift.h"
#endif

#ifndef LIBRARIES_DOS_H
#include <libraries/dos.h>
#endif


struct UIKObjFileSelector
    {
    struct UIKObj       Obj;

    void                (*OKFunc)();
    void                (*CancelFunc)();
    APTR                UserData;

    struct UIKList      PathHead;
    struct UIKObjList   *DevObjList;
    struct UIKList      *VolList;
    struct UIKList      *AssList;
    struct UIKList      *DevList;
    struct UIKObjStrGad *StrGad;
    struct UIKObjStrGad *StrPatGad;
    struct UIKObj       *DevUp;
    struct UIKObj       *DevDown;
    struct UIKObjList   *DirObjList;
    struct UIKObj       *DirLeft;
    struct UIKObj       *DirRight;
    struct UIKObj       *DirUp;
    struct UIKObj       *DirDown;
    struct UIKObjWindow *WHelp;

    WORD                DevWidth;
    WORD                DirWidth;
    ULONG               DirSelClick;
    LONG                DirSelSecs;
    LONG                DirSelMicros;

    struct UIKObjPropGad *DirObjProp;
    struct UIKObj       *DownObjBox;
    struct UIKObj       *DateObjBox;
    struct UIKObj       *SizeObjBox;

    struct FileInfoBlock FIB;
    ULONG               Lock;
    ULONG               SigBit;
    ULONG               SigMask;
    void                (*Vector)();
    struct UIKHook      *UIKHook;
    ULONG               Tmp, From;
    ULONG               ListNode;

    struct UIKHook      *SelectorKeysHook;
    struct UIKHook      *DiskRemovedHook;
    struct UIKHook      *DiskInsertedHook;
    struct UIKHook      *MouseButtonsHook;
    struct UIKHook      *ResizeVerifyHook;
    struct UIKHook      *ResizeNewSizeHook;

    ULONG               Flags;
    UWORD               Counter;
    UWORD               Counter2;
    UBYTE               *GivenPath;
    UBYTE               PatAux[256];
    UBYTE               TotName[256];

    struct IntuitionBase *IntuitionBase;
    struct DosLibrary   *DOSBase;
    struct UIKBase      *UIKBase;

    struct UIKObj       *OKGad;
    struct UIKObj       *CancelGad;
    struct UIKObj       *FileBut1;
    struct UIKObj       *FileBut2;
    struct UIKObj       *FileProp;
    struct UIKObj       *DevBut1;
    struct UIKObj       *DevBut2;
    struct UIKObj       *DevProp;
    UBYTE               *PathString;
    UBYTE               CmplString[40];
    struct UIKObj       *ScanGad;
    struct UIKObj       *StopGad;
    struct DateStamp    LastDate;
    UWORD               RefCount;
    UWORD               padw;
    };


#define FS_DEVLIST_MINWIDTH     40
#define FS_DIRLIST_MINWIDTH     80
#define FS_EXTRA_WIDTH          60
#define FS_MIN_HEIGHT           74


/*---------File Selector Public functions---------- */
#define UIKFUNC_FS_UpdateFile   0
#define UIKFUNC_FS_ChangePath   4
#define UIKFUNC_FS_ChangePattern 8

#define UIKFUNC_FS_LastCmd      8


/*---------File Selector Flags---------- */
#define UIK_FS_VOLUMELIST       0x00000001
#define UIK_FS_ASSIGNLIST       0x00000002
#define UIK_FS_RETURNRETURN     0x00000004
#define UIK_FS_OK_IFNOTEXISTS   0x00000008
#define UIK_FS_DIRSONLY         0x00000010
#define UIK_FS_SHOWICONS        0x00000020
#define UIK_FS_LEAVEPATH        0x00000040
#define UIK_FS_STRINGSTOP       0x00000080
#define UIK_FS_REDISPLAY        0x00000100

/*---------File Selector Tags---------- */
/* Ces Tags doivent rester en correspondance avec ceux
 * de l'objet WinFileSel
 */
#define UIKTAG_FS_OKFUNC        (UIKTAG_USER|0x0001)
#define UIKTAG_FS_CANCELFUNC    (UIKTAG_USER|0x0002)
#define UIKTAG_FS_INITPATH      (UIKTAG_USER|0x0003)
#define UIKTAG_FS_DEVFILES      (UIKTAG_USER|0x0004)
#define UIKTAG_FS_ASSIGNLST     (UIKTAG_USER|0x0005)
#define UIKTAG_FS_ASSIGNLIST    (UIKTAG_USER|0x0005)
#define UIKTAG_FS_PATTERN       (UIKTAG_USER|0x0006)

/*----UIKObj Flags Tags---- */
/* Ces flags doivent rester en correspondance avec ceux
 * de l'objet WinFileSel
 */
#define UIKTAG_FS_Flags         (UIKTAG_USER|0x0020)
 /* sinon un par un... */
/*UIKTAG_FS_ASSIGNLST devrait être ici, mais n'y sera pas... */
#define UIKTAG_FSFl_ReturnReturn    (UIKTAG_USER|0x0021)
#define UIKTAG_FSFl_OK_IfNotExists  (UIKTAG_USER|0x0022)
#define UIKTAG_FSFl_DirsOnly        (UIKTAG_USER|0x0023)
#define UIKTAG_FSFl_ShowIcons       (UIKTAG_USER|0x0024)
#define UIKTAG_FSFl_StringsTop      (UIKTAG_USER|0x0025)
#define UIKTAG_FSFl_LeavePath       (UIKTAG_USER|0x0026)


/*---------------- Object private, subject to change --------------*/

struct pathnode
    {
    struct UIKList  NodeHead;
    UWORD           Vis;
    UWORD           Pos;
    UWORD           Max;        /* pour flag UIKL_FIXED    */
    UWORD           Sel;        /* pour flag UIKL_CLICKSEL */
    ULONG           Lock;
    struct DateStamp Date;
    UWORD           Flags;
    UWORD           Pad;
    UBYTE           NodeName[1];
    };

#define FSLOCK_ANY  3
#define FSLOCK_DIR  1
#define FSLOCK_FILE 2

/* pathnode */
#define DIR_SCANNED 0x0001

/* lines */
#define NAMED_DIRECTORY (UIKN_IMAGEFIRST)

#define DIRECTORY_CHARS "  "
#define DIRECTORY_CHNUM 2


/*-- */

#endif
