#ifndef UIKDEFS_OBJSCREEN

#define UIKDEFS_OBJSCREEN
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

#ifndef UIKDEFS_COLORS
#include "uiki:uikcolors.h"
#endif


struct UIKObjScreen
    {
    struct UIKObj       Obj;
    struct NewScreen    NS;
    struct TextFont     *SFont;
    UWORD               ColorTable[MAX_UIKCOLORS];
    ULONG               ModeId;
    UWORD               TableColorNum;
    struct UIKColors    Colors;

    struct IntuitionBase *IntuitionBase;
    struct DosLibrary   *DOSBase;
    struct GfxBase      *GfxBase;
    struct UIKBase      *UIKBase;

    UWORD               *Pens;
    ULONG               Flags;
    struct TagItem      *IntuiTags;
    UBYTE               Mask;
    UBYTE               padb;
    UWORD               padw;
    };

#define UIKSCRF_PUBLIC      0x0001
#define UIKSCRF_MAXDEPTH    0x0002

/*---------Obj Screen Tags---------- */
#define UIKTAG_ObjScreen_Depth          (UIKTAG_USER|0x0001) /* depth for the screen display */
#define UIKTAG_ObjScreen_CustomBitMap   (UIKTAG_USER|0x0002) /* start with this custom bitmap */
#define UIKTAG_ObjScreen_ViewModes      (UIKTAG_USER|0x0003) /* viewmodes */
#define UIKTAG_ObjScreen_ColorTable     (UIKTAG_USER|0x0004) /* pointeur sur tableau de n UWORD codant chacun le RGB */
#define UIKTAG_OBJScreen_ShowTitle      (UIKTAG_USER|0x0005) /* cache la barre de titre */
#define UIKTAG_OBJScreen_TableColorNum  (UIKTAG_USER|0x0006) /* nombre de UWORD dans la ColorTable (defaut: nombre de couleurs du screen) */
#define UIKTAG_OBJScreen_RedirectColors (UIKTAG_USER|0x0007) /* pointeur sur une struct UIKColors de redirection des couleurs */
#define UIKTAG_OBJScreen_ScreenQuiet    (UIKTAG_USER|0x0008) /* pour que Intuition de dessine pas dans l'écran */
#define UIKTAG_OBJScreen_IntuiPens      (UIKTAG_USER|0x0009) /* pen array pour l'écran (voir Intuition SA_Pens) */
#define UIKTAG_OBJScreen_IntuiTags      (UIKTAG_USER|0x000a) /* TagList of Intuition Screen tags */

/*---------Obj Screen Flags---------- */
#define UIKTAG_OBJScreenFl_Public       (UIKTAG_USER|0x0021)
#define UIKTAG_OBJScreenFl_MaxDepth     (UIKTAG_USER|0x0022)
#define UIKTAG_OBJScreenFl_AutoCenter   (UIKTAG_USER|0x0023)

/*-- */

#endif
