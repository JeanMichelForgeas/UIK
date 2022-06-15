#ifndef UIKDEFS_WFILEBUT

#define UIKDEFS_WFILEBUT
/*
 *      Copyright (C) 1993 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef UIKDEFS_WINFILESEL
#include "uiki:objwinfilesel.h"
#endif

#ifndef UIKDEFS_OBJSTRING
#include "uiki:objstring.h"
#endif


struct UIKObjWFileBut
    {
    struct UIKObj           Obj;
    struct UIKObjWinFileSel *WFS;
    struct UIKObjStrGad     *OS;
    struct UIKObjGad        *OG;
    struct UIKBase          *UIKBase;
    APTR                    DOSBase;
    ULONG                   Flags;
    UIKBUF                  *Pattern;
    ULONG                   SelTitle;
    ULONG                   SelAltTitle;
    void                    (*OKFunc)();
    };

#define WFB_STDWIDTH    100
#define WFB_STDHEIGHT    16

/*---------WFileBut Flags---------- */
#define UIK_WFB_ASSIGNLIST      0x00000002
#define UIK_WFB_RETURNRETURN    0x00000004
#define UIK_WFB_OK_IFNOTEXISTS  0x00000008
#define UIK_WFB_DIRSONLY        0x00000010
#define UIK_WFB_SHOWICONS       0x00000020
#define UIK_WFB_LEAVEPATH       0x00000040
#define UIK_WFB_STRINGSTOP      0x00000080

/* Flags spéciaux */
#define UIK_WFB_STRINGRIGHT     0x01000000
#define UIK_WFB_AUTOHEIGHT      0x02000000
#define UIK_WFB_BUTTONNOPUSH    0x04000000


/*---------WFileBut Tags---------- */
/* Ces Tags doivent rester en correspondance avec ceux
 * de l'objet FileSelector
 */
#define UIKTAG_WFB_OKFunc       (UIKTAG_USER|0x0001)
#define UIKTAG_WFB_InitPath     (UIKTAG_USER|0x0003)
#define UIKTAG_WFB_AssignLst    (UIKTAG_USER|0x0005)
#define UIKTAG_WFB_AssignList   (UIKTAG_USER|0x0005)
#define UIKTAG_WFB_Pattern      (UIKTAG_USER|0x0006)

#define UIKTAG_WFB_ExtraSpace           (UIKTAG_USER|0x0200)
#define UIKTAG_WFB_ButtonWidth          (UIKTAG_USER|0x0201)
#define UIKTAG_WFB_ButtonTitle          (UIKTAG_USER|0x0202)
#define UIKTAG_WFB_ButtonAltTitle       (UIKTAG_USER|0x0203)
#define UIKTAG_WFB_ButtonActInactPens   (UIKTAG_USER|0x0204)
#define UIKTAG_WFB_ButtonShortCut       (UIKTAG_USER|0x0205)
#define UIKTAG_WFB_SelectorTitle        (UIKTAG_USER|0x0206)
#define UIKTAG_WFB_SelectorAltTitle     (UIKTAG_USER|0x0207)


/*----WFileBut Flags Tags---- */
/* Ces flags doivent rester en correspondance avec ceux
 * de l'objet FileSelector
 */
#define UIKTAG_WFB_Flags         (UIKTAG_USER|0x0020)
 /* sinon un par un... */
#define UIKTAG_WFBFl_ReturnReturn    (UIKTAG_USER|0x0021)
#define UIKTAG_WFBFl_OK_IfNotExists  (UIKTAG_USER|0x0022)
#define UIKTAG_WFBFl_DirsOnly        (UIKTAG_USER|0x0023)
#define UIKTAG_WFBFl_ShowIcons       (UIKTAG_USER|0x0024)
#define UIKTAG_WFBFl_StringsTop      (UIKTAG_USER|0x0025)
#define UIKTAG_WFBFl_LeavePath       (UIKTAG_USER|0x0026)

/* Flags spéciaux */
#define UIKTAG_WFBFl_StringRight     (UIKTAG_USER|0x0100)   /* FALSE */
#define UIKTAG_WFBFl_AutoHeight      (UIKTAG_USER|0x0101)   /* TRUE */
#define UIKTAG_WFBFl_ButtonImage     (UIKTAG_USER|0x0102)
#define UIKTAG_WFBFl_ButtonNoPush    (UIKTAG_USER|0x0103)


/*-- */

#endif
