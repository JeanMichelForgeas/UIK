#ifndef UIKDEFS_FONTSELECT

#define UIKDEFS_FONTSELECT
/*
 *      (C) Copyright 1991,1992,1993 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef LIBRARIES_DOS_H
#include <libraries/dos.h>
#endif

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

struct FOSVars
    {
        struct UIKObjList   *lo1;
        struct UIKObjProp   *po1;
        struct UIKObj       *bxo1;
        struct UIKObj       *tbo1;
        struct UIKObj       *bbo1;
        struct UIKObj       *upo1;
        struct UIKObj       *downo1;
        struct UIKObj       *to0;
        struct UIKObjList   *lo2;
        struct UIKObjProp   *po2;
        struct UIKObj       *bxo2;
        struct UIKObj       *tbo2;
        struct UIKObj       *bbo2;
        struct UIKObj       *upo2;
        struct UIKObj       *downo2;
        struct UIKObjStrGad *stro1;
        struct UIKObjStrGad *stro2;
        struct UIKObj       *to1;
        struct UIKObj       *to2;
        struct UIKObj       *to3;
        struct UIKObj       *meo1;
        struct UIKObj       *meo2;
        struct UIKObj       *meo3;
        struct UIKObjGad    *bo;
        struct UIKObj       *oko;
        struct UIKObj       *co;
    };

struct UIKObjFontSelect
    {
    struct UIKObj       Obj;

    struct Library      *DOSBase;
    struct Library      *UIKBase;
    struct Library      *GfxBase;
    struct Library      *IntuitionBase;
    struct Library      *LayersBase;
    struct Library      *DiskfontBase;

    struct UIKList      NodeHead;
    struct UIKList      NameHead;
    struct UIKList      HeightHead;
    struct UIKList      DisplayHead;
    ULONG               Scanned;

    UBYTE               *InsStr;
    UBYTE               *InsStr2;
    ULONG               Index;
    ULONG               Result;
    UBYTE               *FontsPath;
    void                (*OKFunc)();
    void                (*CancelFunc)();

    struct FOSVars      ObjVars;
    UWORD               CurrentSize;
    UBYTE               Style;
    UBYTE               DispStatus;
    ULONG               Nil;
    UIKBUF              *Com;
    ULONG               FFlags;
    UWORD               MinWidth;
    UWORD               MinHeight;
    struct UIKHook      *ResizeHook;
    struct UIKObj       *ResizeWindow;
    struct UIKHook      *KeyHook;
    struct UIKObj       *KeyWindow;
    };

#define OFFS(n) (offsetof(struct FOSVars,n))


/*--------DispStatus-------------------- */
#define NODISPLAY       0
#define MONODISPLAY     1
#define MULTIDISPLAY    2

/*---------Font Selector Flags---------- */
#define UIKF_FOS_WPROP  0x0001
#define UIKF_FOS_WFIXED 0x0002

/*---------Font Selector Tags---------- */
#define UIKTAG_FOS_OKFUNC        (UIKTAG_USER|0x0001)
#define UIKTAG_FOS_CANCELFUNC    (UIKTAG_USER|0x0002)
#define UIKTAG_FOS_FONTSPATH     (UIKTAG_USER|0x0003)
#define UIKTAG_FOS_HighlightFont (UIKTAG_USER|0x0004)

/*----UIKObj Flags Tags---- */
#define UIKTAG_FOS_Flags         (UIKTAG_USER|0x0020)
 /* sinon un par un... */
#define UIKTAG_FOSFl_PropWidth   (UIKTAG_USER|0x0021) /* TRUE */
#define UIKTAG_FOSFl_FixedWidth  (UIKTAG_USER|0x0022) /* TRUE */


/*-- */

#endif
