#ifndef UIKDEFS_OBJREQMODE

#define UIKDEFS_OBJREQMODE

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

#ifndef UIKDEFS_TAGITEM_H
#include "uiki:uiktag.h"
#endif


struct UIKObjReqMode
    {
    struct UIKObj   Obj;

    struct UIKObj   *WO;
    struct UIKObj   *Str1;
    struct UIKObj   *Str2;
    struct UIKObj   *Str3;
    ULONG           Str1Res;
    ULONG           Str2Res;
    ULONG           Str3Res;
    ULONG           Str4Res;

    struct UIKPrefs MPrefs;
    };

/*---------Obj ReqMode Tags---------- */
/* tous les titres de bouton sont stock�s par le programme principal
 * dans ses deux tableaux de titres, chacun en une langue.
 * L'ordre des titres doit �tre (en fran�ais):
 *
 * Il faut passer � ce tag le num�ro d'ordre dans le tableau du la
 * chaine "ReqMode". Les num�ros d'ordre des autres en seront d�duits
 * par +1, +2, etc...
 */
#define UIKTAG_ObjReqMode_FirstTitleNum     (UIKTAG_USER|0x0001)

/*---------Obj ReqMode Flags---------- */
/*#define UIKTAG_OBJReqMode_Flags          (UIKTAG_USER|0x0020)*/
 /* sinon un par un... */
/*#define UIKTAG_OBJReqModeFl_  (UIKTAG_USER|0x0021)*/

/*-- */

#endif
