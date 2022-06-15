#ifndef UIKDEFS_WFONTBUT

#define UIKDEFS_WFONTBUT
/*
 *      Copyright (C) 1993 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef UIKDEFS_WINFONTSEL
#include "uiki:objwinfontsel.h"
#endif

#ifndef UIKDEFS_OBJSTRING
#include "uiki:objstring.h"
#endif


struct UIKObjWFontBut
    {
    struct UIKObj           Obj;
    struct UIKObjWinFontSel *WFS;
    struct UIKObjStrGad     *OS;
    struct UIKObjStrGad     *OSH;
    struct UIKObjGad        *OG;
    struct UIKBase          *UIKBase;
    APTR                    DOSBase;
    APTR                    GfxBase;
    APTR                    IntuitionBase;
    ULONG                   Flags;
    UIKBUF                  *FontPath;
    UBYTE                   FontName[32];
    struct TTextAttr        TTA;
    ULONG                   SelTitle;
    ULONG                   SelAltTitle;
    void                    (*OKFunc)();
    };

#define WFOB_STDWIDTH   100
#define WFOB_STDHEIGHT   16

/*---------WFontBut Flags---------- */
#define UIKF_WFOB_WPROP             0x0001
#define UIKF_WFOB_WFIXED            0x0002

/* Flags spéciaux */
#define UIK_WFOB_STRINGRIGHT        0x01000000
#define UIK_WFOB_AUTOHEIGHT         0x02000000
#define UIK_WFOB_BUTTONNOPUSH       0x04000000


/*---------WFontBut Tags---------- */
/* Ces Tags doivent rester en correspondance avec ceux
 * de l'objet FontSelector
 */
#define UIKTAG_WFOB_OKFunc              (UIKTAG_USER|0x0001)
#define UIKTAG_WFOB_FONTSPATH           (UIKTAG_USER|0x0003)
#define UIKTAG_WFOB_HighlightFont       (UIKTAG_USER|0x0004)

#define UIKTAG_WFOB_ExtraSpace          (UIKTAG_USER|0x0200)
#define UIKTAG_WFOB_ButtonWidth         (UIKTAG_USER|0x0201)
#define UIKTAG_WFOB_ButtonTitle         (UIKTAG_USER|0x0202)
#define UIKTAG_WFOB_ButtonAltTitle      (UIKTAG_USER|0x0203)
#define UIKTAG_WFOB_ButtonActInactPens  (UIKTAG_USER|0x0204)
#define UIKTAG_WFOB_ButtonShortCut      (UIKTAG_USER|0x0205)
#define UIKTAG_WFOB_SelectorTitle       (UIKTAG_USER|0x0206)
#define UIKTAG_WFOB_SelectorAltTitle    (UIKTAG_USER|0x0207)


/*----WFontBut Flags Tags---- */
/* Ces flags doivent rester en correspondance avec ceux
 * de l'objet FontSelector
 */
#define UIKTAG_WFOB_Flags               (UIKTAG_USER|0x0020)
 /* sinon un par un... */
#define UIKTAG_WFOBFl_PropWidth         (UIKTAG_USER|0x0021) /* TRUE */
#define UIKTAG_WFOBFl_FixedWidth        (UIKTAG_USER|0x0022) /* TRUE */

/* Flags spéciaux */
#define UIKTAG_WFOBFl_StringRight       (UIKTAG_USER|0x0100) /* FALSE */
#define UIKTAG_WFOBFl_AutoHeight        (UIKTAG_USER|0x0101) /* TRUE */
#define UIKTAG_WFOBFl_ButtonImage       (UIKTAG_USER|0x0102)
#define UIKTAG_WFOBFl_ButtonNoPush      (UIKTAG_USER|0x0103)


/*-- */

#endif
