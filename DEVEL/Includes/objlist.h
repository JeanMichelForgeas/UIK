#ifndef UIKDEFS_OBJLIST

#define UIKDEFS_OBJLIST
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

#ifndef UIKDEFS_OBJPROPGAD
#include "uiki:objproplift.h"
#endif

struct TextEntry
    {
    UWORD               Flags;
    UBYTE               *String;
    };

struct UIKObjList
    {
    struct UIKObj       Obj;
    struct Gadget       Gad;
    UWORD               Vis, Pos;
    UWORD               Max;
    UWORD               Sel;
    UBYTE               ListPen;
    UBYTE               SelectPen;
    UBYTE               SelectTextPen;
    UBYTE               SelectLen;
    struct UIKObjProp   *FeedBack;
    ULONG               ListFlags;
    struct UIKList      ListText;
    int                 (*SortFunc)();
    };

/*-----Text List flags----*/
#define UIKL_LEFT       0x0000
#define UIKL_JUSTIFY    0x0001
#define UIKL_CENTER     0x0002
#define UIKL_RIGHT      0x0004
#define UIKL_CLICKSEL   0x0008
#define UIKL_SELDISPL   0x0010
#define UIKL_SORTED     0x0020
#define UIKL_FIXED      0x0040

/*-----List Line flags----*/
#define UIKN_IMAGEFIRST     0x0001
#define UIKN_IMAGEAFTER     0x0002

#define UIKN_PRIVATEFLAGS   0x00ff
#define UIKN_USERFLAGS      0xff00

/*----UIKObjList values Tags---- */
#define UIKTAG_OBJList_TextEntry     (UIKTAG_USER|0x0001)
#define UIKTAG_OBJList_ListPen       (UIKTAG_USER|0x0002)
#define UIKTAG_OBJList_SelectPen     (UIKTAG_USER|0x0003)
#define UIKTAG_OBJList_SelectTextPen (UIKTAG_USER|0x0004)
#define UIKTAG_OBJList_MaxLines      (UIKTAG_USER|0x0005)
#define UIKTAG_OBJList_TopPosition   (UIKTAG_USER|0x0006)
#define UIKTAG_OBJList_SelectedLine  (UIKTAG_USER|0x0007)
#define UIKTAG_OBJList_SortFunc      (UIKTAG_USER|0x0008)
#define UIKTAG_OBJList_TextPtr       (UIKTAG_USER|0x0009)

/*----UIKObjList Flags Tags---- */
#define UIKTAG_OBJList_Flags         (UIKTAG_USER|0x0020)
#define UIKTAG_OBJListFl_CenLeft     (UIKTAG_USER|0x0021)
#define UIKTAG_OBJListFl_CenJustify  (UIKTAG_USER|0x0022)
#define UIKTAG_OBJListFl_CenCenter   (UIKTAG_USER|0x0023)
#define UIKTAG_OBJListFl_CenRight    (UIKTAG_USER|0x0024)
#define UIKTAG_OBJListFl_ClickSel    (UIKTAG_USER|0x0025)
#define UIKTAG_OBJListFl_SelDispl    (UIKTAG_USER|0x0026)
#define UIKTAG_OBJListFl_Sorted      (UIKTAG_USER|0x0027)

#endif /* UIKDEFS_OBJLIST */
