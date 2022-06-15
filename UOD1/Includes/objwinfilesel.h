#ifndef UIKDEFS_WINFILESEL

#define UIKDEFS_WINFILESEL
/*
 *      Copyright (C) 1993 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef UIKDEFS_OBJWINDOW
#include "uiki:objwindow.h"
#endif

#ifndef UIKDEFS_FILESELECT
#include "uiki:objfileselector.h"
#endif


struct WinFileSelPrefs {
    struct UIKBox   Box;
    ULONG           DevFileWidth;
    UBYTE           StringsTop;
    UBYTE           ReservedB[3];
    ULONG           ReservedL[3];
    };


struct UIKObjWinFileSel
    {
    struct UIKObj               Obj;
    struct UIKObjWindow         *WO;
    struct UIKObjFileSelector   *FS;
    struct UIKBase              *UIKBase;
    struct DOSBase              *DOSBase;
    struct FileInfoBlock        FIB;
    ULONG                       Flags;
    void                        (*okfunc)();
    void                        (*cancelfunc)();
    UBYTE                       *InitPath;
    UBYTE                       *Pattern;
    struct WinFileSelPrefs      Prefs;
    struct WinFileSelPrefs      OldPrefs;
    };

#define WINFS_STDWIDTH  (6+FS_DEVLIST_MINWIDTH+FS_DIRLIST_MINWIDTH+FS_EXTRA_WIDTH+6)
#define WINFS_STDHEIGHT (3+FS_MIN_HEIGHT+3)


/*---------WinFileSel Flags---------- */
#define UIK_WFS_ASSIGNLIST      0x00000002
#define UIK_WFS_RETURNRETURN    0x00000004
#define UIK_WFS_OK_IFNOTEXISTS  0x00000008
#define UIK_WFS_DIRSONLY        0x00000010
#define UIK_WFS_SHOWICONS       0x00000020
#define UIK_WFS_LEAVEPATH       0x00000040
#define UIK_WFS_STRINGSTOP      0x00000080

/*---------WinFileSel Tags---------- */
/* Ces Tags doivent rester en correspondance avec ceux
 * de l'objet FileSelector
 */
#define UIKTAG_WFS_OKFunc        (UIKTAG_USER|0x0001)
#define UIKTAG_WFS_CancelFunc    (UIKTAG_USER|0x0002)
#define UIKTAG_WFS_InitPath      (UIKTAG_USER|0x0003)
#define UIKTAG_WFS_DevFiles      (UIKTAG_USER|0x0004)
#define UIKTAG_WFS_AssignLst     (UIKTAG_USER|0x0005)
#define UIKTAG_WFS_AssignList    (UIKTAG_USER|0x0005)
#define UIKTAG_WFS_Pattern       (UIKTAG_USER|0x0006)


/*----UIKObj Flags Tags---- */
/* Ces flags doivent rester en correspondance avec ceux
 * de l'objet FileSelector
 */
#define UIKTAG_WFS_Flags         (UIKTAG_USER|0x0020)
 /* sinon un par un... */
#define UIKTAG_WFSFl_ReturnReturn    (UIKTAG_USER|0x0021)
#define UIKTAG_WFSFl_OK_IfNotExists  (UIKTAG_USER|0x0022)
#define UIKTAG_WFSFl_DirsOnly        (UIKTAG_USER|0x0023)
#define UIKTAG_WFSFl_ShowIcons       (UIKTAG_USER|0x0024)
#define UIKTAG_WFSFl_StringsTop      (UIKTAG_USER|0x0025)
#define UIKTAG_WFSFl_LeavePath       (UIKTAG_USER|0x0026)


/*-- */

#endif
