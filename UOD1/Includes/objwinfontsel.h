#ifndef UIKDEFS_WINFONTSEL

#define UIKDEFS_WINFONTSEL
/*
 *      Copyright (C) 1993 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef UIKDEFS_OBJWINDOW
#include "uiki:objwindow.h"
#endif

#ifndef UIKDEFS_FONTSELECT
#include "uiki:objfontselector.h"
#endif


struct WinFontSelPrefs {
    struct UIKBox   Box;
    ULONG           Reserved[4];
    };


struct UIKObjWinFontSel
    {
    struct UIKObj           Obj;
    struct UIKObjWindow     *WO;
    struct UIKObjFontSelect *FS;
    struct UIKBase          *UIKBase;
    struct DOSBase          *DOSBase;
    struct FileInfoBlock    FIB;
    ULONG                   Flags;
    void                    (*okfunc)();
    void                    (*cancelfunc)();
    UBYTE                   *FontPath;
    struct TTextAttr        HighLightTTA;
    struct WinFontSelPrefs  Prefs;
    struct WinFontSelPrefs  OldPrefs;
    };

#define WINFOS_STDWIDTH  (6+300+6)
#define WINFOS_STDHEIGHT (3+120+3)


/*---------WinFontSel Flags---------- */
#define UIKF_WFOS_WPROP  0x0001
#define UIKF_WFOS_WFIXED 0x0002

/*---------WinFontSel Tags---------- */
/* Ces Tags doivent rester en correspondance avec ceux
 * de l'objet FontSelector
 */
#define UIKTAG_WFOS_OKFUNC          (UIKTAG_USER|0x0001)
#define UIKTAG_WFOS_CANCELFUNC      (UIKTAG_USER|0x0002)
#define UIKTAG_WFOS_FONTSPATH       (UIKTAG_USER|0x0003)
#define UIKTAG_WFOS_HighlightFont   (UIKTAG_USER|0x0004)


/*----UIKObj Flags Tags---- */
/* Ces flags doivent rester en correspondance avec ceux
 * de l'objet FontSelector
 */
#define UIKTAG_WFOS_Flags           (UIKTAG_USER|0x0020)
 /* sinon un par un... */
#define UIKTAG_WFOSFl_PropWidth     (UIKTAG_USER|0x0021) /* TRUE */
#define UIKTAG_WFOSFl_FixedWidth    (UIKTAG_USER|0x0022) /* TRUE */


/*-- */

#endif
