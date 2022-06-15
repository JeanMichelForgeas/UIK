#ifndef UIKDEFS_OBJPALETTE

#define UIKDEFS_OBJPALETTE

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

#ifndef UIKDEFS_TAGITEM_H
#include "uiki:uiktag.h"
#endif


struct UIKObjPalette
    {
    struct UIKObj             Obj;

    struct UIKObjWindow       *Wino;
    struct UIKObjPropGad      *rprop,*gprop,*bprop;
    struct UIKObj             *ColFram,*RVBFram;
    struct UIKObj             *rdisp,*gdisp,*bdisp;
    struct UIKObj             *rdisp2,*gdisp2,*bdisp2;
    struct UIKObj             *colname;
    struct UIKObjPaletteDispl *paldsp;
    struct UIKObj             *restore;
    struct UIKObj             *ok;
    };

/*---------Obj Palette Tags---------- */
/* tous les titres de bouton sont stock�s par le programme principal
 * dans ses deux tableaux de titres, chacun en une langue.
 * L'ordre des titres doit �tre (en fran�ais):
 * "Palette", "Couleurs", "R", "V", "B", "Restaure".
 * Il faut passer � ce tag le num�ro d'ordre dans le tableau du la
 * chaine "Palette". Les num�ros d'ordre des autres en seront d�duits
 * par +1, +2, etc...
 */
#define UIKTAG_ObjPalette_FirstTitleNum     (UIKTAG_USER|0x0001)

/*---------Obj Palette Flags---------- */
/*#define UIKTAG_OBJPalette_Flags          (UIKTAG_USER|0x0020)*/
 /* sinon un par un... */
/*#define UIKTAG_OBJPaletteFl_  (UIKTAG_USER|0x0021)*/

/*-- */

#endif
